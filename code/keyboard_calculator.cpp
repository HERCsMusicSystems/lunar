///////////////////////////////////////
// Copyright (C) 2004 Robert P. Wolf //
//        ALL RIGHTS RESERVED        //
///////////////////////////////////////

#include "keyboard_calculator.h"

keyboard_calculator :: keyboard_calculator (int x, int y) {
	kx0 = x;
	ky0 = y;
	first = 17;
	whites = 54;
	set_keyboard_layout_y (66, 44);
	set_keyboard_layout_x (11, 1, 2, 3, 4, 5);
}

int keyboard_calculator :: get (int x, int y) {
	x -= kx0;
	y -= ky0;

	if (x < 0 || x > kx100) return -1;
	x += shift;
	int oct = x / c_key;
	oct *= 12;
	oct += first_c;
	x %= c_key;
	if (y <= white_length && y > black_length) {
		if (x < d_key) return 0 + oct;
		if (x < e_key) return 2 + oct;
		if (x < f_key) return 4 + oct;
		if (x < g_key) return 5 + oct;
		if (x < a_key) return 7 + oct;
		if (x < h_key) return 9 + oct;
		return 11 + oct;
	}
	if (y >= 0 && y <= black_length) {
		if (x < cis_key) return 0 + oct;
		if (x < des_key) return 1 + oct;
		if (x < dis_key) return 2 + oct;
		if (x < es_key) return 3 + oct;
		if (x < f_key) return 4 + oct;
		if (x < fis_key) return 5 + oct;
		if (x < ges_key) return 6 + oct;
		if (x < gis_key) return 7 + oct;
		if (x < as_key) return 8 + oct;
		if (x < ais_key) return 9 + oct;
		if (x < b_key) return 10 + oct;
		return 11 + oct;
	}
	return -1;
}

//void keyboard_calculator :: set_first_f (int f) {first_f = f * 12 + 5;}
void keyboard_calculator :: set_ambitus (int first, int whites) {
	this -> first = first;
	this -> whites = whites;
	kx100 = whites * d_key - 1;
	first_c = first / 12;
	first_c *= 12;
	first %= 12;
	switch (first) {
	case 0: case 1: shift = 0; break;
	case 2: case 3: shift = 1; break;
	case 4: shift = 2; break;
	case 5: case 6: shift = 3; break;
	case 7: case 8: shift = 4; break;
	case 9: case 10: shift = 5; break;
	default: shift = 6; break;
	}
	shift *= d_key;
}
void keyboard_calculator :: set_keyboard_layout_x (int white, int b1, int b2, int b3, int b4, int b5) {
	d_key = white;
	e_key = d_key + white;
	f_key = e_key + white;
	g_key = f_key + white;
	a_key = g_key + white;
	h_key = a_key + white;
	c_key = h_key + white;

	cis_key = d_key - b4;
	des_key = d_key + b2;
	dis_key = e_key - b2;
	es_key = e_key + b4;
	fis_key = g_key - b5;
	ges_key = g_key + b1;
	gis_key = a_key - b3;
	as_key = a_key + b3;
	ais_key = h_key - b1;
	b_key = h_key + b5;

	set_ambitus (first, whites);
}
void keyboard_calculator :: set_keyboard_layout_y (int white, int black) {
	white_length = white;
	black_length = black;
}

