#include "../stdafx.h"

#include <Gem/ConCur/WorkHost.h>
#include <Gem/Resource/Resource.h>




#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

using namespace std;

void Rsrc_Raw::fromFile( WorkHost&, Entry &e, CallBackHndl &&cb  ) {
	ifstream file;
	CStr fn = e.Name;
	file.open(fn.str(), std::ios::in);
	if (!file) {
		cout << "File "<<fn.str()<<" could not be loaded" << endl;
		return;
	}

	if (file.good()) {
		file.seekg(0, std::ios::end);
		u32 len = (u32)file.tellg();
		file.seekg(std::ios::beg);
		if (len == 0)  {
			std::cout << "File "<<fn.str()<<" has no contents " << std::endl;
			file.close();
			return;
		}	
		u32 allc = sizeof(Rsrc_Raw)-1 + len +1;
		Rsrc_Raw& rs = *(Rsrc_Raw*)malloc(allc); memset(&rs, 0, allc);
		
		new (&rs)Rsrc_Raw(e, len);
		
		file.read( const_cast<char*>(&rs.Data[0]), len);		
		file.close();

		e.Res = &rs;
	}
}


Rsrc_Raw::Hndl Rsrc_Raw::fromFile(const CStr &s, CallBackHndl &&cb ) { return Entry::fromFile(s, cb.move()); }

 



Alloc_Singleton(ResourceMan);
