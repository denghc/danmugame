
// MeshExtractorDlg.h : header file
//

#pragma once
#include <vector>
struct Vertex
{
	float x;
	float y;
	float z;
};

// CMeshExtractorDlg dialog
class CMeshExtractorDlg : public CDialogEx
{
// Construction
public:
	CMeshExtractorDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MESHEXTRACTOR_DIALOG };

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
	afx_msg void OnBnClickedSelectinput();
//	CString m_objFileAddr;
//	CString m_bsiAddr;
	afx_msg void OnBnClickedSelectoutput();
	CEdit m_bsiAddr;
	CEdit m_objAddr;
	afx_msg void OnBnClickedButton2();
	// load file and fill vertices
	void LoadObjectModel(CString objFileAddr);
	void NormalizeVertex(void);
	void WriteBsiFile(CString);
//	CString m_objFileName;
	CString m_objFileName;
	CString m_objFileDir;

	std::vector<Vertex> m_vList;
//	BOOL m_isAll;
//	CButton m_isAll;
	CButton m_isAll;
};
