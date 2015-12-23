#ifndef GEM_ORG_MSGQ_H
#define GEM_ORG_MSGQ_H

#include "../Basic.h"

namespace Gem {
	
template<class Obj,  u32 BlkSize = 512 > class msgQ  {
	typedef Obj* ObjP;
public:

	msgQ() {
		clear();
	}

	~msgQ() {
		for( auto b = First.Next; b; ) {
			auto n = b->Next;
			delete b;
			b = n;
		}		
	}

	struct Node {
		//u32 proc()
	};

	Template1 T* add_NCtor() {
		static_assert(sizeof(T) < (BlkSize - sizeof(BlockHeader)-sizeof(sizet) ), "err");
		Assume((sizet)(T*)0 == (sizet)(ObjP)(T*)0);
		T* a = (T*)alloc(sizeof(T));
		auto n = static_cast<Node*>(a);
		return a;
	}
	Template1 void add() { new (add_NCtor<T>()) T (); 	}
	Template2 void add( T2 &&a ) { new (add_NCtor<T>()) T (a); 	}
	Template3 void add( T2 &&b, T3 &&c ) { new (add_NCtor<T>()) T (b, c); }
	Template4 void add( T2 &&b, T3 &&c, T4 &&d ) { new (add_NCtor<T>()) T (b, c, d); }

	void procAll( ptr cntx ) {
		
		Block* blk = &First;
		u8* at = &blk->Data[0];
		for(;;) {

			Assume((sizet)at <= (sizet)&blk[1]);
			sizet c = *(sizet*)at;
			if( (*(sizet*)at) == 0 ) {
				if (blk == Cur) break; //done
				blk = blk->Next;
				at = &blk->Data[0];
			}
			ObjP o = (ObjP)at;
			auto size = o->proc(cntx);
			at += size;
		}
	}

	void clear() {
		Cur = &First;
		Next = &First.Data[0];
		*(sizet*)Next = (sizet)0;
	}
private:

	ptr alloc( sizet s ) {
		u8* lNext = Next;
		Next += s;
		if( (sizet)Next  >= (sizet) &Cur[1] - sizeof(sizet) ) {
			//*(sizet*)lNext = (sizet)..;
			if( Cur->Next == null ) {
				Cur->Next = new Block;
			}
			Cur = Cur->Next;
			lNext = &Cur->Data[0];
			Next = lNext+s;
		} 
		*(sizet*)Next = (sizet)0;
		return lNext;
	}
	struct Block;
	struct BlockHeader {
		BlockHeader() : Next(0) {}
		Block * Next;
	};
	struct Block : public BlockHeader {// : public sListNode<Block> {
		
		u8 Data[BlkSize-sizeof(BlockHeader) ];
	};
	//sList<Block> Blocks;

	Block*		Cur;
	u8*			Next;
	//u32			Remaining;

	Block First;

	SetUncopyable(msgQ);
};
template<class Obj > class msgQNode : public msgQ<Obj>::Node { };

}

#endif //GEM_ORG_MSGQ_H