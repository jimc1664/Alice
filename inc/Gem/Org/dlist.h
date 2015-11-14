#ifndef GEM_ORG_DLIST_H
#define GEM_ORG_DLIST_H

#include "../Basic.h"

namespace Gem {
	
template<class Obj > class dList  {
	typedef Obj* ObjP;
public:
	struct Node {
		Node() : Next(0), Prev(0) {}

		bool isListed() {
			return Next != 0;
		}
	private: friend class dList; friend struct iter;

		void detach( ) {
			Next->Prev = Prev;
			Prev->Next = Next;
			Next = Prev = null;
		}
		void destroy() { //careful  - no explicit override
			delete static_cast<ObjP>(this);
		}
		Node *Next, *Prev;
	};
	typedef Node* NodeP;
	dList() {
		Root.Next = Root.Prev = &Root;
	}
	~dList() {
		Assume(Root.Next == &Root);  //else we have not emptied the list..
	}
	void add( ObjP p ) {
		Node *n = static_cast<Node*>(p);
		Assume(!n->Prev && !n->Next);
		n->Prev = &Root;
		n->Next = Root.Next;
		Root.Next = (Root.Next->Prev = n);
	}
	void detach( ObjP p ) {
		Node *n = static_cast<Node*>(p);
		n->detach();
	}
	void detachAll() {		
		NodeP &n = Root.Next;
		while (n != &Root) detach(static_cast<ObjP>(n));
	}
	void deleteAll() {		
		NodeP &n = Root.Next;
		while (n != &Root) {
			auto o = static_cast<ObjP>(n);
			detach(o);
			o->destroy();
		}
	}

	struct iter {
		operator ObjP() const { return static_cast<ObjP>(Cur); }
		operator bool() const { return Cur != &Root; }
		ObjP operator->() { return static_cast<ObjP>(Cur); }
		void operator++(int) { Cur = Cur->Next; }
		void operator++()	 { Cur = Cur->Next; }
		void operator--(int)	 { Cur = Cur->Prev; }
		void operator--()	 { Cur = Cur->Prev; }

		bool operator==(const iter &i) const { return Cur == i.Cur; }
		bool operator!=(const iter &i) const { return Cur != i.Cur; }

		void detachCur( ) {
			auto c = Cur;
			Cur = c->Next;
			c->detach();
		}
	private: friend class dList;
		iter( NodeP s, Node &r ) : Cur(s), Root(r) {}
		Node *Cur, &Root;
	};


	iter start() { return iter(Root.Next, Root); }
	iter begin() { return iter(Root.Next, Root); }
	iter end() { return iter(&Root, Root); }


private:
	Node Root;
};
template<class Obj > class dListNode : public dList<Obj>::Node { };

}

#endif //GEM_ORG_DLIST_H