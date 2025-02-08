#pragma once

// CFormFrame 프레임

class CFormFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFormFrame)
protected:
	CFormFrame() noexcept;           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CFormFrame();

// 작업입니다.
public:

// 재정의입니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
	#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
protected:
	afx_msg LRESULT OnWinclosing(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


