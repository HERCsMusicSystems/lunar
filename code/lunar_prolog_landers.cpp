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

////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 29th July 2014, 13:48:00 PM. //
////////////////////////////////////////////////////////////////////

#include "lunar_prolog_landers.h"
#include "lunar_landers.h"

orbiter * parameter_block_class :: create_orbiter (PrologElement * parameters) {
	if (parameters -> isEarth ()) return new lunar_parameter_block (core, 0.0);
	if (parameters -> isPair ()) parameters = parameters -> getLeft ();
	if (parameters -> isInteger ()) return new lunar_parameter_block (core, (double) parameters -> getInteger ());
	if (parameters -> isDouble ()) return new lunar_parameter_block (core, parameters -> getDouble ());
	return new lunar_parameter_block (core, 0.0);
}

parameter_block_class :: parameter_block_class (orbiter_core * core) : PrologNativeOrbiterCreator (core) {}
