#include <utility> /*Seria mas fino importar solo pair*/
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <assert.h>
#include <sys/time.h>
#include <vector>
#include <climits>
using namespace std;

state_t initial;
state_t state;
state_t aux_state;
double tiempo = -1;
double childCount = 0;
int h0;
abstraction_t *abst1;
abstraction_t *abst2;
abstraction_t *abst3;
state_map_t *pdb1;
state_map_t *pdb3;
state_map_t *pdb2;

void printing(int len) {
    printf("X, IDA*, PDB5+5+5, 15puzzle, \"");
    print_state(stdout, &initial);
    if (len < 0) {
        printf("\", na, %d, na, na, na\n", h0);
    } else {
        printf("\", %d, %d, %.0lf, %f, %.5e\n", len, h0, childCount, tiempo, 
        	   childCount/tiempo);
    }
}

void sig_handler(int SIG){
    printing(-1);
    exit(0);
}

unsigned int h_pdb(state_t state){
	int total = 0;
    abstract_state(abst1, &state, &aux_state);
    total += *state_map_get(pdb1, &aux_state);
    abstract_state(abst2, &state, &aux_state);
    total += *state_map_get(pdb2, &aux_state);
    abstract_state(abst3, &state, &aux_state);
    total += *state_map_get(pdb3, &aux_state);
    return total;
}

pair<bool,unsigned int> f_bounded_dfs_visit(unsigned int bound, unsigned int g, 
											int history){
	pair<bool,unsigned> result;
	unsigned int f = g + h_pdb(state);
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
	ruleid_iterator_t iterb;
	int ruleid;
	int childHistory;
	init_fwd_iter(&iter, &state);
	init_bwd_iter(&iterb, &state);
	while ((ruleid = next_ruleid(&iter)) >= 0){
		if (fwd_rule_valid_for_history(history, ruleid) == 0) continue;
		apply_fwd_rule(ruleid, &state, &aux_state);
		cost = g + get_fwd_rule_cost(ruleid);
		childHistory = next_fwd_history(history, ruleid);
		copy_state(&state, &aux_state);
		++childCount;
		result = f_bounded_dfs_visit(bound, cost, childHistory);
		if (result.first) return result;
		t = min(t, result.second);
		apply_bwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
	}
	result.first = false;
	result.second = t;
	return result;
}

unsigned int ida_search(){
	unsigned int bound = h_pdb(state);
	int history;
	childCount = 0;
	while (true) {
		history = init_history;
		pair<bool,unsigned int> p = f_bounded_dfs_visit(bound, 0, history);
		if (p.first) return p.second;
		bound = p.second;
	}
}

int main(){
	// VARIABLES FOR INPUT
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

    unsigned int costo;
    FILE *pdb_file;

    abst1 = read_abstraction_from_file("15PuzzleAbs1.abst");
    pdb_file = fopen("15PuzzleAbs1.pdb", "r");
    pdb1 = read_state_map(pdb_file);
    fclose(pdb_file);

    abst2 = read_abstraction_from_file("15PuzzleAbs2.abst");
    pdb_file = fopen("15PuzzleAbs2.pdb", "r");
    pdb2 = read_state_map(pdb_file);
    fclose(pdb_file);

    abst3 = read_abstraction_from_file("15PuzzleAbs3.abst");
    pdb_file = fopen("15PuzzleAbs3.pdb", "r");
    pdb3 = read_state_map(pdb_file);
    fclose(pdb_file);
  
    // Algoritmo de busqueda IDA*
    copy_state(&initial, &state);
    h0 = h_pdb(initial);
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