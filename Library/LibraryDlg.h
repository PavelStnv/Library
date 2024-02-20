#pragma once
#include <atldbcli.h>
#include "NameOnlyDialog.h"
#include "AuthorsData.h"


class CLibraryDlg : public CDialogEx
{
// Construction
public:
	CLibraryDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIBRARY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CDataSource m_oDataSource;
	CSession m_oSession;
	CListBox m_oListBox;
	CEdit m_oAuthorName;
	CNameOnlyDialog m_oNameOnlyDialog;
	BOOL m_bForRefresh;
	CAuthorsData m_oAuthorsData;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	HRESULT ConnectToDB();

public:
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnDeleteAuthor();
	afx_msg void OnOpenAuthor();
	afx_msg void OnBnClickedBtnAddAuthor();
};
