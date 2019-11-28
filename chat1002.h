/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 chatbot.
 */
 
#ifndef _CHAT1002_H
#define _CHAT1002_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* the maximum number of characters we expect in a line of input (including the terminating null)  */
#define MAX_INPUT    256

/* the maximum number of characters allowed in the name of an intent (including the terminating null)  */
#define MAX_INTENT   32

/* the maximum number of characters allowed in the name of an entity (including the terminating null)  */
#define MAX_ENTITY   64

/* the maximum number of characters allowed in a response (including the terminating null) */
#define MAX_RESPONSE 256

/* return codes for knowledge_get() and knowledge_put() */
#define KB_OK        0
#define KB_NOTFOUND -1
#define KB_INVALID  -2
#define KB_NOMEM    -3

/* Linked List to contain all the newly added data */
struct knowledge_node{
	char intent[MAX_INTENT];
	char entity[MAX_ENTITY];
	char answer[MAX_RESPONSE];
	struct knowledge_node *next;
};


typedef struct knowledge_node knowledge;
typedef knowledge *pknowledge;
FILE *history;

/* structure that will contain the data of the file as well as the end character */
typedef struct ini_t ini_t;

int		tictactoe();

ini_t*      ini_load(const char *filename);
void        ini_free(ini_t *ini);
int			ini_get(ini_t *ini, const char *section, const char *key, char *string);
int		    ini_write(ini_t *ini, const char *section, const char *key, const char *string);
int         ini_sget(ini_t *ini, const char *section, const char *key, const char *scanfmt, void *dst);
 
/* functions defined in main.c */
int compare_token(const char *token1, const char *token2);
void prompt_user(char *buf, int n, const char *format, ...);

/* functions defined in chatbot.c */
const char *chatbot_botname();
const char *chatbot_username();
int chatbot_main(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head);
int chatbot_is_exit(const char *intent);
int chatbot_do_exit(int inc, char *inv[], char *response, int n);
int chatbot_is_load(const char *intent);
int chatbot_do_load(int inc, char *inv[], char *response, int n, ini_t **content);
int chatbot_is_question(const char *intent);
int chatbot_do_question(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head);
int chatbot_is_reset(char *response, int n, const char *intent);
int chatbot_do_reset(char *response, int n, pknowledge *head, ini_t **content);
int chatbot_is_save(const char *intent);
int chatbot_do_save(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head);
int chatbot_is_smalltalk(const char *intent);
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n);
int chatbot_is_bored(int inc, char *inv[]);
int chatbot_do_game(int inc, char *inv[], char *response, int n);
int chatbot_is_google(const char *intent);
int chatbot_do_google(int inc, char *inv[], char *response, int n);


/* functions defined in knowledge.c */
int knowledge_get(const char *intent, const char *entity, char *response, int n, ini_t **content, pknowledge *head);
int knowledge_put(const char *intent, const char *entity, const char *response, pknowledge *head);
void knowledge_reset();
int knowledge_read(FILE *f);
int knowledge_write(ini_t **content, pknowledge *head);

#endif