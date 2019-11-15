/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */
struct knowledge_node{
	char intent[MAX_INTENT];
	char entity[MAX_ENTITY];
	char answer[MAX_RESPONSE];
	struct knowledge_node *next;
};
typedef struct knowledge_node knowledge;
typedef knowledge *pknowledge;

int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	int i;
	pknowledge head,temp;
	pknowledge what1002 = (pknowledge)malloc(sizeof(knowledge));
	pknowledge where1002 = (pknowledge)malloc(sizeof(knowledge));
	strcpy(what1002->intent, "what");
	strcpy(what1002->entity, "1002");
	strcpy(what1002->answer, "1002 is a programming module");
	strcpy(where1002->intent, "where");
	strcpy(where1002->entity, "1002");
	strcpy(where1002->answer, "SIT at NYP");
	head = what1002;
	what1002->next = where1002;
	temp = head;
	/* to be implemented */
	while (temp != NULL){
		if (compare_token(intent, temp->intent) == 0 && compare_token(entity, temp->entity) == 0){
			snprintf(response, n, temp->answer);
			return KB_OK;
		}
		temp = temp->next;
	}
	
	return KB_NOTFOUND;
	
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char *intent, const char *entity, const char *response) {
	
	/* to be implemented */
	
	return KB_INVALID;
	
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE *f) {
	
	/* to be implemented */
	
	return 0;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	
	/* to be implemented */
	
}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
void knowledge_write(FILE *f) {
	
	/* to be implemented */
	
}