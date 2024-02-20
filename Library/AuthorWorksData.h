#pragma once
#include <atldbcli.h>
#include "Structures.h"
#include "AuthorWorksTable.h"


class CAuthorWorksData
{
public:
	CAuthorWorksData();
	virtual ~CAuthorWorksData();

public:
	BOOL SelectWhereID(CTypedPtrArray<CPtrArray, AUTHOR_WORKS*>& oArray, CSession oSession, long lAuthorID);
	BOOL InsertRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork);
	BOOL DeleteRecord(CSession oSession, long lWorkID);
	BOOL UpdateRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork);
	BOOL DeleteAllAuthorWorks(CSession oSession, long lAuthorID);

private:
	CAuthorWorksTable m_oAuthorWorksTable;
};

