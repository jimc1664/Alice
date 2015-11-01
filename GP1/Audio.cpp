#include "stdafx.h"

#include "Audio.h"

#pragma warning( disable : 4996 )  
//error C4996: 'alutLoadWAVFile': was declared deprecated

#include "OpenAL\al.h"
#include "OpenAL\alc.h"
#include "OpenAL\alut.h"

#pragma comment (lib, "OpenAL32.lib")
#pragma comment (lib, "alut.lib")


void checkError();
void clearError();

ALCcontext *OALContext = null;
ALCdevice *OALDevice = null;

void Audio::initOAL() {
	clearError();
	OALDevice = alcOpenDevice(NULL);
	if (OALDevice) {
		OALContext = alcCreateContext(OALDevice, NULL);
		alcMakeContextCurrent(OALContext);
	}
	auto err = alcGetError( OALDevice );

	int a = 0;
	checkError();
}


Audio* Audio::load( const CStr &file, bool loop, float gain ) {

	char*        OALData;           //data for the buffer
	ALenum       OALFormat;         //buffer format
	ALsizei      OALFrequency;     //frequency
	long         OALBufferLen;      //bit depth
	int			 OALChannels;		  // Channels
	int			 OALSize;			  // Size
	int			 OALBitRate;		  // Bit Rate
	float        OALLength;		  // Length
	ALboolean    OALLoop;           //loop
	unsigned int OALSource;         //source
	ALuint       OALBuffer;         // Buffer
	ALboolean    OALbEAX;			  // for EAX 2.0 support
	ALboolean    alLoop;			  //loop

	OALData = NULL;           //data for the buffer
	OALFrequency = 0;     //frequency
	OALBitRate = 0;		  // Bit Rate
	OALLength = 0;		  // Length
	OALBuffer = 0;         // Buffer

	// Check for EAX 2.0 support
	OALbEAX = alIsExtensionPresent("EAX2.0");
	// Generate Buffers
	alGetError(); // clear error code

	//load the wave file
	alutLoadWAVFile((ALbyte *)file.str(), &OALFormat, (void **)&OALData, (ALsizei *)&OALBufferLen, &OALFrequency, &alLoop);

	checkError();
	//create a source
	alGenSources(1, &OALSource);

	//create  buffer
	alGenBuffers(1, &OALBuffer);

	//put the data into our sampleset buffer
	alBufferData(OALBuffer, OALFormat, OALData, OALBufferLen, OALFrequency);

	//assign the buffer to this source
	alSourcei(OALSource, AL_BUFFER, OALBuffer);

	//release the data
	alutUnloadWAV(OALFormat, OALData, OALBufferLen, OALFrequency);


	alSourcei(OALSource, AL_LOOPING, loop);
	alSourcef(OALSource, AL_GAIN, gain );
	checkError();

	return (Audio*)(sizet)OALSource;
}


void Audio::play() {
		//play	
	alSourcePlay( (sizet)this );

}