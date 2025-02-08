// ListNew.cpp: 구현 파일
//

#include "pch.h"
#include "GTPPainter.h"
#include "ListNew.h"
#include "HeaderNew.h"


// CListNew

IMPLEMENT_DYNAMIC(CListNew, CListCtrl)

CListNew::CListNew()
{
}

CListNew::~CListNew()
{
	m_hcNew.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CListNew, CListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(GTPPainter_HEADER, &CListNew::OnHeader)
	ON_WM_PAINT()
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGING, &CListNew::OnLvnItemchanging)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CListNew::OnNMCustomdraw)
END_MESSAGE_MAP()



// CListNew 메시지 처리기

BOOL CListNew::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return false;
}

void CListNew::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CListCtrl::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GetClientRect(m_rList);

	SetBkColor(RGB(255, 255, 255));
	SetTextBkColor(RGB(255, 255, 255));
}

void CListNew::PreSubclassWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	PostMessage(GTPPainter_HEADER);

	CListCtrl::PreSubclassWindow();
}

afx_msg LRESULT CListNew::OnHeader(WPARAM wParam, LPARAM lParam)
{
	CHeaderCtrl* pHcTemp = nullptr;
	pHcTemp = GetHeaderCtrl();
	if(pHcTemp != NULL)
		VERIFY(m_hcNew.SubclassWindow(pHcTemp->m_hWnd));

	pHcTemp = nullptr;

	return 0;
}

void CListNew::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CListCtrl::OnPaint()을(를) 호출하지 마십시오.
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);

	CBitmap bmpTemp;
	bmpTemp.CreateCompatibleBitmap(&dc, m_rList.Width() + 2, m_rList.Height() + 2);
	CBitmap* pBmpOld = dcMemory.SelectObject(&bmpTemp);

	CWnd::DefWindowProcW(WM_PAINT, (WPARAM)dcMemory.GetSafeHdc(), 0);

	dc.BitBlt(0, 0, m_rList.Width() + 2, m_rList.Height() + 2, &dcMemory, 2, 2, SRCCOPY);
}

void CListNew::OnLvnItemchanging(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if((pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVNI_SELECTED))
		*pResult = 1;
	else
		*pResult = 0;
}

void CListNew::OnNMCustomdraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLVCUSTOMDRAW pNMCD = reinterpret_cast<LPNMLVCUSTOMDRAW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if(pNMCD->nmcd.dwDrawStage == CDDS_PREPAINT)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if(pNMCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	else if(pNMCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		*pResult = CDRF_DODEFAULT;

		if(pNMCD->nmcd.dwItemSpec == 0 && pNMCD->iSubItem == 1)
		{
			CDC* pDC = CDC::FromHandle(pNMCD->nmcd.hdc);

			CRect rItem;
			GetSubItemRect(0, 1, 0, rItem);
			rItem.left += 2;
			rItem.top += 2;

			CString strTemp = GetItemText(0, 1);

			pDC->PatBlt(100, 0, 150, 100, WHITENESS);
			pDC->DrawTextW(strTemp, rItem, DT_CENTER | DT_WORD_ELLIPSIS);

			ReleaseDC(pDC);

			*pResult = CDRF_SKIPDEFAULT;
		}
	}
}
