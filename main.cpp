#include <iostream>
#include "dictionary.h"
#include "corrector.h"
#include "candidate.h"
#include <string.h>
#include <stdlib.h>
#include <vector>

int main()
{
	vector<Candidate*> candidates;
	Corrector* corrector = new Corrector();
	char* a = strdup("musicchannel");
	char* b = strdup("music");
	candidates = corrector->get_deviation(a, b);
	for (unsigned int i = 0; i < candidates.size(); i++) {
		int start  = candidates[i]->get_start();
		int end = candidates[i]->get_end();
		int dist = candidates[i]->get_dist();
		cout << "[" << start << ", " << end << "] = "<< dist << endl;
	}
	return 0;
}

