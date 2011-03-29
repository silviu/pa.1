#ifndef __CANDIDATE_H__
#define __CANDIDATE_H__

using namespace std;

class Candidate
{	
	public:
		Candidate(char* word, int dist);
		int get_dist();
		char* get_word();
		void set_dist(int new_dist);
		void set_word(char* new_word);
	private:
	    char* word;
	    int dist;
};
#endif // __CANDIDATE_H__
