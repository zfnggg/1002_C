/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
 
#include <stdio.h>
#include <string.h>
#include "chat1002.h"
 
 
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";
	
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";
	
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head) {
	
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n, content);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n, content, head);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n, content, head);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0;
	
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	 
	snprintf(response, n, "Goodbye!");
	 
	return 1;
	 
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	if (compare_token(intent, "load") == 0){
		printf("load detected\n");
	}
	return compare_token(intent, "load") == 0;
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n, ini_t **content) {
	
	// assuming the response includes a file name (change later)
	int i = 1;
	while (compare_token(inv[i], "from") == 0){
		i++;
	}
	*content = ini_load(inv[i]);
	if (content != NULL){
		snprintf(response, n, "Load successful");
	}
	else {
		snprintf(response, n, "Load failed");
	}
	 
	return 0;
	 
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char *intent) {
	
<<<<<<< Updated upstream
<<<<<<< Updated upstream
	/* to be implemented */
=======
=======
>>>>>>> Stashed changes
	/* checks if intent is what, where or who */
	if (compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0){
		printf("Question detected\n");
		return 1;
	}
>>>>>>> Stashed changes
	
	return 0;
	
}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
<<<<<<< Updated upstream
<<<<<<< Updated upstream
int chatbot_do_question(int inc, char *inv[], char *response, int n) {
	
	/* to be implemented */
=======
=======
>>>>>>> Stashed changes
int chatbot_do_question(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head) {
	/* to be implemented */

	char entity[MAX_ENTITY];
	
	// assuming the 3rd word is the entity (will change)
	strcpy(entity, inv[2]);
	
	int status = knowledge_get(inv[0], entity, response, n, content, head);
	if (status == KB_INVALID){
		snprintf(response, n, "I don\'t understand \"%s\"", inv[0]);
	}
	else if (status == KB_NOTFOUND){
		char input[MAX_INPUT];
		snprintf(response, n, "I don\'t know, %s?", *inv);
		printf("%s: %s\n", chatbot_botname(), response);
		printf("%s: ", chatbot_username());
		fgets(input, MAX_INPUT, stdin);
		strtok(input, "\n");
		if (knowledge_put(inv[0], inv[2], input, head) == 0){
			snprintf(response, n, "Answer added successfully");
		}
		else {
			snprintf(response, n, "There was a problem adding answer to database");
		}
	}
	else if (status == KB_OK){
		return 0;
	}
>>>>>>> Stashed changes
	 
	return 0;
	 
}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	
	/* to be implemented */
	
	return 0;
	
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	
	/* to be implemented */
	 
	return 0;
	 
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
	
	if (compare_token(intent, "save") == 0){
		printf("Save detected\n");
		return 1;
	}
	else return 0;

}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head) {
	
	int status = knowledge_write(content, head);
	if (status == 0){
		snprintf(response, n, "Successfully written");
	}
	else if (status == -1){
		snprintf(response, n, "No knowledge base");
	}
	else{
		snprintf(response, n, "No file to write to, Please load a file");
	}
	
	return 0;
	 
}
 
 
/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	
	/* to be implemented */
	
	return 0;
 
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
<<<<<<< Updated upstream
	
	/* to be implemented */
=======
	char st_reply_greeting[5][50]={"Hello!", "Hey there!", "Hihihi!"}; //Store some replies in the arrays
	char st_reply_me[5][50]={"Good day to you too!", "I am good, you?", "Nice weather we're having."};
	char st_reply_bye[5][50]={"Goodbye!", "Bye-bye!", "See you soon!"};
	char greeting[5][20]={"hi", "hey", "hello"};
	char me[5][20]={"good", "how"};
	char bye[5][20]={"goodbye", "bye", "see"};
	int i;
	srand(time(NULL)); //Select a random number to select a random response from array
	int random_reply=rand()%3; //Select a random number to select a random response from array
	for (i=0; i <= 2; i++){ //Same loop as chatbot_is_smalltalk to check for an appropriate response to a greeting
		char *currentgreeting = greeting[i];
		if (compare_token(inv[0], currentgreeting) == 0){
			snprintf(response, n, "%s\n", st_reply_greeting[random_reply]);
		}
	}
	for (i=0; i <= 2; i++){
		char *currentme = me[i];
		if (compare_token(inv[0], currentme) == 0){
			snprintf(response, n, "%s\n", st_reply_me[random_reply]);
		}
	}
	for (i=0; i <= 2; i++){
		char *currentbye = bye[i];
		if (compare_token(inv[0], currentbye) == 0){
			snprintf(response, n, "%s\n", st_reply_bye[random_reply]);
			return 1; //Quits the program if the smalltalk contains a farewell greeting
		}
	}
>>>>>>> Stashed changes
	
	return 0;
	
}
