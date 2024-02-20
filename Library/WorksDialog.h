#pragma once
#include <atldbcli.h>
#include "Structures.h"
#include "NameOnlyDialog.h"
#include "AuthorsData.h"
#include "AuthorWorksData.h"

enum AuthorNameState
{
	AuthorNameStateNotEditting,
	AuthorNameStateInProcessOfEditting
};

class WorksDialog : public CDialogEx
{
	// Construction
public:
	WorksDialog(CSession oSession, CDataSource oDataSource, CString authorName,
		long lAuthorID, CWnd* pParent = nullptr);	// standard constructor
	virtual ~WorksDialog();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIEW_AUTHOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CSession m_oSession;
	CTypedPtrArray<CPtrArray, AUTHOR_WORKS*> m_oArray;
	CListBox m_oWorksListBox;
	CDataSource m_oDataSource;
	CString m_strAuthorName;
	CEdit m_oAuthorName;
	CNameOnlyDialog m_oNameOnlyDialog;
	long m_lCurrentAuthorID;
	AuthorNameState m_eAuthorNameState;
	CAuthorsData m_oAuthorsData;
	CAuthorWorksData m_oAuthorWorksData;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void FillArray(long lAuthorID);
	void FillLSBWithArray();
	void ResetListBox();

	afx_msg void OnBnClickedEdbAddWork();
	afx_msg void OnBnClickedEdbDeleteWork();
	afx_msg void OnBnClickedEdbEditWork();
	afx_msg void OnBnClickedBtnDeleteAllWorks();
	afx_msg void OnBnClickedBtnEditAuthorName();
};
