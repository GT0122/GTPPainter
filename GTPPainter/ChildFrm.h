
// ChildFrm.h: CChildFrame 클래스의 인터페이스
//

#pragma once

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame() noexcept;

// 특성입니다.
protected:
	CSplitterWnd m_wndSplitter;
public:
	int32_t m_i32Thick, m_i32ChildNum;
	COLORREF m_sLineColor;
	bool m_bModify;
	CString m_strFileName, m_strDefaultName;
	CString m_strPath, m_strDefaultPath;
	CComboBox m_cbThick;
	CComboBox m_cbColor;

// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

protected:
	afx_msg LRESULT OnDrawing(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSaving(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSavingAs(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOpening(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClosing(WPARAM wParam, LPARAM lParam);

public:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnClose();

protected:
	afx_msg LRESULT OnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnThickness(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnWinclosing(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
protected:
	afx_msg LRESULT OnDefaultn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddview(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnTopfr(WPARAM wParam, LPARAM lParam);
};
