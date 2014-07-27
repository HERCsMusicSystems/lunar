///////////////////////////////////////
// Copyright (C) 2004 Robert P. Wolf //
//        ALL RIGHTS RESERVED        //
///////////////////////////////////////

#ifndef _KEYBOARD_CALCULATOR
#define _KEYBOARD_CALCULATOR

class keyboard_calculator {
private:
	int first;
	int whites;
	int kx0;
	int ky0;
	int kx100;
	int shift;
	int first_c;
	int white_length;
	int black_length;
	int d_key;
	int e_key;
	int f_key;
	int g_key;
	int a_key;
	int h_key;
	int c_key;
	int cis_key;
	int des_key;
	int dis_key;
	int es_key;
	int fis_key;
	int ges_key;
	int gis_key;
	int as_key;
	int ais_key;
	int b_key;
public:
	keyboard_calculator (int x, int y);
	void set_keyboard_layout_y (int white, int black);
	// 66, 44
	void set_keyboard_layout_x (int white, int b1, int b2, int b3, int b4, int b5);
	// 11, 1, 2, 3, 4, 5
	void set_ambitus (int first, int whites);
	// first = 17, whites = 54
	int get (int x, int y);
};

#endif

