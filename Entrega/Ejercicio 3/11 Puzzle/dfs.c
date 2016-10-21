/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <signal.h>
#include <time.h>

#define  MAX_LINE_LENGTH 999 

bool done = 0;
double nodes = 0;
double t = -1;
state_t inicial;

void printing(int len){
    printf("X, dfid, 11puzzle, \"");
    print_state(stdout, &inicial);
    if(len < 0){
        printf("\", na, na, na, na\n");
    } else {
        printf("\", %d, %.0lf, %f, %.5e\n", len, nodes, t, nodes/t);
    }
}


void dfs(state_t * nodo, int deep, int history){
    nodes++;
    if (deep < 0 && !done)
        return;
    if (is_goal(nodo)){
        done = 1;
        return;
    }
    int childHistory;
    state_t child;
    ruleid_iterator_t iter;
    int ruleid; 
    int childCount;
    init_fwd_iter(&iter, nodo);
    while( (ruleid = next_ruleid(&iter)) >= 0 ) {
        if (fwd_rule_valid_for_history(history, ruleid) == 0) continue;
        childHistory = next_fwd_history(history, ruleid);
        apply_fwd_rule(ruleid, nodo, &child);
        dfs(&child, deep-1, childHistory);
        if (done){
            break;
        }
    }
    return;
}

int iterativedfs(state_t * nodo){
    int i = 0;
    int history;
    while(!done){
        history = init_history;
        dfs(nodo, i, history);
        if(done){
            return i;
        }
        i++;
    }
    return -1;
}

void sig_handler(int SIG){
    printing(-1);
    exit(0);
}

int main(int argc, char **argv ) {
    signal(SIGTERM, sig_handler);
    int largo;
    ssize_t nchars; 
    char str[MAX_LINE_LENGTH + 1];
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }
    nchars = read_state(str, &inicial);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }
    clock_t start = clock(), diff;
    largo = iterativedfs(&inicial);
    diff = clock() - start;
    t = (double) 
    diff / CLOCKS_PER_SEC;
    printing(largo);
    return 0;
}

