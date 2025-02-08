// GTPPainterFormview.cpp: 구현 파일
//

#include "pch.h"
#include "GTPPainter.h"
#include "GTPPainterFormview.h"
#include "GTPPainterFormDoc.h"
#include "PointF64.h"
#include "ViewData.h"
#include "vector"

// CGTPPainterFormView

IMPLEMENT_DYNCREATE(CGTPPainterFormView, CFormView)

CGTPPainterFormView::CGTPPainterFormView()
	: CFormView(IDD_CGTPPainterFormview)
{
	m_pLcView = nullptr;
	m_i32X = 20;
	m_i32Y = 20;
	m_i32Row = 0;
	m_i32Count = 0;
	m_ptScroll = CPoint(0, 0);
}

CGTPPainterFormView::~CGTPPainterFormView()
{
}

void CGTPPainterFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGTPPainterFormView, CFormView)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(GTPPainter_DATA, &CGTPPainterFormView::OnData)
	ON_MESSAGE(GTPPainter_ALLDATA, &CGTPPainterFormView::OnAlldata)
	ON_MESSAGE(GTPPainter_ADDVIEW, &CGTPPainterFormView::OnAddview)
	ON_WM_SIZE()
	ON_MESSAGE(GTPPainter_DELVIEW, &CGTPPainterFormView::OnDelview)
	ON_MESSAGE(GTPPainter_CLOSING, &CGTPPainterFormView::OnClosing)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CGTPPainterFormView 진단

#ifdef _DEBUG
void CGTPPainterFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGTPPainterFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif _DEBUG


// CGTPPainterFormView 메시지 처리기
void CGTPPainterFormView::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GetTopLevelFrame()->PostMessageW(GTPPainter_FORM, (WPARAM)this);

	ModifyStyle(0, WS_CLIPCHILDREN);

	CSize sizeScroll(0, 0);
	SetScrollSizes(MM_TEXT, sizeScroll);
	
	_CrtSetDbgFlag(_CRTDBG_CHECK_DEFAULT_DF);
}

afx_msg LRESULT CGTPPainterFormView::OnData(WPARAM wParam, LPARAM lParam)
{
	CViewData* pVdTemp = (CViewData*)lParam;

	m_pLcView = (CListNew*)GetDlgItem(WM_USER + 1000 + pVdTemp->m_i32Id);

	CString strTemp;

	m_pLcView->SetItemText(0, 1, pVdTemp->m_strFileName);

	if(abs(pVdTemp->m_f64OffsetX) > 1000000)
		strTemp.Format(_T("%.03fe+%.0f"), pVdTemp->m_f64OffsetX / exp(floor(log(abs(pVdTemp->m_f64OffsetX)))), floor(log(abs(pVdTemp->m_f64OffsetX))));
	else
		strTemp.Format(_T("%.05f"), pVdTemp->m_f64OffsetX);

	m_pLcView->SetItemText(1, 1, strTemp);

	if(abs(pVdTemp->m_f64OffsetY) > 1000000)
		strTemp.Format(_T("%.03fe+%.0f"), pVdTemp->m_f64OffsetY / exp(floor(log(abs(pVdTemp->m_f64OffsetY)))), floor(log(abs(pVdTemp->m_f64OffsetY))));
	else
		strTemp.Format(_T("%.05f"), pVdTemp->m_f64OffsetY);

	m_pLcView->SetItemText(2, 1, strTemp);

	strTemp.Format(_T("%.05fx"), pVdTemp->m_f64Scale);
	m_pLcView->SetItemText(3, 1, strTemp);

	m_pLcView->SetItemText(4, 1, pVdTemp->m_bEdited ? _T("Yes") : _T("No"));

	return 0;
}

afx_msg LRESULT CGTPPainterFormView::OnAlldata(WPARAM wParam, LPARAM lParam)
{
	m_vctVdViews = *(std::vector<CViewData>*)lParam;
	std::vector<CViewData>::iterator vctVdIter;

	CRect rView;
	GetClientRect(rView);

	m_i32X = 20;
	m_i32Y = 20;

	CString strTemp;
	CViewData vdTemp;

	CFont font;
	font.CreatePointFont(150, L"Gulim");

	for(vctVdIter = m_vctVdViews.begin(); vctVdIter < m_vctVdViews.end(); vctVdIter++)
	{
		vdTemp = *vctVdIter;

		m_pLcView = new CListNew();

		m_pLcView->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_ALIGNLEFT | LVS_NOSCROLL | WS_BORDER | WS_CLIPCHILDREN | LVS_NOSORTHEADER, CRect(m_i32X, m_i32Y, m_i32X + 250, m_i32Y + 120), this, WM_USER + 1000 + vdTemp.m_i32Id);

		m_pLcView->SetExtendedStyle(LVS_EX_GRIDLINES);

		m_pLcView->InsertColumn(1, _T("Item"), LVCFMT_CENTER, 100, 1);
		m_pLcView->InsertColumn(2, _T("Value"), LVCFMT_CENTER, 150, 2);

		m_pLcView->GetHeaderCtrl()->EnableWindow(false);

		m_pLcView->InsertItem(0, _T("Filename"));
		m_pLcView->InsertItem(1, _T("Offset X"));
		m_pLcView->InsertItem(2, _T("Offset Y"));
		m_pLcView->InsertItem(3, _T("Scale"));
		m_pLcView->InsertItem(4, _T("IsEdited"));

		m_pLcView->SetFont(&font, true);

		m_pLcView->SetItemText(0, 1, vdTemp.m_strFileName);

		if(abs(vdTemp.m_f64OffsetX) > 1000000)
			strTemp.Format(_T("%.03fe+%.0f"), vdTemp.m_f64OffsetX / exp(floor(log(abs(vdTemp.m_f64OffsetX)))), floor(log(abs(vdTemp.m_f64OffsetX))));
		else
			strTemp.Format(_T("%.05f"), vdTemp.m_f64OffsetX);

		m_pLcView->SetItemText(1, 1, strTemp);

		if(abs(vdTemp.m_f64OffsetY) > 1000000)
			strTemp.Format(_T("%.03fe+%.0f"), vdTemp.m_f64OffsetY / exp(floor(log(abs(vdTemp.m_f64OffsetY)))), floor(log(abs(vdTemp.m_f64OffsetY))));
		else
			strTemp.Format(_T("%.05f"), vdTemp.m_f64OffsetY);

		m_pLcView->SetItemText(2, 1, strTemp);

		strTemp.Format(_T("%.05fx"), vdTemp.m_f64Scale);
		m_pLcView->SetItemText(3, 1, strTemp);

		m_pLcView->SetItemText(4, 1, vdTemp.m_bEdited ? _T("Yes") : _T("No"));

		m_i32X += 270;

		if(m_i32X + 270 > rView.Width())
		{
			m_i32X = 20;
			m_i32Y += 140;
		}

		m_vctLcViews.push_back(m_pLcView);
	}

	m_i32Row = max(rView.Width() / 260, 1);
	m_i32Count = (int32_t)m_vctVdViews.size() / m_i32Row;

	if((int32_t)m_vctVdViews.size() % m_i32Row > 0)
		m_i32Count++;

	if(rView.Width() < 290)
		m_sizeScroll.cx = 273;
	else
		m_sizeScroll.cx = 0;

	if(rView.Height() < m_i32Count * 140 + 20)
		m_sizeScroll.cy = m_i32Count * 140 + 3;
	else
		m_sizeScroll.cy = 0;

	SetScrollSizes(MM_TEXT, m_sizeScroll);

	return 0;
}

afx_msg LRESULT CGTPPainterFormView::OnAddview(WPARAM wParam, LPARAM lParam)
{
	int32_t i32Id = (int32_t)wParam;

	CRect rWindow;
	GetWindowRect(rWindow);
	rWindow.right -= 4;
	rWindow.bottom -= 4;

	CFont fontTemp;

	m_pLcView = new CListNew();

	m_pLcView->Create(WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_NOSCROLL | WS_BORDER | WS_CLIPCHILDREN | LVS_NOSORTHEADER, CRect(m_i32X - m_ptScroll.x, m_i32Y - m_ptScroll.y, m_i32X - m_ptScroll.x + 250, m_i32Y - m_ptScroll.y + 120), this, WM_USER + 1000 + i32Id);

	m_pLcView->SetExtendedStyle(LVS_EX_GRIDLINES);

	m_pLcView->InsertColumn(1, _T("Item"), LVCFMT_CENTER, 100, 1);
	m_pLcView->InsertColumn(2, _T("Value"), LVCFMT_CENTER, 150, 2);

	m_pLcView->GetHeaderCtrl()->EnableWindow(false);

	m_pLcView->InsertItem(0, _T("Filename"));
	m_pLcView->InsertItem(1, _T("Offset X"));
	m_pLcView->InsertItem(2, _T("Offset Y"));
	m_pLcView->InsertItem(3, _T("Scale"));
	m_pLcView->InsertItem(4, _T("IsEdited"));

	fontTemp.CreatePointFont(150, L"Gulim");
	m_pLcView->SetFont(&fontTemp, true);

	m_vctVdViews.push_back(CViewData(i32Id));
	m_vctLcViews.push_back(m_pLcView);

	m_i32X += 270;

	if(m_i32X + 270 > rWindow.Width())
	{
		m_i32X = 20;
		m_i32Y += 140;
	}

	m_i32Row = max(rWindow.Width() / 270, 1);
	m_i32Count = (int32_t)m_vctLcViews.size() / m_i32Row;

	if((int32_t)m_vctLcViews.size() % m_i32Row > 0)
		m_i32Count++;

	if(rWindow.Width() < 290)
		m_sizeScroll.cx = 273;
	else
		m_sizeScroll.cx = 0;

	if(rWindow.Height() < m_i32Count * 140 + 20)
		m_sizeScroll.cy = m_i32Count * 140 + 3;
	else
		m_sizeScroll.cy = 0;

	SetScrollSizes(MM_TEXT, m_sizeScroll);

	return 0;
}

void CGTPPainterFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_i32Row = max(cx / 270, 1);
	m_i32Count = ((int32_t)m_vctLcViews.size() - 1) / m_i32Row + 1;

	CRect rWindow;
	GetWindowRect(rWindow);
	rWindow.right -= 4;
	rWindow.bottom -= 4;

	cx = rWindow.Width();
	cy = rWindow.Height();
	
	if(rWindow.Width() < 290)
	{
		m_sizeScroll.cx = 273;
		m_ptScroll.x = max(0, min(m_ptScroll.x, 290 - cx));
	}
	else
	{
		m_sizeScroll.cx = 0;
		m_ptScroll.x = 0;
	}

	if(rWindow.Height() < m_i32Count * 140 + 20)
	{
		m_sizeScroll.cy = m_i32Count * 140 + 3;
		m_ptScroll.y = max(0, min(m_ptScroll.y, (m_i32Count * 140 + 20) - cy));
	}
	else
	{
		m_sizeScroll.cy = 0;
		m_ptScroll.y = 0;
	}
	
	SetScrollSizes(MM_TEXT, m_sizeScroll);

	m_i32X = 20;
	m_i32Y = 20;

	HDWP hdwp = ::BeginDeferWindowPos((int32_t)m_vctLcViews.size());

	for(m_vctLcIter = m_vctLcViews.begin(); m_vctLcIter < m_vctLcViews.end(); m_vctLcIter++)
	{
		CRect rList;

		(*m_vctLcIter)->GetWindowRect(&rList);
		ScreenToClient(&rList);

		hdwp = ::DeferWindowPos(hdwp, (*m_vctLcIter)->GetSafeHwnd(), HWND_TOP, m_i32X - m_ptScroll.x, m_i32Y - m_ptScroll.y, 250, 120, SWP_NOOWNERZORDER | SWP_NOSIZE);

		m_i32X += 270;

		if(m_i32X + 270 > cx)
		{
			m_i32X = 20;
			m_i32Y += 140;
		}
	}

	::EndDeferWindowPos(hdwp);

	SetScrollPos(SB_HORZ, m_ptScroll.x, 0);
	SetScrollPos(SB_VERT, m_ptScroll.y, 0);
}

afx_msg LRESULT CGTPPainterFormView::OnDelview(WPARAM wParam, LPARAM lParam)
{
	int32_t i32Id = (int32_t)wParam;

	CRect rWindow;
	GetWindowRect(rWindow);
	rWindow.right -= 4;
	rWindow.bottom -= 4;

	bool bBegin = true;

	m_i32X = 20;
	m_i32Y = 20;

	CListNew* pLcTemp = nullptr;
	std::vector<CListNew*>::iterator vctLcIter;
	std::vector<CListNew*>::iterator vctLcDelIter;

	for(vctLcIter = m_vctLcViews.begin(); vctLcIter < m_vctLcViews.end(); vctLcIter++)
	{
		pLcTemp = *vctLcIter;

		if((*vctLcIter)->GetDlgCtrlID() == (WM_USER + 1000 + i32Id))
		{
			m_vctLcViews.erase(vctLcIter);
			delete pLcTemp;
			pLcTemp = nullptr;
			break;
		}
	}

	HDWP hdwp = ::BeginDeferWindowPos((int32_t)m_vctLcViews.size());

	for(vctLcIter = m_vctLcViews.begin(); vctLcIter < m_vctLcViews.end(); vctLcIter++)
	{
		if(m_i32X + 270 > rWindow.Width() && !bBegin)
		{
			m_i32X = 20;
			m_i32Y += 140;
		}

		::DeferWindowPos(hdwp, (*vctLcIter)->GetSafeHwnd(), HWND_TOP, m_i32X - m_ptScroll.x, m_i32Y - m_ptScroll.y, 250, 120, SWP_NOOWNERZORDER | SWP_NOSIZE | SWP_NOCOPYBITS);

		m_i32X += 270;

		bBegin = false;
	}

	::EndDeferWindowPos(hdwp);

	std::vector<CViewData>::iterator vctVdIter;

	for(vctVdIter = m_vctVdViews.begin(); vctVdIter < m_vctVdViews.end(); vctVdIter++)
	{
		if(vctVdIter->m_i32Id == i32Id)
		{
			m_vctVdViews.erase(vctVdIter);
			break;
		}
	}

	m_i32Row = max(rWindow.Width() / 260, 1);
	m_i32Count = ((int32_t)m_vctVdViews.size()) / m_i32Row;

	if(((int32_t)m_vctVdViews.size()) % m_i32Row > 0)
		m_i32Count++;

	if(rWindow.Width() < 290)
	{
		m_sizeScroll.cx = 273;
		m_ptScroll.x = max(0, min(m_ptScroll.x, 290 - rWindow.Width()));
	}
	else
	{
		m_sizeScroll.cx = 0;
		m_ptScroll.x = 0;
	}

	if(rWindow.Height() < m_i32Count * 140 + 20)
	{
		m_sizeScroll.cy = m_i32Count * 140 + 3;
		m_ptScroll.y = max(0, min(m_ptScroll.y, (m_i32Count * 140 + 20) - rWindow.Height()));
	}
	else
	{
		m_sizeScroll.cy = 0;
		m_ptScroll.y = 0;
	}

	SetScrollSizes(MM_TEXT, m_sizeScroll);

	return 0;
}

afx_msg LRESULT CGTPPainterFormView::OnClosing(WPARAM wParam, LPARAM lParam)
{
	std::vector<CListNew*>::iterator vctLcIter;
	CListNew* pLcTemp = nullptr;

	for(vctLcIter = m_vctLcViews.begin(); vctLcIter < m_vctLcViews.end(); vctLcIter++)
	{
		pLcTemp = *vctLcIter;
		delete pLcTemp;
	}

	m_vctLcViews.clear();
	m_vctVdViews.clear();

	return 0;
}

BOOL CGTPPainterFormView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CRect rList;
	GetClientRect(&rList);

	pDC->PatBlt(0, 0, rList.Width(), rList.Height(), WHITENESS);

	return false;
}

BOOL CGTPPainterFormView::OnScrollBy(CSize sizeScroll, BOOL bDoScroll)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_ptScroll = GetScrollPosition();

	return CFormView::OnScrollBy(sizeScroll, bDoScroll);
}