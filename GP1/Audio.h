#ifndef GP1_AUDIO_H
#define GP1_AUDIO_H

#include <Gem/Basic.h>
#include <Gem/String/CStr.h>

class Audio {
public:
	static void initOAL();
	static Audio* load( const CStr &file, bool looping = false, float gain = 1.0f  );
	void play();
};



#endif //GP1_AUDIO_H

