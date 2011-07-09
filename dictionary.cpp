#include "dictionary.h"
#include <fstream>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_SIZE 64

Dictionary::Dictionary()
{
	populate();
	it = word_map.begin();
}

int Dictionary::get_frequency(string searched_word)
{
	map_t::iterator iter = word_map.find(searched_word);
    if (iter != word_map.end())
    	return iter->second;
    return -1;
}

void Dictionary::populate()
{
	char word[MAX_WORD_SIZE];
	int frequency;
	ifstream input_file;
	
	input_file.open("dict.txt", ifstream::in);
	if (!input_file.is_open()) {
		fprintf(stderr, "Error opening dict.txt");
		exit(EXIT_FAILURE);
	}
	
	while(input_file.good()) {
		input_file >> word >> frequency;
		if (!strcmp(word,"et"))
			continue;
		word_map.insert(pair<string, int>(word, frequency));
	}
	
	input_file.close();
}

void Dictionary::print()
{
	map_t::iterator it;
	for (it = word_map.begin(); it != word_map.end(); ++it)
		cout << it->first << " " << it->second << endl;
}

void Dictionary::reset_iterator()
{
	it = word_map.begin();
}

bool Dictionary::has_more_words()
{
	return (it != word_map.end());
}

char* Dictionary::get_next_word()
{
	string word = it->first;
	it++;
	char* ret = (char*) word.c_str();
	return ret;
}


