
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include "ViewData.h"
#include "vector"

class CMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame() noexcept;

// 특성입니다.
public:
	int32_t m_i32Thick, m_i32ChildIndex, m_i32ChildNum, m_i32ChildId;
	COLORREF m_sLineColor;
	bool m_bModify, m_bInform, m_bInformFront;
	CString m_strTitle, m_strFileName, m_strDefaultName;
	CComboBox m_cbThick;
	CComboBox m_cbColor;
	CString m_strPath, m_strDefaultPath;
	CView* m_pViewInform;
	std::vector<CViewData> m_vctVdViews;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif
	void OnCbThicknessChange();
	void OnCbColorChange();
	bool CheckCrc32(CString strFile, CString strPath);
	void CalcCrc32(uint32_t* pU32Crc32val, int8_t* pI8Char, const int32_t i32Size);
	uint32_t ConvertToInt(int8_t* pI8Char);

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnColorCustom();
	afx_msg void OnBlack();
	afx_msg void OnLred();
	afx_msg void OnLime();
	afx_msg void OnBlue();
	afx_msg void OnUpdateBlack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLred(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLime(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBlue(CCmdUI* pCmdUI);
	afx_msg void OnUpdateColorCustom(CCmdUI* pCmdUI);
	afx_msg void OnThick1();
	afx_msg void OnThick3();
	afx_msg void OnThick5();
	afx_msg void OnUpdateThick1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateThick5(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSaveNew();
	afx_msg void OnFileSaveAsNew();
	afx_msg void OnCloseNew();
	afx_msg void OnUpdateFileSaveNew(CCmdUI* pCmdUI);
	afx_msg void OnUpdateClose(CCmdUI* pCmdUI);

protected:
	afx_msg LRESULT OnDrawing(WPARAM wParam, LPARAM lParam);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClose();

protected:
	afx_msg LRESULT OnClosing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThickness(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWinclosing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnForm(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnNewInform();
	afx_msg void OnUpdateNewInform(CCmdUI* pCmdUI);
protected:
	afx_msg LRESULT OnFvclosing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnFvtop(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
public:
	afx_msg void OnNewFiles(NMHDR* pNMHDR, LRESULT* pResult);
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnMaximize(WPARAM wParam, LPARAM lParam);
};


