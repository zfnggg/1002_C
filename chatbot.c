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
#include <unistd.h>
#include "ini.h"
 
 
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
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n, content, head);
	else if (chatbot_is_reset(response, n, inv[0]))
		return chatbot_do_reset(response, n, head, content);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n, content, head);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n, content, head);
	else if (chatbot_is_bored(inc, inv))
		return chatbot_do_game(inc, inv, response, n);
	else if (chatbot_is_google(inv[0]))
		return chatbot_do_google(inc, inv, response, n);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}

int chatbot_is_google(const char *intent){
	if (compare_token(intent, "google") == 0 || compare_token(intent, "search") == 0)
		return 1;


	return 0;

}


int chatbot_do_google(int inc, char *inv[], char *response, int n){
	int i;
	char google_search[MAX_RESPONSE] = "START www.google.com/search?q=";
	char entity[MAX_ENTITY] = "";
	for (i=1; i < inc; i++){
		if (compare_token(inv[i], "for") == 0)
			i++;
		strcat(google_search, inv[i]);
		strcat(entity, inv[i]);
		if (inv[i+1] != '\0'){
			strcat(google_search, "+");
		 	strcat(entity, " ");
		}
	}
	snprintf(response, n, "Alright, I will search Google for %s.", entity);
	system(google_search);

	return 0;
}

int chatbot_is_bored(int inc, char *inv[]){
	int i;
	for (i=0; i < inc; i++){
		if (compare_token(inv[i], "bored") == 0){
		return 1;
		}
	}
	return 0;
}

int chatbot_do_game(int inc, char *inv[], char *response, int n){
	char input[MAX_INPUT];
	printf("%s: Would you like to play a game?\n", chatbot_botname());
	printf("%s: ", chatbot_username());
	fgets(input, MAX_INPUT, stdin);
	strtok(input, "\n");
	if (compare_token(input, "yes") == 0){
		system("tictactoe.exe");
		snprintf(response, n, "Thank you for playing with me");
		return 0;
	}
	else {
		snprintf(response, n, ":( Okay..");
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
	fclose(history);
	 
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
		printf("Load detected\n");
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
int chatbot_do_load(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head) {
	int i = 1;
	while (compare_token(inv[i], "from") == 0){
		i++;
	}
	*content = ini_load(inv[i]);
	if (access(inv[i], F_OK) != -1){
		snprintf(response, n, "Load successful");
	}
	else {
		snprintf(response, n, "Load failed or file does not exist");
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
	const char *question_words[11] = {"what", "where", "who", "waht", "wher", "whr", "whre", "whoo", "woh", "wat"};
	int i=0;
	while (question_words[i] != NULL){
		if (compare_token(intent, question_words[i]) == 0){
			return 1;
		}
		i++;
	}
	
	
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
int chatbot_do_question(int inc, char *inv[], char *response, int n, ini_t **content, pknowledge *head) {
	char entity[MAX_ENTITY]="", filler[5] = "";
	int status;
	/*List of function words to exclude from entity*/
	const char *function_words[10] = {"the", "of", "that", "this", "those", "these", "which", "a", "an"};
	
	// assuming the 3rd word is the entity (will change)
	int i=1, j=0;
	while (inv[i] != NULL){
		
		if (compare_token(inv[i], "is") == 0 || compare_token(inv[i], "are") == 0 ){
			strcpy(filler, inv[i]);
			i++;
		}
		while (function_words[j] != NULL){
			if (compare_token(inv[i], function_words[j]) == 0)
				i++;

			j++;
		}
		strcat(entity, inv[i]);
		if (inv[i+1] != '\0')
			strcat(entity, " ");
		
		i++;
	}
	if (compare_token(inv[0], "whr") == 0 || compare_token(inv[0], "wher") == 0 || compare_token(inv[0], "whre") == 0)
		inv[0] = "where";
	else if (compare_token(inv[0], "woh") == 0 || compare_token(inv[0], "whoo") == 0)
		inv[0] = "who";
	else if (compare_token(inv[0], "waht") == 0 || compare_token(inv[0], "wat") == 0)
		inv[0] = "what";
	
	status = knowledge_get(inv[0], entity, response, n, content, head);
		
	if (status == KB_OK){
		return 0;
	}
	// if (status == KB_INVALID){
	// 	snprintf(response, n, "I don\'t understand \"%s\"", inv[0]);
	// }
	else{ //(status == KB_NOTFOUND){
		char input[MAX_INPUT];
		if (compare_token(filler, "") == 0){
			snprintf(response, n, "I don\'t know, %s %s?", inv[0], entity);
		}
		else {
			snprintf(response, n, "I don\'t know, %s %s %s?", inv[0], filler, entity);
		}
		printf("%s: %s\n", chatbot_botname(), response);
		printf("%s: ", chatbot_username());
		fgets(input, MAX_INPUT, stdin);
		strtok(input, "\n");
		if (knowledge_put(inv[0], entity, input, head) == 0){
			snprintf(response, n, "Answer added successfully");
		}
		else {
			snprintf(response, n, "There was a problem adding answer to database");
		}
	}
	
	
	 
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
int chatbot_is_reset(char *response, int n, const char *intent) {

	if (compare_token(intent, "reset") == 0)
	{
		snprintf(response, n, "Initialising Reset");

		return 1;
	}
	else
	{
		return 0;
	}
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
int chatbot_do_reset(char* response, int n, pknowledge *head, ini_t **content)
{
	
	pknowledge temp = *head;
	pknowledge prev = *head;
	if (*content == NULL && temp == NULL){
		snprintf(response, n, "Nothing to reset");
		return 0;
	}

	else if (*content != NULL){
		ini_free(*content);
		while (temp != NULL)
		{
			if (temp->next != NULL)
			{
				prev = temp;
				temp = temp->next;
				*head = temp;
				free(prev);
				snprintf(response, n, "Reset Complete");
				return 0;
			}
			else
			{
				*head = temp;
				free(temp);
				*head = NULL;
				snprintf(response, n, "Reset Complete");
				return 0;
			}
		}
	}
	else{
		while (temp != NULL)
		{
			if (temp->next != NULL)
			{
				prev = temp;
				temp = temp->next;
				*head = temp;
				free(prev);
				snprintf(response, n, "Reset Complete");
				return 0;
			}
			else
			{
				*head = temp;
				free(temp);
				*head = NULL;
				snprintf(response, n, "Reset Complete");
				return 0;
			}
		}
	}
	
	snprintf(response, n, "Reset Complete");
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
	return 0; 

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
		return 0;
	}
	else if (status == -1){
		snprintf(response, n, "No knowledge base");
		return 0;
	}
	else{
		snprintf(response, n, "No file to write to, Please load a file");
		return 0;
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
	char greeting[5][20]={"hi", "hey", "hello"}; //Store some possible greetings intents into arrays
	char me[5][20]={"good", "how"};
	char bye[5][20]={"goodbye", "bye", "see"};
	int i;
	for (i=0; i <= 2; i++){ //Loop through and see if first word matches greeting intents
		char *currentgreeting = greeting[i];
		if (compare_token(intent, currentgreeting) == 0){
			return 1;
		}
	}
	for (i=0; i <= 2; i++){
		char *currentme = me[i];
		if (compare_token(intent, currentme) == 0){
			return 1;
		}
	}
	for (i=0; i <= 2; i++){
		char *currentbye = bye[i];
		if (compare_token(intent, currentbye) == 0){
			return 1;
		}
	}
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
			snprintf(response, n, st_reply_greeting[random_reply]);
		}
	}
	for (i=0; i <= 2; i++){
		char *currentme = me[i];
		if (compare_token(inv[0], currentme) == 0){
			snprintf(response, n, st_reply_me[random_reply]);
		}
	}
	for (i=0; i <= 2; i++){
		char *currentbye = bye[i];
		if (compare_token(inv[0], currentbye) == 0){
			snprintf(response, n, st_reply_bye[random_reply]);
			return 1; //Quits the program if the smalltalk contains a farewell greeting
		}
	}
	
	return 0;
}
