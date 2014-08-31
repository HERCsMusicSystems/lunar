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
	keyboard_calculator.o lunar_prolog_moonbase.o lunar_moonbase.o lunar_prolog_landers.o lunar_landers.o lunar_wave.o\
	lunar_prolog_vector.o graphics2d.o graphic_resources.o multiplatform_audio.o\
	prcgtk.o

prc_resources = $(pi)studio_prc.o $(pi)conductor_prc.o $(pi)midi_prc.o $(pi)http_prc.o $(pi)store_prc.o $(pi)f1_prc.o $(pi)help_prc.o $(pi)record_prc.o\
	$(pi)neural_prc.o $(pi)keyboard_prc.o $(pi)sql_prc.o $(pi)test_prc.o $(pi)xml_prc.o $(pi)distribution_prc.o $(pi)control_prc.o lunar_prc.o

multiplatform_audio.o: ../code/multiplatform_audio.cpp ../code/multiplatform_audio.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/multiplatform_audio.cpp -c -o multiplatform_audio.o

keyboard_calculator.o: ../code/keyboard_calculator.cpp ../code/keyboard_calculator.h
	g++ -Wno-write-strings ../code/keyboard_calculator.cpp -c -o keyboard_calculator.o

lunar_core.o: ../code/lunar.h ../code/lunar.cpp
	g++ -Wno-write-strings ../code/lunar.cpp -c -o lunar_core.o

lunar_prolog_oscilloscope.o: ../code/lunar_prolog_oscilloscope.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_oscilloscope.cpp -c -o lunar_prolog_oscilloscope.o `pkg-config --cflags gtk+-2.0`

lunar_moonbase.o: ../code/lunar_moonbase.cpp ../code/lunar_moonbase.h ../code/lunar.h ../code/lunar_landers.h
	g++ -Wno-write-strings ../code/lunar_moonbase.cpp -c -o lunar_moonbase.o

lunar_prolog_operator.o: ../code/lunar_prolog_operator.cpp ../code/prolog_lunar.h ../code/lunar_operator.h\
	../code/lunar.h ../code/lunar_prolog_landers.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_operator.cpp -c -o lunar_prolog_operator.o

lunar_operator.o: ../code/lunar_operator.cpp ../code/lunar_operator.h ../code/lunar.h ../code/lunar_wave.h
	g++ -Wno-write-strings ../code/lunar_operator.cpp -c -o lunar_operator.o

lunar_landers.o: ../code/lunar_landers.cpp ../code/lunar_landers.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_landers.cpp -c -o lunar_landers.o

lunar_wave.o: ../code/lunar_wave.cpp ../code/lunar_wave.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_wave.cpp -c -o lunar_wave.o

lunar_prolog_moonbase.o: ../code/lunar_prolog_moonbase.cpp ../code/lunar_moonbase.h ../code/prolog_lunar.h ../code/multiplatform_audio.h\
	$(pc)prolog.h ../code/lunar.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_moonbase.cpp -c -o lunar_prolog_moonbase.o

lunar_prolog_landers.o: ../code/lunar_prolog_landers.cpp ../code/lunar_prolog_landers.h\
			 ../code/prolog_lunar.h $(pc)prolog.h ../code/lunar.h ../code/lunar_landers.h ../code/lunar_wave.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_landers.cpp -c -o lunar_prolog_landers.o

lunar_prolog_keyboard.o: ../code/lunar_prolog_keyboard.cpp ../code/lunar.h\
			 ../code/prolog_lunar.h $(pc)prolog.h ../code/keyboard_calculator.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_keyboard.cpp -c -o lunar_prolog_keyboard.o `pkg-config --cflags gtk+-2.0`

lunar_prolog_vector.o: ../code/lunar_prolog_vector.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_vector.cpp -c -o lunar_prolog_vector.o `pkg-config --cflags gtk+-2.0`

prolog_lunar.o: ../code/prolog_lunar.h ../code/prolog_lunar.cpp ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prolog_lunar.cpp -c -o prolog_lunar.o

graphics2d.o: $(pc)graphics2d.cpp $(pc)graphics2d.h
	g++ -Wno-write-strings -I. -I$(pc) $(pc)graphics2d.cpp -c -o graphics2d.o

graphic_resources.o: ../code/graphic_resources.h ../code/graphic_resources.cpp
	g++ -Wno-write-strings ../code/graphic_resources.cpp -c -o graphic_resources.o `pkg-config --cflags gtk+-2.0`

small_keyboard.o: ../graphics/small_keyboard.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_small_keyboard_png_start=resource_small_keyboard_start\
		--redefine-sym _binary____graphics_small_keyboard_png_end=resource_small_keyboard_end\
		--redefine-sym _binary____graphics_small_keyboard_png_size=resource_small_keyboard_size\
		../graphics/small_keyboard.png small_keyboard.o
	objdump -t small_keyboard.o

keyboard.o: ../graphics/keyboard.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_keyboard_png_start=resource_keyboard_start\
		--redefine-sym _binary____graphics_keyboard_png_end=resource_keyboard_end\
		--redefine-sym _binary____graphics_keyboard_png_size=resource_keyboard_size\
		../graphics/keyboard.png keyboard.o
	objdump -t keyboard.o

big_keyboard.o: ../graphics/big_keyboard.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_big_keyboard_png_start=resource_big_keyboard_start\
		--redefine-sym _binary____graphics_big_keyboard_png_end=resource_big_keyboard_end\
		--redefine-sym _binary____graphics_big_keyboard_png_size=resource_big_keyboard_size\
		../graphics/big_keyboard.png big_keyboard.o
	objdump -t big_keyboard.o

vector_frame.o: ../graphics/vector_frame.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_vector_frame_png_start=resource_vector_frame_start\
		--redefine-sym _binary____graphics_vector_frame_png_end=resource_vector_frame_end\
		--redefine-sym _binary____graphics_vector_frame_png_size=resource_vector_frame_size\
		../graphics/vector_frame.png vector_frame.o
	objdump -t vector_frame.o

vector_handle.o: ../graphics/vector_handle.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_vector_handle_png_start=resource_vector_handle_start\
		--redefine-sym _binary____graphics_vector_handle_png_end=resource_vector_handle_end\
		--redefine-sym _binary____graphics_vector_handle_png_size=resource_vector_handle_size\
		../graphics/vector_handle.png vector_handle.o
	objdump -t vector_handle.o

lunar_prc.o: ../code/lunar.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_lunar_prc_start=resource_lunar\
		../code/lunar.prc lunar_prc.o

prcgtk.o: ../code/prcgtk.cpp ../code/prolog_lunar.h\
	$(pc)prolog.h $(pc)prolog_conductor.h $(pc)prolog_midi.h $(pc)prolog_neural.h $(pc)prolog_xml.h\
	$(pc)prolog_http.h $(pc)prolog_distribution.h $(pc)prolog_control.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prcgtk.cpp -c -o prcgtk.o `pkg-config --cflags gtk+-2.0`

graphics = small_keyboard.o keyboard.o big_keyboard.o vector_frame.o vector_handle.o

~/bin/lunar: $(lunar_objects) $(objects) $(graphics) $(prc_resources)
	g++ $(lunar_objects) $(objects) $(prc_resources) $(graphics) \
	-o ~/bin/lunar -lpthread -ldl -lreadline -lasound `pkg-config --libs gtk+-2.0`

lunar: ~/bin/lunar

