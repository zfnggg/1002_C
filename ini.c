/**
 * Copyright (c) 2016 rxi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "chat1002.h"

struct ini_t {
  char *data;
  char *end;
};


/* Case insensitive string compare */
static int strcmpci(const char *a, const char *b) {
  for (;;) {
    int d = tolower(*a) - tolower(*b);
    if (d != 0 || !*a) {
      return d;
    }
    a++, b++;
  }
}

/* Returns the next string in the split data */
static char* next(ini_t *ini, char *p) {
  p += strlen(p);
  while (p < ini->end && *p == '\0') {
    p++;
  }
  return p;
}

static void trim_back(ini_t *ini, char *p) {
  while (p >= ini->data && (*p == ' ' || *p == '\t' || *p == '\r')) {
    *p-- = '\0';
  }
}

static char* discard_line(ini_t *ini, char *p) {
  while (p < ini->end && *p != '\n') {
    *p++ = '\0';
  }
  return p;
}


static char *unescape_quoted_value(ini_t *ini, char *p) {
  /* Use `q` as write-head and `p` as read-head, `p` is always ahead of `q`
   * as escape sequences are always larger than their resultant data */
  char *q = p;
  p++;
  while (p < ini->end && *p != '"' && *p != '\r' && *p != '\n') {
    if (*p == '\\') {
      /* Handle escaped char */
      p++;
      switch (*p) {
        default   : *q = *p;    break;
        case 'r'  : *q = '\r';  break;
        case 'n'  : *q = '\n';  break;
        case 't'  : *q = '\t';  break;
        case '\r' :
        case '\n' :
        case '\0' : goto end;
      }

    } else {
      /* Handle normal char */
      *q = *p;
    }
    q++, p++;
  }
end:
  return q;
}


/* Splits data in place into strings containing section-headers, keys and
 * values using one or more '\0' as a delimiter. Unescapes quoted values */
static void split_data(ini_t *ini) {
  char *value_start, *line_start;
  char *p = ini->data; //p points to the start of the data block

  while (p < ini->end) {
    switch (*p) {
      case '\r':
      case '\n':
      case '\t':
      case ' ':
        *p = '\0';
        /* Fall through */

      case '\0':
        p++;
        break;

      case '[':
        p += strcspn(p, "]\n"); //strcspn calculates the first occurance of a "]\n" in the string
        *p = '\0'; //change the "]" into "\0"
        break;

      case ';':
        p = discard_line(ini, p); // *p++ = '\0'
        break;

      default:
        line_start = p; 
        p += strcspn(p, "=\n"); //Shift P pointer to the first occurance of '='

        /* Is line missing a '='? */
        if (*p != '=') {
          p = discard_line(ini, line_start); //*line_start++ = "\0"
          break;
        }
        trim_back(ini, p - 1); //*p-2 = "\0"

        /* Replace '=' and whitespace after it with '\0' */
        do {
          *p++ = '\0';
        } while (*p == ' ' || *p == '\r' || *p == '\t');

        /* Is a value after '=' missing? */
        if (*p == '\n' || *p == '\0') {
          p = discard_line(ini, line_start); //*line_start++ = "\0" 
          break;
        }

        if (*p == '"') {
          /* Handle quoted string value */
          value_start = p;
          p = unescape_quoted_value(ini, p);

          /* Was the string empty? */
          if (p == value_start) {
            p = discard_line(ini, line_start);
            break;
          }

          /* Discard the rest of the line after the string value */
          p = discard_line(ini, p); //*p++ = '\0'

        } else {
          /* Handle normal value */
          p += strcspn(p, "\n"); //Shift the p pointer to the first occurance of \n
          trim_back(ini, p - 1); // *p=2 = '\0'
        }
        break;
    }
  }
}



ini_t* ini_load(const char *filename) {
  ini_t *ini = NULL;
  FILE *fp = NULL;
  int n, sz;

  /* Init ini struct */
  ini = malloc(sizeof(*ini));
  if (!ini) {
    goto fail;
  }
  memset(ini, 0, sizeof(*ini)); //Change content of ini string to '0's

  /* Open file */
  fp = fopen(filename, "rb");
  if (!fp) {
    goto fail;
  }

  /* Get file size */
  fseek(fp, 0, SEEK_END); //Move fp to the End of the file
  sz = ftell(fp); //Get size of the entire file
  rewind(fp); //Reset the fp to the start of the file

  /* Load file content into memory, null terminate, init end var */
  ini->data = malloc(sz + 1); //Allocate memory location of file size + 1 extra btye
  ini->data[sz] = '\0'; //Store '\0' into the 1 extra btye allocated 
  ini->end = ini->data  + sz; //Set end pointer to point to the end of the file
  n = fread(ini->data, 1, sz, fp); //fread (point to the block of memory to read, number pf btye for each memory block, number of blocks, file stream)
  if (n != sz) {
    goto fail;
  }

  /* Prepare data */
  split_data(ini);

  /* Clean up and return */
  fclose(fp);
  return ini;

fail:
  if (fp) fclose(fp);
  if (ini) ini_free(ini);
  return NULL;
}


void ini_free(ini_t *ini) {
  free(ini->data);
  free(ini);
}


int ini_get(ini_t *ini, const char *section, const char *key, char *string) {
  char *current_section = "";
  char *val;
  char *p = ini->data;
  int section_found = 0;

  if (*p == '\0') {
    p = next(ini, p);
  }

  while (p < ini->end) {
    if (*p == '[') {
      /* Handle section */
      current_section = p + 1;

    } else {
      /* Handle key */
      val = next(ini, p);
      if (!section || !strcmpci(section, current_section)) {
        section_found = 1;
        if (!strcmpci(p, key)) {
          strcpy(string, val);
          return 0;
        }
      }
      p = val;
    }

    p = next(ini, p);
  }

  if (section_found){
    return -1;
  }

  return -2;
}

int ini_write(ini_t *ini, const char *section, const char *key, const char *string) {
  // Creates a file handle using a given filename
  // section is intent, key is entity, string is answer
  FILE *fp;
  fp = fopen("output.ini", "w");
  if (!fp){
    printf("File cannot be open");
    return -2;
  }
  int first = 1;

  char *current_section = ""; //empty string
  char *val; //char pointer
  char *p = ini->data; //ini has ini->data and ini->end *p points to the start of the string in ini->data
  char output[MAX_RESPONSE];
  char temp[MAX_RESPONSE];
  int key_found = 0;
  int section_found = 0;
  int correct_section;

  if (output == NULL || temp == NULL){
    return -3;
  }

  if (*p == '\0') {
    p = next(ini, p);
  }

  while (p < ini->end) {
    if (*p == '[') {
      current_section = p + 1;
      section_found = strcmpci(current_section, section);
      sprintf(output, "[%s]\n", current_section);
    } 
    else {
      val = next(ini, p);
      if (!section_found && !strcmpci(p, key)) {
        sprintf(output, "%s=%s\n", p, string);
        key_found = 1;
      }
      else {
        sprintf(output, "%s=%s\n", p, val);
      }
      p = val;
    }

    if (!section_found && !key_found && (*next(ini, p)=='[' || *next(ini, p)==*(ini->end))){
      sprintf(temp, "%s=%s\n", key, string);
      fwrite(temp, 1, strlen(temp), fp);
    }

    if (first && *output == '['){
      first = 0;
    }
    else if (*output == '['){
      fwrite("\n", 1, 1, fp);
    }

    fwrite(output, 1, strlen(output), fp);

    p = next(ini, p);
  }

  // free(temp);
  // free(output);
  fclose(fp);

  if (section_found){
    return 0;
  }
  else {
    return -1;
  }
}