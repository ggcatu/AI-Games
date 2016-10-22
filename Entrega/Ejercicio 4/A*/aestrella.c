#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <vector>
#include <iostream>
#include <signal.h>
#include <queue>
#include <map>
#include <cmath>
#include <time.h>

#define  MAX_LINE_LENGTH 999 

using namespace std;
double childCount = 0;
double t = -1;
int h0;
state_t stateI;
state_t abst_state1, abst_state2, abst_state3;
abstraction_t *abst1;
abstraction_t *abst2;
abstraction_t *abst3;
state_map_t *pdb1;
state_map_t *pdb3;
state_map_t *pdb2;

unsigned mtable0[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned mtable1[16] = {1,0,1,2,2,1,2,3,3,2,3,4,4,3,4,5};
unsigned mtable2[16] = {2,1,0,1,3,2,1,2,4,3,2,3,5,4,3,4};
unsigned mtable3[16] = {3,2,1,0,4,3,2,1,5,4,3,2,6,5,4,3};
unsigned mtable4[16] = {1,2,3,4,0,1,2,3,1,2,3,4,2,3,4,5};
unsigned mtable5[16] = {2,1,2,3,1,0,1,2,2,1,2,3,3,2,3,4};
unsigned mtable6[16] = {3,2,1,2,2,1,0,1,3,2,1,2,4,3,2,3};
unsigned mtable7[16] = {4,3,2,1,3,2,1,0,4,3,2,1,5,4,3,2};
unsigned mtable8[16] = {2,3,4,5,1,2,3,4,0,1,2,3,1,2,3,4};
unsigned mtable9[16] = {3,2,3,4,2,1,2,3,1,0,1,2,2,1,2,3};
unsigned mtable10[16] = {4,3,2,3,3,2,1,2,2,1,0,1,3,2,1,2};
unsigned mtable11[16] = {5,4,3,2,4,3,2,1,3,2,1,0,4,3,2,1};
unsigned mtable12[16] = {3,4,5,6,2,3,4,5,1,2,3,4,0,1,2,3};
unsigned mtable13[16] = {4,3,4,5,3,2,3,4,2,1,2,3,1,0,1,2};
unsigned mtable14[16] = {5,4,3,4,4,3,2,3,3,2,1,2,2,1,0,1};
unsigned mtable15[16] = {6,5,4,3,5,4,3,2,4,3,2,1,3,2,1,0};

unsigned* mtable[16] = {mtable0,mtable1,mtable2,mtable3,mtable4,mtable5,mtable6,mtable7,mtable8,mtable9,mtable10,mtable11,mtable12,mtable13,mtable14,mtable15};

void printing(int len, int h0){
    printf("X, A*, manhattan, 15puzzle, \"");
    print_state(stdout, &stateI);
    if(len < 0){
        printf("\", na, %d, na, na, na\n",h0);
    } else {
        printf("\", %d, %d, %.0lf, %f, %.5e\n",len, h0, childCount, t, childCount/t);
    }
}

void sig_handler(int SIG){
    printing(-1,h0);
    exit(0);
}


class Node{
    state_t state;
    Node* padre;
    int ruleid;
    int g;
    int fh;
    int history;

    public:

        Node(){};

        Node(state_t est, Node* p, int id, int g, int h, int hs): state(est),padre(p),ruleid(id),g(g),fh(h), history(hs){};

        Node make_node(state_t s, int id, int g, int h, int hs){
            return Node(s,this,id,g + get_fwd_rule_cost(id), h,hs);
        }

        Node make_root_node(state_t s){
            return Node(s,NULL,-1,0,0,init_history);
        }

        int gap(){
            int gaps = 0;
            for(int i = 0; i <= 26; i++){
                if(this->state.vars[i] != (this->state.vars[i+1] -1) && this->state.vars[i] != (this->state.vars[i+1] + 1)){
                    gaps++;
                }
            }
            if(this->state.vars[27] != 27){
                gaps++;
            }
            return this->g+gaps;
        }

        int pdb(){
            abstract_state(abst1, &this->state, &abst_state1);
            abstract_state(abst2, &this->state, &abst_state2);
            abstract_state(abst3, &this->state, &abst_state3);

            int *d1 = state_map_get(pdb1, &abst_state1);
            int *d2 = state_map_get(pdb2, &abst_state2);
            int *d3 = state_map_get(pdb3, &abst_state3);

            return this->g+*d1+*d2+*d3;
        }

        int manh(){
            int total = 0;
            for(int i = 0; i < 16; i++){
                total += mtable[this->state.vars[i]][i];
            }
            return this->g+total;
        }

        int heuristica(int tipo){
            if(tipo == 0){
                return manh();
            }
            else if(tipo == 1){
                return pdb();
            }
            else{
                return gap();
            }
        }

        bool operator<(Node const& n1) const{
            return this->fh > n1.fh;    
        }

        int aestrella(int tipo){
            priority_queue<Node, vector<Node> > open;
            state_map_t *costos = new_state_map();
            state_t state, child,aux;
            ruleid_iterator_t iter; 
            Node actual;
            Node hijo;
            Node ax;
            int ruleid;
            childCount = 0;
            int g;
            aux = this->state;
            open.push(*this);
            while(!open.empty()){
                actual = open.top();
                open.pop();
                state = actual.state;
                g = actual.g;
                if(is_goal(&state)){
                    return g;
                }
                const int *best_dist = state_map_get(costos, &state);
                if( !best_dist || *best_dist > g ){
                    state_map_add(costos, &state, g);
                    init_fwd_iter(&iter, &state);
                    while( (ruleid = next_ruleid(&iter)) >= 0 ) {
                        if (fwd_rule_valid_for_history(actual.history, ruleid) == 0) continue;
                        apply_fwd_rule(ruleid, &state, &child);
                        childCount++;
                        hijo = actual.make_node(child,ruleid,g,0,next_fwd_history(actual.history, ruleid));
                        hijo.fh = hijo.heuristica(tipo);
                        open.push(hijo);
                    }
                }        
            }
            return -1;
        }   
};


int main(int argc, char **argv ) {
    int tipo = 0;
    if(argc > 1){
        tipo = atoi(argv[1]);
    }
    signal(SIGTERM, sig_handler);
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    int resp = 0;
    if(tipo == 1){
        abst1 = read_abstraction_from_file("15PuzzleAbs1.abst");
        FILE *pdb_file1 = fopen("15PuzzleAbs1.pdb", "r");
        pdb1 = read_state_map(pdb_file1);
        fclose(pdb_file1);

        abst2 = read_abstraction_from_file("15PuzzleAbs2.abst");
        FILE *pdb_file2 = fopen("15PuzzleAbs2.pdb", "r");
        pdb2 = read_state_map(pdb_file2);
        fclose(pdb_file2);

        abst3 = read_abstraction_from_file("15PuzzleAbs3.abst");
        FILE *pdb_file3 = fopen("15PuzzleAbs3.pdb", "r");
        pdb3 = read_state_map(pdb_file3);
        fclose(pdb_file3);
    } 

    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    nchars = read_state(str, &stateI);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    Node raiz;
    raiz = raiz.make_root_node(stateI);
    h0 = raiz.heuristica(tipo);
    clock_t start = clock(), diff;
    try {
      resp = raiz.aestrella(tipo);
    }
    catch (const std::bad_alloc&) {
      printing(-1,h0);
      exit(0);
    }
    diff = clock() - start; 
    t = (double)diff / CLOCKS_PER_SEC;
    printing(resp,h0);
    return 0;
}

