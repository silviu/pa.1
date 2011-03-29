#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string>
#include <map>

using namespace std;

typedef map<string, int> map_t;

class Dictionary
{	
	public:
		Dictionary();
		int get_frequency(string word);
		void print();
		void reset_iterator();
		bool has_more_words();
		char* get_next_word();
		
	private:
		map_t word_map;
		map_t::iterator it;
		void populate();
};
#endif // __DICTIONARY_H__
