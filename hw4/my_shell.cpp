#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <vector>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;

void execCmd(vector <char *> &parsed) {
	//parse words vector
	bool isRegular = true;
	char **args, **temp;
	int fdIn, fdOut, currentCount = 0;
	vector <char *> command;
	vector <char **> commands_forPipe;
	args = (char **) malloc(sizeof(char*) * (parsed.size() + 1));
	for(int i = 0; i < parsed.size(); i++) {
		args[i] = parsed[i];
	}
	args[parsed.size()] = NULL;

	//check if command contains file redirection, otherwise push word into commands buffer
	for(int i = 0; i < parsed.size(); i++) {
		if(strcmp(args[i], "<") == 0) {
			isRegular = false;
			fdIn = open(args[i + 1], O_RDONLY, 0);
			if(fdIn == -1) {
				perror("ERROR: ");
				exit(1);
			}
			temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
			if(temp == NULL) {
				perror("ERROR: ");
				exit(1);
			}
			switch(fork()) {
				case 0:
				for(int j = 0; j < command.size(); j++) {
					temp[j] = command[j];
				}
				temp[command.size()] = NULL;
				command.clear();

				//fdIn = open(args[i + 1], O_RDONLY, 0);
				if(dup2(fdIn, 0) == -1) {
					perror("ERROR: ");
					exit(1);
				}
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
			if(temp == NULL) {
				perror("ERROR: ");
				exit(1);
			}
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_TRUNC, 0777);
			if(fdOut == -1) {
				perror("ERROR: ");
				exit(1);
			}
			switch(fork()) {
				case 0:
				for(int j = 0; j < command.size(); j++) {
					temp[j] = command[j];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);m
				if(dup2(fdOut, 1) == -1) {
					perror("ERROR: ");
					exit(1);
				}
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
			if(temp == NULL) {
				perror("ERROR: ");
				exit(1);
			}
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_APPEND, 0777);
			if(fdOut == -1) {
				perror("ERROR: ");
				exit(1);
			}
			switch(fork()) {
				case 0:
				for(int j = 0; j < command.size(); j++) {
					temp[j] = command[j];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);m
				if(dup2(fdOut, 1) == -1) {
					perror("ERROR: ");
					exit(1);
				}
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
			if(temp == NULL) {
				perror("ERROR: ");
				exit(1);
			}
			fdOut = open(args[i + 1], O_CREAT|O_WRONLY|O_TRUNC, 0777);
			if(fdOut == -1) {
				perror("ERROR: ");
				exit(1);
			}
			switch(fork()) {
				case 0:
				for(int j = 0; j < command.size(); j++) {
					temp[j] = command[j];
				}
				temp[command.size()] = NULL;
				command.clear();

				// fdOut = open(args[i + 1], O_CREAT|O_WRONLY, 0777);
				if(dup2(fdOut, 2) == -1) {
					perror("ERROR: ");
					exit(1);
				}
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

		else if(strcmp(args[i], "|") == 0) {
			isRegular = false;
			int pipeCounter = 0;
			command.clear();
			//count number of total pipes needed, push back words if not a pipe
			for(int k = 0; k < parsed.size(); k++) {
				if(strcmp(args[k], "|") == 0 || k == parsed.size() - 1) {
					pipeCounter++;
					temp = (char **) malloc(sizeof(char*) * (command.size() + 1));
					for(int j = 0; j < command.size(); j++) {
						temp[j] = command[j];
					}
					temp[command.size()] = NULL;
					commands_forPipe.push_back(temp);
					free(temp);
					command.clear();
				}
				else {
					command.push_back(args[k]);
				}
			}

			//create needed file descriptors for pipes
			int pipeFds[2 * pipeCounter];

			int commandCounter = 0;
			switch(fork()) {
				case 0:
				if(commandCounter != 0) {
					dup2(pipeFds[(commandCounter - 1) * 2], 0);
				}

				if(commandCounter != commands_forPipe.size()) {
					dup2(pipeFds[commandCounter*2+1], 1);
				}

				for(int p = 0; p < sizeof(pipeFds); p++) {
					close(pipeFds[p]);
				}
				fprintf(stderr, "%i\n", sizeof(**commands_forPipe[commandCounter][0]));
			}
		}

		else {
			//printf("******PUSHING******\n");
			command.push_back(args[i]);
			++currentCount;
		}
	}

	//do a normal fork/exec if command contains no file redirection or pipes
	if(isRegular) {
		//call fork, exec and wait
		switch(fork()) {
			case 0:
			execvp(args[0], args);
			printf("Command not found\n");
			exit(1);
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
			//check command, exit if given, otherwise execute the command
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