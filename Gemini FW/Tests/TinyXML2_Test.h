
#include <stdio.h>
#include <time.h>

#include "Gem/Main.h"


int tinyXML2_main( int, const char** );
int tinyXML2_main2( int, const char** );

class Main { 
friend class Gem::Main_Hlpr;
	Main() {

		const char* cmdL[] = {
			{ "hi", },
		};

		u32 t1=0,t2 =0;
		for( auto i = 1; i--; ) {
			auto c1 =  clock();
			tinyXML2_main2( 0, cmdL );
			auto c2 = clock();
		//	tinyXML2_main( 0, cmdL );
			auto c3 = clock();
			t1 += c2-c1;
			t2 += c3-c2;
		}

		printf("\n  times  %d  : %d \n", t1, t2 ); 
	}

	void proc() {
		
		printf("hey ho mark2\n"); 

		for( ;; ) {

		}
	}
	

};

DEF_MainClass( Main );

