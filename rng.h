#ifndef _RNG_H
#define _RNG_H
#pragma once
class rng {
	int c, p, x;
public:
	void set_values(int, int, int);
	double get_rnd();
	float get_nMinusOne();
	int getValueC();
	int getValueP();
	int getValueX();
};

#endif