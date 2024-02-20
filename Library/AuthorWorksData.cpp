#include "pch.h"
#include "AuthorWorksData.h"


CAuthorWorksData::CAuthorWorksData()
{

}

CAuthorWorksData::~CAuthorWorksData()
{

}

BOOL CAuthorWorksData::SelectWhereID(CTypedPtrArray<CPtrArray, AUTHOR_WORKS*>& oArray, CSession oSession, long lAuthorID)
{
	if (!m_oAuthorWorksTable.SelectWhereID(oArray, oSession, lAuthorID))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorWorksData::InsertRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork)
{
	if (!m_oAuthorWorksTable.InsertRecord(oSession, recNewAuthorWork))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorWorksData::DeleteRecord(CSession oSession, long lWorkID)
{
	if (!m_oAuthorWorksTable.DeleteRecord(oSession, lWorkID))
	{
		return FALSE;
	}

	return TRUE;
}
BOOL CAuthorWorksData::UpdateRecord(CSession oSession, AUTHOR_WORKS& recNewAuthorWork)
{
	if (!m_oAuthorWorksTable.UpdateRecord(oSession, recNewAuthorWork))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CAuthorWorksData::DeleteAllAuthorWorks(CSession oSession, long lAuthorID)
{
	if (!m_oAuthorWorksTable.DeleteAllAuthorWorks(oSession, lAuthorID))
	{
		return FALSE;
	}

	return TRUE;
}
