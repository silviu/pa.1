#include "candidate.h"

Candidate::Candidate(char* pword, int pdist)
{
	word = pword;
	dist = pdist;
}

int Candidate::get_dist()
{
	return dist;
}

char* Candidate::get_word()
{
	return word;
}


void Candidate::set_dist(int new_dist)
{
	dist = new_dist;
}

void Candidate::set_word(char* new_word)
{
	word = new_word;
}
