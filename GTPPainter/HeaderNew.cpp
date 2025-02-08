// HeaderNew.cpp: 구현 파일
//

#include "pch.h"
#include "GTPPainter.h"
#include "HeaderNew.h"


// CHeaderNew

IMPLEMENT_DYNAMIC(CHeaderNew, CHeaderCtrl)

CHeaderNew::CHeaderNew()
{

}

CHeaderNew::~CHeaderNew()
{
}


BEGIN_MESSAGE_MAP(CHeaderNew, CHeaderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CHeaderNew 메시지 처리기




BOOL CHeaderNew::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return false;
}

void CHeaderNew::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.
	CDC dcMemory;
	dcMemory.CreateCompatibleDC(&dc);
	CRect rClient;
	GetClientRect(rClient);

	CBitmap bmpTemp;
	bmpTemp.CreateCompatibleBitmap(&dc, rClient.Width(), rClient.Height());
	CBitmap* pBmpOld = dcMemory.SelectObject(&bmpTemp);

	CWnd::DefWindowProcW(WM_PAINT, (WPARAM)dcMemory.GetSafeHdc(), 0);

	dc.BitBlt(0, 0, rClient.Width(), rClient.Height(), &dcMemory, 0, 0, SRCCOPY);
}
