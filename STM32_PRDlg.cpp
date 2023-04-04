
// STM32_PRDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "STM32_PR.h"
#include "STM32_PRDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


COLORREF g_color;
#define PROCESS_TIMER	0x01
long g_count = 0;
long g_countsuccess = 0;
long g_countfailed = 0;

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CSTM32PRDlg 对话框



CSTM32PRDlg::CSTM32PRDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_STM32_PR_DIALOG, pParent)
	, m_obStr(_T(""))
	, m_toolpathStr(_T(""))
	, m_filepath(_T(""))
	, m_strPort(_T(""))
	, m_strBaudrate(_T(""))
	, m_strChecksum(_T(""))
	, m_strDatabit(_T(""))
	, m_strStopbit(_T(""))
	, m_strDownloadmode(_T(""))
	, m_strBaseaddr(_T(""))
	, m_present(_T(""))
	, m_lockob(TRUE)
	, m_autodownload(FALSE)
	, m_logenable(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CSTM32PRDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_portnum);
	DDX_Text(pDX, IDC_EDIT2, m_obStr);
	DDX_Text(pDX, IDC_EDIT3, m_filepath);
	DDX_Control(pDX, IDC_COMBO2, m_baudrate);
	DDX_Control(pDX, IDC_COMBO3, m_checksum);
	DDX_Control(pDX, IDC_COMBO4, m_databit);
	DDX_Control(pDX, IDC_COMBO5, m_stopbit);
	DDX_Control(pDX, IDC_COMBO6, m_downloadmode);
	DDX_CBString(pDX, IDC_COMBO1, m_strPort);
	DDX_CBString(pDX, IDC_COMBO2, m_strBaudrate);
	DDX_CBString(pDX, IDC_COMBO3, m_strChecksum);
	DDX_CBString(pDX, IDC_COMBO4, m_strDatabit);
	DDX_CBString(pDX, IDC_COMBO5, m_strStopbit);
	DDX_CBString(pDX, IDC_COMBO6, m_strDownloadmode);
	DDX_Text(pDX, IDC_EDIT5, m_strBaseaddr);
	DDX_Control(pDX, IDC_RICHEDIT21, m_MsgBox);
	DDX_Control(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_PROGRESS1, m_progress);
	DDX_Text(pDX, IDC_progressstatus, m_present);
	DDX_Check(pDX, IDC_CHECK1, m_lockob);
	DDX_Check(pDX, IDC_CHECK2, m_autodownload);
	DDX_Check(pDX, IDC_CHECK3, m_logenable);
}

BEGIN_MESSAGE_MAP(CSTM32PRDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CSTM32PRDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSTM32PRDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK3, &CSTM32PRDlg::OnBnClickedOk3)
//	ON_WM_CTLCOLOR()
ON_WM_CTLCOLOR()
ON_WM_TIMER()
ON_BN_CLICKED(IDOK4, &CSTM32PRDlg::OnBnClickedOk4)
//ON_NOTIFY(NM_THEMECHANGED, IDC_CHECK1, &CSTM32PRDlg::OnNMThemeChangedCheck1)
ON_BN_CLICKED(IDC_CHECK1, &CSTM32PRDlg::OnBnClickedCheck1)
ON_BN_CLICKED(IDC_CHECK2, &CSTM32PRDlg::OnBnClickedCheck2)
ON_BN_CLICKED(IDC_CHECK3, &CSTM32PRDlg::OnBnClickedCheck3)
END_MESSAGE_MAP()


// CSTM32PRDlg 消息处理程序

BOOL CSTM32PRDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	if (m_lockob)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
		pEdit->EnableWindow(FALSE);
	}
	m_obStr = _T("-ob DUAL_BANK=0 nBOOT_SEL=0 IWDG_STOP=0");
	m_filepath = _T("E:\\Temp\\F4Q_Loader_20221219.bin");
	m_strBaseaddr = _T("0x08000000");
	UpdateData(FALSE);
	CString str;
	for (int i = 0; i < 25; i++)
	{
		str = _T("");
		str.Format(_T("COM%d"), i+1);
		m_portnum.AddString(str);
	}

	m_downloadmode.SetCurSel(0); // uart mode
	m_portnum.SetCurSel(0); // COM1
	m_baudrate.SetCurSel(2); // 115200
	m_checksum.SetCurSel(2); // even
	m_databit.SetCurSel(0); // 8
	m_stopbit.SetCurSel(0); // 1

	// 字的大小和格式
	m_font_present.CreatePointFont(100, _T("Times New Roman"));
	m_font.CreatePointFont(200, _T("Times New Roman"));
	// 画刷的颜色，GetSysColor(COLOR_3DFACE)是当前对话框的背景颜色
	m_brush.CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	// 初始化进度条
	m_progress.SetRange(0, 100);
	m_progress.SetStep(2);
	m_progress.SetPos(0);
	m_progress.SetBarColor(RGB(0, 0, 255));

//	m_MsgBox.EnableWindow(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSTM32PRDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CSTM32PRDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CSTM32PRDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSTM32PRDlg::KMeansSegment()
{
	m_portnum.EnableWindow(FALSE);
	m_baudrate.EnableWindow(FALSE);
	m_checksum.EnableWindow(FALSE);
	m_databit.EnableWindow(FALSE);
	m_stopbit.EnableWindow(FALSE);
	m_downloadmode.EnableWindow(FALSE);

	UpdateData(TRUE);
	CString strCmd;
	if (m_downloadmode.GetCurSel() == UART_MODE)
	{
		strCmd = CString("STM32_Programmer_CLI.exe") + _T(" ") + _T("-c") + _T(" ") + _T("port=") + m_strPort
			+ _T(" ") + _T("p=") + m_strChecksum + _T(" ") + _T("br=") + m_strBaudrate + _T(" ") + _T("db=") + m_strDatabit + _T(" ") + _T("sb=") + m_strStopbit + _T(" ")
			+ _T("-log ../log/trace.log") + _T(" ") + _T("-e all -d") + _T(" ") + _T("\"") + m_filepath + _T("\"") + _T(" ") + m_strBaseaddr + _T(" ")
			+ _T("-v") + _T(" ") + m_obStr;
	}
	else if (m_downloadmode.GetCurSel() == STLINK_SWD)
	{
		strCmd = CString("STM32_Programmer_CLI.exe") + _T(" ") + _T("-c") + _T(" ") + _T("port=swd mode=NORMAL") + _T(" ")
			+ _T("-log ../log/trace.log") + _T(" ") + _T("-e all -d") + _T(" ") + _T("\"") + m_filepath + _T("\"") + _T(" ") + m_strBaseaddr + _T(" ")
			+ _T("-v") + _T(" ") + m_obStr;
	}
	TRACE(strCmd.GetString());

	CString str;
	CString retStr;
	CButton* pButton = (CButton*)GetDlgItem(IDOK);
	if (pButton != NULL) {
		pButton->EnableWindow(FALSE);
		
		// 初始化进度条
		m_progress.SetPos(0);
		m_present = _T("");
		UpdateData(FALSE);
		// 设置定时器1s中断
		SetTimer(PROCESS_TIMER, 1000, NULL);
		PostMsg(_T("等待下载程序..."), RGB(0, 0, 250), TRUE);
		retStr = ExecuteCmd(strCmd);
		if(m_logenable)
			PostMsg(retStr, RGB(0, 0, 250), TRUE);

		int low = 0, high = 0;
		m_progress.GetRange(low, high);
		m_progress.SetPos(high);
		KillTimer(PROCESS_TIMER);
		pButton->EnableWindow(TRUE);
	}
	if ((-1 != retStr.Find(_T("File download complete"))) 
		&& (-1 != retStr.Find(_T("Download verified successfully")))
		|| (-1 != retStr.Find(_T("Option Bytes successfully programmed")) 
			|| -1 != retStr.Find(_T("Option Bytes are unchanged, Data won't be downloaded"))))
	{
		g_color = RGB(36, 177, 16);
		m_status.SetWindowText(_T("PASS"));
		m_present = _T("100%");
		PostMsg(m_present, RGB(0, 0, 250), TRUE);
		g_countsuccess++;
		PostMsg(_T("########下载程序完成，PASS！！！########"), RGB(36, 177, 16), TRUE);
//		TRACE1(_T("下载成功次数：%d\r\n"), g_countsuccess);
	}
	else
	{
		g_color = RGB(255, 0, 0);
		m_status.SetWindowText(_T("FAIL"));
		g_countfailed++;
		PostMsg(_T("########下载程序失败########"), RGB(255, 0, 0), TRUE);
//		TRACE1(_T("下载失败次数：%d\r\n"), g_countfailed);
	}
	m_progress.SetPos(0);
	UpdateData(FALSE);
	g_count++;

	if (m_autodownload)
	{
		str.Format(_T("5s后开始第%d次下载..."), g_count+1);
		PostMsg(str, RGB(0, 0, 250), TRUE);
		Sleep(5000); // 等待5s后再开始下一次
		OnBnClickedOk();
		return;
	}

	m_portnum.EnableWindow(TRUE);
	m_baudrate.EnableWindow(TRUE);
	m_checksum.EnableWindow(TRUE);
	m_databit.EnableWindow(TRUE);
	m_stopbit.EnableWindow(TRUE);
	m_downloadmode.EnableWindow(TRUE);
}

// 下载进程
UINT downloadproc(LPVOID lParam)
{
	// 将窗口指针赋给无类型指针
	CSTM32PRDlg* pWnd = (CSTM32PRDlg*)lParam;
	// 要执行的函数
	pWnd->KMeansSegment();
	return 1;
}

/*
-c	port mode等	建立连接
-d	file addr	下载程序
-rst			重启
-e				开发地址 结束地址，或全部擦除-all	擦除flash
-s				程序开始运行
*/
void CSTM32PRDlg::OnBnClickedOk()
{
	AfxBeginThread(downloadproc, (LPVOID)this);
}


void CSTM32PRDlg::OnBnClickedCancel()
{

}


void CSTM32PRDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
}


void CSTM32PRDlg::OnBnClickedOk3()
{
	static TCHAR BASED_CODE szFilter[] = 
		_T("binary files (*.bin)|*.bin|All Files (*.*)|*.*||");
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter);

	INT_PTR ret = fileDlg.DoModal();
	if (ret == IDCANCEL)
		return;

	m_filepath = fileDlg.GetPathName();
	UpdateData(FALSE);
}

// 执行cmd命令
CString CSTM32PRDlg::ExecuteCmd(CString str)
{
	SECURITY_ATTRIBUTES sa;
	HANDLE hRead = NULL, hWrite = NULL;

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	if (!CreatePipe(&hRead, &hWrite, &sa, 0))
	{
		m_err = theApp.GetLastError(GetLastError());
		TRACE(m_err);
		PostMsg(m_err, RGB(255, 0, 0), TRUE);
		return NULL;
	}

	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	si.hStdError = hWrite;
	si.hStdOutput = hWrite;
	si.wShowWindow = SW_HIDE;
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	TCHAR* cmdline = StringToChar(str);
	BOOL ret = CreateProcess(NULL, cmdline, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);

	// 进程释放句柄资源
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if (!ret)
	{
		m_err = theApp.GetLastError(GetLastError());
		TRACE(m_err);
		PostMsg(m_err, RGB(255, 0, 0), TRUE);
		return NULL;
	}
	CloseHandle(hWrite);

	char buffer[4096];
	memset(buffer, 0, 4096);
	CString output;
	DWORD byteRead;
	while (true)
	{
		if (ReadFile(hRead, buffer, 4095, &byteRead, NULL) == NULL)
			break;

		buffer[byteRead] = '\0'; // 附加字符串结束标志
		output += buffer;

		memset(buffer, 0, byteRead); // 清除缓冲区，避免出现字符堆叠问题
	}

	CloseHandle(hRead);
	return output;
}

// 其中用到一个外部函数：void StringToChar(&str);
// 这个函数的作用是CString格式的命令转化为TCHAR格式
TCHAR* CSTM32PRDlg::StringToChar(CString& str)
{
	int len = str.GetLength();
	TCHAR* tr = str.GetBuffer(len);
	str.ReleaseBuffer();
	return tr;
}

/*
 * 输出log信息到richedit编辑框
 * 参数：
 *		【str】显示的内容
 *		【col】显示的颜色，缺省黑色
 *		【bNewLine】输出完成后是否换行，TRUE：换行，FLASE：不换行，缺省FALSE
 **/
void CSTM32PRDlg::PostMsg(CString str, COLORREF col, BOOL bNewLine)
{
	CHARFORMAT cf;

	ZeroMemory(&cf, sizeof(CHARFORMAT));
	cf.cbSize = sizeof(CHARFORMAT);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_FACE | CFM_ITALIC | CFM_SIZE | CFM_UNDERLINE;
	cf.dwEffects = 0;
	cf.yHeight = 10 * 24; // 文字高度
	cf.crTextColor = col; // 文字颜色
	_tccpy(cf.szFaceName, _T("黑体")); // 设置字体

	m_MsgBox.SetSelectionCharFormat(cf);
	m_MsgBox.ReplaceSel(str);

	if (bNewLine)
		m_MsgBox.ReplaceSel(_T("\r\n"));

	for (int i = 0; i < m_MsgBox.GetLineCount(); ++i)
		m_MsgBox.LineScroll(LineScroll_STEP);
}



HBRUSH CSTM32PRDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATUS)
	{
		//pDC->SetBkColor(RGB(0, 255, 0));//背景色为绿色
		pDC->SetTextColor(g_color);//文字为
		pDC->SelectObject(&m_font);//文字为初始化文
		return m_brush;
	}
	if (pWnd->GetDlgCtrlID() == IDC_progressstatus)
	{
		pDC->SetTextColor(RGB(0, 0, 250));//文字为蓝
		pDC->SelectObject(&m_font_present);//文字为初始化文字
		return m_brush;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CSTM32PRDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case PROCESS_TIMER:
		{
			int i, j;
			m_progress.GetRange(i, j);//获取进度条表示的范围
			int npos = m_progress.GetPos();//当前进度条位置
			npos += 2; // 单击此按钮一次使进度条进度自动加5
			if (npos > j) // 如果进度条当前的进度比进度条所能表示的进度大
				npos = j; // 则进度条进度到最大位置不再前进

			m_progress.SetPos(npos); // 设置进度条新的进度       
			m_present.Format(_T("%4.0f%%"), (float)npos / (float)(j - i) * 100.0);//在static控件中显示当前的进度
			PostMsg(m_present, RGB(0, 0, 250), TRUE);
			UpdateData(FALSE);
		}
		break;
	default:
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}


/*
 * log文件流输出回调函数
 **/
static DWORD CALLBACK LogStreamOutCallback(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG* pcb)
{
	CFile* pFile = (CFile*)dwCookie;

	pFile->Write(pbBuff, cb);
	*pcb = cb;
	return 0;
}

/*
 * log文件流输出格式设置（支持普通文本格式和rtf格式）
 * 参数：
 *		【path】文件名称及路径
 **/
void CSTM32PRDlg::OnSaveLog(CString path)
{
	if (path.Find(_T(".")) == -1)
		path += _T(".rtf");

	CFile cFile(path, CFile::modeCreate | CFile::modeWrite);
	EDITSTREAM es;

	es.dwCookie = (DWORD_PTR)&cFile;
	es.pfnCallback = LogStreamOutCallback; // 注册回调函数

	int nPos = path.ReverseFind('\\');
	CString fileName = path.Right(path.GetLength() - nPos - 1);
	nPos = fileName.ReverseFind('.');
	fileName = fileName.Right(fileName.GetLength() - nPos - 1);

	if (_tccmp(fileName, _T("rtf")) == 0)
		m_MsgBox.StreamOut(SF_RTF, es);
	else
		m_MsgBox.StreamOut(SF_TEXT, es);

	PostMsg(_T("日志保存成功！"), RGB(255, 0, 0), TRUE);
}

/*
 * 响应保存log按钮事件
 **/
void CSTM32PRDlg::OnBnClickedOk4()
{
	static TCHAR BASED_CODE szFilter[] =
		_T("hex Files (*.rtf)|*.rtf|")
		_T("All Files (*.*)|*.*||");

	CString date;
	COleDateTime tim = COleDateTime::GetCurrentTime();
	date = tim.Format(_T("%Y-%m-%d_%H-%M-%S"));

	CFileDialog dlg(
		FALSE, _T(".rtf"), date,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter,
		NULL, 0, TRUE);

	INT_PTR ret = dlg.DoModal();

	if (ret == IDCANCEL)
		return;

	CString szPath = dlg.GetPathName();
	OnSaveLog(szPath);
}


//void CSTM32PRDlg::OnNMThemeChangedCheck1(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	// 该功能要求使用 Windows XP 或更高版本。
//	// 符号 _WIN32_WINNT 必须 >= 0x0501。
//	// TODO: 在此添加控件通知处理程序代码
//	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
//	if(m_lockob)
//		pEdit->EnableWindow(FALSE);
//	else
//		pEdit->EnableWindow(TRUE);
//	*pResult = 0;
//}


void CSTM32PRDlg::OnBnClickedCheck1()
{
	UpdateData();
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_EDIT2);
	if(m_lockob)
		pEdit->EnableWindow(FALSE);
	else
		pEdit->EnableWindow(TRUE);
}


void CSTM32PRDlg::OnBnClickedCheck2()
{
	UpdateData();

	UpdateData(FALSE);
}


void CSTM32PRDlg::OnBnClickedCheck3()
{
	UpdateData();

	UpdateData(FALSE);
}
