
// STM32_PRDlg.h: 头文件
//

#pragma once

// Edit控件滚动条滚动步进
#define LineScroll_STEP		1

// CSTM32PRDlg 对话框
class CSTM32PRDlg : public CDialogEx
{
// 构造
public:
	CSTM32PRDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_STM32_PR_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void KMeansSegment();
private:
	CProgressCtrl m_progress;
	CComboBox m_portnum;
	CString m_obStr;
	CEdit m_toolpath;
private:
	CString m_toolpathStr;
	CString m_err;
public:
	afx_msg void OnClose();
private:
	CString m_filepath;
public:
	afx_msg void OnBnClickedOk3();
	CString ExecuteCmd(CString str);
	TCHAR* StringToChar(CString& str);
	void PostMsg(CString str, COLORREF col = RGB(0, 0, 250), BOOL bNewLine = FALSE);

private:
	CComboBox m_baudrate;
	CComboBox m_checksum;
	CComboBox m_databit;
	CComboBox m_stopbit;
	CComboBox m_downloadmode;
	CString m_strPort;
	CString m_strBaudrate;
	CString m_strChecksum;
	CString m_strDatabit;
	CString m_strStopbit;
	CString m_strDownloadmode;
	CString m_strBaseaddr;
	CRichEditCtrl m_MsgBox;
	CStatic m_status;
	CBrush m_brush;
	CFont m_font;
	CFont m_font_present;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnSaveLog(CString path);
private:
	CString m_present;
	BOOL m_lockob;
public:
	afx_msg void OnBnClickedOk4();
//	afx_msg void OnNMThemeChangedCheck1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedCheck1();
private:
	BOOL m_autodownload;
public:
	afx_msg void OnBnClickedCheck2();
private:
	BOOL m_logenable;
public:
	afx_msg void OnBnClickedCheck3();
};

enum download_mode
{
	UART_MODE = 0,
	STLINK_SWD,
	USB_DFU,
	CAN_MODE,
};