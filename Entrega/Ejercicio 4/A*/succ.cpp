/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

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
state_t stateI;

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

void printing(int len){
    printf("X, A*, gap, pancake28, \"");
    print_state(stdout, &stateI);
    if(len < 0){
        printf("\", na, na, na, na\n");
    } else {
        printf("\", %d, %.0lf, %f, %.5e\n", len, childCount, t, childCount/t);
    }
}

void sig_handler(int SIG){
    printing(-1);
    exit(0);
}


class Node{
    state_t state;
    Node* padre;
    int ruleid;
    int g;
    int fh;

    public:

        Node(){};

        Node(state_t est, Node* p, int id, int g, int h): state(est),padre(p),ruleid(id),g(g),fh(h){};

        Node make_node(state_t s, int id, int g, int h){
            return Node(s,this,id,g + get_fwd_rule_cost(id), h);
        }

        Node make_root_node(state_t s){
            return Node(s,NULL,-1,0,0);
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
            ruleid_iterator_t iter; // ruleid_terator_t is the type defined by the PSVN API successor/predecessor iterators.
            Node actual;
            Node hijo;
            Node ax;
            int ruleid; // an iterator returns a number identifying a rule
            childCount = 0;
            int g;
            aux = this->state;
            open.push(*this);
            while(!open.empty()){
                actual = open.top();
                open.pop();
                state = actual.state;
                g = actual.g;
                //printf("Hijos: %d\n",childCount);
                //printf("The state is: ");
                //print_state(stdout, &state);
                //printf("\n");
                //printf("Su heuristica es: %d\n", actual.fh);
                if(is_goal(&state)){
                    return g;
                }
                const int *best_dist = state_map_get(costos, &state);
                if( !best_dist || *best_dist > g ){
                    state_map_add(costos, &state, g);
                    init_fwd_iter(&iter, &state);
                    while( (ruleid = next_ruleid(&iter)) >= 0 ) {
                        apply_fwd_rule(ruleid, &state, &child);
                        childCount++;
                        hijo = actual.make_node(child,ruleid,g,0);
                        hijo.fh = hijo.heuristica(tipo);
                        open.push(hijo);
                    }
                }        
            }
            return -1;
        }   
};


int main(int argc, char **argv ) {
    // VARIABLES FOR INPUT
    signal(SIGTERM, sig_handler);
    char str[MAX_LINE_LENGTH + 1];
    ssize_t nchars; 
    int resp = 0;
    // VARIABLES FOR ITERATING THROUGH state's SUCCESSORS

    // READ A LINE OF INPUT FROM stdin
    //printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // CONVERT THE STRING TO A STATE
    nchars = read_state(str, &stateI);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    //printf("The state you entered is: ");
    //print_state(stdout, &stateI);
    //printf("\n");

    Node raiz;
    raiz = raiz.make_root_node(stateI);
    clock_t start = clock(), diff;
    try {
      resp = raiz.aestrella(0);
    }
    catch (const std::bad_alloc&) {
      printing(-1);
      exit(0);
    }
    diff = clock() - start; 
    t = (double) 
    diff / CLOCKS_PER_SEC;
    printing(resp);
    return 0;
}

