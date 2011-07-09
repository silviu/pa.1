#ifndef __CANDIDATE_H__
#define __CANDIDATE_H__

using namespace std;

class Candidate
{	
	public:
		Candidate(char* word, int dist);
		Candidate();
	    ~Candidate();
		int get_dist();
		char* get_word();
		int get_freq();
		int get_nr_words();
		
		void set_dist(int new_dist);
		void set_word(char* new_word);
		void set_freq(int pfreq);
		void set_nr_words(int pnr_word);
	private:
        int dist;
	    char* word;
	    int freq;
	    int nr_words;
};
#endif // __CANDIDATE_H__
