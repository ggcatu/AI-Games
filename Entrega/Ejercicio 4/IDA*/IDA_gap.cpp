#include <utility> /*Seria mas fino importar solo pair*/
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <vector>
#include <climits>
#include <iostream>
#include <time.h>
using namespace std;

state_t state;
state_t aux_state;
vector<const char*> path;
double t = -1;
double childCount = 0;

void printing(int len){
    printf("X, A*, gap, pancake28, \"");
    print_state(stdout, &state);
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

unsigned int h_gap_28p(state_t state){
	unsigned int gaps = 0;
	for (int i = 0; i < 27; i++){
		if (abs(state.vars[i] - state.vars[i+1]) > 1) {
			gaps++;
		}
	}
	if (state.vars[27] != 27) {
		gaps++;
	}
	return gaps;
}

pair<bool,unsigned int> f_bounded_dfs_visit(unsigned int bound, unsigned int g){
	/* base-case */ 
	pair<bool,unsigned> result;
	unsigned int f = g + h_gap_28p(state);
	if (f > bound) {
		result.first = false;
		result.second = f;
		return result;
	} 
	if (is_goal(&state)){
		result.first = true;
		result.second = g;
		return result;
	}
	unsigned int t = UINT_MAX;
	unsigned int cost;
	ruleid_iterator_t iter;
	int ruleid;
	init_fwd_iter(&iter, &state);
	while ((ruleid = next_ruleid(&iter)) >= 0){
		cost = g + get_fwd_rule_cost(ruleid);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
		++childCount;
		if (h_gap_28p(state) < UINT_MAX) {
			path.push_back(get_fwd_rule_label(ruleid));
			result = f_bounded_dfs_visit(bound, cost);
			if (result.first) return result;
			t = min(t, result.second);
			path.pop_back();
		}
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
	}
	result.first = false;
	result.second = t;
	return result;
}

unsigned int ida_search(){
	unsigned int bound = h_gap_28p(state);
	while (true) {
		childCount = 0;
		pair<bool,unsigned int> p = f_bounded_dfs_visit(bound, 0);
		if (p.first) return p.second;
		bound = p.second;
	}
}

int main(){
	// VARIABLES FOR INPUT
	unsigned int costo;
	signal(SIGTERM, sig_handler);
    char str[256];
    ssize_t nchars; 
    // Leer un estado de la entrada estandar.
    //printf("Please enter a state followed by ENTER: ");
    if( fgets(str, sizeof str, stdin) == NULL ) {
        printf("Error: empty input line.\n");
        return 0; 
    }

    // Convertir el string en un estado state_t.
    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: invalid state entered.\n");
        return 0; 
    }

    // Algoritmo de busqueda IDA*
    clock_t start = clock(), diff;
    try {
    	costo = ida_search();
    }
    catch (const std::bad_alloc&) {
      printing(-1);
      exit(0);
    }
    diff = clock() - start; 
    t = (double)diff / CLOCKS_PER_SEC;
    printing(costo);
    

    // Impresion de resultados de la busqueda
    //printf("The final state is: ");
    //print_state(stdout, &state);
    //printf("\n");
    //printf("El numero de hijos explorados es: %d\n", childCount);
    //printf("El costo de la solucion es: %d\n", costo);
    //unsigned int size = path.size();
    //cout << "Acciones realizadas: ";
  	//for (unsigned i=0; i<size; i++) cout << ' ' << path[i];
  	//cout << '\n';
    return 1;
}