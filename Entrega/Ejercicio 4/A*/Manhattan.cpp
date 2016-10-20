#include <iostream>
#include <map>
#include <vector>
#include <cmath>
#include <string>

using namespace std;


map<int,pair<int,int> > mapita;
int estado [16] = {4,1,2,3,0,5,6,7,8,9,10,11,12,13,14,15};

void llenarMapa(){
	mapita[0] = make_pair(0,0);
	mapita[1] = make_pair(0,1);
	mapita[2] = make_pair(0,2);
	mapita[3] = make_pair(0,3);
	mapita[4] = make_pair(1,0);
	mapita[5] = make_pair(1,1);
	mapita[6] = make_pair(1,2);
	mapita[7] = make_pair(1,3);
	mapita[8] = make_pair(2,0);
	mapita[9] = make_pair(2,1);
	mapita[10] = make_pair(2,2);
	mapita[11] = make_pair(2,3);
	mapita[12] = make_pair(3,0);
	mapita[13] = make_pair(3,1);
	mapita[14] = make_pair(3,2);
	mapita[15] = make_pair(3,3);
}

int manh(){
	int total,fila,columna;
	total = 0;
	pair<int,int> pareja;
	for(int i = 0; i < 16; i++){
		fila = i/4;
		columna = i%4;
		if(estado[i] != 0) continue;
		pareja = mapita[estado[i]];
		total += (abs(fila-pareja.first) + abs(columna-pareja.second));
	}
	return total;
}

int main(){
	llenarMapa();
	cout << manh() << endl;

}
