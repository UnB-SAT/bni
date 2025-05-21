#include "val.h"

int main(void) {
	initialize();
	char input[256];
	while (fgets(input, 256, stdin) != NULL) {
		if (input[0] == ';') continue;
		char ar = apply_actions(input);
		if (ar == 2) {
			printf("\033[31;1mATTENTION: \033[90mUnrecognised command. Check spelling and try again.\033[0m\n");
			exit(0);
		} else if (ar == 1) {
			printf("\033[31;1mATTENTION: \033[90mAction with invalid parameters.\033[0m\n");
			exit(0);
		}
	}
	if (checktrue_goal()) printf("VALID PLAN\n");
	else printf("INCOMPLET PLAN\n");
	return 0;
}
