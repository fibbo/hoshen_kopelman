#include "rng.h"

// congruential random number generator (rng) with parameters
// int c, int p
// and seed
// int x
void rng::set_values(int ci, int pi, int xi) {
	c = ci;
	p = pi;
	x = xi;
}

// returns new random number
// double z
double rng::get_rnd() {

	x = (int)(c * x) % p;
	return (double) x / (double) p;
}

// returns seed if get_rnd() never called or
// z_(n-1) 
float rng::get_nMinusOne() {
	return (double)x / (double)p;
}

// retruns parameter c
int rng::getValueC(){
	return c;
}

//returns paramter p
int rng::getValueP(){
	return p;
}

//returns x
int rng::getValueX(){
	return x;
}