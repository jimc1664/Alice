#ifndef GEM_INTERNAL_MAIN_H
#define GEM_INTERNAL_MAIN_H

#include "Gem/Main.h"
#include "Gem/Basic/Singleton.h"

namespace Gem {

class Gem_Main : public Singleton<Gem_Main>, private Main_Hlpr {
private: friend struct Singleton<Gem_Main>;

	Gem_Main();
	~Gem_Main();
	void _shutdown();
};

}

#endif //GEM_INTERNAL_MAIN_H