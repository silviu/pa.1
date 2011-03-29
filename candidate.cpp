#include "candidate.h"

Candidate::Candidate(char* pword, int pdist)
{
	word = pword;
	dist = pdist;
}

Candidate::Candidate()
{
	word = (char*)"NULL";
	dist = -1;
	freq = -1;
	nr_words = -1;
}

int Candidate::get_dist()
{
	return dist;
}

char* Candidate::get_word()
{
	return word;
}

int Candidate::get_freq()
{
	return freq;
}

int Candidate::get_nr_words()
{
	return nr_words;
}



void Candidate::set_dist(int new_dist)
{
	dist = new_dist;
}

void Candidate::set_word(char* new_word)
{
	word = new_word;
}

void Candidate::set_freq(int pfreq)
{
	freq = pfreq;
}

void Candidate::set_nr_words(int pnr_word)
{
	nr_words = pnr_word;
}

