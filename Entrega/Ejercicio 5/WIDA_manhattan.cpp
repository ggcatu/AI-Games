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
float peso;
double tiempo = -1;
double childCount = 0;
int h0;

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

unsigned* mtable[16] = {mtable0,mtable1,mtable2,mtable3,mtable4,mtable5,
						mtable6,mtable7,mtable8,mtable9,mtable10,mtable11,
						mtable12,mtable13,mtable14,mtable15};

void printing(int len) {
    printf("X, IDA*, manhattan, %f, 15Puzzle, \"", peso);
    print_state(stdout, &initial);
    if (len < 0) {
        printf("\", na, %d, na, na, na\n", h0);
    } else {
        printf("\", %d, %d, %.0lf, %f, %.5e\n",len, h0, childCount, tiempo, 
        	   childCount/tiempo);
    }
}

void sig_handler(int SIG){
    printing(-1);
    exit(0);
}

unsigned int h_manhattan(state_t state){
	int total = 0;
    for (int i = 0; i < 16; i++) {
        total += mtable[state.vars[i]][i];
    }
    return total;
}

pair<bool,unsigned int> f_bounded_dfs_visit(unsigned int bound, unsigned int g, 
									 float peso, int history) {
	pair<bool,unsigned int> result;
	unsigned int f = g + (int)(peso * h_manhattan(state));
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
		result = f_bounded_dfs_visit(bound, cost, peso, childHistory);
		if (result.first) return result;
		t = min(t, result.second);
		apply_bwd_rule(ruleid, &state, &aux_state);
		copy_state(&state, &aux_state);
	}
	result.first = false;
	result.second = t;
	return result;
}

int wida_search(float peso){
	unsigned int bound = peso * h_manhattan(state);
	int history;
	childCount = 0;
	while (true) {
		history = init_history;
		pair<bool,float> p = f_bounded_dfs_visit(bound, 0, peso, history);
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
    if (fgets(str, sizeof str, stdin) == NULL) {
        printf("Error: linea de entrada vacia.\n");
        return 0; 
    }

    nchars = read_state(str, &state);
    if( nchars <= 0 ) {
        printf("Error: estado de entrada invalido.\n");
        return 0; 
    }

    // Algoritmo de busqueda IDA*
    copy_state(&initial, &state);
    h0 = peso * h_manhattan(initial);
    clock_t start = clock(), diff;
    try {
    	costo = wida_search(peso);
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