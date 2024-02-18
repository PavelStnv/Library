#pragma once
#include <atldbcli.h>
#include "Structures.h"

class CNameOnlyDialog : public CDialogEx
{
	// Construction
public:
	CNameOnlyDialog( CWnd* pParent = nullptr);	// standard constructor
	~CNameOnlyDialog();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NAME_ONLY };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;
	
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	
	DECLARE_MESSAGE_MAP()
public:
	CString m_strName;
	CEdit m_edbName;
	CString m_strTitle;
	BOOL m_bOpenedForAdding;
	void EndTheDialog(int Result);

	afx_msg void OnButtonOK();
};
