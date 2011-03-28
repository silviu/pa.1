#ifndef __CANDIDATE_H__
#define __CANDIDATE_H__

using namespace std;

class Candidate
{	
	public:
		Candidate(int start, int end, int dist);
		int get_start();
		int get_end();
		int get_dist();
	private:
	    int start;
	    int end;
	    int dist;
};
#endif // __CANDIDATE_H__
