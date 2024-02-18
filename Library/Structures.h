#pragma once
#include <tchar.h>
#include <WinBase.h>

struct AUTHORS
{
	long lID;
	LPCWSTR szName;

	AUTHORS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};

struct AUTHOR_WORKS
{
	long lID;
	long lAuthorID;
	LPCWSTR szName;

	AUTHOR_WORKS()
	{
		SecureZeroMemory(this, sizeof(*this));
	}
};
