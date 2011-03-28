#include "dictionary.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAX_WORD_SIZE 64

Dictionary::Dictionary()
{
	dictionary = populate();
	//printf("%s\n", dictionary);
}

Dictionary::~Dictionary()
{
	free(dictionary);
}

int get_file_size(FILE* f)
{
	int rt = fseek(f, 0, SEEK_END);
	if (rt == -1) {
		fprintf(stderr, "Error getting file size.");
		return -1;
	}
	int size = ftell(f);
	rt = fseek(f, 0, SEEK_SET);
	if (rt == -1) {
		fprintf(stderr, "Error seeking back at start of file.");
		return -1;
	}
	return size;
}

char* Dictionary::populate()
{
	char word[MAX_WORD_SIZE];
	int freq;
	FILE* f = fopen("dict.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "Error opening file.\n");
		exit(EXIT_FAILURE);
	}
	
	int file_size = get_file_size(f);
	//printf("FILESIZE = %d\n", file_size);
	
	char* dict = (char*)malloc(file_size);
	int added = 0;
	
	while(!feof(f)) {
		fscanf(f, "%s %d", word, &freq);
		memcpy(dict + added, word, strlen(word));
		added += strlen(word);
		memcpy(dict + added, "$", 1);
		added++;
	}
	
	fclose(f);
	return dict;
}

