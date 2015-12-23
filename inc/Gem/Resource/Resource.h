#ifndef GEM_RESOURCE_RESOURCE_H
#define GEM_RESOURCE_RESOURCE_H

#include "../Basic.h"
#include "../String/DStr.h"
#include "../Org/Ary.h"
//#include "../Org/Ary.h"
#include "../ConCur/ConCur.h"
#include "../ConCur/CritSec.h"
#include "../ConCur/RefCounter.h"
#include "../ConCur/Atomic.h"

#include <map> //todo...
#include <string> 
namespace Gem { 

class WorkHost;

class Resource {

};


//yetAnotherCallbackSystem
class CallBack { 
friend class CallBackHndl;
	virtual void proc() = 0; 
	RefCounter Ref;

protected: 
	CallBack() : Ref(1) {}
//public:

};

class CallBackHndl_MoveHlpr { //
	CallBackHndl_MoveHlpr(  CallBack * cb ) : Cb(cb) {
		
	}
	friend class CallBackHndl;
	CallBack * Cb;
public:
	CallBackHndl_MoveHlpr( CallBackHndl_MoveHlpr &&o ) : Cb(o.Cb) {
		o.Cb = null;
	}
	CallBackHndl_MoveHlpr( const CallBackHndl_MoveHlpr &o ) : Cb(o.Cb) {
		const_cast<CallBackHndl_MoveHlpr*>(&o)->Cb = null;  //force
	}
	void operator = ( const CallBackHndl_MoveHlpr &o ) {
		Assume(!Cb);
		Cb = o.Cb;
		const_cast<CallBackHndl_MoveHlpr*>(&o)->Cb = null;  //force
	}
	~CallBackHndl_MoveHlpr() {
		Assume(!Cb);
	}
	
};

class CallBackHndl {
public: 
	CallBackHndl( Nothing ) : Cb(null) { }
	CallBackHndl( const CallBackHndl_MoveHlpr &h) : Cb(h.Cb) { 
		const_cast<CallBackHndl_MoveHlpr*>(&h)->Cb = null; }
	CallBackHndl( CallBackHndl &&o ) : Cb(o.Cb) {
		o.Cb = null;
	}
	~CallBackHndl() {
		if(Cb && Cb->Ref.decRef())
			Cb->proc();	
	}
	static CallBackHndl create(CallBack * cb) { return CallBackHndl(cb); }

	CallBackHndl_MoveHlpr moveHlpr() {
		auto cb = Cb; 
		Cb = null;
		return CallBackHndl_MoveHlpr(cb);;
	}
	/* void dropCheck() { 
		if(Cb && Cb->Ref.decRef())
			Cb->proc();	

		Cb = null;
	} */
	CallBackHndl move() {
		auto cb = Cb; Cb = null;
		return CallBackHndl(cb);
	}
	CallBackHndl copy() const {
		Assume(Cb);
		u32 prev = Cb->Ref.incRef(); 
		Assume(prev != 0);
		return CallBackHndl(Cb);
	}
	void set(CallBack * cb  ) {
	
	}
	void operator = (CallBackHndl &&o  ) {
		Assume(!Cb);
		Cb = o.Cb;
		o.Cb = null;
	}

	operator bool() const { return Cb != null;  }
protected:
	CallBackHndl( CallBack * cb ) : Cb(cb) {
		
	}
	CallBack * Cb;
	SetUncopyable(CallBackHndl);
};


template<class T > struct CallBackHlpr : public CallBack {
	CallBackHlpr( const T& f ) : Foo(f) {
	//	TaskHost<Cntx>::task( this );
	}
	void proc( ) override {
		Foo();
		//delete this;
		this->~CallBackHlpr();
		_aligned_free(this);
	}
//	static TaskHlpr* ctor( const T &a ) { return new CallbackHlpr(a); }
	T Foo;
};

template<class T, class P> struct CallBackHlprPrm : public CallBack {
	CallBackHlprPrm( const T& f  ) : Foo(f) {
	//	TaskHost<Cntx>::task( this );
	}
	void proc( ) override {
		Foo( Prm );
		//delete this;
		this->~CallBackHlprPrm();
		_aligned_free(this);
	}
	T Foo;

	class Hndl : public CallBackHndl {
	public: 
		Hndl( Hndl &&o ) : CallBackHndl(o) { }
		static Hndl create(CallBack * cb) { return Hndl(cb); }
		P* operator->() const { return &static_cast<CallBackHlprPrm*>(Cb)->Prm; }
	private:
		Hndl( CallBack * cb ) : CallBackHndl(cb) { }
		SetUncopyable(Hndl);
	};
public:
	P Prm;
};

template<class T> CallBackHndl callBackHlpr( const T & a ) {
	return CallBackHndl::create( new(_aligned_malloc(sizeof(CallBackHlpr<T>), 32) )  CallBackHlpr<T>(a) );
}
template<class P, class T> typename CallBackHlprPrm<T,P>::Hndl callBackHlprPrm( const T & a ) {
	return CallBackHlprPrm<T,P>::Hndl::create( new(_aligned_malloc(sizeof(CallBackHlprPrm<T,P>), 32) )  CallBackHlprPrm<T,P>(a) );
}
#define newCallBack( foo ) callBackHlpr( [=]() { foo; }  )

#define newCallBackPrm( prm, foo ) callBackHlprPrm<prm>( [=] ( prm &p ) { foo }); 
		
class ResourceEntry_Base {
public:
	ResourceEntry_Base( const CStr &s) : Res(0), Name(s), RefC(1), Lock(1) {}

	volatile Resource* Res;
	DStr Name;

protected:
	virtual ~ResourceEntry_Base() {}	 //todo... aligned...
	RefCounter RefC;
	Atomic Lock;
	SetUncopyable(ResourceEntry_Base);
//	Base* get(
};



template<class ResTyp> class ResourceEntry : public ResourceEntry_Base {
	ResourceEntry( const CStr &s ) : ResourceEntry_Base(s) {}
	virtual ~ResourceEntry() {}
public:
	class ResourceHndl {
	friend class ResourceEntry;
		ResourceHndl( ResourceEntry& re ) :  Re(&re) { }

	public:
		ResourceHndl(  ) :  Re(0) {  }
		 
		ResourceHndl( const ResourceHndl& a ) :  Re(a.Re) { 
			if(Re) Re->incRef();
		}
		 
		void operator=( const ResourceHndl& a ) {
			Assume(Re == 0);
			Re = a.Re;
			if(Re) Re->incRef();
		}		 
		void operator=( ResourceHndl&& a ) {
			Assume(Re == 0);
			Re = a.Re;
			a.Re = 0;
		}
		ResourceHndl( ResourceHndl &&a ) : Re(a.Re) {
			a.Re = 0;
		}
		~ResourceHndl() {
			if(Re) Re->decRef();
		}

		const ResTyp* operator ->() const{ return static_cast<ResTyp*>( (Resource*)Re->Res ); }
	private:
		ResourceEntry * Re;
	};
	static ResourceHndl fromFile(const CStr &s, CallBackHndl &&cb );
private:	friend class ResourceHndl;

	u32 incRef();
	void decRef(); 

	SetUncopyable(ResourceEntry);
};


class Rsrc_ParsedShader; class Rsrc_Raw;

class ResourceMan : public Singleton<ResourceMan> {

public:
	CritSec CS;
	template<class ResTyp> std::map<CStr, ResourceEntry<ResTyp>*>& resListing();
private:
#define AddResourceListing( ResTyp, Listing ) \
	std::map<CStr, ResourceEntry<ResTyp>*> Listing;   \
	friend class ResourceEntry<ResTyp>; \
	template<> std::map<CStr, ResourceEntry<ResTyp>*>& resListing() { return Listing; }

	AddResourceListing(Rsrc_Raw, RawResources);
	AddResourceListing(Rsrc_ParsedShader, ParsedShaders );		
};



template<class Base> class Resource_T : public Resource {
public:
	typedef ResourceEntry<Base> Entry;
	typedef typename Entry::ResourceHndl Hndl;
protected: 
	Resource_T(  Entry &e ) : Ent(e) {}	
	Entry &Ent;
};  
class Rsrc_Raw : public Resource_T<Rsrc_Raw>  {
public:
	static Hndl fromFile(const CStr &s, CallBackHndl &&cb = Nothing() );
	CStr str() const { return CStr(Data, Sz); }
private: friend class Entry;
	static void fromFile( WorkHost&, Entry &e, CallBackHndl &&cb  );
	Rsrc_Raw( Entry &e, const u32 s ) : Resource_T(e), Sz(s) {}
	const u32 Sz;
	char Data[1];
};



template<class ResTyp> class ResourceDat  {
protected:
	volatile ResTyp* Hndl;
	CritSec CS; //todo - don't really need CS per resource handle... more elegant solution trivial

	template<typename ResDat> ResTyp* get( ResDat &rd  ) {
		if(Hndl) return (ResTyp*)Hndl;
		CritSec::Handle h(CS);
		if(Hndl) return (ResTyp*)Hndl;
		else 
			return (ResTyp*)(Hndl = rd.prep() );
	}
public:
	ResourceDat() { 	Hndl = 0; }
};


template<typename ResTyp, typename Typ1, typename Typ2 = Nothing, typename Typ3 = Nothing> class Res_From_T;
template<typename ResTyp, typename Typ1> class Res_From_T<ResTyp, Typ1, Nothing, Nothing> : public ResourceDat<ResTyp> {
	Typ1 A;
friend class ResourceDat<ResTyp>;
	ResTyp* prep() { return ResTyp::prep(A); }
public:
	Template1 Res_From_T( T &&a ) : A(a) {}
	operator ResTyp& () { return *get(*this);  }
};
template<typename ResTyp, typename Typ1, typename Typ2> class Res_From_T<ResTyp, Typ1, Typ2, Nothing> : public ResourceDat<ResTyp> {
	Typ1 A; Typ2 B;
friend class ResourceDat<ResTyp>;
	ResTyp* prep() { return ResTyp::prep(A, B); }
public:
	Template2 Res_From_T( T &&a, T2 &&b) : A(a), B(b) {}
	operator ResTyp& () { return *get(*this);  }
};


template<class ResTyp> u32 ResourceEntry<ResTyp>::incRef() {

	return RefC.incRef();
}
template<class ResTyp> void ResourceEntry<ResTyp>::decRef() {

	bool f = RefC.decRef();
	int a = 0;
}

template<class ResTyp> typename ResourceEntry<ResTyp>::ResourceHndl ResourceEntry<ResTyp>::fromFile(const CStr &s, CallBackHndl &&cb ) { 
	
	auto &rm = ResourceMan::ptr();

	CritSec::Handle hndl(rm.CS); //todo - read write lock?? //https://msdn.microsoft.com/en-us/library/windows/desktop/aa904937(v=vs.85).aspx
		//also this scope may not be optimal

	printf("  Worker %d :: fromfile %s --get %s \n", Thread::id(), typeid(ResTyp).name(), s.str());

	auto& resources = rm.resListing<ResTyp>();
	auto it = resources.find(s);
	if( it != resources.end() ) {
		Assume(dynamic_cast<ResourceEntry*>(it->second));		
		auto &re = *static_cast<ResourceEntry*>(it->second);
		 
		u32 preRef = re.incRef(); Assume(preRef != 0);
		if(  !re.Res ) {
			if( !re.Lock && re.Lock.trySet(1, 0 ) ) { //not currently loading and this thread now has transferrable priority  ...  ( double  !re.Res   is intentional )
				if( !re.Res ) {
					auto reP = &re;
					auto pass = cb.moveHlpr();
					TaskSchedule(WorkHost,  //todo - no new thread if this is allready worker thread  -- also in this case important to release rm.CS efficently 
						printf("  Worker %d :: fromfile %s -- create %s \n", Thread::id(), typeid(ResTyp).name(), reP->Name.str());
						ResTyp::fromFile(cntx, *reP, CallBackHndl(pass).move() );				
						//CallBackHndl cb(pass);
						//printf("  Worker %d :: fromfile %s -- done %s \n", Thread::id(), typeid(ResTyp).name(), reP->Name.str());
					);
				}
			} else {
				printf("  Worker %d :: fromfile %s -- spinning for %s \n", Thread::id(), typeid(ResTyp).name(), s.str());
				//something else is loading it..  need to append callback to loading threads callback to run our callback (in another thread)  - non trivial - do later 
				hndl.forceUnlock();
				for(; !re.Res;) 
					ConCur::sleep(1);   //for now spin-sleep till it's done    ------ obvious if thing fails to load we're stuck    -- nicely crashing not priority
				hndl.forceLock();
			}
		}
		return ResourceHndl(re); 
	} else {

		auto p = _aligned_malloc(sizeof(ResourceEntry), 32);
		auto &re = * new(p) ResourceEntry(s); 

		auto reP = &re;
		auto pass = cb.moveHlpr();
		TaskSchedule(WorkHost,  //todo - no new thread if this is allready worker thread  -- also in this case important to release rm.CS efficently 
			printf("  Worker %d :: fromfile %s  -- create %s \n", Thread::id(), typeid(ResTyp).name(), reP->Name.str());
			//CallBackHndl cb(pass);
			ResTyp::fromFile(cntx, *reP, CallBackHndl(pass).move() );
			//printf("  Worker %d :: fromfile %s -- done %s \n", Thread::id(), typeid(ResTyp).name(), reP->Name.str());
		);
		resources[re.Name] = &re;


		return ResourceHndl(re);
	}	
}

}

#endif //GEM_RESOURCE_RESOURCE_H 