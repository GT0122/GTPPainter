#pragma once

#include "ViewData.h"
#include "vector"
#include "ListNew.h"

// CGTPPainterFormView 폼 보기

class CGTPPainterFormView : public CFormView
{
	DECLARE_DYNCREATE(CGTPPainterFormView)

protected:
	CGTPPainterFormView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGTPPainterFormView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CGTPPainterFormview };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
protected:
	CListNew* m_pLcView;
	int32_t m_i32X, m_i32Y;
	int32_t m_i32Row, m_i32Count;
	CSize m_sizeScroll;
	CPoint m_ptScroll;
	std::vector<CViewData> m_vctVdViews;
	std::vector<CListNew*> m_vctLcViews;
	std::vector<CListNew*>::iterator m_vctLcIter;

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
	afx_msg LRESULT OnData(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAlldata(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAddview(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDelview(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnClosing(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
};


