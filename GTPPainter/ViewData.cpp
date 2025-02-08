#include "pch.h"
#include "ViewData.h"

CViewData::CViewData()
{
	m_i32Id = 0;
	m_strFileName = L"";
	m_f64OffsetX = 0;
	m_f64OffsetY = 0;
	m_f64Scale = 0;
	m_bEdited = false;
}

CViewData::CViewData(int32_t i32Id)
{
	m_i32Id = i32Id;
	m_strFileName = L"";
	m_f64OffsetX = 0;
	m_f64OffsetY = 0;
	m_f64Scale = 0;
	m_bEdited = false;
}

CViewData::CViewData(double f64OffsetX, double f64OffsetY, double f64Scale, bool bEdited)
{
	m_i32Id = 0;
	m_strFileName = L"";
	m_f64OffsetX = f64OffsetX;
	m_f64OffsetY = f64OffsetY;
	m_f64Scale = f64Scale;
	m_bEdited = bEdited;
}

CViewData::CViewData(int32_t i32Id, CString strFileName, double f64OffsetX, double f64OffsetY, double f64Scale, bool bEdited)
{
	m_i32Id = i32Id;
	m_strFileName = strFileName;
	m_f64OffsetX = f64OffsetX;
	m_f64OffsetY = f64OffsetY;
	m_f64Scale = f64Scale;
	m_bEdited = bEdited;
}

CViewData::~CViewData()
{
}
