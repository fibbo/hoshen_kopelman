#include <stdlib.h>
#include "rng.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

///////////////////////////////////
//for debug purposes///////////////
bool toggle_lattice = true;
bool toggle_origin_lattice = false;
bool toggle_mass_list = false;
///////////////////////////////////

///////////////////////////////////
//////Mass and Cluster Array///////
int M[N*N] = {0};
int T[N*N] = {0};
///////////////////////////////////



///////////////////////////////////
//////recursive loop for real k////
int get_real_k(int k_inp) {
	if (M[k_inp] >= 0)
		return k_inp;
	else
		return get_real_k(-M[k_inp]);
}
///////////////////////////////////


///////////////////////////////////
////////////alogrithm//////////////
void hkalg(double P, int *clusterSize) {

	//////local variables//////
	int k = 2;
	int kmax = 2;
	srand(time(NULL));
	rng myRNG; //thats the RNG from Exercise1
	myRNG.set_values(313, 81916, rand()%20);
	int lat[N*N];

	//////reset mass, cluster array//////
	for (int i = 0; i<N*N;i++) {
		M[i]=0;
		T[i]=0;
	}


	//////initialize new lattice//////
	for (int i=0; i<N*N; i++) {
		if(myRNG.get_rnd()<P) {
			lat[i]=1; //tree
		}
		else {
			lat[i]=0;  //empty	
		}
	}


	//////detecting first occupied location///////
	int firstSiteLoc;
	bool firstSitefound = false;
	while(!firstSitefound) {
		int i = 0;
		if (lat[i]==1) {
			firstSitefound = true;
			firstSiteLoc = i;
			T[i] = k;
			M[k] = 1;
		}
		i++;
	}


	//////main loop//////////
	for (int i = firstSiteLoc+1; i<N*N; i++) {

		bool bottomRow = false;
		bool leftColumn = false;
		if (i%N == 0) leftColumn = true;
		if (i-N < 0) bottomRow = true;

		if(lat[i]) {

			//check if bottom and left side are empty - if its the bottomrow, bottom site is considered empty
			if (((lat[i-1]==0) || leftColumn) && ((lat[i-N]==0) || bottomRow)) {
				k += 1;
				M[k] = 1;
				T[i] = k;
				kmax = k;
			}

			//check if both are occupied but with different cluster indexes
			else if (((lat[i-1] && !leftColumn) && (lat[i-N] && !bottomRow)) && get_real_k(T[i-1]) != get_real_k(T[i-N])) {
				if (get_real_k(T[i-1]) < get_real_k(T[i-N])) { //choose the smaller clusternumber to use e.g. k1
					T[i] = get_real_k(T[i-1]); //assign parent cluster number
					M[T[i]] += M[get_real_k(M[T[i-N]])] + 1; //assign the proper masses m(k1) = m(k1) + m(k2) + 1
					M[T[i-N]] = -T[i-1]; //link k2 to k1
				}
				else {
					T[i] = get_real_k(T[i-N]); //case if k2<k1
					M[T[i]] += M[get_real_k(M[T[i-1]])] + 1;
					M[T[i-1]] = -T[i-N];
				}

			}
			//check if one or both of them has value (aka is occupied) and if both are occupied make sure they have the same cluster number
			else if (((lat[i-1] && !leftColumn) || (lat[i-N] && !bottomRow)) || (((lat[i-1] && !leftColumn) && (lat[i-N] && !bottomRow)) && get_real_k(T[i-1]) == get_real_k(T[i-N]))) {
				if (lat[i-1] && !leftColumn) { //also make sure you dont connect clusters within rows in case only the 2nd statement of the previous if is true
					T[i] = get_real_k(T[i-1]);
					M[get_real_k(T[i-1])] += 1;
				}
				else if (!bottomRow) {
					T[i] = get_real_k(T[i-N]);;
					M[get_real_k(T[i-N])] += 1;
				}
			}


		}
		else
			T[i] = 0;
	}


	//////if M[j] has mass m, count +1 at clusterSize[m]//////
	for (int j=2; j<=kmax; j++){
		if(M[j] >=0){
			clusterSize[M[j]] += 1;
		}
	}


	//////for debug purpose only//////
	if(toggle_mass_list){
		ofstream myFile2;
		myFile2.open("Masses.txt");
		for (int i=0; i<N*N;i++)
			myFile2 << M[i] << " " << endl;
	}

	if (toggle_lattice){
		ofstream myFile3;
		myFile3.open("lattice.txt");
		for (int n = N-1; n >= 0; n--){
			myFile3 << endl;
			for (int m = n*N; m < (n*N + (N-1)); m++)
				myFile3 << T[m] << "\t";
		}
	}


	if (toggle_origin_lattice){
		ofstream myFile4;
		myFile4.open("Origin_latt.txt");
		for (int n = N-1; n >= 0; n--){
			myFile4 << endl;
			for (int m = n*N; m < (n*N + (N-1)); m++)
				myFile4 << lat[m] << "\t";
		}
	}
}

