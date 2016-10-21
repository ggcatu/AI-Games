#include <utility> /*Seria mas fino importar solo pair*/
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <cfloat>
#include <time.h>
using namespace std;

state_t initial;
state_t state;
state_t aux_state;
float peso;
double tiempo = -1;
double childCount = 0;
int h0;

void printing(int len) {
    printf("X, WIDA*, gap, %d, pancakes28, \"", peso);
    print_state(stdout, &initial);
    if (len < 0) {
        printf("\", %d, na, na, na, na\n", h0);
    } else {
        printf("\", %d, %d, %.0lf, %f, %.5e\n", len, h0, childCount, tiempo, 
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

pair<bool,float> f_bounded_dfs_visit(float bound, float g, float peso, int history) {
	pair<bool,float> result;
	float f = g + (peso * (float)h_gap_28p(state));
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
	float t = FLT_MAX;
	float cost;
	ruleid_iterator_t iter;
	int ruleid;
	int childHistory;
	init_fwd_iter(&iter, &state);
	while ((ruleid = next_ruleid(&iter)) >= 0){
		if (fwd_rule_valid_for_history(history, ruleid) == 0) continue;
		cost = g + (float)get_fwd_rule_cost(ruleid);
		childHistory = next_fwd_history(history, ruleid);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
		++childCount;
		result = f_bounded_dfs_visit(bound, cost, peso, childHistory);
		if (result.first) return result;
		t = min(t, result.second);
		apply_fwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
	}
	result.first = false;
	result.second = t;
	return result;
}

float wida_search(float peso){
	float bound = peso * (float)h_gap_28p(state);
	int history;
	while (true) {
		childCount = 0;
		history = init_history;
		pair<bool,unsigned int> p = f_bounded_dfs_visit(bound, 0, peso, history);
		if (p.first) return p.second;
		bound = p.second;
	}
}

int main(int argc, char *argv[]){
	if (argc != 2) {
		printf("Uso: %s pesoHeuristica\n", argv[0]);
		return 0;
	}
	peso = atof(argv[1]);
	if (peso < 1) {
		printf("El peso de la heuristica no puede ser negativo, ni menor a 1.\n");
		return 0;
	}	
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
    	costo = (int)wida_search(peso);
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