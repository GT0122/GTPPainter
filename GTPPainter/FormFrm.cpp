// FormFrm.cpp: 구현 파일
//

#include "pch.h"
#include "GTPPainter.h"
#include "FormFrm.h"


// CFormFrame

IMPLEMENT_DYNCREATE(CFormFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CFormFrame, CMDIChildWnd)
	ON_WM_CLOSE()
	ON_MESSAGE(GTPPainter_WINCLOSING, &CFormFrame::OnWinclosing)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_SIZE()
END_MESSAGE_MAP()

CFormFrame::CFormFrame() noexcept
{

}

CFormFrame::~CFormFrame()
{
}

BOOL CFormFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if(!CMDIChildWnd::PreCreateWindow(cs))
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CFormFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CFormFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG


// CFormFrame 메시지 처리기

void CFormFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	GetActiveView()->SendMessageW(GTPPainter_CLOSING);

	GetTopLevelFrame()->PostMessageW(GTPPainter_FVCLOSING);

	CMDIChildWnd::OnClose();
}


afx_msg LRESULT CFormFrame::OnWinclosing(WPARAM wParam, LPARAM lParam)
{
	GetTopLevelFrame()->PostMessageW(GTPPainter_WINCLOSING, -1);

	OnClose();

	return 0;
}


void CFormFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(lpwndpos->hwndInsertAfter == 0)
		GetTopLevelFrame()->PostMessageW(GTPPainter_FVTOP);
}

void CFormFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(nType == SIZE_MINIMIZED || nType == SIZE_RESTORED)
		GetTopLevelFrame()->SendMessageW(GTPPainter_FVTOP);
	else if(nType == SIZE_MAXIMIZED)
		GetTopLevelFrame()->PostMessageW(GTPPainter_MAXIMIZE);
}