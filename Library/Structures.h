#pragma once
#include <tchar.h>
#include <WinBase.h>

struct AUTHORS
{
	long lID;
	TCHAR szName[51];

	AUTHORS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

struct AUTHOR_WORKS
{
	long lID;
	long lAuthorID;
	TCHAR szName[51];

	AUTHOR_WORKS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};
