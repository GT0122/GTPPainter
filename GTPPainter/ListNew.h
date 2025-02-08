#pragma once

#define GTPPainter_HEADER WM_USER + 123

#include "HeaderNew.h"

// CListNew

class CListNew : public CListCtrl
{
	DECLARE_DYNAMIC(CListNew)

public:
	CListNew();
	virtual ~CListNew();

protected:
	DECLARE_MESSAGE_MAP()

protected:
	CRect m_rList;
	CHeaderNew m_hcNew;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual void PreSubclassWindow();
protected:
	afx_msg LRESULT OnHeader(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnPaint();
	afx_msg void OnLvnItemchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult);
};