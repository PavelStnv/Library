#pragma once
#include <afxtempl.h>


class CMemoryManager
{
public:
	template <class T>
	static void FreeMemoryFromArray(CTypedPtrArray<CPtrArray, T*>& oArray, int size)
	{
		for (int i = 0; i < size; i++)
		{
			T* pHeap = oArray.GetAt(i);
			delete pHeap;
		}
	}
};