#include <iostream>
#include "dictionary.h"
#include "corrector.h"
#include "candidate.h"
#include <string.h>
#include <stdlib.h>
#include <vector>

int main()
{
	pair<char*, vector<Candidate*> > candidate;
	Corrector* corrector = new Corrector();
	char* a = strdup("musicchannel");
	//char* b = strdup("music");
	
	char* answer = corrector->correct(a);
	cout << "Did you mean: " << answer << " ?" << endl;
	return 0;
}

