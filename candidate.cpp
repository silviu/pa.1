#include "candidate.h"

Candidate::Candidate(int pstart, int pend, int pdist)
{
	start = pstart;
	end = pend;
	dist = pdist;
}

int Candidate::get_start()
{
	return start;
}

int Candidate::get_end()
{
	return end;
}

int Candidate::get_dist()
{
	return dist;
}

