
// LocalPlayDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IPC_Test.h"
#include "IPC_TestDlg.h"
#include "afxdialogex.h"

#include "StringUtility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CLocalPlayDlg dialog




CLocalPlayDlg::CLocalPlayDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLocalPlayDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLocalPlayDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CLocalPlayDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_COPYDATA()

	ON_BN_CLICKED(IDC_BUTTON1, &CLocalPlayDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLocalPlayDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CLocalPlayDlg message handlers

BOOL CLocalPlayDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetDlgItemText(IDC_EDIT2, L"abPhoto ");
	SetDlgItemText(IDC_EDIT3, L"--command load --type Photo --pbjectId \"123456789 987654321\"");
	SetDlgItemText(IDC_EDIT4, L"C:\\Program Files (x86)\\Acer\\abPhoto\\abPhoto.exe");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLocalPlayDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLocalPlayDlg::OnPaint()
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
HCURSOR CLocalPlayDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CLocalPlayDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct)
{
	string strRecievedText = (LPCSTR) (pCopyDataStruct->lpData);

	AppendLog("OnCopyData: " + strRecievedText);

	return CDialogEx::OnCopyData(pWnd, pCopyDataStruct);
}


void CLocalPlayDlg::AppendLog(string text)
{
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT1);
	int nLength = edit->GetWindowTextLength();
	edit->SetSel(nLength, nLength);

#ifdef _UNICODE
	wstring tmp = StringConv::s2ws(text)+ wstring(L"\n");
	edit->ReplaceSel(tmp.c_str());
#else
	string tmp = text + string("\n");
	edit->ReplaceSel(tmp.c_str());
#endif
}


void CLocalPlayDlg::OnBnClickedButton1()
{
	CString targetWindowName;
	if(GetDlgItemText(IDC_EDIT2, targetWindowName) == 0)
	{
		AppendLog("empty target window name");
		return;
	}

	CString data;
	if(GetDlgItemText(IDC_EDIT3, data) == 0)
	{
		AppendLog("empty data");
		return;
	}

	HWND hwnd = ::FindWindow(NULL, targetWindowName);
	if(hwnd != NULL)
	{
		string message = StringConv::cs2s(data);
		//wstring message(data);

		COPYDATASTRUCT request;
		request.dwData = 0;
		request.cbData = (message.length() + 1)/* * sizeof(wchar_t)*/;
		request.lpData = (void*)message.c_str();
		LRESULT retVal = ::SendMessage(hwnd, WM_COPYDATA, (WPARAM)m_hWnd, (LPARAM)&request);
		stringstream ss;
		ss << "data has been send..." << retVal;
		AppendLog(ss.str());
	}
	else
		AppendLog("can't find target window");

}


void CLocalPlayDlg::OnBnClickedButton2()
{
	CString targetExecutablePath;
	if(GetDlgItemText(IDC_EDIT4, targetExecutablePath) == 0)
	{
		AppendLog("empty target executable path");
		return;
	}

	CString data;
	if(GetDlgItemText(IDC_EDIT3, data) == 0)
	{
		AppendLog("empty data");
		return;
	}

	int ret = (int)ShellExecute(NULL, _T("open"), targetExecutablePath, data, NULL, SW_SHOWDEFAULT);
	if(ret > 32)
		AppendLog("launch succeed");
	else if(ret == ERROR_FILE_NOT_FOUND)
		AppendLog("The specified file was not found.");
	else
	{
		stringstream ss;
		ss << "launch failed " << ret;
		AppendLog(ss.str());
	}
}
