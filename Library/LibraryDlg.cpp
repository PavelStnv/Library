
// LibraryDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Library.h"
#include "LibraryDlg.h"
#include "afxdialogex.h"
#include "AuthorsTable.h"
#include <afxtempl.h>
#include "WorksDialog.h"
#include "Structures.h"
#include "MemoryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLibraryDlg dialog

CLibraryDlg::CLibraryDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LIBRARY_DIALOG, pParent), m_bForRefresh(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDB_AUTHOR_NAME, m_oAuthorName);
	DDX_Control(pDX, IDC_LSB_AUTHORS, m_oListBox);
}

BEGIN_MESSAGE_MAP(CLibraryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CLibraryDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_DELETE_AUTHOR, &CLibraryDlg::OnDeleteAuthor)
	ON_LBN_DBLCLK(IDC_LSB_AUTHORS, &CLibraryDlg::OnOpenAuthor)
	ON_BN_CLICKED(IDC_BTN_ADD_AUTHOR, &CLibraryDlg::OnBnClickedBtnAddAuthor)
END_MESSAGE_MAP()


// CLibraryDlg message handlers

BOOL CLibraryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_LIBRARY));
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CoInitialize(nullptr);
	ConnectToDB();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLibraryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLibraryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLibraryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HRESULT CLibraryDlg::ConnectToDB()
{
	CDBPropSet oDBPropSet(DBPROPSET_DBINIT);

	oDBPropSet.AddProperty(DBPROP_INIT_DATASOURCE, _T("DESKTOP-1K8NK9B"));
	oDBPropSet.AddProperty(DBPROP_AUTH_INTEGRATED, _T("SSPI"));
	oDBPropSet.AddProperty(DBPROP_INIT_CATALOG, _T("Library"));
	oDBPropSet.AddProperty(DBPROP_AUTH_PERSIST_SENSITIVE_AUTHINFO, false);
	oDBPropSet.AddProperty(DBPROP_INIT_LCID, 1033L);
	oDBPropSet.AddProperty(DBPROP_INIT_PROMPT, static_cast<short>(4));

	return m_oDataSource.Open(_T("SQLOLEDB.1"), &oDBPropSet);
}

void CLibraryDlg::OnBnClickedBtnSearch()
{
	CString strUserInput;
	m_oAuthorName.GetWindowTextW(strUserInput);
	strUserInput = strUserInput.Trim();

	if (strUserInput.IsEmpty())
	{
		MessageBox(_T("Моля въведете име на автор."), _T("Грешка"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_oSession.Open(m_oDataSource);

	CTypedPtrArray<CPtrArray, AUTHORS*> oAuthorsArray;

	if (!m_oAuthorsData.SelectAll(oAuthorsArray, m_oSession, strUserInput))
	{
		MessageBox(_T("Грешка при операция с база данни."), _T("Грешка"), MB_OK | MB_ICONEXCLAMATION);
		m_oSession.Close();

		return;
	}

	m_oListBox.ResetContent();

	for (int i = 0; i < oAuthorsArray.GetSize(); i++)
	{
		m_oListBox.InsertString(i, oAuthorsArray.GetAt(i)->szName);
		m_oListBox.SetItemData(i, oAuthorsArray.GetAt(i)->lID);
	}

	if (!m_bForRefresh && oAuthorsArray.GetSize() == 0)
	{
		MessageBox(_T("Няма открити автори с това име."), _T("Съобщение"), MB_OK | MB_ICONINFORMATION);
	}

	CMemoryManager::FreeMemoryFromArray(oAuthorsArray, oAuthorsArray.GetCount());
	m_oSession.Close();
}

void CLibraryDlg::OnOpenAuthor()
{
	int curSelID = m_oListBox.GetItemData(m_oListBox.GetCurSel());

	if (curSelID == -1)
	{
		MessageBox(_T("Моля изберете автор."), _T("Съобщение"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CString authorName;
	m_oListBox.GetText(m_oListBox.GetCurSel(), authorName);
	WorksDialog worksDialog(m_oSession, m_oDataSource, authorName, m_oListBox.GetItemData(m_oListBox.GetCurSel()));
	worksDialog.FillArray(curSelID);
	worksDialog.DoModal();
}


void CLibraryDlg::OnDeleteAuthor()
{
	int curSelID = m_oListBox.GetItemData(m_oListBox.GetCurSel());

	if (curSelID == -1)
	{
		MessageBox(_T("Моля изберете автор."), _T("Съобщение"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	
	m_oSession.Open(m_oDataSource);

	if (!m_oAuthorsData.DeleteRecord(m_oSession, curSelID))
	{
		MessageBox(_T("Грешка при операция с база данни."), _T("Грешка"), MB_OK | MB_ICONEXCLAMATION);
		m_oSession.Close();

		return;
	}

	m_oSession.Close();

	m_bForRefresh = TRUE;
	OnBnClickedBtnSearch();
	m_bForRefresh = FALSE;
}


void CLibraryDlg::OnBnClickedBtnAddAuthor()
{
	m_oNameOnlyDialog.m_bOpenedForAdding = TRUE;
	m_oNameOnlyDialog.m_strTitle = _T("Добави автор");

	if (m_oNameOnlyDialog.DoModal() == IDOK)
	{
		AUTHORS oAuthor;
		_tcscpy_s(oAuthor.szName, m_oNameOnlyDialog.m_strName);

		m_oSession.Open(m_oDataSource);

		if (!m_oAuthorsData.InsertRecord(m_oSession, oAuthor))
		{
			MessageBox(_T("Грешка при операция с база данни."), _T("Грешка"), MB_OK | MB_ICONEXCLAMATION);
			m_oSession.Close();

			return;
		}

		m_oSession.Close();
	}
}
