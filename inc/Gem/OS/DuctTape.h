#ifndef GEM_OS_DUCTTAPE_H
#define GEM_OS_DUCTTAPE_H

namespace Gem {		namespace DuctTape {

#ifdef DEBUG

#ifdef _WINDOWS_
	void activateForModule( HMODULE mod = NULL );
	void deactivateForModule( HMODULE mod = NULL );
	void activateForModule( const char *name );  //todo char?
	void deactivateForModule( const char *name );
#else 
	void activateForModule( const char *name = NULL);  //todo char?
	void deactivateForModule( const char *name = NULL);
#endif
	void printAll( bool unloadSymbolsAfter = false );
	void shutdown(); //deactivateForModule + printAll
#else
	void activateForModule( ptr mod = NULL ) {}
	void deactivateForModule( ptr mod = NULL ) {}
	void printAll() {}
	void shutdown() {}
#endif //DEBUG
}	}


#endif //GEM_OS_DUCTTAPE_H