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
using namespace std;

state_t state;
state_t aux_state;
vector<const char*> path;
int childCount;

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
	while ((ruleid = next_ruleid(&iter)) >= 0) {
		cost = g + get_fwd_rule_cost(ruleid);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
		++childCount;
		path.push_back(get_fwd_rule_label(ruleid));
		result = f_bounded_dfs_visit(bound, cost);
		if (result.first) return result;
		t = min(t, result.second);
		path.pop_back();
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
    char str[256];
    ssize_t nchars; 
    // Leer un estado de la entrada estandar.
    printf("Please enter a state followed by ENTER: ");
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
    unsigned int costo = ida_search();

    // Impresion de resultados de la busqueda
    printf("The final state is: ");
    print_state(stdout, &state);
    printf("\n");
    printf("El numero de hijos explorados es: %d\n", childCount);
    printf("El costo de la solucion es: %d\n", costo);
    unsigned int size = path.size();
    cout << "Acciones realizadas: ";
  	for (unsigned i=0; i<size; i++) cout << ' ' << path[i];
  	cout << '\n';
    return 1;
}