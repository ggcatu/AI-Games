#include <utility> /*Seria mas fino importar solo pair*/
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <climits>
#include <time.h>
using namespace std;

state_t initial;
state_t state;
state_t aux_state;
double tiempo = -1;
double childCount = 0;
int h0;

void printing(int len) {
    printf("X, IDA*, gap, pancakes28, \"");
    print_state(stdout, &initial);
    if (len < 0) {
        printf("\", %d, na, na, na, na\n", h0);
    } else {
        printf("\", %d, %d, %.0lf, %f, %.5e\n", h0, len, childCount, tiempo, 
        	   childCount/tiempo);
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

pair<bool,unsigned int> f_bounded_dfs_visit(unsigned int bound, unsigned int g, 
											int history) {
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
	int childHistory;
	init_fwd_iter(&iter, &state);
	while ((ruleid = next_ruleid(&iter)) >= 0){
		if (fwd_rule_valid_for_history(history, ruleid) == 0) {
			printf("Hola boludo\n");
			continue;
		}
		cost = g + get_fwd_rule_cost(ruleid);
		printf("Ruleid: %d\n", ruleid);
		childHistory = next_fwd_history(history, ruleid);
		printf("Child history: %d\n", childHistory);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
		++childCount;
		result = f_bounded_dfs_visit(bound, cost, childHistory);
		if (result.first) return result;
		t = min(t, result.second);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
	}
	result.first = false;
	result.second = t;
	return result;
}

unsigned int ida_search(){
	unsigned int bound = h_gap_28p(state);
	int history;
	while (true) {
		childCount = 0;
		history = init_history;
		pair<bool,unsigned int> p = f_bounded_dfs_visit(bound, 0, history);
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
    if (fgets(str, sizeof str, stdin) == NULL) {
        printf("Error: linea de entrada vacia.\n");
        return 0; 
    }

    // Convertir el string en un estado state_t.
    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: estado de entrada invalido.\n");
        return 0; 
    }

    // Algoritmo de busqueda IDA*
    copy_state(&initial, &state);
    h0 = h_gap_28p(initial);
    clock_t start = clock(), diff;
    try {
    	costo = ida_search();
    }
    catch (const std::bad_alloc&) {
      printing(-1);
      exit(0);
    }
    diff = clock() - start; 
    tiempo = (double)diff / CLOCKS_PER_SEC;
    printing(costo);
    return 1;
}