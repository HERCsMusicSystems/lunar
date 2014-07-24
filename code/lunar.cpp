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

//////////////////////////////////////////////////////////////////////
// This file was created on Tuesday, 15th July 2014 at 15:55:28 PM. //
//////////////////////////////////////////////////////////////////////

#include "lunar.h"
#include <stdio.h>
#include <string.h>

int orbiter_count = 0;

orbiter_core :: orbiter_core (void) {
	root = 0;
	pthread_mutex_init (& main_mutex, 0);
	pthread_mutex_init (& maintenance_mutex, 0);
}

orbiter_core :: ~ orbiter_core (void) {
	pthread_mutex_destroy (& main_mutex);
	pthread_mutex_destroy (& maintenance_mutex);
}

void orbiter_core :: move_modules (void) {
	if (root == 0) return;
	root -> move ();
	orbiter * orp = root -> next;
	while (orp != root) {orp -> move (); orp = orp -> next;}
}

void orbiter_core :: propagate_signals (void) {
	if (root == 0) return;
	root -> propagate_signals ();
}

int orbiter :: numberOfInputs (void) {return 0;}
int orbiter :: numberOfOutputs (void) {return 1;}
char * orbiter :: inputName (int ind) {return "VOID";}
char * orbiter :: outputName (int ind) {return "SIGNAL";}
int orbiter :: inputIndex (char * name) {for (int ind = 0; ind < numberOfInputs (); ind++) {if (strcmp (name, inputName (ind)) == 0) return ind;} return -1;}
int orbiter :: outputIndex (char * name) {for (int ind = 0; ind < numberOfOutputs (); ind++) {if (strcmp (name, outputName (ind)) == 0) return ind;} return -1;}
double orbiter :: output (int ind) {return 0.0;}
double orbiter :: output (char * name) {int ind = outputIndex (name); if (ind < 0 || ind >= numberOfOutputs ()) return 0.0; return output (ind);}
void orbiter :: input (double signal, int ind) {}
void orbiter :: input (double signal, char * name) {int ind = inputIndex (name); if (ind < 0 || ind >= numberOfInputs ()) return; input (signal, ind);}
double * orbiter :: inputAddress (int ind) {return 0;}
double * orbiter :: outputAddress (int ind) {return ind == 0 ? & signal : 0;}
void orbiter :: move (void) {}
void orbiter :: hold (void) {
	pthread_mutex_lock (& core -> main_mutex);
	references++;
	pthread_mutex_unlock (& core -> main_mutex);
}
void orbiter :: release (void) {
	pthread_mutex_t * mt = & core -> main_mutex;
	pthread_mutex_lock (mt);
	if (-- references > 0) {pthread_mutex_unlock (mt); return;}
	dock_pointer * connectors_to_delete = connectors;
	int number_to_delete = number_of_connections;
	delete this;
	pthread_mutex_unlock (mt);
	if (connectors_to_delete == 0) return;
	for (int ind = 0; ind < number_to_delete; ind++) {
		if (connectors_to_delete [ind] != 0)
			delete connectors_to_delete [ind];
	}
	delete [] connectors_to_delete;
}
bool orbiter :: connect (int destination, orbiter * source, int port) {
	if (destination < 0) return false;
	if (destination >= number_of_connections) return false;
	if (source == 0) return false;
	if (port < 0) return false;
	if (port >= source -> numberOfOutputs ()) return false;
	double * location = source -> outputAddress (port);
	if (location == 0) return false;
	source -> hold ();
	connectors [destination] = new dock (source, port, connection_addresses [destination], connectors [destination]);
	return true;
}
bool orbiter :: connect (char * destination, orbiter * source, char * port) {return source == 0 ? false : connect (inputIndex (destination), source, source -> inputIndex (port));}

void orbiter :: propagate_signals (void) {
}

void orbiter :: activate (void) {
	number_of_connections = numberOfInputs ();
	connectors = number_of_connections > 0 ? new dock_pointer [number_of_connections] : 0;
	connection_addresses = number_of_connections > 0 ? new connection_address [number_of_connections] : 0; 
	for (int ind = 0; ind < number_of_connections; ind++) {connectors [ind] = 0; connection_addresses [ind] = inputAddress (ind);}
	pthread_mutex_lock (& core -> main_mutex);
	if (core -> root == 0) {
		core -> root = this;
		next = previous = this;
	} else {
		next = core -> root;
		previous = core -> root -> previous;
		next -> previous = this;
		previous -> next = this;
		core -> root = this;
	}
	pthread_mutex_unlock (& core -> main_mutex);
	printf ("ORBITER ACTIVATED [%i]\n", orbiter_count);
}

orbiter :: orbiter (orbiter_core * core) {
	references = 0;
	this -> core = core;
	orbiter_count++;
	number_of_connections = 0;
	connectors = 0;
	connection_addresses = 0;
	previous = next = 0;
}

orbiter :: ~ orbiter (void) {
	if (next == this || previous == this) core -> root = 0;
	else {
		next -> previous = previous;
		previous -> next = next;
		if (core -> root == this) core -> root = next;
	}
	//if (connectors != 0) {
	//	for (int ind = 0; ind < number_of_connections; ind++) delete connectors [ind];
	//	delete [] connectors;
	//}
	if (connection_addresses != 0) delete [] connection_addresses;
	orbiter_count--;
	printf ("ORBITER DESTROYED [%i]\n", orbiter_count);
}

dock :: dock (orbiter * source, int port, double * destination, dock * next) {
	this -> source = source;
	this -> port = port;
	this -> destination = destination;
	this -> next = next;
}

dock :: ~ dock (void) {
	source -> release ();
	if (next != 0) delete next;
}