
// MeshExtractorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MeshExtractor.h"
#include "MeshExtractorDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMeshExtractorDlg dialog




CMeshExtractorDlg::CMeshExtractorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMeshExtractorDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//  m_objFileAddr = _T("");
	//  m_bsiAddr = _T("");
	//  m_objFileName = _T("");
	m_objFileDir = _T("");
}

void CMeshExtractorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_OBJADDR, m_objFileAddr);
	//  DDX_Text(pDX, IDC_BSIADDR, m_bsiAddr);
	DDX_Control(pDX, IDC_BSIADDR, m_bsiAddr);
	DDX_Control(pDX, IDC_OBJADDR, m_objAddr);
	//  DDX_Check(pDX, IDC_PROCESSALL, m_isAll);
	//  DDX_Control(pDX, IDC_PROCESSALL, m_isAll);
	DDX_Control(pDX, IDC_PROCESSALL, m_isAll);
}

BEGIN_MESSAGE_MAP(CMeshExtractorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SELECTINPUT, &CMeshExtractorDlg::OnBnClickedSelectinput)
	ON_BN_CLICKED(IDC_SELECTOUTPUT, &CMeshExtractorDlg::OnBnClickedSelectoutput)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshExtractorDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMeshExtractorDlg message handlers

BOOL CMeshExtractorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMeshExtractorDlg::OnPaint()
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
HCURSOR CMeshExtractorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMeshExtractorDlg::OnBnClickedSelectinput()
{
	// TODO: Add your control notification handler code here
	CString FilePathName;

	CFileDialog dlg(TRUE);///TRUE为OPEN对话框，FALSE为SAVE AS对话框

	if(dlg.DoModal()==IDOK)
	{
		FilePathName=dlg.GetPathName();
		m_objAddr.SetWindowTextW(FilePathName);
		// get objfilename and objfiledir
		int tmpos = FilePathName.ReverseFind('\\');
		m_objFileName = FilePathName.Right(FilePathName.GetLength() - tmpos - 1);
		m_objFileDir = FilePathName.Left(tmpos + 1);
	}




}


void CMeshExtractorDlg::OnBnClickedSelectoutput()
{
	// TODO: Add your control notification handler code here
	CString m_MonitorPath;  
	BROWSEINFO browseInfo;  
	::ZeroMemory(&browseInfo, sizeof(BROWSEINFO));//CString strPath = _T("");  
	TCHAR szBuffer[MAX_PATH]  = {0};  
	browseInfo.hwndOwner      = NULL;  
	browseInfo.pszDisplayName = szBuffer;  
	browseInfo.ulFlags        = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;  
	LPITEMIDLIST pIDList      = SHBrowseForFolder(&browseInfo);  
	if(pIDList == NULL)  
		return;  
	SHGetPathFromIDList(pIDList, szBuffer);  
	LPMALLOC lpMalloc;  
	if(FAILED(SHGetMalloc(&lpMalloc)))   
		return;  
	lpMalloc->Free(pIDList);  
	lpMalloc->Release();  
	m_bsiAddr.SetWindowTextW(szBuffer);
}


void CMeshExtractorDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

	
	if (!m_isAll.GetCheck())
	{
		CString tmp;
		//open file get all the vertices
		m_objAddr.GetWindowTextW(tmp);
		LoadObjectModel(tmp);
		//normalize
		NormalizeVertex();
		//output

		m_bsiAddr.GetWindowTextW(tmp);
		CString m_bsiFileName = CString(m_objFileName);
		int l = m_bsiFileName.GetLength();
		m_bsiFileName.SetAt(l - 1, 'i'); 
		m_bsiFileName.SetAt(l - 2, 's');
		m_bsiFileName.SetAt(l - 3, 'b');

		WriteBsiFile(tmp + L"\\" + m_bsiFileName);
	}
	else//process all
	{
		CString dirStr;
		CString tmp;
		int kk;
		//open file get all the vertices
		m_objAddr.GetWindowTextW(tmp);
		kk = tmp.ReverseFind('\\');
		tmp = tmp.Left(kk + 1);
		tmp += L"*.obj";

		CFileFind ff;
		CString strPath;
		BOOL res = ff.FindFile(tmp); 
		while( res ) 
		{ 
			res = ff.FindNextFile(); 
			strPath = ff.GetFilePath(); 
			if(ff.IsDirectory() && !ff.IsDots()) 
				;
			else if(!ff.IsDirectory() && !ff.IsDots())
			{
				//int l = strPath.GetLength();
				
	
					// this is an obj file
				LoadObjectModel(strPath);
					//normalize
				NormalizeVertex();
					//output

				m_bsiAddr.GetWindowTextW(tmp);
				int tmpos = strPath.ReverseFind('\\');
				CString m_bsiFileName = strPath.Right(strPath.GetLength() - tmpos - 1);
				int l = m_bsiFileName.GetLength();
				m_bsiFileName.SetAt(l - 1, 'i'); 
				m_bsiFileName.SetAt(l - 2, 's');
				m_bsiFileName.SetAt(l - 3, 'b');

				WriteBsiFile(tmp + L"\\" + m_bsiFileName);

			} //如果目标是个文件，则对它进行处理 
		} 
		ff.Close(); 
	}

	
}


// load file and fill vertices
void CMeshExtractorDlg::LoadObjectModel(CString objFileAddr)
{
	CStdioFile csf;
	csf.Open(objFileAddr, CFile::modeRead);
	m_vList.clear();
	CString line;
	while (csf.ReadString(line))
	{
		//check if line starts with "v "
		if (line[0] == 'v' && line[1] == ' ')
		{
			Vertex nv;
			line = line.Right(line.GetLength() - 2);
			
			CString tmp;
			int tmpk;
			
			tmpk = line.Find(' ');
			tmp = line.Left(tmpk);
			LPTSTR  chValue = tmp.GetBuffer( tmp.GetLength() ); 
			nv.x = wcstod( (WCHAR*)chValue, NULL ); 
			tmp.ReleaseBuffer();
			line = line.Right(line.GetLength() - tmp.GetLength() - 1);
			
			tmpk = line.Find(' ');
			tmp = line.Left(tmpk);
			chValue = tmp.GetBuffer( tmp.GetLength() ); 
			nv.y = wcstod( (WCHAR*)chValue, NULL ); 
			tmp.ReleaseBuffer();
			line = line.Right(line.GetLength() - tmp.GetLength() - 1);

			tmpk = line.Find(' ');
			if (tmpk != -1) 
				tmp = line.Left(tmpk);
			chValue = tmp.GetBuffer( tmp.GetLength() ); 
			nv.z = wcstod( (WCHAR*)chValue, NULL ); 
			tmp.ReleaseBuffer();
			line = line.Right(line.GetLength() - tmp.GetLength() - 1);

			m_vList.push_back(nv);
		}
	}
	csf.Close();
}


void CMeshExtractorDlg::NormalizeVertex(void)
{
	float dxmax = 0, dymax = 0, dzmax = 0;
	float dxmin = 200.0f, dymin = 200.0f, dzmin = 200.0f;
	float dd;
	for (int i = 0; i < m_vList.size(); ++i)
	{
		if (m_vList[i].x > dxmax)
		{
			dxmax = m_vList[i].x;
		}
		if (m_vList[i].y > dymax)
		{
			dymax = m_vList[i].y;
		}
		if (m_vList[i].z > dzmax)
		{
			dzmax = m_vList[i].z;
		}
		if (m_vList[i].x < dxmin)
		{
			dxmin = m_vList[i].x;
		}
		if (m_vList[i].y < dymin)
		{
			dymin = m_vList[i].y;
		}
		if (m_vList[i].z < dzmin)
		{
			dzmin = m_vList[i].z;
		}
	}

	dd = dxmax - dxmin;
	if (dd < dymax - dymin)
	{
		dd = dymax - dymin;
	}
	if (dd < dzmax - dzmin)
	{
		dd = dzmax - dzmin;
	}

	//go back through all vertices and set the normal size
	float scale = 1 / dd;
	for (int i = 0; i < m_vList.size(); ++i)
	{
		m_vList[i].x *= scale;
		m_vList[i].y *= scale;
		m_vList[i].z *= scale;
	}
}


void CMeshExtractorDlg::WriteBsiFile(CString fileName)
{
	CStdioFile csf;
	csf.Open(fileName, CFile::modeWrite | CFile::modeCreate);
	
	CString line;
	line.Format(L"%d\r\n", m_vList.size());
	csf.WriteString(line);
	for (int i = 0; i < m_vList.size(); ++i)
	{
		line.Format(L"%f %f %f\r\n", m_vList[i].x, m_vList[i].y, m_vList[i].z);
		csf.WriteString(line);
	}
	csf.Close();
}
