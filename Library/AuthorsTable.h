#pragma once
#include <atldbcli.h>
#include "Structures.h"

class CAuthorsAccessor
{
protected:
	AUTHORS m_recRecord;

	BEGIN_ACCESSOR_MAP(CAuthorsAccessor, 2)
		BEGIN_ACCESSOR(0, true)
		COLUMN_ENTRY(1, m_recRecord.lID)
		END_ACCESSOR()

		BEGIN_ACCESSOR(1, true)
		COLUMN_ENTRY(2, m_recRecord.szName)
		END_ACCESSOR()
	END_ACCESSOR_MAP()
};

class CAuthorsTable : private CCommand<CAccessor<CAuthorsAccessor>>
{
public:
	CAuthorsTable();
	virtual ~CAuthorsTable();

	BOOL SelectAll(CTypedPtrArray<CPtrArray, AUTHORS*>& oArray, CSession oSession, CString strUserInput);
	BOOL DeleteRecord(CSession oSession, long lID);
	BOOL InsertRecord(CSession oSession, AUTHORS& recNewAuthor);
	BOOL EditAuthorName(CSession oSession, long lAuthorID, CString strNewName);
};