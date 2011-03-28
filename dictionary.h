#ifndef __DICTIONARY_H__
#define __DICTIONARY_H__

#include <iostream>
#include <string>
#include <map>

using namespace std;

class Dictionary
{	
	public:
		Dictionary();
		~Dictionary();
	private:
	    char* dictionary;
   		char* populate();
};
#endif // __DICTIONARY_H__
