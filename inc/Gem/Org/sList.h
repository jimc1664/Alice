#ifndef GEM_ORG_SLIST_H
#define GEM_ORG_SLIST_H

#include "../Basic.h"

namespace Gem {
	
template<class Obj > class sList  {
	typedef Obj* ObjP;
public:
	struct Node {
		Node() : Next(0) {}
	private: friend class sList; friend struct iter;
		Node *Next;
	};
	sList() {
		Root = null;
	}
	~sList() {
		Assume(Root == null);  //else we have not emptied the list..
	}
	void add( ObjP p ) {
		Node *n = static_cast<Node*>(p);
		Assume(!n->Next);
		n->Next = Root;
		Root = n;
	}
	//void detachAll() {
	//	Root = null;
	//}
	/*void detach( ObjP p ) {
		Node *n = static_cast<Node*>(p);
		n->Next->Prev = n->Prev;
		n->Prev->Next = n->Next;
	}*/


	ObjP pop() {
		Node* n = Root;
		Root = n->Next;
		n->Next = null;
		return static_cast<ObjP>(n);
	}

	struct iter {
		operator ObjP() const { return static_cast<ObjP>(Cur); }
		operator bool() const { return Cur != null; }
		ObjP operator->()		{ return static_cast<ObjP>(Cur); }
		void operator++(int)	{ Cur = Cur->Next; }
		void operator++()		{ Cur = Cur->Next; }

		bool operator==(const iter &i) const { return Cur == i.Cur; }
		bool operator!=(const iter &i) const { return Cur != i.Cur; }
	private: friend class sList;
		iter( Node *a ) : Cur(a) {}
		Node *Cur;
	};
	iter start() { return iter(Root); }
	iter begin() { return iter(Root); }
	iter end() { return iter(null); }


	bool isEmpty() const { return Root == null; }
private:
	Node *Root; 
};
template<class Obj > class sListNode : public sList<Obj>::Node { };

}

#endif //GEM_ORG_SLIST_H