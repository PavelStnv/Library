#pragma once
#include <atldbcli.h>
#include "Structures.h"
#include "AuthorsTable.h"


class CAuthorsData
{
public:
	CAuthorsData();
	virtual ~CAuthorsData();

public:
	BOOL SelectAll(CTypedPtrArray<CPtrArray, AUTHORS*>& oArray, CSession oSession, CString strUserInput);
	BOOL DeleteRecord(CSession oSession, long lID);
	BOOL InsertRecord(CSession oSession, AUTHORS& recNewAuthor);
	BOOL EditAuthorName(CSession oSession, long lAuthorID, CString strNewName);

private:
	CAuthorsTable m_oAuthorsTable;
};

