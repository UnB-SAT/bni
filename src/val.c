#include "val.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s <plan_file>\n", argv[0]);
		return 1;
	}
	initialize();
	size_t linecap = 0;
	char token, action[256], start = 0, end = 0, *line = NULL, i = 0, *planfile = argv[1];
	FILE *f = fopen(planfile, "r");
	while (fscanf(f, "%c", &token) != EOF) {
		if (token == ';') getline(&line, &linecap, f);
		else if (token == '(') action[i] = token, start = 1, i++;
		else if (token == ')') action[i] = token, start = 0, end = 1, i++;
		else if (token == '-' && start) action[i] = '_', i++;
		else if (start) action[i] = token, i++;
		if (end) {
			i = 0, end = 0;
			char ar = apply_actions(action);
			if (ar == 2) {
				printf("\033[31;1mATTENTION: \033[90mUnrecognised command. Check spelling and try again.\033[0m\n");
				exit(0);
			} else if (ar == 1) {
				printf("\033[31;1mATTENTION: \033[90mAction with invalid parameters.\033[0m\n");
				exit(0);
			}
		}
	}
	if (checktrue_goal()) printf("VALID PLAN\n");
	else printf("INCOMPLET PLAN\n");
	fclose(f);
	return 0;
}
