#ifndef GEM_DIS_MAIN_H
#define GEM_DIS_MAIN_H

#include "../Basic.h"
#include "../Basic/Singleton.h"
#include "../ConCur/TaskQ.h"

namespace Gem { 

	class Gem_Main;
	
namespace Dis {

///NOTE!!  - this class is derived from in a hacky fucked up way...
class DisMain : private Singleton<DisMain>, public TaskHost<DisMain> {
//	friend class Gem_Main;
public: //friend struct Singleton<DisMain>; 
	static void construct();
	static void destruct();
	static void shutdown();
};

struct RenderingCntx;
DisMain& renderingCntx_DM(RenderingCntx&rc);

} }

#endif //GEM_DIS_MAIN_H