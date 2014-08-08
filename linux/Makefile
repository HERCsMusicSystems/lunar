pi = ../../prolog/linux/
pc = ../../prolog/code/prolog/

objects = \
	$(pi)prolog_dll_loader.o\
	$(pi)midi_stream.o $(pi)object_tracker.o $(pi)prolog_atom.o $(pi)prolog_command.o $(pi)prolog_directory.o \
	$(pi)prolog_element.o $(pi)prolog_loader.o $(pi)prolog_match.o \
	$(pi)prolog_native_code.o $(pi)prolog_query.o $(pi)prolog_reader.o $(pi)prolog_resolution.o \
	$(pi)prolog_root.o $(pi)prolog_service_class.o \
	$(pi)prolog_studio.o \
	$(pi)prolog_transport.o \
	$(pi)prolog_variable.o \
	$(pi)prolog_linux_console.o\
	$(pi)prolog_xml.o $(pi)tinyxml.o $(pi)prolog_neural.o $(pi)prolog_distribution.o $(pi)encoder.o\
	$(pi)prolog_midi.o $(pi)prolog_conductor.o $(pi)prolog_http.o $(pi)prolog_control.o $(pi)joystick_linux.o\
	prolog_lunar.o lunar_core.o lunar_prolog_oscilloscope.o lunar_prolog_keyboard.o lunar_prolog_operator.o lunar_operator.o\
	keyboard_calculator.o lunar_prolog_moonbase.o lunar_moonbase.o lunar_prolog_landers.o lunar_landers.o\
	multiplatform_audio.o\
	prolog_resources.o prcgtk.o

prc_objects = \
	$(pc)studio.prc\
	$(pc)conductor.prc\
	$(pc)midi.prc\
	$(pc)http.prc\
	$(pc)store.prc\
	$(pc)f1.prc\
	$(pc)help.prc\
	$(pc)record.prc\
	$(pc)neural.prc\
	$(pc)keyboard.prc\
	$(pc)sql.prc\
	$(pc)test.prc\
	$(pc)xml.prc\
	$(pc)distribution.prc\
	$(pc)control.prc\
	../code/lunar.prc

prolog_resources.cpp: $(prc_objects) $(pi)resource_maker.exe
	$(pi)resource_maker.exe prolog_resources.cpp $(prc_objects)

prolog_resources.o: prolog_resources.cpp
	g++ prolog_resources.cpp -c -o prolog_resources.o

multiplatform_audio.o: ../code/multiplatform_audio.cpp ../code/multiplatform_audio.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/multiplatform_audio.cpp -c -o multiplatform_audio.o

keyboard_calculator.o: ../code/keyboard_calculator.cpp ../code/keyboard_calculator.h
	g++ -Wno-write-strings ../code/keyboard_calculator.cpp -c -o keyboard_calculator.o

lunar_core.o: ../code/lunar.h ../code/lunar.cpp
	g++ -Wno-write-strings ../code/lunar.cpp -c -o lunar_core.o

lunar_prolog_oscilloscope.o: ../code/lunar_prolog_oscilloscope.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_oscilloscope.cpp -c -o lunar_prolog_oscilloscope.o `pkg-config --cflags gtk+-2.0`

lunar_moonbase.o: ../code/lunar_moonbase.cpp ../code/lunar_moonbase.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_moonbase.cpp -c -o lunar_moonbase.o

lunar_prolog_operator.o: ../code/lunar_prolog_operator.cpp ../code/prolog_lunar.h ../code/lunar_operator.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_operator.cpp -c -o lunar_prolog_operator.o

lunar_operator.o: ../code/lunar_operator.cpp ../code/lunar_operator.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_operator.cpp -c -o lunar_operator.o

lunar_landers.o: ../code/lunar_landers.cpp ../code/lunar_landers.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_landers.cpp -c -o lunar_landers.o

lunar_prolog_moonbase.o: ../code/lunar_prolog_moonbase.cpp ../code/lunar_moonbase.h ../code/prolog_lunar.h ../code/multiplatform_audio.h\
	$(pc)prolog.h ../code/lunar.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_moonbase.cpp -c -o lunar_prolog_moonbase.o

lunar_prolog_landers.o: ../code/lunar_prolog_landers.cpp ../code/lunar_prolog_landers.h ../code/prolog_lunar.h $(pc)prolog.h ../code/lunar.h ../code/lunar_landers.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_landers.cpp -c -o lunar_prolog_landers.o

lunar_prolog_keyboard.o: ../code/lunar_prolog_keyboard.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h ../code/keyboard_calculator.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_keyboard.cpp -c -o lunar_prolog_keyboard.o `pkg-config --cflags gtk+-2.0`

prolog_lunar.o: ../code/prolog_lunar.h ../code/prolog_lunar.cpp ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prolog_lunar.cpp -c -o prolog_lunar.o

keyboard.o: keyboard.txt
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386 keyboard.txt keyboard.o
prcgtk.o: ../code/prcgtk.cpp ../code/prolog_lunar.h\
	$(pc)prolog.h $(pc)prolog_conductor.h $(pc)prolog_midi.h $(pc)prolog_neural.h $(pc)prolog_xml.h\
	$(pc)prolog_http.h $(pc)prolog_distribution.h $(pc)prolog_control.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prcgtk.cpp -c -o prcgtk.o `pkg-config --cflags gtk+-2.0`

~/bin/lunar: $(lunar_objects) $(objects) keyboard.o
	g++ $(lunar_objects) $(objects) keyboard.o -o ~/bin/lunar -lpthread -ldl -lreadline -lasound `pkg-config --libs gtk+-2.0`

lunar: ~/bin/lunar

