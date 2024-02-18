#pragma once
#include "Structures.h"
#include <atldbcli.h>

class CAuthorWorksAccessor
{
protected:
	AUTHOR_WORKS m_recRecord;

	BEGIN_ACCESSOR_MAP(CAuthorWorksAccessor, 2)
		BEGIN_ACCESSOR(0, true)
		COLUMN_ENTRY(1, m_recRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, true)
		COLUMN_ENTRY(2, m_recRecord.lAuthorID)
		COLUMN_ENTRY(3, m_recRecord.szName)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

class CAuthorWorksTable : private CCommand<CAccessor<CAuthorWorksAccessor>>
{
public:
	CAuthorWorksTable();
	virtual ~CAuthorWorksTable();

	BOOL SelectWhereID(CTypedPtrArray<CPtrArray, AUTHOR_WORKS*>& oArray, CSession oSession, long lAuthorID);
	BOOL InsertRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork);
	BOOL DeleteRecord(CSession oSession, long lWorkID);
	BOOL UpdateRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork);
	BOOL DeleteAllAuthorWorks(CSession oSession, long lAuthorID);
};