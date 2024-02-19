#include "pch.h"
#include "WorksDialog.h"
#include "resource.h"
#include "AuthorWorksTable.h"
#include "AuthorsTable.h"
#include "MemoryManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WorksDialog::WorksDialog(CSession oSession, CDataSource oDataSource, CString authorName, long lAuthorID, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIEW_AUTHOR, pParent), m_oSession(oSession), m_oDataSource(oDataSource),
	m_strAuthorName(authorName), m_lCurrentAuthorID(lAuthorID)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

WorksDialog::~WorksDialog()
{
	CMemoryManager::FreeMemoryFromArray(m_oArray, m_oArray.GetCount());
}

void WorksDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(WorksDialog, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_EDB_ADD_WORK, &WorksDialog::OnBnClickedEdbAddWork)
	ON_BN_CLICKED(IDC_EDB_DELETE_WORK, &WorksDialog::OnBnClickedEdbDeleteWork)
	ON_BN_CLICKED(IDC_EDB_EDIT_WORK, &WorksDialog::OnBnClickedEdbEditWork)
	ON_BN_CLICKED(IDC_BTN_DELETE_ALL_WORKS, &WorksDialog::OnBnClickedBtnDeleteAllWorks)
	ON_BN_CLICKED(IDC_BTN_EDIT_AUTHOR_NAME, &WorksDialog::OnBnClickedBtnEditAuthorName)
END_MESSAGE_MAP()


BOOL WorksDialog::OnInitDialog()
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
	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_WORKS));
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_oWorksListBox = (CListBox*)GetDlgItem(IDC_LSB_WORKS);
	FillLSBWithArray();
	m_oAuthorName = (CEdit*)GetDlgItem(IDC_EDB_AUTHOR_NAME2);
	m_oAuthorName->SetWindowTextW(m_strAuthorName);
	m_oAuthorName->EnableWindow(FALSE);
	m_oAuthorName->SetLimitText(50);
	m_eAuthorNameState = AuthorNameStateNotEditting;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void WorksDialog::OnPaint()
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
HCURSOR WorksDialog::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void WorksDialog::FillArray(long lAuthorID)
{
	m_oSession.Open(m_oDataSource);

	CAuthorWorksTable oAuthorWorksTable;
	oAuthorWorksTable.SelectWhereID(m_oArray, m_oSession, lAuthorID);

	m_oSession.Close();
}

void WorksDialog::FillLSBWithArray()
{
	m_oWorksListBox->ResetContent();

	for (int i = 0; i < m_oArray.GetSize(); i++)
	{
		m_oWorksListBox->InsertString(i, m_oArray.GetAt(i)->szName);
		m_oWorksListBox->SetItemData(i, m_oArray.GetAt(i)->lID);
	}
}

void WorksDialog::ResetListBox()
{
	CMemoryManager::FreeMemoryFromArray(m_oArray, m_oArray.GetCount());
	m_oArray.RemoveAll();
	FillArray(m_lCurrentAuthorID);
	FillLSBWithArray();
}

void WorksDialog::OnBnClickedEdbAddWork()
{
	m_oNameOnlyDialog.m_bOpenedForAdding = TRUE;
	m_oNameOnlyDialog.m_strTitle = _T("Добави творба.");

	if (m_oNameOnlyDialog.DoModal() == IDOK)
	{
		AUTHOR_WORKS oAuthorWorks;
		oAuthorWorks.lAuthorID = m_lCurrentAuthorID;
		_tcscpy_s(oAuthorWorks.szName, m_oNameOnlyDialog.m_strName);

		m_oSession.Open(m_oDataSource);

		CAuthorWorksTable oAuthorWorksTable;
		oAuthorWorksTable.InsertRecord(m_oSession, oAuthorWorks);

		m_oSession.Close();

		ResetListBox();
	}
}


void WorksDialog::OnBnClickedEdbDeleteWork()
{
	int curSelID = m_oWorksListBox->GetItemData(m_oWorksListBox->GetCurSel());

	if (curSelID == -1)
	{
		MessageBox(_T("Моля изберете творба."), _T("Съобщение"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	m_oSession.Open(m_oDataSource);

	CAuthorWorksTable oAuthorWorksTable;
	oAuthorWorksTable.DeleteRecord(m_oSession, curSelID);

	m_oSession.Close();

	ResetListBox();
}


void WorksDialog::OnBnClickedEdbEditWork()
{
	int curSelID = m_oWorksListBox->GetItemData(m_oWorksListBox->GetCurSel());

	if (curSelID == -1)
	{
		MessageBox(_T("Моля изберете творба."), _T("Съобщение"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	CString strSelectedWorkName;
	m_oWorksListBox->GetText(m_oWorksListBox->GetCurSel(), strSelectedWorkName);
	m_oNameOnlyDialog.m_strName = strSelectedWorkName;
	m_oNameOnlyDialog.m_bOpenedForAdding = FALSE;
	m_oNameOnlyDialog.m_strTitle = _T("Редактирай творба.");

	if (m_oNameOnlyDialog.DoModal() == IDOK)
	{
		AUTHOR_WORKS oAuthorWorks;
		oAuthorWorks.lAuthorID = m_lCurrentAuthorID;
		oAuthorWorks.lID = curSelID;
		_tcscpy_s(oAuthorWorks.szName, m_oNameOnlyDialog.m_strName);


		m_oSession.Open(m_oDataSource);

		CAuthorWorksTable oAuthorWorksTable;
		oAuthorWorksTable.UpdateRecord(m_oSession, oAuthorWorks);

		m_oSession.Close();

		ResetListBox();
	}
}


void WorksDialog::OnBnClickedBtnDeleteAllWorks()
{
	if (MessageBox(_T("Сигурни ли сте че искате да изтриете всички творби на автора?"), _T("Съобщение"), MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
	{
		m_oSession.Open(m_oDataSource);

		CAuthorWorksTable oAuthorWorksTable;
		oAuthorWorksTable.DeleteAllAuthorWorks(m_oSession, m_lCurrentAuthorID);

		m_oSession.Close();

		ResetListBox();
	}
}


void WorksDialog::OnBnClickedBtnEditAuthorName()
{
	// TODO: Add your control notification handler code here
	if (m_eAuthorNameState == AuthorNameStateNotEditting)
	{
		m_oAuthorName->EnableWindow(TRUE);
		m_eAuthorNameState = AuthorNameStateInProcessOfEditting;
		SetDlgItemText(IDC_BTN_EDIT_AUTHOR_NAME, _T("Запази"));
	}
	else if (m_eAuthorNameState = AuthorNameStateInProcessOfEditting)
	{
		CString strNewName;
		m_oAuthorName->GetWindowTextW(strNewName);

		if (m_strAuthorName.Compare(strNewName))
		{
			m_oSession.Open(m_oDataSource);

			CAuthorsTable oAuthorsTable;
			oAuthorsTable.EditAuthorName(m_oSession, m_lCurrentAuthorID, strNewName);

			m_oSession.Close();
		}

		m_oAuthorName->EnableWindow(FALSE);
		m_eAuthorNameState = AuthorNameStateNotEditting;
		SetDlgItemText(IDC_BTN_EDIT_AUTHOR_NAME, _T("Редактирай"));
	}
}
