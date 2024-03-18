#include <iostream>
#include <cstring>
#include <ctime>
#include <chrono>
using namespace std;
enum{
    NPREF = 2,
    NHASH = 5,
    MAXGEN = 100000
};
char NONWORD[] = "\n";
enum{MULTIPLIER = 31};

typedef struct State State;
typedef struct Suffix Suffix;

struct State{
    char *pref[NPREF];
    Suffix *suf;
    State *next;
};
struct Suffix{
    char *word;
    Suffix *next;
};

State *statetab[NHASH];

void generate(int nwords);
void build(char *prefix[NPREF], FILE *f);
void addsuffix(State *sp, char *suffix);
void add(char *prefix[NPREF], char *suffix);
unsigned int myhash(char *s[NPREF]);
char *mystrdup(char *s);
State *lookup(char *prefix[NPREF], int create);
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

int main(void){
    stopwatch timer;
    timer.start();
    int i;
    int nwords = MAXGEN;
    char *prefix[NPREF];

    for(i = 0; i< NPREF; i++){
        prefix[i] = NONWORD;
    }
    FILE *file;
    file = fopen("file1.txt", "r");
    if(file){
        build(prefix, file);
        fclose(file);
    }

    add(prefix, NONWORD);
    generate(nwords);
    timer.stop();
    return 0;
}

void generate(int nwords){
    State *sp;
    Suffix *suf;
    char *prefix[NPREF], *w;
    int i;
    int nmatch;
    for(i = 0; i < NPREF; i++){
        prefix[i] = NONWORD;
    }
    for(i = 0; i < nwords; i++){
        sp = lookup(prefix, 0);
        nmatch = 0;
        
        for(suf = sp->suf; suf != NULL; suf = suf->next){
            if(rand() % ++nmatch == 0){
                w = suf->word;
            }
        }
        if(strcmp(w, NONWORD) == 0){
            break;
        }
        printf("%s\n", w);
        memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
        prefix[NPREF-1] = w;
    }
}


void build(char *prefix[NPREF], FILE *f){
    char buf[100];
    char fmt[10];
    sprintf(fmt, "%%%lds", sizeof(buf)-1);
    printf("fmt=\"%s\"\n", fmt);
    while(fscanf(f, fmt, buf) != EOF){
        printf("%s ", buf);
        add(prefix, mystrdup(buf));
    }
    putchar('\n');
}

void addsuffix(State *sp, char *suffix){
    Suffix *suf;
    suf = (Suffix *)malloc(sizeof(Suffix));
    if(suf == NULL){
        perror("Memory allocation error");
        exit(1);
    }
    suf->word = suffix;
    suf->next = sp->suf;
    sp->suf = suf;
}


void add(char *prefix[NPREF], char *suffix){
    State *sp;
    sp = lookup(prefix, 1);
    addsuffix(sp, suffix);
    memmove(prefix, prefix+1, (NPREF-1)*sizeof(prefix[0]));
    prefix[NPREF-1] = suffix;
}

State *lookup(char *prefix[NPREF], int create){
    int i;
    State *sp;
    int h = myhash(prefix);
    for(sp = statetab[h]; sp != NULL; sp = sp->next){
        for(i = 0; i < NPREF; i++){
            if(strcmp(prefix[i], sp->pref[i]) != 0){
                break;
            }
        }
        if(i == NPREF){
            return sp;
        }
    }
    if(create){
        sp = (State *)malloc(sizeof(State));
        if(sp == NULL){
            perror("Memory allocation error.");
            exit(1);
        }
        for(i = 0; i < NPREF; i++){
            sp->pref[i] = prefix[i];
        }
        sp->suf = NULL;
        sp->next = statetab[h];
        statetab[h] = sp;
    }
    return sp;
}

unsigned int myhash(char *s[NPREF]){
    unsigned int h = 0;
    unsigned char *p;
    for(int i=0; i<NPREF; i++){
        for(p = (unsigned char *)s[i]; *p !='\0'; p++){
            h = MULTIPLIER*h + *p;
        }
    }
    return h % NHASH;
}

char *mystrdup(char *s){
    char *t;
    t = (char *)malloc(strlen(s)+1);
    if(t == NULL){
        perror("Error: strdup failed.");
        exit(1);
    }
    strcpy(t, s);
    return t;
}
