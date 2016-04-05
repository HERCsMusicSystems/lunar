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
	keyboard_calculator.o lunar_prolog_moonbase.o lunar_moonbase.o lunar_prolog_landers.o chromatic.o lunar_landers.o lunar_wave.o\
	lunar_prolog_vector.o lunar_prolog_control_panel.o graphics2d.o graphic_resources.o multiplatform_audio.o\
	lunar_prolog_parameter_block_panel.o lunar_prolog_adsr_panel.o lunar_prolog_eg_panel.o\
	lunar_prolog_fm4_panel.o lunar_prolog_core_panel.o lunar_prolog_lfo_panel.o lunar_prolog_filter_panel.o\
	lunar_prolog_delay_panel.o lunar_prolog_chorus_panel.o lunar_prolog_schorus_panel.o lunar_prolog_freeverb_panel.o lunar_prolog_panel_base.o\
	lunar_linux_midi.o prcgtk.o

prc_resources = $(pi)studio_prc.o $(pi)conductor_prc.o $(pi)midi_prc.o $(pi)http_prc.o $(pi)store_prc.o $(pi)f1_prc.o $(pi)help_prc.o $(pi)record_prc.o\
	$(pi)neural_prc.o $(pi)keyboard_prc.o $(pi)sql_prc.o $(pi)test_prc.o $(pi)xml_prc.o $(pi)distribution_prc.o $(pi)control_prc.o lunar_prc.o

multiplatform_audio.o: ../code/multiplatform_audio.cpp ../code/multiplatform_audio.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/multiplatform_audio.cpp -c -o multiplatform_audio.o -fPIC

keyboard_calculator.o: ../code/keyboard_calculator.cpp ../code/keyboard_calculator.h
	g++ -Wno-write-strings ../code/keyboard_calculator.cpp -c -o keyboard_calculator.o -fPIC

lunar_core.o: ../code/lunar.h ../code/lunar.cpp
	g++ -fopenmp -Wno-write-strings ../code/lunar.cpp -c -o lunar_core.o -fPIC

lunar_prolog_oscilloscope.o: ../code/lunar_prolog_oscilloscope.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h $(pc)graphics2d.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_oscilloscope.cpp -c -o lunar_prolog_oscilloscope.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_moonbase.o: ../code/lunar_moonbase.cpp ../code/lunar_moonbase.h ../code/lunar.h ../code/lunar_landers.h
	g++ -Wno-write-strings ../code/lunar_moonbase.cpp -c -o lunar_moonbase.o -fPIC

lunar_prolog_operator.o: ../code/lunar_prolog_operator.cpp ../code/prolog_lunar.h ../code/lunar_operator.h\
	../code/lunar.h ../code/lunar_landers.h ../code/lunar_prolog_landers.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_operator.cpp -c -o lunar_prolog_operator.o -fPIC

lunar_operator.o: ../code/lunar_operator.cpp ../code/lunar_operator.h ../code/lunar.h ../code/lunar_wave.h
	g++ -Wno-write-strings ../code/lunar_operator.cpp -c -o lunar_operator.o -fPIC

lunar_landers.o: ../code/lunar_landers.cpp ../code/lunar_landers.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_landers.cpp -c -o lunar_landers.o -fPIC

lunar_wave.o: ../code/lunar_wave.cpp ../code/lunar_wave.h ../code/lunar.h
	g++ -Wno-write-strings ../code/lunar_wave.cpp -c -o lunar_wave.o -fPIC

lunar_prolog_moonbase.o: ../code/lunar_prolog_moonbase.cpp ../code/lunar_moonbase.h ../code/prolog_lunar.h ../code/multiplatform_audio.h\
	$(pc)prolog.h ../code/lunar.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_moonbase.cpp -c -o lunar_prolog_moonbase.o -fPIC

lunar_prolog_landers.o: ../code/lunar_prolog_landers.cpp ../code/lunar_prolog_landers.h ../code/chromatic.h\
			 ../code/prolog_lunar.h $(pc)prolog.h ../code/lunar.h ../code/lunar_landers.h ../code/lunar_wave.h ../code/lunar_moonbase.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_landers.cpp -c -o lunar_prolog_landers.o -fPIC

chromatic.o: ../code/chromatic.h ../code/chromatic.cpp $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/chromatic.cpp -c -o chromatic.o -fPIC

lunar_prolog_keyboard.o: ../code/lunar_prolog_keyboard.cpp ../code/lunar.h\
			 ../code/prolog_lunar.h $(pc)prolog.h ../code/keyboard_calculator.h ../code/graphic_resources.h $(pc)graphics2d.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_keyboard.cpp -c -o lunar_prolog_keyboard.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_vector.o: ../code/lunar_prolog_vector.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h\
			$(pc)prolog.h $(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_vector.cpp -c -o lunar_prolog_vector.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_control_panel.o: ../code/lunar_prolog_control_panel.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h\
			../code/lunar_prolog_panel_base.h $(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_control_panel.cpp -c -o lunar_prolog_control_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_parameter_block_panel.o: ../code/lunar_prolog_parameter_block_panel.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h ../code/lunar_prolog_panel_base.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_parameter_block_panel.cpp -c -o lunar_prolog_parameter_block_panel.o \
		`pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_adsr_panel.o: ../code/lunar_prolog_adsr_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_adsr_panel.cpp -c -o lunar_prolog_adsr_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_eg_panel.o: ../code/lunar_prolog_eg_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_eg_panel.cpp -c -o lunar_prolog_eg_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_fm4_panel.o: ../code/lunar_prolog_fm4_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_fm4_panel.cpp -c -o lunar_prolog_fm4_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_core_panel.o: ../code/lunar_prolog_core_panel.cpp ../code/lunar.h ../code/prolog_lunar.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_core_panel.cpp -c -o lunar_prolog_core_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_lfo_panel.o: ../code/lunar_prolog_lfo_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_lfo_panel.cpp -c -o lunar_prolog_lfo_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_filter_panel.o: ../code/lunar_prolog_filter_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_filter_panel.cpp -c -o lunar_prolog_filter_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_delay_panel.o: ../code/lunar_prolog_delay_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_delay_panel.cpp -c -o lunar_prolog_delay_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_chorus_panel.o: ../code/lunar_prolog_chorus_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_chorus_panel.cpp -c -o lunar_prolog_chorus_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_schorus_panel.o: ../code/lunar_prolog_schorus_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_schorus_panel.cpp -c -o lunar_prolog_schorus_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_freeverb_panel.o: ../code/lunar_prolog_freeverb_panel.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_freeverb_panel.cpp -c -o lunar_prolog_freeverb_panel.o `pkg-config --cflags gtk+-2.0` -fPIC

lunar_prolog_panel_base.o: ../code/lunar_prolog_panel_base.cpp ../code/lunar.h ../code/prolog_lunar.h ../code/lunar_prolog_panel_base.h $(pc)prolog.h\
			$(pc)graphics2d.h ../code/graphic_resources.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_prolog_panel_base.cpp -c -o lunar_prolog_panel_base.o `pkg-config --cflags gtk+-2.0` -fPIC

prolog_lunar.o: ../code/prolog_lunar.h ../code/prolog_lunar.cpp ../code/lunar.h $(pc)prolog.h ../code/lunar_linux_midi.h ../code/lunar_prolog_landers.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prolog_lunar.cpp -c -o prolog_lunar.o -fPIC

lunar_linux_midi.o: ../code/lunar_linux_midi.h ../code/lunar_linux_midi.cpp $(pc)prolog.h ../code/chromatic.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_linux_midi.cpp -c -o lunar_linux_midi.o -fPIC

lunar_integrated_components.o: ../code/lunar_integrated_components.cpp ../code/lunar_integrated_components.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_integrated_components.cpp -c -o lunar_integrated_components.o -fPIC

lunar_integrated_alarm.o: ../code/lunar_integrated_alarm.cpp ../code/lunar_prolog_landers.h ../code/prolog_lunar.h ../code/chromatic.h\
			../code/lunar_moonbase.h ../code/lunar_landers.h ../code/lunar_integrated_components.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_integrated_alarm.cpp -c -o lunar_integrated_alarm.o -fPIC

lunar_integrated_microdot.o: ../code/lunar_integrated_microdot.cpp ../code/lunar_prolog_landers.h ../code/prolog_lunar.h ../code/chromatic.h\
			../code/lunar_moonbase.h ../code/lunar_landers.h ../code/lunar_integrated_components.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_integrated_microdot.cpp -c -o lunar_integrated_microdot.o -fPIC

lunar_integrated_abakos.o: ../code/lunar_integrated_abakos.cpp ../code/lunar_prolog_landers.h ../code/prolog_lunar.h ../code/chromatic.h\
			../code/lunar_moonbase.h ../code/lunar_landers.h ../code/lunar_integrated_components.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_integrated_abakos.cpp -c -o lunar_integrated_abakos.o -fPIC

lunar_integrated_phobos.o: ../code/lunar_integrated_phobos.cpp ../code/lunar_prolog_landers.h ../code/prolog_lunar.h ../code/chromatic.h\
			../code/lunar_moonbase.h ../code/lunar_landers.h ../code/lunar_integrated_components.h ../code/lunar.h $(pc)prolog.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/lunar_integrated_phobos.cpp -c -o lunar_integrated_phobos.o -fPIC

graphics2d.o: $(pc)graphics2d.cpp $(pc)graphics2d.h
	g++ -Wno-write-strings -I. -I$(pc) $(pc)graphics2d.cpp -c -o graphics2d.o -fPIC

graphic_resources.o: ../code/graphic_resources.h ../code/graphic_resources.cpp $(pc)graphics2d.h
	g++ -Wno-write-strings -I$(pc) ../code/graphic_resources.cpp -c -o graphic_resources.o `pkg-config --cflags gtk+-2.0` -fPIC

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

knob.o: ../graphics/knob.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_knob_png_start=resource_knob_start\
		--redefine-sym _binary____graphics_knob_png_end=resource_knob_end\
		--redefine-sym _binary____graphics_knob_png_size=resource_knob_size\
		../graphics/knob.png knob.o
	objdump -t knob.o

knob_surface.o: ../graphics/knob_surface.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_knob_surface_png_start=resource_knob_surface_start\
		--redefine-sym _binary____graphics_knob_surface_png_end=resource_knob_surface_end\
		--redefine-sym _binary____graphics_knob_surface_png_size=resource_knob_surface_size\
		../graphics/knob_surface.png knob_surface.o
	objdump -t knob_surface.o

knob_handle.o: ../graphics/knob_handle.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_knob_handle_png_start=resource_knob_handle_start\
		--redefine-sym _binary____graphics_knob_handle_png_end=resource_knob_handle_end\
		--redefine-sym _binary____graphics_knob_handle_png_size=resource_knob_handle_size\
		../graphics/knob_handle.png knob_handle.o
	objdump -t knob_handle.o

display.o: ../graphics/display.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_display_png_start=resource_display_start\
		--redefine-sym _binary____graphics_display_png_end=resource_display_end\
		../graphics/display.png display.o
	objdump -t display.o

button_on.o: ../graphics/button_on.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_button_on_png_start=resource_button_on_start\
		--redefine-sym _binary____graphics_button_on_png_end=resource_button_on_end\
		../graphics/button_on.png button_on.o
	objdump -t button_on.o

button_off.o: ../graphics/button_off.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_button_off_png_start=resource_button_off_start\
		--redefine-sym _binary____graphics_button_off_png_end=resource_button_off_end\
		../graphics/button_off.png button_off.o
	objdump -t button_off.o

encoder_surface.o: ../graphics/encoder_surface.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_encoder_surface_png_start=resource_encoder_surface_start\
		--redefine-sym _binary____graphics_encoder_surface_png_end=resource_encoder_surface_end\
		../graphics/encoder_surface.png encoder_surface.o
	objdump -t encoder_surface.o

encoder_handle.o: ../graphics/encoder_handle.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_encoder_handle_png_start=resource_encoder_handle_start\
		--redefine-sym _binary____graphics_encoder_handle_png_end=resource_encoder_handle_end\
		../graphics/encoder_handle.png encoder_handle.o
	objdump -t encoder_handle.o

slider_surface.o: ../graphics/slider_surface.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_slider_surface_png_start=resource_slider_surface_start\
		--redefine-sym _binary____graphics_slider_surface_png_end=resource_slider_surface_end\
		../graphics/slider_surface.png slider_surface.o
	objdump -t slider_surface.o

slider_handle.o: ../graphics/slider_handle.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_slider_handle_png_start=resource_slider_handle_start\
		--redefine-sym _binary____graphics_slider_handle_png_end=resource_slider_handle_end\
		../graphics/slider_handle.png slider_handle.o
	objdump -t slider_handle.o

command_centre.o: ../graphics/command_centre.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_command_centre_png_start=resource_command_centre_start\
		--redefine-sym _binary____graphics_command_centre_png_end=resource_command_centre_end\
		../graphics/command_centre.png command_centre.o
	objdump -t command_centre.o

adsr_panel.o: ../graphics/adsr_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_adsr_panel_png_start=resource_adsr_panel_start\
		--redefine-sym _binary____graphics_adsr_panel_png_end=resource_adsr_panel_end\
		../graphics/adsr_panel.png adsr_panel.o
	objdump -t adsr_panel.o

eg_panel.o: ../graphics/eg_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_eg_panel_png_start=resource_eg_panel_start\
		--redefine-sym _binary____graphics_eg_panel_png_end=resource_eg_panel_end\
		../graphics/eg_panel.png eg_panel.o
	objdump -t eg_panel.o

fm4_panel.o: ../graphics/fm4_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_fm4_panel_png_start=resource_fm4_panel_start\
		--redefine-sym _binary____graphics_fm4_panel_png_end=resource_fm4_panel_end\
		../graphics/fm4_panel.png fm4_panel.o
	objdump -t fm4_panel.o

core_panel.o: ../graphics/core_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_core_panel_png_start=resource_core_panel_start\
		--redefine-sym _binary____graphics_core_panel_png_end=resource_core_panel_end\
		../graphics/core_panel.png core_panel.o
	objdump -t core_panel.o

lfo_panel.o: ../graphics/lfo_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_lfo_panel_png_start=resource_lfo_panel_start\
		--redefine-sym _binary____graphics_lfo_panel_png_end=resource_lfo_panel_end\
		../graphics/lfo_panel.png lfo_panel.o
	objdump -t lfo_panel.o

filter_panel.o: ../graphics/filter_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_filter_panel_png_start=resource_filter_panel_start\
		--redefine-sym _binary____graphics_filter_panel_png_end=resource_filter_panel_end\
		../graphics/filter_panel.png filter_panel.o
	objdump -t filter_panel.o

delay_panel.o: ../graphics/delay_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_delay_panel_png_start=resource_delay_panel_start\
		--redefine-sym _binary____graphics_delay_panel_png_end=resource_delay_panel_end\
		../graphics/delay_panel.png delay_panel.o
	objdump -t delay_panel.o

chorus_panel.o: ../graphics/chorus_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_chorus_panel_png_start=resource_chorus_panel_start\
		--redefine-sym _binary____graphics_chorus_panel_png_end=resource_chorus_panel_end\
		../graphics/chorus_panel.png chorus_panel.o
	objdump -t chorus_panel.o

stereo_chorus_panel.o: ../graphics/stereo_chorus_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_stereo_chorus_panel_png_start=resource_stereo_chorus_panel_start\
		--redefine-sym _binary____graphics_stereo_chorus_panel_png_end=resource_stereo_chorus_panel_end\
		../graphics/stereo_chorus_panel.png stereo_chorus_panel.o
	objdump -t stereo_chorus_panel.o

freeverb_panel.o: ../graphics/freeverb_panel.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_freeverb_panel_png_start=resource_freeverb_panel_start\
		--redefine-sym _binary____graphics_freeverb_panel_png_end=resource_freeverb_panel_end\
		../graphics/freeverb_panel.png freeverb_panel.o
	objdump -t freeverb_panel.o

lfo_vibrato.o: ../graphics/lfo_vibrato_knob.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_lfo_vibrato_knob_png_start=resource_lfo_vibrato_start\
		--redefine-sym _binary____graphics_lfo_vibrato_knob_png_end=resource_lfo_vibrato_end\
		../graphics/lfo_vibrato_knob.png lfo_vibrato.o
	objdump -t lfo_vibrato.o

lfo_tremolo.o: ../graphics/lfo_tremolo_knob.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_lfo_tremolo_knob_png_start=resource_lfo_tremolo_start\
		--redefine-sym _binary____graphics_lfo_tremolo_knob_png_end=resource_lfo_tremolo_end\
		../graphics/lfo_tremolo_knob.png lfo_tremolo.o
	objdump -t lfo_tremolo.o

lfo_wah_wah.o: ../graphics/lfo_wah_wah_knob.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_lfo_wah_wah_knob_png_start=resource_lfo_wah_wah_start\
		--redefine-sym _binary____graphics_lfo_wah_wah_knob_png_end=resource_lfo_wah_wah_end\
		../graphics/lfo_wah_wah_knob.png lfo_wah_wah.o
	objdump -t lfo_wah_wah.o

lfo_pan.o: ../graphics/lfo_pan_knob.png
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____graphics_lfo_pan_knob_png_start=resource_lfo_pan_start\
		--redefine-sym _binary____graphics_lfo_pan_knob_png_end=resource_lfo_pan_end\
		../graphics/lfo_pan_knob.png lfo_pan.o
	objdump -t lfo_pan.o

lunar_prc.o: ../code/lunar.prc
	objcopy --input binary --output elf64-x86-64 --binary-architecture i386\
		--redefine-sym _binary____code_lunar_prc_start=resource_lunar\
		../code/lunar.prc lunar_prc.o
	objdump -t lunar_prc.o

prcgtk.o: ../code/prcgtk.cpp ../code/prolog_lunar.h ../code/lunar.h\
	$(pc)prolog.h $(pc)prolog_conductor.h $(pc)prolog_midi.h $(pc)prolog_neural.h $(pc)prolog_xml.h\
	$(pc)prolog_http.h $(pc)prolog_distribution.h $(pc)prolog_control.h
	g++ -Wno-write-strings -I. -I$(pc) ../code/prcgtk.cpp -c -o prcgtk.o `pkg-config --cflags gtk+-2.0`

graphics = small_keyboard.o keyboard.o big_keyboard.o vector_frame.o vector_handle.o knob.o knob_surface.o\
	knob_handle.o display.o button_on.o button_off.o encoder_surface.o encoder_handle.o slider_surface.o slider_handle.o\
	command_centre.o adsr_panel.o eg_panel.o fm4_panel.o core_panel.o\
	lfo_panel.o lfo_vibrato.o lfo_tremolo.o lfo_wah_wah.o lfo_pan.o filter_panel.o\
	delay_panel.o chorus_panel.o stereo_chorus_panel.o freeverb_panel.o

lunar_objects = \
	prolog_lunar.o lunar_core.o lunar_prolog_oscilloscope.o lunar_prolog_keyboard.o lunar_prolog_operator.o lunar_operator.o\
	keyboard_calculator.o lunar_prolog_moonbase.o lunar_moonbase.o lunar_prolog_landers.o chromatic.o lunar_landers.o lunar_wave.o\
	lunar_prolog_vector.o lunar_prolog_control_panel.o graphics2d.o graphic_resources.o multiplatform_audio.o\
	lunar_prolog_parameter_block_panel.o lunar_prolog_adsr_panel.o lunar_prolog_eg_panel.o\
	lunar_prolog_fm4_panel.o lunar_prolog_core_panel.o lunar_prolog_lfo_panel.o lunar_prolog_filter_panel.o\
	lunar_prolog_delay_panel.o lunar_prolog_chorus_panel.o lunar_prolog_schorus_panel.o lunar_prolog_freeverb_panel.o lunar_prolog_panel_base.o\
	lunar_integrated_components.o lunar_integrated_alarm.o lunar_integrated_microdot.o lunar_integrated_abakos.o lunar_integrated_phobos.o\
	lunar_linux_midi.o

~/bin/lunar: $(lunar_objects) $(objects) $(graphics) $(prc_resources)
	g++ $(lunar_objects) $(objects) $(prc_resources) $(graphics) \
	-o ~/bin/lunar -lpthread -ldl -lreadline -lasound `pkg-config --libs gtk+-2.0` -fopenmp

/usr/lib/lunar.so: $(lunar_objects) lunar_prc.o $(graphics)
	g++ $(lunar_objects) lunar_prc.o $(graphics) \
	-o /usr/lib/lunar.so -lpthread -lreadline -lasound -ldl -shared -fPIC `pkg-config --libs gtk+-2.0` -fopenmp

#lunar: ~/bin/lunar
lunar: /usr/lib/lunar.so

