#include "repl.h"

int SIZENAMES = 30;
char **action_names = NULL;

int main(void) {
	rl_attempted_completion_function = action_name_completion;
	printheader();
	initialize();
	action_names = malloc(SIZENAMES * sizeof(char*));
	if (action_names == NULL) perror("Error allocating initial memory for action_names"), exit(1);
	char *input, sactions[] = "/tmp/actions.XXXXXX";
	int iactions = mkstemp(sactions);
	for ( ; ; ) {
		if (checktrue_goal())
			if(!ask_yes_no("Goal has been hit! Do u want to continue? (y/N)\n")) break;
		printf("Please enter one of the currently available actions:\n");
		check_show_actions(sactions);
		show_actions(sactions);
		input = readline(">> ");
		if (!input || strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) break;
		if (*input) add_history(input);
		char ar = apply_actions(input);
		if (ar == 2) printf("\033[31;1mATTENTION: \033[90mUnrecognised command. Check spelling and try again.\033[0m\n");
		else if (ar == 1) printf("\033[31;1mATTENTION: \033[90mAction with invalid parameters.\033[0m\n");
		free_names(), free(input);
	}
	free_names(), free(action_names);
	// free(input), 
	remove(sactions);
	return 0;
}

char** action_name_completion(const char *text, int start, int end) {
	rl_attempted_completion_over = 1;
	return rl_completion_matches(text, action_name_generator);
}

char* action_name_generator(const char *text, int state) {
	static int list_index, len;
	char *name;
	if (!state) {
		list_index = 0;
		len = strlen(text);
	}
	while ((name = action_names[list_index++]))
		if (strncmp(name, text, len) == 0)
			return strdup(name);
	return NULL;
}

void show_actions(const char *filename) {
	FILE *f = fopen(filename, "r");
	if (f == NULL) perror("Error opening file for action_names"), exit(1);
	int i = 0;
	char str[500];
	while (fscanf(f, "%[^\n]s", str) != EOF) {
		fscanf(f, "\n");
		if (i == SIZENAMES-1) {
			SIZENAMES *= 2; 
			action_names = realloc(action_names, SIZENAMES * sizeof(char*)); 
			if (action_names == NULL) perror("Error reallocating memory for action_names"), exit(1);
		}
		printf("  - %s\n", str);
		action_names[i] = malloc(strlen(str) + 1);
		if (action_names[i] == NULL) perror("Error allocating memory for action names"), exit(1);
		strcpy(action_names[i++], str);
	}
	action_names[i] = NULL;
	fclose(f);
}

void free_names(void) {
	int i = 0;
	while (action_names[i] != NULL)
		free(action_names[i++]);
	action_names[0] = NULL;
}

int ask_yes_no(const char *question) {
	char *response;
	for ( ; ; ) {
		response = readline(question);
		to_uppercase(response);
		if (response) {
			if (strcmp(response, "Y") == 0 || strcmp(response, "YES") == 0) {
				free(response);
				return 1;
			} else if (strcmp(response, "N") == 0 || strcmp(response, "NO") == 0 || (response && response[0] == '\0')) {
				free(response);
				return 0;
			}
			free(response);
		}
		printf("Please answer Y or N.\n");
	}
}

void to_uppercase(char *str) {
	for (int i = 0; str[i] != '\0'; i++)
		str[i] = toupper((unsigned char) str[i]);
}

void printheader(void) {
	const char *header =
		"\033[90m################################################################################################\n"
		"\033[90m# " "\033[31m _             _     \033[32m _____   \033[33m _____   \033[34m _____   \033[35m _          \033[36m _____  \033[37m ______ \033[91m _____   _"       "       \033[90m#\n" 
		"\033[90m# " "\033[31m| |           (_)   \033[32m |  __ \\ \033[33m |  __ \\ \033[34m |  __ \\ \033[35m | |        \033[36m |  __ \\ \033[37m|  ____|\033[91m|  __ \\ | |"   "      \033[90m#\n"
		"\033[90m# " "\033[31m| |__   _ __   _    \033[32m | |__) |\033[33m | |  | |\033[34m | |  | |\033[35m | |        \033[36m | |__) |\033[37m| |__   \033[91m| |__) || |"        "      \033[90m#\n"
		"\033[90m# " "\033[31m| '_ \\ | '_ \\ | |   \033[32m |  ___/  \033[33m| |  | |\033[34m | |  | |\033[35m | |        \033[36m |  _  / \033[37m|  __|  \033[91m|  ___/ | |"      "      \033[90m#\n"
		"\033[90m# " "\033[31m| |_) || | | || |   \033[32m | |      \033[33m| |__| | \033[34m| |__| |\033[35m | |____    \033[36m | | \\ \\ \033[37m| |____ \033[91m| |     | |____"  "  \033[90m#\n"
		"\033[90m# " "\033[31m|_.__/ |_| |_||_|   \033[32m |_|      \033[33m|_____/  \033[34m|_____/  \033[35m|______|   \033[36m |_|  \\_\\\033[37m|______|\033[91m|_|     |______|" " \033[90m#\n"
		"\033[90m################################################################################################\n"
		"\033[0m";
	printf("\e[1;1H\e[2J");
	printf("%s\n", header);
}
