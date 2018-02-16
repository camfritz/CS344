#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
using namespace std;

// struct command_node {
// 	char **command;
// 	struct command_node *next;
// };

void execCmd(vector <char *> &parsed) {
	//parse words vector
	bool isRegular = true;
	char **args, **temp;
	int fdIn, fdOut, currentCount = 0;
	vector <char *> command;
	args = (char **) malloc(sizeof(char*) * (parsed.size() + 1));
	for(int i = 0; i < parsed.size(); i++) {
		args[i] = parsed[i];
	}
	args[parsed.size()] = NULL;

	//check if command contains file redirection
	for(int i = 0; i < parsed.size(); i++) {
		if(strcmp(args[i], "<") == 0) {
			isRegular = false;
			fdIn = open(args[i + 1], O_RDONLY, 0);
			temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
			switch(fork()) {
				case 0:
				for(int i = 0; i < command.size(); i++) {
					temp[i] = command[i];
				}
				temp[command.size()] = NULL;
				command.clear();

				//fdIn = open(args[i + 1], O_RDONLY, 0);
				dup2(fdIn, 0);
				execvp(temp[0], temp);
				default:
				wait(NULL);
				currentCount = 0;
				//printf("******FREEING******\n");
				free(temp);
				close(fdIn);
				break;
			}
		}

		else if(strcmp(args[i], ">") == 0) {
			isRegular = false;
			temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_TRUNC, 0777);
			switch(fork()) {
				case 0:
				for(int i = 0; i < command.size(); i++) {
					temp[i] = command[i];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);
				dup2(fdOut, 1);
				execvp(temp[0], temp);

				default:
				wait(NULL);
				currentCount = 0;
				//printf("******FREEING******\n");
				free(temp);
				close(fdOut);
				i++;
				break;
			}
		}

		else if(strcmp(args[i], ">>") == 0) {
			isRegular = false;
			temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_APPEND, 0777);
			switch(fork()) {
				case 0:
				for(int i = 0; i < command.size(); i++) {
					temp[i] = command[i];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);
				dup2(fdOut, 1);
				execvp(temp[0], temp);

				default:
				wait(NULL);
				currentCount = 0;
				//printf("******FREEING******\n");
				free(temp);
				close(fdOut);
				i++;
				break;
			}
		}

		else if(strcmp(args[i], "2>") == 0) {
			isRegular = false;
			temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_TRUNC, 0777);
			switch(fork()) {
				case 0:
				for(int i = 0; i < command.size(); i++) {
					temp[i] = command[i];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);
				dup2(fdOut, 2);
				execvp(temp[0], temp);

				default:
				wait(NULL);
				currentCount = 0;
				//printf("******FREEING******\n");
				free(temp);
				close(fdOut);
				i++;
				break;
			}
		}

		else {
			//printf("******PUSHING******\n");
			command.push_back(args[i]);
			++currentCount;
		}
	}

	if(isRegular) {
		//call fork, exec and wait
		switch(fork()) {
			case 0:
			execvp(args[0], args);
			default:
			wait(NULL);
		}
	}
	//printf("******FREEING******\n");
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
		printf("my_shell> ");
		while(gets(line) != NULL) {
			ptr = strtok(line, " ");
			while(ptr != NULL) {
				words.push_back(ptr);
				ptr = strtok(NULL, " ");
			}
			if(words.size() != 0) {
				if(strcmp(words[0], "exit") == 0) {
					exit(0);
				}
				else {
					execCmd(words);
				}

			}
			printf("my_shell> ");
		}
	}
	else {
		printf("%s> ", argv[1]);
		while(gets(line) != NULL) {
			printf("%s> ", argv[1]);
			ptr = strtok(line, " ");
			while(ptr != NULL) {
				words.push_back(ptr);
				ptr = strtok(NULL, " ");
			}
			if(strcmp(words[0], "exit") == 0) {
				exit(0);
			}
			else {
				execCmd(words);
			}
		}
	}
}