#include "pch.h"
#include "AuthorsData.h"


CAuthorsData::CAuthorsData()
{

}

CAuthorsData::~CAuthorsData()
{

}

BOOL CAuthorsData::SelectAll(CTypedPtrArray<CPtrArray, AUTHORS*>& oArray, CSession oSession, CString strUserInput)
{
	if (!m_oAuthorsTable.SelectAll(oArray, oSession, strUserInput))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorsData::DeleteRecord(CSession oSession, long lID)
{
	if (!m_oAuthorsTable.DeleteRecord(oSession, lID))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorsData::InsertRecord(CSession oSession, AUTHORS& recNewAuthor)
{
	if (!m_oAuthorsTable.InsertRecord(oSession, recNewAuthor))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorsData::EditAuthorName(CSession oSession, long lAuthorID, CString strNewName)
{
	if (!m_oAuthorsTable.EditAuthorName(oSession, lAuthorID, strNewName))
	{
		return FALSE;
	}

	return TRUE;
}
