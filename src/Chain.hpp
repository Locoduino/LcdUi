//-------------------------------------------------------------------
#ifndef __LCDUIChain_H__
#define __LCDUIChain_H__
//-------------------------------------------------------------------

template<class T> class LCDUICHAINEDLISTITEM
{
public:
	T *Obj;					// Element carried by the link.
	LCDUICHAINEDLISTITEM *pNext;

	inline LCDUICHAINEDLISTITEM<T>() { this->pNext = NULL; }
};

template<class T> class LCDUICHAINEDLIST
{
public:							
	LCDUICHAINEDLISTITEM<T> *pFirst;		// Address of next element in chain, or NULL for none.
	LCDUICHAINEDLISTITEM<T> *pCurrentItem;	// Address of the cutrent item.
	
	inline LCDUICHAINEDLIST() { this->pFirst = NULL; this->pCurrentItem = NULL; }
	void AddItem(T *t);
	void NextCurrent();
	void PreviousCurrent();
	byte GetCount();
};

#define CHAIN_ENUMERATE(T, list, func) LCDUICHAINEDLISTITEM<T> *pCurr = list.pFirst; while (pCurr != NULL) { func(pCurr->Obj); pCurr = pCurr->pNext; }

// This function appends element into chain.
template<class T> void LCDUICHAINEDLIST<T>::AddItem(T *t)
{
	LCDUICHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (pCurr == NULL)
	{
		this->pFirst = new LCDUICHAINEDLISTITEM<T>();
		this->pCurrentItem = this->pFirst;
		pCurr = this->pFirst;
	}
	else
	{
		while (pCurr->pNext != NULL)
			pCurr = pCurr->pNext;

		pCurr->pNext = new LCDUICHAINEDLISTITEM<T>();
		pCurr = pCurr->pNext;
	}

	pCurr->pNext = NULL;
	pCurr->Obj = t;
}

// This function move the current item to the next in the chain.
template<class T> void LCDUICHAINEDLIST<T>::NextCurrent()
{
	if (this->pFirst == NULL)
		return;

	this->pCurrentItem = this->pCurrentItem->pNext;

	if (this->pCurrentItem == NULL)
		this->pCurrentItem = this->pFirst;

	return;
}

// This function move the current item to the next in the chain.
template<class T> void LCDUICHAINEDLIST<T>::PreviousCurrent()
{
	if (this->pFirst == NULL)
		return;

	LCDUICHAINEDLISTITEM<T> *pCurr = this->pFirst;

	if (this->pCurrentItem == pCurr)
		this->pCurrentItem = NULL;

	while (pCurr != NULL || this->pCurrentItem == NULL)
	{
		if (pCurr->pNext == this->pCurrentItem)
		{
			this->pCurrentItem = pCurr;
			return;
		}
		pCurr = pCurr->pNext;
	}
}

// This function count the actual number of items..
template<class T> byte LCDUICHAINEDLIST<T>::GetCount()
{
	if (this->pFirst == NULL)
		return 0;

	byte count = 1;
	LCDUICHAINEDLISTITEM<T> *pCurr = this->pFirst->pNext;

	while (pCurr != NULL)
	{
		pCurr = pCurr->pNext;
		count++;
	}

	return count;
}
#endif