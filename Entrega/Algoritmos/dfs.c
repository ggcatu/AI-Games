/* This program reads a state from stdin and prints out its successors.

Copyright (C) 2013 by the PSVN Research Group, University of Alberta
*/

#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <math.h>       //Logaritmo

#define PROFUNDIDAD 7
int profundidad[PROFUNDIDAD];

double factorial(int n)
{
  return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}

double superiore(double bFactor, double problemSpace){
    return log(problemSpace)/log(bFactor);
}

void dfs(state_t * nodo, int deep, int history){
    if (deep >= PROFUNDIDAD)
        return;
    // Declaraciones
    int childHistory;
    state_t child;
    ruleid_iterator_t iter;
    int ruleid; 
    int childCount;
    init_bwd_iter(&iter, nodo);
    profundidad[deep]++;
    while( (ruleid = next_ruleid(&iter)) >= 0 ) {
        if (bwd_rule_valid_for_history(history, ruleid) == 0) continue;
        childHistory = next_bwd_history(history, ruleid);
        apply_bwd_rule(ruleid, nodo, &child);
        dfs(&child, deep+1, childHistory);
    }
    return;
}

int main(int argc, char **argv ) {
    state_t inicial; 
    int inicial_id;
    int history = init_history;
    double superior, bFactor;
    first_goal_state(&inicial, &inicial_id);
    printf("El nodo inicial: ");
    print_state(stdout, &inicial);
    printf("\n");
    dfs(&inicial, 0, history);
    int i;
    for(i = 0; i < PROFUNDIDAD; i++){
       printf("Profundidad %d : %d \n", i, profundidad[i]);
    }
    bFactor = profundidad[PROFUNDIDAD-1] / profundidad[PROFUNDIDAD-2];
    printf("Branching factor: %f\n", bFactor);
    superior = superiore(bFactor, factorial(24));
    printf("Profundidad para nodos mayores a estados: %f \nSuperio|| %f \nEstados|| %f \n", superior, pow(bFactor, superior) ,factorial(24) );
    return 0;
}

