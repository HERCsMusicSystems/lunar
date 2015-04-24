
#include "multiplatform_audio.h"
#include <stdio.h>
#include <string.h>

void AudioBuffers :: insertStereo (double left, double right) {}
void AudioBuffers :: insertMono (double value) {}
double AudioBuffers :: getMono (void) {return 0.0;}
double AudioBuffers :: getStereoLeftRight (void) {return 0.0;}
void * AudioBuffers :: getRawData (void) {return NULL;}

static int selected_input_device = -1;
static int selected_output_device = -1;
static MultiplatformAudio :: AudioCallback input_callback = NULL;
static MultiplatformAudio :: AudioCallback output_callback = NULL;

static int number_of_input_devices = 0;
static int number_of_output_devices = 0;
int MultiplatformAudio :: getNumberOfInputDevices (void) {return number_of_input_devices;}
int MultiplatformAudio :: getNumberOfOutputDevices (void) {return number_of_output_devices;}

static char * input_device_names [128];
static char * output_device_names [128];
char * MultiplatformAudio :: getInputDeviceName (int ind) {if (ind < 0 || ind >= number_of_input_devices) return NULL; return input_device_names [ind];}
char * MultiplatformAudio :: getOutputDeviceName (int ind) {if (ind < 0 || ind >= number_of_output_devices) return NULL; return output_device_names [ind];}

int MultiplatformAudio :: getSelectedInputDevice (void) {return selected_input_device;}
int MultiplatformAudio :: getSelectedOutputDevice (void) {return selected_output_device;}

static void * input_source = 0;
static void * output_source = 0;
void MultiplatformAudio :: installInputCallback (AudioCallback callback, void * source) {input_source = source; input_callback = callback;}
void MultiplatformAudio :: installOutputCallback (AudioCallback callback, void * source) {output_source = source; output_callback = callback;}

static FILE * input_file = NULL;
static bool input_file_active = false;
static int file_sampling_freq = 127;
static int file_bytes_per_second = 127;
static int file_bytes_per_frame = 127;
static int file_bits_per_sample = 127;
static int file_size = -1;
static short int file_buffer [65536];
static int get16 (FILE * file) {
	int ret = getc (file);
	ret += getc (file) << 8;
	return ret;
}
static int get32 (FILE * file) {
	int ret = getc (file);
	ret += getc (file) << 8;
	ret += getc (file) << 16;
	ret += getc (file) << 24;
	return ret;
}
static int get_sample_16 (FILE * file) {
	int ret = getc (file);
	ret += getc (file) << 8;
	if (ret >= 16384) ret -= 65536;
	return ret;
}

static FILE * output_file = NULL;
static int record_frames = 0;
static int recorded_frames = 0;
static bool stop_recording = false;
class audio_dx_stereo_buffers;
static audio_dx_stereo_buffers * record_root = 0;
static audio_dx_stereo_buffers * record_current = 0;
static int record_sampling_freq = 127;
static char * output_file_name = 0;
static void insert_long (FILE * tc, int ind) {
	fputc (ind & 0xff, tc);
	ind >>= 8;
	fputc (ind & 0xff, tc);
	ind >>= 8;
	fputc (ind & 0xff, tc);
	ind >>= 8;
	fputc (ind & 0xff, tc);
}
static void insert_short (FILE * tc, int ind) {
	fputc (ind & 0xff, tc);
	ind >>= 8;
	fputc (ind & 0xff, tc);
}
void MultiplatformAudio :: selectOutputFile (double seconds, char * file_name) {
	if (record_frames > 0 || recorded_frames > 0) {printf ("currently recording....\n"); return;}
	int to_record = (int) (seconds * (double) record_sampling_freq);
	if (to_record < 1) return;
	if (output_file_name == 0 && file_name != 0) {
		char * copy = new char [strlen (file_name) + 4];
		strcpy (copy, file_name);
		output_file_name = copy;
	}
	recorded_frames = 0;
	stop_recording = false;
	record_frames = to_record;
	printf ("Recording started (frames = %i, seconds = %f, file = %s).\n", record_frames, seconds, output_file_name != 0 ? output_file_name : "<unknown>");
}
void MultiplatformAudio :: stopRecording (char * file_name) {
	if (output_file_name == 0 && file_name != 0) {
		char * copy = new char [strlen (file_name) + 4];
		strcpy (copy, file_name);
		output_file_name = copy;
	}
	printf ("stopping recording....\n");
	stop_recording = true;
}

bool MultiplatformAudio :: inputFileActive (void) {return input_file != NULL;}
bool MultiplatformAudio :: outputFileActive (void) {return record_frames > 0 || recorded_frames > 0;}

class audio_dx_stereo_buffers : public AudioBuffers {
public:
	short int * from;
	audio_dx_stereo_buffers * next;
	int size;
	audio_dx_stereo_buffers (short int * from) {this -> from = from; next = 0; size = -1;}
	audio_dx_stereo_buffers (int size) {this -> size = size + size; this -> from = new short int [this -> size]; next = 0;}
	audio_dx_stereo_buffers (short int * data, int size) {
		size += size;
		this -> size = size;
		this -> from = new short int [size];
		next = 0;
		for (int ind = 0; ind < size; ind++) from [ind] = data [ind];
	}
	void drop_to_file (FILE * tc) {
		if (tc == 0) return;
		for (int ind = 0; ind < size; ind++) {
			short int value = from [ind];
			fputc (value & 0xff, tc);
			fputc (value >> 8, tc);
		}
	}
	~ audio_dx_stereo_buffers (void) {
		//if (size >= 0 && from != 0) {printf ("DELETE [%i]\n", size); delete [] from;}
		if (size >= 0 && from != 0) delete [] from;
	}
	virtual void insertStereo (double left, double right) {
		int out = (int) (left * 32768.0);
		if (out > 32767) out = 32767; if (out < -32768) out = -32768;
		* from++ = out;
		out = (int) (right * 32768.0);
		if (out > 32767) out = 32767; if (out < -32768) out = -32768;
		* from++ = out;
	}
	virtual void insertMono (double value) {
		int out = (int) (value * 32768.0);
		if (out > 32767) out = 32767; if (out < -32768) out = -32768;
		* from++ = out;
		* from++ = out;
	}
	virtual double getStereoLeftRight (void) {
		double out = (double) (* from++);
		out *= 0.000030517578125; // /= 32768.0;
		return out;
	}
	virtual double getMono (void) {
		double out = (double) * from++;
		out += (double) * from++;
		return out * 0.0000152587890625; // / 65536.0;
	}
	virtual void * getRawData (void) {return from;}
};

static void delete_record_root (FILE * tc) {
	if (record_root == 0) return;
	audio_dx_stereo_buffers * dx = record_root;
	while (dx != 0) {
		audio_dx_stereo_buffers * dd = dx -> next;
		dx -> drop_to_file (tc);
		delete dx;
		dx = dd;
	}
	record_root = record_current = 0;
}

static void * drop_audio_file (void * empty) {
	if (output_file_name != 0) {
		printf ("writing audio file [%s]\n", output_file_name);
		FILE * output_file = fopen (output_file_name, "wb");
		delete [] output_file_name;
		output_file_name = 0;
		if (output_file == 0) {delete_record_root (0); return 0;}
		fprintf (output_file, "RIFF");
		insert_long (output_file, 36 + (recorded_frames << 2));
		fprintf (output_file, "WAVEfmt ");
		insert_long (output_file, 16);
		insert_short (output_file, 1);
		insert_short (output_file, 2);
		insert_long (output_file, record_sampling_freq);
		insert_long (output_file, record_sampling_freq << 2);
		insert_short (output_file, 4);
		insert_short (output_file, 16);
		fprintf (output_file, "data");
		insert_long (output_file, recorded_frames << 2);
		delete_record_root (output_file);
		fclose (output_file);
	} else {
		delete_record_root (0);
		printf ("Recording forgot => no output file name provided.\n");
	}
	record_frames = recorded_frames = 0;
	stop_recording = false;
	return 0;
}

class audio_dx_mono_buffers : public AudioBuffers {
public:
	short int * from;
	bool right;
	audio_dx_mono_buffers (short int * from) {this -> from = from; right = false;}
	virtual void insertStereo (double left, double right) {
		int out = (int) ((left + right) * 16384.0);
		if (out > 32767) out = 32767; if (out < -32768) out = -32768;
		* from++ = out;
	}
	virtual void insertMono (double value) {
		int out = (int) (value * 32768.0);
		if (out > 32767) out = 32767; if (out < -32768) out = -32768;
		* from++ = out;
	}
	virtual double getStereoLeftRight (void) {
		double out = (double) * from;
		out *= 0.000030517578125; // /= 32768.0;
		if (right) from++;
		right = ! right;
		return out;
	}
	virtual double getMono (void) {return (double) (* from++) / 32768.0;}
	virtual void * getRawData (void) {return from;}
};

#ifdef LINUX_ALSA

static char * input_device_ids [128];
static char * output_device_ids [128];

#include <alsa/asoundlib.h>
#include <pthread.h>

static snd_pcm_t * output_pcm = NULL;
static snd_pcm_t * input_pcm = NULL;
static int pcm_sampling_freq = 44100;
static int pcm_channels = 2;
static int pcm_block_size = 1024;
static bool playback_running = false;
static bool capture_running = false;

static bool prestart_pcm_capture (void) {
	if (selected_input_device >= number_of_input_devices) {printf ("INPUT: Input device too high.\n"); return false;}
	int res;
	if ((res = snd_pcm_open (& input_pcm, input_device_ids [selected_input_device], SND_PCM_STREAM_CAPTURE, 0)) < 0) {printf ("INPUT: snd_pcm_open: %s\n", snd_strerror (res)); return false;}
	snd_pcm_hw_params_t * hw_params;
	if ((res = snd_pcm_hw_params_malloc (& hw_params)) < 0) {printf ("INPUT: snd_pcm_hw_params_malloc: %s\n", snd_strerror (res)); return false;}
	if ((res = snd_pcm_hw_params_any (input_pcm, hw_params)) < 0) {printf ("INPUT: snd_pcm_hw_params_any: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	if ((res = snd_pcm_hw_params_set_access (input_pcm, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {printf ("INPUT: snd_pcm_hw_params_set_access: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	if ((res = snd_pcm_hw_params_set_format (input_pcm, hw_params, SND_PCM_FORMAT_S16_LE)) < 0) {printf ("INPUT: snd_pcm_hw_params_set_format: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	unsigned int rate = (unsigned int) pcm_sampling_freq;
	if ((res = snd_pcm_hw_params_set_rate_near (input_pcm, hw_params, & rate, 0)) < 0) {printf ("INPUT: snd_pcm_hw_params_set_near: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	if ((res = snd_pcm_hw_params_set_channels (input_pcm, hw_params, pcm_channels)) < 0) {printf ("INPUT: snd_pcm_hw_params_set_channels: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	if ((res = snd_pcm_hw_params (input_pcm, hw_params)) < 0) {printf ("INPUT: snd_pcm_hw_params: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_params); return false;}
	snd_pcm_hw_params_free (hw_params);
}

static void * start_pcm_capture (void * parameters) {
	int res;
	short int buffer [32768];
	capture_running = true;
	while (capture_running) {
		audio_dx_stereo_buffers buffers (buffer);
		res = snd_pcm_readi (input_pcm, buffer, pcm_block_size);
		if (input_callback != NULL) input_callback (res, & buffers, input_source);
	}
	capture_running = true;
	return 0;
}

static bool prestart_pcm (void) {
//	printf ("Starting: %s\n", output_device_names [selected_output_device]);
	if (selected_output_device >= number_of_output_devices) {printf ("Output device too high.\n"); return false;}
	int res;
	if ((res = snd_pcm_open (& output_pcm, output_device_ids [selected_output_device], SND_PCM_STREAM_PLAYBACK, 0)) < 0) {printf ("snd_pcm_open: %s\n", snd_strerror (res)); return false;}
	snd_pcm_hw_params_t * hw_parameters;
	if ((res = snd_pcm_hw_params_malloc (& hw_parameters)) < 0) {printf ("snd_pcm_hw_params_malloc: %s\n", snd_strerror (res)); return false;}
	if ((res = snd_pcm_hw_params_any (output_pcm, hw_parameters)) < 0) {printf ("snd_pcm_hw_params_any: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	if ((res = snd_pcm_hw_params_set_access (output_pcm, hw_parameters, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {printf ("snd_pcm_hw_params_set_access: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	if ((res = snd_pcm_hw_params_set_format (output_pcm, hw_parameters, SND_PCM_FORMAT_S16_LE)) < 0) {printf ("snd_pcm_hw_params_set_format: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	unsigned int rate = (unsigned int) pcm_sampling_freq;
	if ((res = snd_pcm_hw_params_set_rate_near (output_pcm, hw_parameters, & rate, 0)) < 0) {printf ("snd_pcm_hw_params_set_rate_near: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	if ((res = snd_pcm_hw_params_set_channels (output_pcm, hw_parameters, pcm_channels)) < 0) {printf ("snd_pcm_hw_params_set_channels: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	if ((res = snd_pcm_hw_params_set_buffer_size (output_pcm, hw_parameters, pcm_block_size * 2)) < 0) {printf ("snd_pcm_hw_params_set_channels: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
//	printf ("block size => %i\n", res);
	if ((res = snd_pcm_hw_params (output_pcm, hw_parameters)) < 0) {printf ("snd_pcm_hw_params: %s\n", snd_strerror (res)); snd_pcm_hw_params_free (hw_parameters); return false;}
	snd_pcm_hw_params_free (hw_parameters);
	snd_pcm_sw_params_t * sw_parameters;
	if ((res = snd_pcm_sw_params_malloc (& sw_parameters)) < 0) {printf ("snd_pcm_sw_params_malloc: %s\n", snd_strerror (res)); return false;}
	if ((res = snd_pcm_sw_params_current (output_pcm, sw_parameters)) < 0) {printf ("snd_pcm_sw_params_current: %s\n", snd_strerror (res)); snd_pcm_sw_params_free (sw_parameters); return false;}
	if ((res = snd_pcm_sw_params_set_avail_min (output_pcm, sw_parameters, pcm_block_size)) < 0) {printf ("snd_pcm_sw_params_set_avail_min: %s\n", snd_strerror (res)); snd_pcm_sw_params_free (sw_parameters); return false;}
	if ((res = snd_pcm_sw_params_set_start_threshold (output_pcm, sw_parameters, 0U)) < 0) {printf ("snd_pcm_sw_params_set_start_threshold: %s\n", snd_strerror (res)); snd_pcm_sw_params_free (sw_parameters); return false;}
	if ((res = snd_pcm_sw_params (output_pcm, sw_parameters)) < 0) {printf ("snd_pcm_sw_params: %s\n", snd_strerror (res)); snd_pcm_sw_params_free (sw_parameters); return false;}
	if ((res = snd_pcm_prepare (output_pcm)) < 0) {printf ("snd_pcm_prepare: %s\n", snd_strerror (res)); snd_pcm_sw_params_free (sw_parameters); return false;}
	snd_pcm_sw_params_free (sw_parameters);
	return true;
}

static void * start_pcm (void * parameters) {
	int res;
	playback_running = true;
	short int buffer [32768];
	for (int ind = 0; ind < 32768; ind++) {buffer [ind] = 0;}
//	printf (".... playing\n");
	printf ("audio running [channels=%i sampling=%i buffer=%i device=%s]\n", pcm_channels, pcm_sampling_freq, pcm_block_size, output_device_names [selected_output_device]);
	while (playback_running) {
		if ((res = snd_pcm_wait (output_pcm, 1000)) < 0) {printf ("snd_pcm_wait: %s\n", snd_strerror (res)); playback_running = false; return 0;}
		if ((res = snd_pcm_avail_update (output_pcm)) < 0) {printf ("snd_pcm_avail_update: %s\n", snd_strerror (res)); playback_running = false; return 0;}
		if (res > pcm_block_size) res = pcm_block_size;
		if (output_callback != NULL) {
			audio_dx_stereo_buffers buffers (buffer);
			output_callback (res, & buffers, output_source);
			if (record_frames > 0) {
				audio_dx_stereo_buffers * buf = new audio_dx_stereo_buffers (buffer, res);
				if (record_current != 0) record_current -> next = buf;
				record_current = buf;
				if (record_root == 0) record_root = buf;
				record_frames -= res; recorded_frames += res;
				//printf ("RES [%i %i %i]\n", record_frames, recorded_frames, res);
				if (record_frames <= 0 || stop_recording) {
					record_frames = 0;
					pthread_t thread;
					pthread_create (& thread, 0, drop_audio_file, 0);
					pthread_detach (thread);
				}
			}
		}
		res = snd_pcm_writei (output_pcm, buffer, res);
		if (res < 0) {
			printf ("snd_pcm_writei: %s\n", snd_strerror (res));
			res = snd_pcm_recover (output_pcm, res, 0);
			if (res < 0) {printf ("recovery failed: %s\n", snd_strerror (res)); playback_running = false; return 0;}
		}
	}
	snd_pcm_close (output_pcm);
	if (record_root != 0) drop_audio_file (0);
//	printf ("		.... playback stopped!\n");
	playback_running = true;
	return 0;
}

bool MultiplatformAudio :: selectInputDevice (int ind) {
	int res;
	if (ind >= getNumberOfInputDevices ()) {printf ("Input device out of range.\n"); return false;}
	if (selected_input_device >= 0) {
		if (capture_running) {
			capture_running = false;
			while (! capture_running) {usleep (100000);}
			printf ("Audio capture loop stopped [%s].\n", input_device_names [selected_input_device]);
			capture_running = false;
		}
	}
	if (ind < 0) ind = -1;
	selected_input_device = ind;
	if (selected_input_device >= 0) {
		if (prestart_pcm_capture ()) {pthread_t threader; pthread_create (& threader, 0, start_pcm_capture, 0); pthread_detach (threader); return true;}
		return false;
	}
	return true;
}

bool MultiplatformAudio :: selectOutputDevice (int ind) {
	int res;
	if (ind >= getNumberOfOutputDevices ()) {printf ("Output device out of range.\n"); return false;}
	if (selected_output_device >= 0) {
		if (playback_running) {
			playback_running = false;
			while (! playback_running) {usleep (100000);}
			printf ("Audio loop stopped [%s].\n", output_device_names [selected_output_device]);
			playback_running = false;
		}
	}
	if (ind < 0) ind = -1;
	selected_output_device = ind;
	if (selected_output_device >= 0) {
		if (prestart_pcm ()) {pthread_t threader; pthread_create (& threader, 0, start_pcm, 0); pthread_detach (threader); return true;}
		return false;
	}
	return true;
}

void MultiplatformAudio :: setChannels (int channels) {pcm_channels = channels < 1 ? 1 : channels;}
void MultiplatformAudio :: setSamplingFrequency (int sampling_frequency) {record_sampling_freq = pcm_sampling_freq = sampling_frequency < 1 ? 1 : sampling_frequency;}
int MultiplatformAudio :: getSamplingFrequency (void) {return pcm_sampling_freq;}
void MultiplatformAudio :: setLatencyBufferSize (int size) {pcm_block_size = size;}
int MultiplatformAudio :: getLatencyBufferSize (void) {return pcm_block_size;}

static void replace_enter (char * command) {
	if (command == 0) return;
	char * cp = command;
	while ((cp = strchr (cp, '\n')) != 0) * cp = ' ';
}

MultiplatformAudio :: MultiplatformAudio (void * hwnd) {
	void * * names;
	int res = snd_device_name_hint (-1, "pcm", & names);
	if (res < 0) {printf ("Error [%s]\n", snd_strerror (res)); return;}
	char * * hint = (char * *) names;
	int ind = 0;
	while (* hint != 0) {
		char * name = snd_device_name_get_hint (* hint, "NAME");
		char * description = snd_device_name_get_hint (* hint, "DESC");
		char * io = snd_device_name_get_hint (* hint, "IOID");
		//printf ("%i DEVICE %s\n", ind++, * hint);
		if (io == 0 || strstr (io, "nput") != 0) {
			input_device_names [number_of_input_devices] = new char [strlen (description) + 1];
			input_device_ids [number_of_input_devices] = new char [strlen (name) + 1];
			strcpy (input_device_names [number_of_input_devices], description);
			strcpy (input_device_ids [number_of_input_devices], name);
			replace_enter (input_device_names [number_of_input_devices]);
			number_of_input_devices++;
		}
		if (io == 0 || strstr (io, "utput") != 0) {
			output_device_names [number_of_output_devices] = new char [strlen (description) + 1];
			output_device_ids [number_of_output_devices] = new char [strlen (name) + 1];
			strcpy (output_device_names [number_of_output_devices], description);
			strcpy (output_device_ids [number_of_output_devices], name);
			replace_enter (output_device_names [number_of_output_devices]);
			number_of_output_devices++;
		}
		free (name);
		free (description);
		if (io != 0) free (io);
		hint++;
	}
	snd_device_name_free_hint (names);
}

MultiplatformAudio :: ~ MultiplatformAudio (void) {
	selectInputDevice (-1);
	selectOutputDevice (-1);
	for (int ind = 0; ind < number_of_input_devices; ind++) {delete [] input_device_names [ind]; delete [] input_device_ids [ind];}
	for (int ind = 0; ind < number_of_output_devices; ind++) {delete [] output_device_names [ind]; delete [] output_device_ids [ind];}
	number_of_input_devices = number_of_output_devices = 0;
	printf ("Audio deallocated.\n");
}

#endif

#ifdef MAC_OS_X_CORE_AUDIO

#include <CoreAudio/AudioHardware.h>

static int number_of_devices = 0;
static AudioDeviceID * device_list = NULL;
static AudioDeviceID * input_device_list = NULL;
static AudioDeviceID * output_device_list = NULL;
static AudioDeviceIOProcID * input_callback_ids = NULL;
static AudioDeviceIOProcID * output_callback_ids = NULL;

class CoreStereoAudioBuffers : public AudioBuffers {
public:
	float * data;
	CoreStereoAudioBuffers (float * data) {this -> data = data;}
	virtual void insertStereo (double left, double right) {
		if (left > 1.0) left = 1.0; if (left < -1.0) left = -1.0;
		if (right > 1.0) right = 1.0; if (right < -1.0) right = -1.0;
		* (data++) = (float) left;
		* (data++) = (float) right;
	}
	virtual void insertMono (double value) {
		if (value > 1.0) value = 1.0; if (value < -1.0) value = -1.0;
		* (data++) = (float) value;
		* (data++) = (float) value;
	}
	virtual double getStereoLeftRight (void) {return (double) (* (data++));}
	virtual double getMono (void) {double value = (double) (* (data++)); value += (double) (* (data++)); return value;}
	virtual void * getRawData (void) {return data;}
};

class CoreMonoAudioBuffers : public AudioBuffers {
public:
	float * data;
	bool next;
	CoreMonoAudioBuffers (float * data) {this -> data = data; next = false;}
	virtual void insertStereo (double left, double right) {
		float value = (float) left + (float) right;
		if (value > 1.0) value = 1.0; if (value < -1.0) value = -1.0;
		* (data++) = value;
	}
	virtual void insertMono (double value) {
		if (value > 1.0) value = 1.0; if (value < -1.0) value = -1.0;
		* (data++) = value;
	}
	virtual double getStereoLeftRight (void) {double ret = (double) (* data); if (next) data++; next = ! next; return ret;}
	virtual double getMono (void) {return (double) (* (data++));}
	virtual void * getRawData (void) {return data;}
};

static OSStatus AudioCallbackProcedure (AudioDeviceID device_in, const AudioTimeStamp * now, const AudioBufferList * buffers_in, const AudioTimeStamp * time_in,
						AudioBufferList * buffers_out, const AudioTimeStamp * time_out, void * client_data) {
	if (input_callback != NULL) {
		AudioBuffer * input = (AudioBuffer *) & buffers_in -> mBuffers [0];
		int input_size = input -> mDataByteSize >> 2; // / sizeof (float);
		float * input_data = (float *) input -> mData;
		if (input_data != NULL) {
			if (input -> mNumberChannels == 2) {
				if (input_file_active) {
					int sub = 0;
					int number_of_samples = input_size >> 1;
					for (int ind = 0; ind < number_of_samples && file_size > 0; ind++) {
						file_buffer [sub++] = get_sample_16 (input_file);
						file_buffer [sub++] = get_sample_16 (input_file);
						file_size -= file_bytes_per_frame;
					}
					if (file_size <= 0) {
						input_file_active = false;
						fclose (input_file);
						input_file = NULL;
					}
					audio_dx_stereo_buffers buffers (file_buffer);
					input_callback (number_of_samples, & buffers);
				} else {
					CoreStereoAudioBuffers input_buffers (input_data);
					input_callback (input_size >> 1, & input_buffers);
				}
			} else if (input -> mNumberChannels == 1) {
				CoreMonoAudioBuffers input_buffers (input_data);
				input_callback (input_size, & input_buffers);
			}
		}
	}
	if (output_callback != NULL) {
		AudioBuffer * output = & buffers_out -> mBuffers [0];
		int output_size = output -> mDataByteSize >> 2; // / sizeof (float);
		float * output_data = (float *) output -> mData;
		if (output_data != NULL) {
			if (output -> mNumberChannels == 2) {
				CoreStereoAudioBuffers output_buffers (output_data);
				output_callback (output_size >> 1, & output_buffers);
				if (output_file != NULL) {
					int sentinel = output_size >> 1;
					float data;
					short int value;
					float * fp = output_data;
					for (int ind = 0; ind < sentinel; ind++) {
//						data = * fp++;
						data = * fp; * fp++ = 0.0f;
						data *= 32768.0;
						if (data > 32767.0) data = 32767.0;
						if (data < -32768.0) data = -32768.0;
						value = (short int) data;
						fputc (value & 0xff, output_file);
						fputc (value >> 8, output_file);
//						data = * fp++;
						data = * fp; * fp++ = 0.0f;
						data *= 32768.0;
						if (data > 32767.0) data = 32767.0;
						if (data < -32768.0) data = -32768.0;
						value = (short int) data;
						fputc (value & 0xff, output_file);
						fputc (value >> 8, output_file);
						if (--record_frames <= 0) {
							fclose (output_file);
							output_file = NULL;
							ind = sentinel;
						}
					}
				}
			} else if (output -> mNumberChannels == 1) {
				CoreMonoAudioBuffers output_buffers (output_data);
				output_callback (output_size, & output_buffers);
			}
		}
	}
	return 0;
}

MultiplatformAudio :: MultiplatformAudio (void * hwnd, int channels, int sampling_freq, int latency_samples) {
	record_sampling_freq = sampling_freq;
	UInt32 size;
	OSStatus status = AudioHardwareGetPropertyInfo (kAudioHardwarePropertyDevices, & size, NULL);
	number_of_devices = size / sizeof (AudioDeviceID);
	device_list = new AudioDeviceID [number_of_devices];
	input_device_list = new AudioDeviceID [number_of_devices];
	output_device_list = new AudioDeviceID [number_of_devices];
	input_callback_ids = new AudioDeviceIOProcID [number_of_devices];
	output_callback_ids = new AudioDeviceIOProcID [number_of_devices];
	status = AudioHardwareGetProperty (kAudioHardwarePropertyDevices, & size, device_list);
	text command;
	for (int ind = 0; ind < number_of_devices; ind++) {
		status = AudioDeviceGetPropertyInfo (device_list [ind], 0, false, kAudioDevicePropertyDeviceName, & size, NULL);
		status = AudioDeviceGetProperty (device_list [ind], 0, false, kAudioDevicePropertyDeviceName, & size, command);
		status = AudioDeviceGetPropertyInfo (device_list [ind], 0, false, kAudioDevicePropertyStreams, & size, NULL);
		int number_of_streams = size / sizeof (AudioStreamID);
		if (number_of_streams > 0) {
			strcpy (output_device_names [number_of_output_devices], command);
			output_device_list [number_of_output_devices] = device_list [ind];
			status = AudioDeviceCreateIOProcID (device_list [ind], AudioCallbackProcedure, NULL, & output_callback_ids [number_of_output_devices]);
			number_of_output_devices++;
		}
		status = AudioDeviceGetPropertyInfo (device_list [ind], 0, true, kAudioDevicePropertyStreams, & size, NULL);
		number_of_streams = size / sizeof (AudioStreamID);
		if (number_of_streams > 0) {
			strcpy (input_device_names [number_of_input_devices], command);
			input_device_list [number_of_input_devices] = device_list [ind];
			status = AudioDeviceCreateIOProcID (device_list [ind], AudioCallbackProcedure, NULL, & input_callback_ids [number_of_input_devices]);
			number_of_input_devices++;
		}
	}
}

MultiplatformAudio :: ~ MultiplatformAudio (void) {
	selectInputDevice (-1);
	selectOutputDevice (-1);
	if (input_callback_ids != NULL) {
		for (int ind = 0; ind < number_of_input_devices; ind++) AudioDeviceDestroyIOProcID (input_device_list [ind], input_callback_ids [ind]);
		delete [] input_callback_ids; input_callback_ids = NULL;
	}
	if (output_callback_ids != NULL) {
		for (int ind = 0; ind < number_of_output_devices; ind++) AudioDeviceDestroyIOProcID (output_device_list [ind], output_callback_ids [ind]);
		delete [] output_callback_ids; output_callback_ids = NULL;
	}
	if (device_list != NULL) delete [] device_list; device_list = NULL;
	if (input_device_list != NULL) delete [] input_device_list; input_device_list = NULL;
	if (output_device_list != NULL) delete [] output_device_list; output_device_list = NULL;
}

void MultiplatformAudio :: selectInputDevice (int ind) {
	if (ind >= number_of_input_devices) return;
	if (selected_input_device >= 0) {AudioDeviceStop (input_device_list [selected_input_device], input_callback_ids [selected_input_device]);}
	if (ind < 0) ind = -1;
	selected_input_device = ind;
	if (selected_input_device >= 0) {AudioDeviceStart (input_device_list [selected_input_device], input_callback_ids [selected_input_device]);}
}
void MultiplatformAudio :: selectOutputDevice (int ind) {
	if (ind >= number_of_output_devices) return;
	if (selected_output_device >= 0) {AudioDeviceStop (output_device_list [selected_output_device], output_callback_ids [selected_output_device]);}
	if (ind < 0) ind = -1;
	selected_output_device = ind;
	if (selected_output_device >= 0) {AudioDeviceStart (output_device_list [selected_output_device], output_callback_ids [selected_output_device]);}
}

#endif


#ifdef WINDOWS_DIRECT_X

#include <dsound.h>
#include "process.h"
#include "pthread.h"
static int audio_channels = 2;
static int audio_sampling_freq = 22050;
static HANDLE audio_notification_events [2];
static HANDLE audio_capture_notification_events [2];
static DWORD audio_capture_locked_index = 0;
static int audio_block_size = 1024;
static int number_of_samples = 256;
static void set_audio_block_size (int latency) {
	number_of_samples = latency;
	audio_block_size = latency << 1;
	audio_block_size *= audio_channels;
}
static LPDIRECTSOUND8 audio_lpds = NULL;
static LPDIRECTSOUNDBUFFER audio_lpdsb = NULL;
static DSBPOSITIONNOTIFY audio_position_notify [2];
static DSBPOSITIONNOTIFY audio_capture_position_notify [2];

static LPDIRECTSOUNDCAPTURE8 audio_capture_lpdsc = NULL;
static LPDIRECTSOUNDCAPTUREBUFFER audio_capture_lpdscb = NULL;

static LPGUID input_device_guid [32];
static LPGUID output_device_guid [32];

static HWND main_window_hwnd = NULL;

static void audio_capture_dx_get (int location) {
	LPVOID address;
	DWORD capture_position, read_position, length;
	if (! (DS_OK == audio_capture_lpdscb -> GetCurrentPosition (& capture_position, & read_position))) printf ("Direct X: Failed to get current position.\n");
//	length = capture_position - read_position;
	if (! (DS_OK == audio_capture_lpdscb -> Lock (location, audio_block_size, & address, & length, NULL, NULL, 0))) printf ("Direct X: Buffer not locked.\n");
	if (input_callback != NULL) {
		if (input_file_active) {
			if (audio_channels == 2) {
				int sub = 0;
				for (int ind = 0; ind < number_of_samples && file_size > 0; ind++) {
					file_buffer [sub++] = get_sample_16 (input_file);
					file_buffer [sub++] = get_sample_16 (input_file);
					file_size -= file_bytes_per_frame;
				}
				if (file_size <= 0) {
					input_file_active = false;
					fclose (input_file);
					input_file = NULL;
				}
//				filling_required = true;
				audio_dx_stereo_buffers buffers (file_buffer);
				input_callback (number_of_samples, & buffers, input_source);
			} else if (audio_channels == 1) {
//				CoreMonoAudioBuffers input_buffers (input_data);
//				input_callback (input_size, & input_buffers);
			}
		} else {
			if (audio_channels == 1) {
				audio_dx_mono_buffers buffers ((short int *) address);
				input_callback (length >> 1, & buffers, input_source);
			} else {
				audio_dx_stereo_buffers buffers ((short int *) address);
				input_callback (length >> 2, & buffers, input_source);
			}
		}
	}
	audio_capture_lpdscb -> Unlock (address, length, NULL, 0);
}

static volatile bool capture_cont = false;

static DWORD current_location = 0;
static void AudioCaptureNotificationProc (void * lpParameter) {
	capture_cont = true;
	DWORD dwResult;
	MSG msg;
	while (capture_cont) {
		dwResult = MsgWaitForMultipleObjects (2, audio_capture_notification_events, FALSE, INFINITE, QS_ALLEVENTS);
		switch (dwResult) {
		case WAIT_OBJECT_0:
			audio_capture_dx_get (0);
			break;
		case WAIT_OBJECT_0 + 1:
			audio_capture_dx_get (audio_block_size);
			break;
//		case WAIT_OBJECT_0: case WAIT_OBJECT_0 + 1:
//			audio_capture_dx_get (current_location);
//			if (current_location == 0) current_location = audio_block_size;
//			else current_location = 0;
//			break;
		case WAIT_OBJECT_0 + 2:
			while (PeekMessage (& msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg . message == WM_QUIT) capture_cont = false;
			}
			break;
		default: break;
		}
	}
	capture_cont = true;
}

static BOOL CALLBACK DSCEnumCallbackProc (LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext) {
	input_device_guid [number_of_input_devices] = lpGuid;
	input_device_names [number_of_input_devices] = new char [strlen ((char *) lpcstrDescription) + 1];
	strcpy (input_device_names [number_of_input_devices++], (char *) lpcstrDescription);
	return TRUE;
}
static void audio_capture_dx_init (void) {
	HRESULT hr;
	if (FAILED (hr = DirectSoundCaptureEnumerate (DSCEnumCallbackProc, NULL))) printf ("Direct X: Failed to enumerate capture devices.\n");
}
static void audio_capture_dx_stop (void) {
	if (capture_cont) {
		capture_cont = false;
		while (! capture_cont) Sleep (20);
		capture_cont = false;
	}
	if (audio_capture_lpdscb != NULL) audio_capture_lpdscb -> Stop ();
	for (int ind = 0; ind < 2; ind++) CloseHandle (audio_capture_notification_events [ind]);
	if (audio_capture_lpdscb != NULL) audio_capture_lpdscb -> Release (); audio_capture_lpdscb = NULL;
	if (audio_capture_lpdsc != NULL) audio_capture_lpdsc -> Release (); audio_capture_lpdsc = NULL;
}
static bool audio_capture_dx_start (HWND hwnd, int device_index) {
	HRESULT hr;
	if (FAILED (hr = DirectSoundCaptureCreate8 (input_device_guid [device_index], & audio_capture_lpdsc, NULL))) {printf ("Direct X: Failed to create capture interface.\n"); return false;}
	DSCCAPS capture_caps;
	capture_caps . dwSize = sizeof (DSCCAPS);
	if (FAILED (hr = audio_capture_lpdsc -> GetCaps (& capture_caps))) {printf ("Direct X: Failed to get device caps.\n"); return false;}
//	char command [256];
//	sprintf (command, "channels [%x %x]", capture_caps . dwChannels, capture_caps . dwFormats);
//	MessageBox (GetActiveWindow (), command, "INFO", MB_OK);
	DSCBUFFERDESC dscbdesc;
	WAVEFORMATEX wfx = {WAVE_FORMAT_PCM, audio_channels, audio_sampling_freq, audio_sampling_freq * 2 * audio_channels, audio_channels * 2, 16, 0};
	dscbdesc . dwSize = sizeof (DSCBUFFERDESC);
	dscbdesc . dwFlags = 0;
	dscbdesc . dwBufferBytes = number_of_samples * wfx . nBlockAlign * 2;
	dscbdesc . dwReserved = 0;
	dscbdesc . lpwfxFormat = & wfx;
	dscbdesc . dwFXCount = 0;
	dscbdesc . lpDSCFXDesc = NULL;
	if (FAILED (hr = audio_capture_lpdsc -> CreateCaptureBuffer (& dscbdesc, & audio_capture_lpdscb, NULL))) {printf ("Direct X: Failed to create capture buffer.\n"); return false;}
	LPDIRECTSOUNDNOTIFY8 lpdscNotify;
	if (FAILED (hr = audio_capture_lpdscb -> QueryInterface (IID_IDirectSoundNotify, (LPVOID *) & lpdscNotify))) {printf ("Direct X: Can not get notification addresses.\n"); return false;}
	for (int ind = 0; ind < 2; ind++) {
		audio_capture_notification_events [ind] = CreateEvent (NULL, FALSE, FALSE, NULL);
//		audio_capture_position_notify [ind] . dwOffset = audio_block_size * ind + 100;
		audio_capture_position_notify [ind] . dwOffset = audio_block_size * ind + audio_block_size - 1;
		audio_capture_position_notify [ind] . hEventNotify = audio_capture_notification_events [ind];
	}
	lpdscNotify -> SetNotificationPositions (2, audio_capture_position_notify);
//	lpdscNotify -> Release ();
	_beginthread (AudioCaptureNotificationProc, 0, main_window_hwnd);
	if (FAILED (hr = audio_capture_lpdscb -> Start (DSCBSTART_LOOPING))) {printf ("Direct X: Failed to start looping.\n"); return false;}
	return true;
}

static void audio_transmission_dx_fill_in (DWORD index) {
	LPVOID address;
	DWORD length;
	if (! (DS_OK == audio_lpdsb -> Lock (index, audio_block_size, & address, & length, NULL, NULL, 0))) {printf ("Direct X: Buffer not locked.\n"); return;}
	if (output_callback != NULL) {
		if (audio_channels == 1) {
			audio_dx_mono_buffers buffers ((short int *) address);
			output_callback (length >> 1, & buffers, output_source);
		} else {
			audio_dx_stereo_buffers buffers ((short int *) address);
			output_callback (length >> 2, & buffers, output_source);
			if (record_frames > 0) {
				int res = length >> 2;
				audio_dx_stereo_buffers * buf = new audio_dx_stereo_buffers ((short int *) address, res);
				if (record_current != 0) record_current -> next = buf;
				record_current = buf;
				if (record_root == 0) record_root = buf;
				record_frames -= res; recorded_frames += res;
				//printf ("RES [%i %i %i]\n", record_frames, recorded_frames, res);
				if (record_frames <= 0 || stop_recording) {
					record_frames = 0;
					pthread_t thread;
					pthread_create (& thread, 0, drop_audio_file, 0);
					pthread_detach (thread);
				}
			}
		}
	}
	audio_lpdsb -> Unlock (address, length, NULL, 0);
}

static volatile bool transmission_cont = false;
static void AudioNotificationProc (void * lpParameter) {
	transmission_cont = true;
	DWORD dwResult;
	MSG msg;
	while (transmission_cont) {
		dwResult = MsgWaitForMultipleObjects (2, audio_notification_events, FALSE, INFINITE, QS_ALLEVENTS);
		switch (dwResult) {
		case WAIT_OBJECT_0: audio_transmission_dx_fill_in (audio_block_size); break;
		case WAIT_OBJECT_0 + 1: audio_transmission_dx_fill_in (0); break;
		case WAIT_OBJECT_0 + 2:
			while (PeekMessage (& msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg . message == WM_QUIT) transmission_cont = false;
			}
			break;
		}
	}
	transmission_cont = true;
}
//#include "MMSystem.h"
static BOOL CALLBACK DSEnumCallbackProc (LPGUID lpGuid, LPCSTR lpcstrDescription, LPCSTR lpcstrModule, LPVOID lpContext) {
	output_device_guid [number_of_output_devices] = lpGuid;
	output_device_names [number_of_output_devices] = new char [strlen ((char *) lpcstrDescription) + 1];
	strcpy (output_device_names [number_of_output_devices++], (char *) lpcstrDescription);
	return TRUE;
}
static void audio_transmission_dx_init (void) {
	HRESULT hr;
	if (FAILED (hr = DirectSoundEnumerate (DSEnumCallbackProc, NULL))) printf ("Direct X: Failed to enumerate transmission devices.\n");
}
static void audio_transmission_dx_stop (void) {
	if (audio_lpds == NULL) return;
	if (transmission_cont) {
		transmission_cont = false;
		while (! transmission_cont) Sleep (20);
		transmission_cont = false;
	}
	if (audio_lpdsb != NULL) audio_lpdsb -> Stop ();
	for (int ind = 0; ind < 2; ind++) CloseHandle (audio_notification_events [ind]);
	if (audio_lpdsb != NULL) audio_lpdsb -> Release (); audio_lpdsb = NULL;
	if (audio_lpds != NULL) audio_lpds -> Release (); audio_lpds = NULL;
}
static bool audio_transmission_dx_start (HWND hwnd, int device_index) {
	HRESULT hr;
	if (FAILED (hr = DirectSoundCreate8 (output_device_guid [device_index], & audio_lpds, NULL))) {printf ("Direct X: Direct sound create failed.\n"); return false;}
	if (FAILED (hr = audio_lpds -> SetCooperativeLevel (hwnd, DSSCL_PRIORITY))) {printf ("Direct X: Cooperative level failed.\n"); return false;}
	PCMWAVEFORMAT pcmwf;
	DSBUFFERDESC dsbdesc;
	memset (& pcmwf, 0, sizeof (PCMWAVEFORMAT));
	pcmwf . wf . wFormatTag = WAVE_FORMAT_PCM;
	pcmwf . wf . nChannels = audio_channels;
	pcmwf . wf . nSamplesPerSec = audio_sampling_freq;
	pcmwf . wf . nBlockAlign = audio_channels * 2;
	pcmwf . wf . nAvgBytesPerSec = pcmwf . wf . nSamplesPerSec * pcmwf . wf . nBlockAlign;
	pcmwf . wBitsPerSample = 16;
	memset (& dsbdesc, 0, sizeof (DSBUFFERDESC));
	dsbdesc . dwSize = sizeof (DSBUFFERDESC);
	dsbdesc . dwFlags = DSBCAPS_CTRLPOSITIONNOTIFY | DSBCAPS_GETCURRENTPOSITION2;
	dsbdesc . dwBufferBytes = audio_block_size + audio_block_size; //BUFFER_SIZE;
	dsbdesc . lpwfxFormat = (LPWAVEFORMATEX) & pcmwf;
	if (FAILED (hr = audio_lpds -> CreateSoundBuffer (& dsbdesc, & audio_lpdsb, NULL))) {printf ("Direct X: Buffer not created.\n"); return false;}
	LPDIRECTSOUNDNOTIFY8 lpdsnotify;
	if (FAILED (hr = audio_lpdsb -> QueryInterface (IID_IDirectSoundNotify8, (LPVOID *) & lpdsnotify))) {printf ("Direct X: Interface not availlable.\n"); return false;}
	for (int ind = 0; ind < 2; ind++) {
		audio_notification_events [ind] = CreateEvent (NULL, FALSE, FALSE, NULL);
		audio_position_notify [ind] . dwOffset = audio_block_size * ind + 100;
		audio_position_notify [ind] . hEventNotify = audio_notification_events [ind];
	}
	lpdsnotify -> SetNotificationPositions (2, audio_position_notify);
	_beginthread (AudioNotificationProc, 0, hwnd);
	// start playback
	audio_transmission_dx_fill_in (0);
	audio_lpdsb -> SetCurrentPosition (0);
	hr = audio_lpdsb -> Play (0, 0, DSBPLAY_LOOPING);
	return true;
}

static HWAVEIN hwavein;
static WAVEHDR wave_headers [8];
static short int audio_data [65536];
class direct_x_mono_buffers : public AudioBuffers {
public:
	short int * from;
	bool right;
	direct_x_mono_buffers (short int * from) {this -> from = from; right = false;}
	virtual double getMono (void) {return (double) (* from++) / 32768.0;}
	virtual double getStereoLeftRight (void) {
		double ret = (double) (* from) / 32768.0;
		if (right) from++;
		right = ! right;
		return ret;
	}
};

static void CALLBACK wave_callback (HWAVEIN hwavein, UINT msg, DWORD dwInstance, DWORD param1, DWORD param2) {
	int ind;
	bool filling_required = false;
	if (msg != WIM_DATA) return;
	WAVEHDR * wp = (LPWAVEHDR) ((void *) param1);
	ind = (int) wp -> dwUser;
	short int * ip = (short int *) wp -> lpData;
	if (input_callback != NULL) {
		if (audio_channels == 1) {
			audio_dx_mono_buffers buffers (ip);
			input_callback (number_of_samples, & buffers, input_source);
		} else {
			if (input_file_active) {
				int sub = 0;
				for (int ind = 0; ind < number_of_samples && file_size > 0; ind++) {
					file_buffer [sub++] = get_sample_16 (input_file);
					file_buffer [sub++] = get_sample_16 (input_file);
					file_size -= file_bytes_per_frame;
				}
				if (file_size <= 0) {
					input_file_active = false;
					fclose (input_file);
					input_file = NULL;
				}
				filling_required = true;
				audio_dx_stereo_buffers buffers (file_buffer);
				input_callback (number_of_samples, & buffers, input_source);
			} else {
				audio_dx_stereo_buffers buffers (ip);
				input_callback (number_of_samples, & buffers, input_source);
			}
		}
	}
	waveInAddBuffer (hwavein, wp, sizeof (WAVEHDR));
//	if (filling_required) {
//		int sub = 0;
//		for (int ind = 0; ind < number_of_samples && file_size > 0; ind++) {
//			file_buffer [sub++] = get_sample_16 (input_file);
//			file_buffer [sub++] = get_sample_16 (input_file);
//			file_size -= file_butes_per_frame;
//		}
//	}
}
static void oscilloscope_audio_capture_dx_init (int sampling_frequency) {
	char command [8192];
	int sentinel;
	sentinel = (int) waveInGetNumDevs ();
	sprintf (command, "devices [%i]", sentinel);
//	MessageBox (GetActiveWindow (), command, "INFO", MB_OK);
	UINT id;
	WAVEINCAPS caps;
	for (id = 0; id < (unsigned int) sentinel; id++) {
		waveInGetDevCaps (id, & caps, sizeof (caps));
//		MessageBox (GetActiveWindow (), caps . szPname, "INFO", MB_OK);
	}
	WAVEFORMATEX info;
	info.wFormatTag = WAVE_FORMAT_PCM;
	info.nChannels = audio_channels;
	info.nSamplesPerSec = sampling_frequency;
	info.wBitsPerSample = 16;
	info.nBlockAlign = (info.nChannels * info.wBitsPerSample) / 8;
	info.nAvgBytesPerSec = info.nSamplesPerSec * info.nBlockAlign;
	info.cbSize = 0;
	if (MMSYSERR_NOERROR != waveInOpen (& hwavein, WAVE_MAPPER, & info, (DWORD_PTR) wave_callback, NULL, CALLBACK_FUNCTION)) {MessageBox (GetActiveWindow (), "Failure!\n", "INFO", MB_OK); return;}
	for (int ind = 0; ind < 8; ind++) {
		wave_headers [ind] . lpData = (char *) & audio_data [ind * number_of_samples * audio_channels];
		wave_headers [ind] . dwBufferLength = number_of_samples * 2 * audio_channels;
		wave_headers [ind] . dwUser = ind;
		wave_headers [ind] . dwFlags = 0;
//		wave_headers [ind] . lpNext = ind == 7 ? & wave_headers [0] : & wave_headers [ind + 1];
		if (MMSYSERR_NOERROR != waveInPrepareHeader (hwavein, & wave_headers [ind], sizeof (WAVEHDR))) {MessageBox (GetActiveWindow (), "Failed to prepare header.", "INFO", MB_OK); return;}
		if (MMSYSERR_NOERROR != waveInAddBuffer (hwavein, & wave_headers [ind], sizeof (WAVEHDR))) {MessageBox (GetActiveWindow (), "Failed to add buffer.", "INFO", MB_OK); return;}
	}
	if (MMSYSERR_NOERROR != waveInStart (hwavein)) {MessageBox (GetActiveWindow (), "Failed to start", "INFO", MB_OK); return;}
}

void MultiplatformAudio :: setChannels (int channels) {audio_channels = channels < 1 ? 1 : channels;}
void MultiplatformAudio :: setSamplingFrequency (int sampling_frequency) {record_sampling_freq = audio_sampling_freq = sampling_frequency < 1 ? 1 : sampling_frequency;}
int MultiplatformAudio :: getSamplingFrequency (void) {return audio_sampling_freq;}
void MultiplatformAudio :: setLatencyBufferSize (int size) {set_audio_block_size (size);}
int MultiplatformAudio :: getLatencyBufferSize (void) {return number_of_samples;}
MultiplatformAudio :: MultiplatformAudio (void * hwnd) {
	if (hwnd == 0) hwnd = GetForegroundWindow ();
	if (hwnd == 0) hwnd = GetDesktopWindow ();
	audio_channels = 2;
	record_sampling_freq = audio_sampling_freq = 48000;
	set_audio_block_size (128);
	main_window_hwnd = (HWND) hwnd;
	audio_transmission_dx_init ();
	audio_capture_dx_init ();
}
MultiplatformAudio :: ~ MultiplatformAudio (void) {
	installInputCallback (0);
	installOutputCallback (0);
	audio_transmission_dx_stop ();
	for (int ind = 0; ind < number_of_input_devices; ind++) delete [] input_device_names [ind];
	for (int ind = 0; ind < number_of_output_devices; ind++) delete [] output_device_names [ind];
	number_of_input_devices = number_of_output_devices = 0;
	printf ("Audio deallocated.\n");
}

bool MultiplatformAudio :: selectInputDevice (int ind) {
	if (ind >= getNumberOfInputDevices ()) return false;
	if (selected_input_device >= 0) audio_capture_dx_stop ();
	if (ind < 0) ind = -1;
	selected_input_device = ind;
	if (selected_input_device >= 0) return audio_capture_dx_start (main_window_hwnd, selected_input_device);
	return true;
}

//void MultiplatformAudio :: selectInputDevice (int ind) {oscilloscope_audio_capture_dx_init (audio_sampling_freq);}

bool MultiplatformAudio :: selectOutputDevice (int ind) {
	if (ind >= getNumberOfOutputDevices ()) return false;
	if (selected_output_device >= 0) audio_transmission_dx_stop ();
	if (ind < 0) ind = -1;
	selected_output_device = ind;
	if (selected_output_device >= 0) return audio_transmission_dx_start (main_window_hwnd, selected_output_device);
	return true;
}

#endif

void MultiplatformAudio :: selectInputFile (char * file_name) {
	input_file = fopen (file_name, "rb");
	if (input_file == NULL) return;
	if (get32 (input_file) != 0x46464952) {fclose (input_file); input_file = NULL; input_file_active = false; return;}
	get32 (input_file);
	if (get32 (input_file) != 0x45564157) {fclose (input_file); input_file = NULL; input_file_active = false; return;}
	if (get32 (input_file) != 0x20746d66) {fclose (input_file); input_file = NULL; input_file_active = false; return;}
	if (get32 (input_file) != 0x10) {fclose (input_file); input_file = NULL; input_file_active = false; return;}
	get16 (input_file); // type
	get16 (input_file); // channels
	file_sampling_freq = get32 (input_file);
	file_bytes_per_second = get32 (input_file);
	file_bytes_per_frame = get16 (input_file);
	file_bits_per_sample = get16 (input_file);
	if (get32 (input_file) != 0x61746164) {fclose (input_file); input_file = NULL; input_file_active = false; return;}
	file_size = get32 (input_file);
	input_file_active = true;
	//	fclose (input_file);
}







