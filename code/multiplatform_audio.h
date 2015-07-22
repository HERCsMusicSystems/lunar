///////////////////////////////////////
// Copyright (C) 2008 Robert P. Wolf //
//        ALL RIGHTS RESERVED        //
///////////////////////////////////////

#ifndef _MULTIPLATFORM_AUDIO_
#define _MULTIPLATFORM_AUDIO_

// choose one
//#define MAC_OS_X_CORE_AUDIO
//#define LINUX_JACK
//#define WINDOWS_DIRECT_X
//#define WINDOWS_MEDIA

#ifdef WIN32
#define WINDOWS_OPERATING_SYSTEM
#else
#define LINUX_OPERATING_SYSTEM
#endif

#ifdef LINUX_OPERATING_SYSTEM
#ifdef MAC_OPERATING_SYSTEM
#define MAC_OS_X_CORE_AUDIO
#else
#define LINUX_ALSA
#endif
#endif

#ifdef WINDOWS_OPERATING_SYSTEM
#define WINDOWS_DIRECT_X
#endif

class AudioBuffers {
public:
	virtual void insertMono (double value);
	virtual void insertStereo (double left, double right);
	virtual double getMono (void);
	virtual double getStereoLeftRight (void);
	virtual void * getRawData (void);
};

class MultiplatformAudio {
public:
	typedef void (* AudioCallback) (int frames, AudioBuffers * buffers, void * source);
	int getNumberOfInputDevices (void);
	int getNumberOfOutputDevices (void);
	char * getInputDeviceName (int ind);
	char * getOutputDeviceName (int ind);
	int getSelectedInputDevice (void);
	int getSelectedOutputDevice (void);
	bool selectInputDevice (int ind);
	bool selectOutputDevice (int ind);
	void installInputCallback (AudioCallback callback, void * source = 0);
	void installOutputCallback (AudioCallback callback, void * source = 0);
	void setChannels (int channels);
	void setSamplingFrequency (int sampling_frequency);
	int getSamplingFrequency (void);
	void setLatencyBufferSize (int size);
	int getLatencyBufferSize (void);
	MultiplatformAudio (void * hwnd = 0);
	~ MultiplatformAudio (void);
	void selectInputFile (char * file_name);
	void selectOutputFile (double seconds, char * file_name);
	void stopRecording (char * file_name = 0);
	bool inputFileActive (void);
	bool outputFileActive (void);
};

#endif

