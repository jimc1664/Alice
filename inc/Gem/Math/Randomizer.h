#ifndef GEM_MATH_RANDOMIZER_H
#define GEM_MATH_RANDOMIZER_H

#include "../OS/Time.h"


namespace Gem {

struct Randomize{};
class Randomizer { //very simple random generater, also very fast
public:
	Randomizer() {}
	Randomizer( Randomize ) {randomize();} 
	Randomizer( const u32 &seed ) : Seed(seed) {}

	u32 rand( const u32& scale = 32768 ) { return  rawRand()%scale; } //returns between 0 and scale-1, max scle = 32768
	f32 randf() { return f32((s32)rand())/32767.0f; }  //returns between 0.0 and 1.0

	void setSeed( const u32 &s ) { Seed = s; }
	void incSeed( const u32 &s ) { Seed += s; }
	void randomize() { setSeed((u32)OS::Time::Current()); } //value returned by current makes a pretty good seed

	//const s32& mn  -inclusive , const s32& mx   -exclusive
	s32 range( const s32& mn, const s32& mx ) { return rand(mx - mn) + mn; }
	f32 range( const f32& mn, const f32& mx ) { return randf()*(mx-mn) + mn; }

private:
	u32 rawRand() {
		Seed = Seed * 1103515245 +12345;
		return ( Seed / 65536);
	};
	u32 Seed;
};

}
#endif //GEM_MATH_RANDOMIZER_H