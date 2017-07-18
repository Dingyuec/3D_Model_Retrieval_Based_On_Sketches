
// Retrieval SystemDlg.h : 头文件
//

#pragma once
#include "stdafx.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "cv.h"  
#include "cxcore.h"  
#include "highgui.h" 
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>  
#include <string>  
#include <sstream> 
#include <afxtempl.h>




#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>  
//#include "CvvImage.h"
#include "afxeditbrowsectrl.h"




// CRetrievalSystemDlg 对话框
class CRetrievalSystemDlg : public CDialogEx
{
// 构造
public:
	CRetrievalSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_RETRIEVALSYSTEM_DIALOG };

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
};
