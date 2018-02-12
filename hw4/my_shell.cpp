#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
using namespace std;

void execCmd(vector <char *> &parsed) {
	//parse words vector
	char **args;
	args = (char **) malloc(sizeof(char*) * (parsed.size() + 1));
	for(int i = 0; i < parsed.size(); i++) {
		args[i] = parsed[i];
	}
	args[parsed.size()] = NULL;

	//check if command is to exit
	if(strcmp(args[0], "exit") == 0) {
		exit(0);
	}
	//check if command contains file redirection
	FILE *p;
	for(int i = 0; i < sizeof(args); i++) {
		if(strcmp(args[i], "<") == 0) {
			//open file for reading
			if(!(p = fopen(args[i + 1], "rb"))) {
				perror("ERROR: ");
				exit(1);
			}

			//execute command with file as standard input

			//close file

		}
		else if(strcmp(args[i], ">") == 0) {

		}
		else if(strcmp(args[i], ">>") == 0) {

		}
		else if(strcmp(args[i], "2>") == 0) {

		}
	}

	//call fork, exec and wait
	switch(fork()) {
		case 0:
		execvp(args[0], args);
		default:
		wait(NULL);
	}
	parsed.clear();
	free(args);
}

int main(int argc, char **argv) {
	//check command line args for prompt, display prompt if applicable
	char line[100], *ptr;
	vector <char *> words;
	if(argc > 2) {
		printf("Usage: ./my_shell [PROMPT]\n");
		exit(1);
	}
	if(argc < 2) {
		do {
			printf("my_shell> ");
			ptr = strtok(line, " ");
			while(ptr != NULL) {
				words.push_back(ptr);
				ptr = strtok(NULL, " ");
			}
			if(words.size() != 0) {
				execCmd(words);
			}
		}
		while(gets(line) != NULL);
	}
	else {
		do {
			printf("%s> ", argv[1]);
		}
		while(gets(line) != NULL);
	}
}