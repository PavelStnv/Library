#include "pch.h"
#include "AuthorsTable.h"
#include "AuthorWorksTable.h"
#include <afxtempl.h>
#include <tchar.h>
#include <afxdb.h>


CAuthorsTable::CAuthorsTable()
{

}

CAuthorsTable::~CAuthorsTable()
{

}

BOOL CAuthorsTable::SelectAll(CTypedPtrArray<CPtrArray, AUTHORS*>& oArray, CSession oSession, CString strUserInput)
{
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM AUTHORS WHERE NAME LIKE '%%%s%%'"), strUserInput);

	Open(oSession, strQuery);

	while (MoveNext() == S_OK)
	{
		AUTHORS* pAuthor = new AUTHORS;
		pAuthor->lID = m_recRecord.lID;
		pAuthor->szName = _wcsdup(m_recRecord.szName);

		oArray.Add(pAuthor);
	}

	Close();

	return TRUE;
}

BOOL CAuthorsTable::DeleteRecord(CSession oSession, long lID)
{
	CTypedPtrArray<CPtrArray, AUTHOR_WORKS*> oArray;
	CAuthorWorksTable oAuthorWorksTable;
	oAuthorWorksTable.SelectWhereID(oArray, oSession, lID);

	if (oArray.GetCount() > 0)
	{
		MessageBox(GetActiveWindow(), _T("Трябва първо да изтриете творбите на автора."), _T("Съобщение"), MB_OKCANCEL | MB_ICONQUESTION);
		oArray.RemoveAll();
		return FALSE;
	}

	oArray.RemoveAll();

	if (MessageBox(GetActiveWindow(), _T("Сигурни ли сте, че искате да изтриете автора?"), _T("Съобщение"), MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CString strQuery;

		strQuery.Format(_T("SELECT * FROM AUTHORS WHERE ID = %d"), lID);

		CDBPropSet oDeleteDBPropSet(DBPROPSET_ROWSET);
		oDeleteDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

		Open(oSession, strQuery, &oDeleteDBPropSet);
		MoveFirst();
		Delete();
		Close();

		MessageBox(GetActiveWindow(), _T("Авторът е изтрит успешно."), _T("Съобщение"), MB_OK | MB_ICONINFORMATION);
	}

	return TRUE;
}

BOOL CAuthorsTable::InsertRecord(CSession oSession, AUTHORS& recNewAuthor)
{
	CDBPropSet oInsertDBPropSet(DBPROPSET_ROWSET);
	oInsertDBPropSet.AddProperty(DBPROP_IRowsetScroll, true);
	oInsertDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

	CString strQuery;
	strQuery.Format(_T("SELECT TOP (0) * FROM AUTHORS"));

	Open(oSession, strQuery, &oInsertDBPropSet);

	MoveLast();
	m_recRecord = recNewAuthor;
	Insert(1);
	MoveNext();
	Close();

	return TRUE;
}

BOOL CAuthorsTable::EditAuthorName(CSession oSession, long lAuthorID, CString strNewName)
{
	CString strQuery;
	strQuery.Format(_T("SELECT * FROM AUTHORS WITH (UPDLOCK) WHERE ID = %d"), lAuthorID);

	AUTHORS oAuthor;
	oAuthor.lID = lAuthorID;
	oAuthor.szName = strNewName;

	CDBPropSet oUpdateDBPropSet(DBPROPSET_ROWSET);
	oUpdateDBPropSet.AddProperty(DBPROP_IRowsetChange, true);

	Open(oSession, strQuery, &oUpdateDBPropSet);

	MoveFirst();
	m_recRecord = oAuthor;
	SetData(1);
	oSession.Commit();
	Close();

	return TRUE;
}
