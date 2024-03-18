#include <iostream>
#include <vector>
#include <map>
#include <deque>
#include <string>
#include <ctime>
#include <chrono>

using namespace std;

//typedef deque<string> Prefix;
typedef vector<string> Prefix; 
//map<Prefix, vector<string>> statetab;
multimap<Prefix, string> statetab;

enum {
    NPREF = 2,
    NHASH = 5,
    MAXGEN = 10000
};
char NONWORD[] = "\n";

void add(Prefix& prefix, const string& s){
    if(prefix.size() == NPREF){
		
        // statetab[prefix].push_back(s);
        statetab.insert(make_pair(prefix, s)); 
        //prefix.pop_front();
        prefix.erase(prefix.begin());
		
    }
    prefix.push_back(s);
}

void build(Prefix& prefix, istream& in){
    string buf;
    while(in >> buf){
        add(prefix, buf);
    }
}

void generate(int nwords){
    Prefix prefix;
    int i;
    for(i = 0; i<NPREF; i++){
        add(prefix, NONWORD);
    }
    for(i=0; i<nwords; i++){
        
		// vector<string>& suf = statetab[prefix];
        // const string& w = suf[rand() % suf.size()];
        auto range = statetab.equal_range(prefix); 
        auto it = range.first;
        advance(it, rand() % distance(range.first, range.second)); 
        string w = it->second;
		
        if(w == NONWORD){
            break;
        }
        cout << w << "\n";
        //prefix.pop_front();
        prefix.erase(prefix.begin());
        prefix.push_back(w);
    }
}

class stopwatch {
    private:
        chrono::steady_clock::time_point startTime;
        double elapsedTime;
        
    public:
        stopwatch(){
            elapsedTime = 0.0;
        }
        
        ~stopwatch(){
            stop();
            cout << "Elapsed time: " << elapsedTime << " microseconds" << endl;
        }
        
        void start(){
            startTime = chrono::steady_clock::now();
        }
        
        void stop() {
			auto endTime = chrono::steady_clock::now();
			auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
			elapsedTime = duration.count();
		}
        
};

int main(){
    stopwatch timer;
    timer.start();
    int nwords = MAXGEN;
    Prefix prefix;
    for(int i=0; i<NPREF; i++){
        add(prefix, NONWORD);
    }
    build(prefix, cin);
    add(prefix,NONWORD);
    generate(nwords);
    timer.stop();
    return 0;
}
