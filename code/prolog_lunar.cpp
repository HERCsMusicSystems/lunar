///////////////////////////////////////////////////////////////////////////////////
//                       Copyright (C) 2014 Robert P. Wolf                       //
//                                                                               //
// Permission is hereby granted, free of charge, to any person obtaining a copy  //
// of this software and associated documentation files (the "Software"), to deal //
// in the Software without restriction, including without limitation the rights  //
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     //
// copies of the Software, and to permit persons to whom the Software is         //
// furnished to do so, subject to the following conditions:                      //
//                                                                               //
// The above copyright notice and this permission notice shall be included in    //
// all copies or substantial portions of the Software.                           //
//                                                                               //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   //
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, //
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     //
// THE SOFTWARE.                                                                 //
///////////////////////////////////////////////////////////////////////////////////

#include "prolog_lunar.h"

void PrologLunarServiceClass :: init (PrologRoot * root) {this -> root = root;}
void PrologLunarServiceClass :: init_directory (PrologDirectory * directory) {this -> directory = directory;}

PrologNativeCode * PrologLunarServiceClass :: getNativeCode (char * name) {
	if (strcmp (name, "small_keyboard") == 0) return new keyboard_class (root, 1);
	if (strcmp (name, "keyboard") == 0) return new keyboard_class (root, 2);
	if (strcmp (name, "big_keyboard") == 0) return new keyboard_class (root, 3);
	if (strcmp (name, "oscilloscope") == 0) return new oscilloscope_class ();
	if (strcmp (name, "moonbase") == 0) return new moonbase_class ();
	return 0;
}
