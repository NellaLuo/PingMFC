// pingMFCDlg.h : header file
//

#if !defined(AFX_PINGMFCDLG_H__CE1366A6_5592_4C44_B5B9_B33D4DA90415__INCLUDED_)
#define AFX_PINGMFCDLG_H__CE1366A6_5592_4C44_B5B9_B33D4DA90415__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CPingMFCDlg dialog

class CPingMFCDlg : public CDialog
{
// Construction
public:
	void fill_icmp_data(char * icmp_data, int datasize);
	USHORT checksum(USHORT *buffer, int size);
	int decode_resp(char *buf, int bytes,struct sockaddr_in *from);
	void Usage(char *progname);
	CPingMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CPingMFCDlg)
	enum { IDD = IDD_PINGMFC_DIALOG };
	CString	m_ip;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPingMFCDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CPingMFCDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnCancel();
	afx_msg void Onping();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PINGMFCDLG_H__CE1366A6_5592_4C44_B5B9_B33D4DA90415__INCLUDED_)
