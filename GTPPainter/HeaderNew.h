#pragma once


// CHeaderNew

class CHeaderNew : public CHeaderCtrl
{
	DECLARE_DYNAMIC(CHeaderNew)

public:
	CHeaderNew();
	virtual ~CHeaderNew();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};


