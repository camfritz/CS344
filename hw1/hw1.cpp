#include <cstdlib>
#include <stdio.h>
#include <map>
#include <vector>
#include <string.h> //unused, included for compiling on linux servers
using namespace std;

struct Person {
	char *name;
	int age;
	double weight;
	double height;
};

int main (int argc, char **argv) {
	//Check for correct command line usage
	if(argc != 2) {
		fprintf(stderr, "%s", "Error: No input file\nUsage: './hw1 input_filename'\n");
		exit(1);
	}
//-------------------------------
//Temp Variables & Data Structures
	char nameStr[100];
	int a;
	double w, h;
	vector<Person*> people;
	map<double, char*> heightPair;
	map<double, char*> weightPair;
	map<int, char*, greater<int> > agePair;
	map<double, char*>::iterator it;
	map<int, char*>::iterator it2;
	Person *p;
//-------------------------------

	//error check for valid file, open file and read data
	FILE *inFile = fopen(argv[1], "r");
	if(inFile != NULL) {
		//read data from file, allocate memory to new struct, store struct into map with appropriate order value
		while(fscanf(inFile, "%s %d %lf %lf", nameStr, &a, &w, &h) > 0) {
			p = (Person*) malloc(sizeof(Person));
			if(p == NULL) {
				perror("ERROR ALLOCATING MEMORY");
				exit(1);
			}
			p->name = strdup(nameStr);
			if(a <= 0) {
				printf("%s's age must be Non-negative\n", nameStr);
				fclose(inFile);
				exit(1);
			}
			else if(w <= 0) {
				printf("%s's weight must be Non-negative\n", nameStr);
				fclose(inFile);
				exit(1);
			}
			else if(h <= 0) {
				printf("%s's height must be Non-negative\n", nameStr);
				fclose(inFile);
				exit(1);
			}
			p->age = a;
			p->weight = w;
			p->height = h;
			people.push_back(p);
		}
		fclose(inFile);

		//order data in height:name pairs
		for(int i = 0; i < people.size(); i++) {
			heightPair[people[i]->height] = people[i]->name;
		}

		//order data in weight:name pairs
		for(int i = 0; i < people.size(); i++) {
			weightPair[people[i]->weight] = people[i]->name;
		}

		//order data in age:name pairs
		for(int i = 0; i < people.size(); i++) {
			agePair[people[i]->age] = people[i]->name;
		}

		//print names
		printf("%s\n", "====order====");
		for(int i = 0; i < people.size(); i++) {
			printf("%s\n", people[i]->name);
		}

		//print height:name pairs
		printf("%s\n", "====height====");
		for(it = heightPair.begin(); it != heightPair.end(); it++) {
			printf("%.3f:%s\n", it->first, it->second);
		}

		//print weight:name pairs
		printf("%s\n", "====weight====");
		for(it = weightPair.begin(); it != weightPair.end(); it++) {
			printf("%.3f:%s\n", it->first, it->second);
		}

		//print age:name pairs
		printf("%s\n", "====age====");
		for(it2 = agePair.begin(); it2 != agePair.end(); it2++) {
			printf("%d:%s\n", it2->first, it2->second);
		}

		//free memory
		for(int i = 0; i < people.size(); i++) {
			p = people[i];
			free(p->name);
			free(p);
		}
	}
	else {
		perror("ERROR OPENING FILE");
		exit(1);
	}
}