#include <iostream>
#include "corrector.h"
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <string>

int main()
{
	pair<char*, vector<Candidate*> > candidate;
	Corrector* corrector = new Corrector();
	string search_terms;
	getline(cin, search_terms);
	
	char* answer = corrector->correct((char*)search_terms.c_str());
	cout << answer << endl;
	return 0;
}

