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
<<<<<<< Updated upstream
=======

int linked_lst_get(const char *intent, const char *entity, char *response, int n, pknowledge *head){
	/* checks the linked list for any answer given the intent and the entity 
	*  returns 0 upon successful, 1 upon not found
	*/
	int obtained = 0; // checks whether a value has beeen obtained
	pknowledge temp = *head;
	while (temp != NULL){
		if (strcmp(temp->intent, intent)==0 && strcmp(temp->entity, entity)==0){
			obtained = 1;
			strcpy(response, temp->answer);
		}
		temp = temp->next;
	}
	if (obtained){
		return 0;
	}
	else{
		return 1;
	}
}
>>>>>>> Stashed changes

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
<<<<<<< Updated upstream
int knowledge_get(const char *intent, const char *entity, char *response, int n) {
	
	/* to be implemented */
	
	return KB_NOTFOUND;
	
=======

int knowledge_get(const char *intent, const char *entity, char *response, int n, ini_t **content, pknowledge *head) {

	if (linked_lst_get(intent, entity, response, n, head) == 1){
		if (*content == NULL){
			return -1;
		}
		return ini_get(*content, intent, entity, response);
	}
	return 0;
>>>>>>> Stashed changes
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
int knowledge_put(const char *intent, const char *entity, const char *response, pknowledge *head) {
	
	pknowledge new = malloc(sizeof(knowledge));
	if (new == NULL){
		return -1;
	}
	strcpy(new->intent, intent);
	strcpy(new->entity, entity);
	strcpy(new->answer, response);
	new->next = NULL;

	if (*head == NULL){
		*head = new;
	}
	else {
		pknowledge temp = *head;
		while (temp->next != NULL){
			temp = temp->next;
		}
		temp->next = new;
	}
	
	return 0;
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
int knowledge_write(ini_t **content, pknowledge *head) {
	
	if (*head == NULL){
		return -1;
	}
	else if (*content == NULL){
		return -2;
	}
	else{
		pknowledge temp = *head;
		*head = NULL;
		while (temp != NULL){
			ini_write(*content, temp->intent, temp->entity, temp->answer);
			*content = ini_load("output.ini");
			pknowledge to_destroy = temp;
			temp = temp->next;
			free(to_destroy);
		}
		return 0;
	}

	
}