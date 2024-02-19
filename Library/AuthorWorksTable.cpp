#include "pch.h"
#include "AuthorWorksTable.h"
#include <afxtempl.h>
#include <tchar.h>
#include <afxdb.h>
#include "MemoryManager.h"

CAuthorWorksTable::CAuthorWorksTable()
{

}

CAuthorWorksTable::~CAuthorWorksTable()
{

}

BOOL CAuthorWorksTable::SelectWhereID(CTypedPtrArray<CPtrArray, AUTHOR_WORKS*>& oArray, CSession oSession, long lAuthorID)
{
	CMemoryManager::FreeMemoryFromArray(oArray, oArray.GetCount());

	CString strQuery;
	strQuery.Format(_T("SELECT * FROM AUTHOR_WORKS WHERE AUTHOR_ID = %d"), lAuthorID);

	Open(oSession, strQuery);

	while (MoveNext() == S_OK)
	{
		AUTHOR_WORKS* pAuthorWorks = DEBUG_NEW AUTHOR_WORKS;
		*pAuthorWorks = m_recRecord;

		oArray.Add(pAuthorWorks);
	}

	CCommand::Close();

	return TRUE;
}

BOOL CAuthorWorksTable::InsertRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork)
{
	CDBPropSet oInsertDBPropSet(DBPROPSET_ROWSET);
	oInsertDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oInsertDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

	CString query;
	query.Format(_T("SELECT TOP (0) * FROM AUTHOR_WORKS"));

	Open(oSession, query, &oInsertDBPropSet);

	MoveLast();
	m_recRecord = recNewAuthorWork;
	Insert(1);
	MoveNext();
	Close();

	return TRUE;
}

BOOL CAuthorWorksTable::DeleteRecord(CSession oSession, long lWorkID)
{
	if (MessageBox(GetActiveWindow(), _T("Сигурни ли сте, че искате да изтриете творбата?"), _T("Съобщение"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CString query;
		query.Format(_T("SELECT * FROM AUTHOR_WORKS WHERE ID = %d"), lWorkID);

		CDBPropSet oDeleteDBPropSet(DBPROPSET_ROWSET);
		oDeleteDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

		Open(oSession, query, &oDeleteDBPropSet);
		MoveFirst();
		Delete();
		Close();

		MessageBox(GetActiveWindow(), _T("Творбата е изтрита успешно."), _T("Съобщение"), MB_OK | MB_ICONINFORMATION);
	}

	return TRUE;
}

BOOL CAuthorWorksTable::UpdateRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork)
{
	CString query;
	query.Format(_T("SELECT * FROM AUTHOR_WORKS WITH (UPDLOCK) WHERE ID = %d"), recNewAuthorWork.lID);

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

	Open(oSession, query, &oUpdateDBPropSet);

	MoveFirst();
	m_recRecord = recNewAuthorWork;
	SetData(1);
	oSession.Commit();
	Close();

	return TRUE;
}

BOOL CAuthorWorksTable::DeleteAllAuthorWorks(CSession oSession, long lAuthorID)
{
	CString query;
	query.Format(_T("SELECT * FROM AUTHOR_WORKS WHERE AUTHOR_ID = %d"), lAuthorID);

	CDBPropSet oDeleteDBPropSet(DBPROPSET_ROWSET);
	oDeleteDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

	Open(oSession, query, &oDeleteDBPropSet);

	while (MoveNext() == S_OK)
	{
		Delete();
	}

	Close();

	MessageBox(GetActiveWindow(), _T("Творбите са изтрити успешно."), _T("Съобщение"), MB_OK | MB_ICONINFORMATION);

	return TRUE;
}