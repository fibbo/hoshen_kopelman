#include "rng.h"
#include "percolation.h"
#include "cluster.h"
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <time.h>

#define samp 1000


using namespace std;

int main ()
{
	int k = 0;
	int clusters[N*N]={0};
	while (k < 1){
		{
			hkalg(0.59, clusters);
		}
		k++;
	}
	ofstream Datas;
	Datas.open("ClusterDistr.txt");
		for (int i=0;i<N*N;i++)
		Datas << clusters[i] << endl;
}

