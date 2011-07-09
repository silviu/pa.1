#ifndef __CORRECTOR_H__
#define __CORRECTOR_H__

#include <iostream>
#include <string>
#include <map>
#include "dictionary.h"
#include "candidate.h"
#include <vector>

using namespace std;

class Corrector
{	
	public:
		Corrector();
        Candidate**  get_deviation(char* a, char* b);
        char* correct(char* search_terms);
	private:
		Dictionary* dictionary;
		int minim(int a, int b, int c);
		char* dinamic(Candidate*** d, int n);
		char* remove_spaces(char* word);
};
#endif // __CORRECTOR_H__
