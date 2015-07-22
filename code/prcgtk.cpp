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
// This file was created on Monday, 14th July 2014 at 8:54:16 AM.   //
// The accompanied Visual Studio project was created at 8:53:00 AM. //
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <gtk/gtk.h>
#include <prolog.h>

#ifdef LINUX_OPERATING_SYSTEM
#include "prolog_linux_console.h"
#include <string.h>
extern char resource_studio;
extern char resource_conductor;
extern char resource_midi;
extern char resource_http;
extern char resource_store;
extern char resource_f1;
extern char resource_help;
extern char resource_record;
extern char resource_neural;
extern char resource_keyboard;
extern char resource_sql;
extern char resource_test;
extern char resource_xml;
extern char resource_distribution;
extern char resource_control;
extern char resource_lunar;
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		char * ret = 0;
		if (strcmp (name, "studio.prc") == 0) ret = & resource_studio;
		if (strcmp (name, "conductor.prc") == 0) ret = & resource_conductor;
		if (strcmp (name, "midi.prc") == 0) ret = & resource_midi;
		if (strcmp (name, "http.prc") == 0) ret = & resource_http;
		if (strcmp (name, "store.prc") == 0) ret = & resource_store;
		if (strcmp (name, "f1.prc") == 0) ret = & resource_f1;
		if (strcmp (name, "help.prc") == 0) ret = & resource_help;
		if (strcmp (name, "record.prc") == 0) ret = & resource_record;
		if (strcmp (name, "neural.prc") == 0) ret = & resource_neural;
		if (strcmp (name, "keyboard.prc") == 0) ret = & resource_keyboard;
		if (strcmp (name, "sql.prc") == 0) ret = & resource_sql;
		if (strcmp (name, "test.prc") == 0) ret = & resource_test;
		if (strcmp (name, "xml.prc") == 0) ret = & resource_xml;
		if (strcmp (name, "distribution.prc") == 0) ret = & resource_distribution;
		if (strcmp (name, "control.prc") == 0) ret = & resource_control;
		if (strcmp (name, "lunar.prc") == 0) ret = & resource_lunar;
		return ret;
	}
} resource_loader;
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
#include "prolog_windows_console.h"
#include "resource.h"
#include "lunar_resource.h"
class resource_loader_class : public PrologResourceLoader {
public:
	char * load (char * name) {
		HRSRC resource = 0;
		if (strcmp (name, "studio.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STUDIO_PRC), RT_RCDATA);
		if (strcmp (name, "conductor.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (CONDUCTOR_PRC), RT_RCDATA);
		if (strcmp (name, "midi.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (MIDI_PRC), RT_RCDATA);
		if (strcmp (name, "http.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HTTP_PRC), RT_RCDATA);
		if (strcmp (name, "store.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (STORE_PRC), RT_RCDATA);
		if (strcmp (name, "f1.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (F1_PRC), RT_RCDATA);
		if (strcmp (name, "help.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (HELP_PRC), RT_RCDATA);
		if (strcmp (name, "record.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (RECORD_PRC), RT_RCDATA);
		if (strcmp (name, "neural.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (NEURAL_PRC), RT_RCDATA);
		if (strcmp (name, "keyboard.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (KEYBOARD_PRC), RT_RCDATA);
		if (strcmp (name, "sql.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (SQL_PRC), RT_RCDATA);
		if (strcmp (name, "test.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (TEST_PRC), RT_RCDATA);
		if (strcmp (name, "xml.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (XML_PRC), RT_RCDATA);
		if (strcmp (name, "distribution.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (DISTRIBUTION_PRC), RT_RCDATA);
		if (strcmp (name, "control.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (CONTROL_PRC), RT_RCDATA);
		if (strcmp (name, "lunar.prc") == 0) resource = FindResource (NULL, MAKEINTRESOURCE (LUNAR_PRC), RT_RCDATA);
		if (! resource) return 0;
		HGLOBAL loader = LoadResource (0, resource);
		if (! loader) return 0;
		return (char *) LockResource (loader);
	}
} resource_loader;
#endif

#include "prolog_conductor.h"
#include "prolog_midi.h"
#include "prolog_neural.h"
#include "prolog_xml.h"
#include "prolog_distribution.h"
#include "prolog_control.h"
#ifdef LINUX_OPERATING_SYSTEM
#include "prolog_http.h"
#endif
#include "prolog_lunar.h"

class service_class_loader_class : public PrologServiceClassLoader {
public:
	PrologServiceClass * load (char * name) {
		if (strcmp (name, "prolog.conductor") == 0) return new PrologConductorServiceClass ();
		if (strcmp (name, "prolog.midi") == 0) return new PrologMidiServiceClass ();
#ifdef LINUX_OPERATING_SYSTEM
		if (strcmp (name, "prolog.http") == 0) return new PrologHttpServiceClass ();
#endif
		if (strcmp (name, "prolog.neural") == 0) return new PrologNeuralServiceClass ();
		if (strcmp (name, "prolog.xml") == 0) return new PrologXMLServiceClass ();
		if (strcmp (name, "prolog.distribution") == 0) return new PrologDistributionServiceClass ();
		if (strcmp (name, "prolog.control") == 0) return new PrologControlServiceClass ();
		if (strcmp (name, "lunar") == 0) return new PrologLunarServiceClass ();
		return NULL;
	}
} service_class_loader;

static int ARGS;
static char * * ARGV;

void * studio_runner (void * parameter) {
	PrologRoot * root = new PrologRoot ();
	root -> get_search_directories_from_environment ("STUDIO_HOME");
	root -> setResourceLoader (& resource_loader);
	root -> setServiceClassLoader (& service_class_loader);
	root -> set_uap32_captions ();
	PROLOG_STRING name; strcpy (name, "");
	for (int ind = 1; ind < ARGS; ind++) {
		if (strlen (name) < 1) strcpy (name, ARGV [ind]);
		else root -> addArg (ARGV [ind]);
	}
	root -> auto_atoms = true;
	#ifdef LINUX_OPERATING_SYSTEM
	PrologCommand * command = new PrologLinuxConsole ();
	#endif
	#ifdef WINDOWS_OPERATING_SYSTEM
	PrologCommand * command = new PrologWindowsConsole ();
	#endif
	root -> insertCommander (command);
	if (strlen (name) == 0) {
		root -> resolution ();
		/*PrologLoader * loader = new PrologLoader (root);
		loader -> load ("studio.prc");
		loader -> load ("lunar.prc");
		delete loader;
		PrologElement * query = root -> pair (root -> earth (), root -> pair (
			root -> pair (root -> atom ("CorePanel"), root -> pair (root -> atom ("Core"), root -> earth ())),
			root -> pair (root -> pair (root -> atom ("command"), root -> earth ()),
			root -> earth ())));
		root -> resolution (query);
		delete query;*/
	} else {
		if (strstr (name, ".prc") == 0 && strstr (name, ".prb") == 0) strcat (name, ".prc");
		root -> resolution (name);
	}
	delete root;
	delete command;
	drop_object_counter ();
	gtk_main_quit ();
	return 0;
}

int main (int args, char * * argv) {
	gtk_init (& args, & argv);
	ARGS = args; ARGV = argv;
	pthread_t thread; pthread_create (& thread, 0, studio_runner, 0); pthread_detach (thread);
	gtk_main ();
	#ifdef WIN32
	getchar ();
	#endif
}

