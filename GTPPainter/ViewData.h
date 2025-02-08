#pragma once

class CViewData
{
public:
	CViewData();
	CViewData(int32_t i32Id);
	CViewData(double f64OffsetX, double f64OffsetY, double f64Scale, bool bEdited);
	CViewData(int32_t i32Id, CString m_strFileName, double f64OffsetX, double f64OffsetY, double f64Scale, bool bEdited);
	virtual ~CViewData();

public:
	int32_t m_i32Id;
	CString m_strFileName;
	double m_f64OffsetX, m_f64OffsetY, m_f64Scale;
	bool m_bEdited;
};

