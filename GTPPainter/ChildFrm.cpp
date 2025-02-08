
// ChildFrm.cpp: CChildFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "GTPPainter.h"
#include "ChildFrm.h"
#include "ViewData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CChildFrame

IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_MESSAGE(GTPPainter_DRAWING, &CChildFrame::OnDrawing)
	ON_WM_CONTEXTMENU()
	ON_WM_ACTIVATEAPP()
	ON_WM_CLOSE()
	ON_MESSAGE(GTPPainter_SAVING, &CChildFrame::OnSaving)
	ON_MESSAGE(GTPPainter_SAVING_AS, &CChildFrame::OnSavingAs)
	ON_MESSAGE(GTPPainter_OPENING, &CChildFrame::OnOpening)
	ON_MESSAGE(GTPPainter_CLOSING, &CChildFrame::OnClosing)
	ON_MESSAGE(GTPPainter_COLOR, &CChildFrame::OnColor)
	ON_MESSAGE(GTPPainter_THICKNESS, &CChildFrame::OnThickness)
	ON_MESSAGE(GTPPainter_WINCLOSING, &CChildFrame::OnWinclosing)
	ON_WM_WINDOWPOSCHANGED()
	ON_MESSAGE(GTPPainter_DEFAULTN, &CChildFrame::OnDefaultn)
	ON_MESSAGE(GTPPainter_DATA, &CChildFrame::OnData)
	ON_MESSAGE(GTPPainter_ADDVIEW, &CChildFrame::OnAddview)
	ON_WM_SIZE()
	ON_MESSAGE(GTPPainter_TOPFR, &CChildFrame::OnTopfr)
END_MESSAGE_MAP()

// CChildFrame 생성/소멸

CChildFrame::CChildFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bAutoMenuEnable = FALSE;
	m_i32Thick = 1;
	m_sLineColor = RGB(0, 0, 0);
	m_bModify = false;
	m_strFileName = "";
	m_i32ChildNum = 0;
}

CChildFrame::~CChildFrame()
{
}


BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서 Window 클래스 또는 스타일을 수정합니다.
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}

// CChildFrame 진단

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}
#endif //_DEBUG

// CChildFrame 메시지 처리기

afx_msg LRESULT CChildFrame::OnDrawing(WPARAM wParam, LPARAM lParam)
{
	if(lParam == 0)
		m_bModify = (bool)wParam;

	GetTopLevelFrame()->PostMessageW(GTPPainter_DRAWING, m_bModify);

	return 0;
}

void CChildFrame::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu menuPopup;

	menuPopup.LoadMenuW(IDR_POPUP);
	CMenu* pMenuSubPopup = menuPopup.GetSubMenu(0);

	pMenuSubPopup->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());

	GetActiveView()->PostMessage(GTPPainter_POPUP);
}

void CChildFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(!m_bModify)
	{
		GetTopLevelFrame()->PostMessageW(GTPPainter_CLOSING, m_i32ChildNum);
		CFrameWnd::OnClose();
	}
	else
	{
		int32_t i32Result = MessageBox(_T("There is an unsaved document. Do you want to save it?"), _T("Close"), MB_YESNOCANCEL);

		if(i32Result == IDYES)
		{
			if(m_strFileName == "")
			{
				CFileDialog dlgFile(FALSE, _T(".flp"), m_strDefaultName, OFN_OVERWRITEPROMPT, _T("Fourth Logic Paint(*.flp)|*.flp|모든 파일(*.*)|*.*|"), this);
				i32Result = (int32_t)dlgFile.DoModal();

				if(i32Result == IDOK)
				{
					m_strFileName = dlgFile.GetFileName();
					m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

					m_strPath = dlgFile.GetPathName();
					m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));

					GetActiveDocument()->SetPathName(m_strPath);
					GetActiveDocument()->SetTitle(m_strFileName);
					GetActiveView()->PostMessage(GTPPainter_SAVING, -1, false);
				}
			}
			else
			{
				GetActiveDocument()->SetTitle(m_strFileName);
				GetActiveView()->PostMessage(GTPPainter_SAVING, -1, false);
			}
		}
		else if(i32Result == IDNO)
		{
			GetTopLevelFrame()->PostMessageW(GTPPainter_CLOSING, m_i32ChildNum);
			CFrameWnd::OnClose();
		}
	}
}

afx_msg LRESULT CChildFrame::OnClosing(WPARAM wParam, LPARAM lParam)
{
	if((bool)wParam)
	{
		if((bool)lParam)
			GetTopLevelFrame()->PostMessageW(GTPPainter_WINCLOSING, m_i32ChildNum);
		else
			GetTopLevelFrame()->PostMessageW(GTPPainter_CLOSING, m_i32ChildNum);

		CFrameWnd::OnClose();
	}
	else
		OnClose();
	
	return 0;
}

afx_msg LRESULT CChildFrame::OnSaving(WPARAM wParam, LPARAM lParam)
{
	if(m_bModify)
	{
		if(m_strFileName != "")
		{
			SetWindowText(m_strFileName);
			GetActiveDocument()->SetTitle(m_strFileName);
			GetActiveView()->SendMessage(GTPPainter_SAVING);
		}
		else
		{
			CFileDialog dlgFile(FALSE, _T(".flp"), m_strDefaultName, OFN_OVERWRITEPROMPT, _T("Fourth Logic Paint(*.flp)|*.flp|모든 파일(*.*)|*.*|"), this);
			int32_t i32Result = (int32_t)dlgFile.DoModal();

			if(i32Result == IDOK)
			{
				m_strFileName = dlgFile.GetFileName();
				m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

				m_strPath = dlgFile.GetPathName();
				m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));

				GetActiveDocument()->SetPathName(m_strPath);
				GetActiveDocument()->SetTitle(m_strFileName);

				SetWindowText(m_strFileName);

				GetActiveView()->SendMessage(GTPPainter_SAVING);
			}

			GetActiveView()->PostMessage(GTPPainter_POPUP);
		}
	}

	return 0;
}

afx_msg LRESULT CChildFrame::OnSavingAs(WPARAM wParam, LPARAM lParam)
{
	CFileDialog dlgFile(FALSE, _T(".flp"), m_strDefaultName, OFN_OVERWRITEPROMPT, _T("Fourth Logic Paint(*.flp)|*.flp|모든 파일(*.*)|*.*|"), this);
	int32_t i32Result = (int32_t)dlgFile.DoModal();

	if(i32Result == IDOK)
	{
		m_strFileName = dlgFile.GetFileName();
		m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

		m_strPath = dlgFile.GetPathName();
		m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));

		GetActiveDocument()->SetPathName(m_strPath);
		GetActiveDocument()->SetTitle(m_strFileName);

		SetWindowText(m_strFileName);

		GetActiveView()->SendMessage(GTPPainter_SAVING);
	}

	GetActiveView()->PostMessage(GTPPainter_POPUP);

	return 0;
}

afx_msg LRESULT CChildFrame::OnOpening(WPARAM wParam, LPARAM lParam)
{
	m_strFileName = GetActiveDocument()->GetTitle();

	GetActiveView()->PostMessage(GTPPainter_LOADING);

	return 0;
}

afx_msg LRESULT CChildFrame::OnColor(WPARAM wParam, LPARAM lParam)
{
	m_sLineColor = (COLORREF)wParam;

	GetActiveView()->PostMessage(GTPPainter_COLOR, m_sLineColor);

	return 0;
}

afx_msg LRESULT CChildFrame::OnThickness(WPARAM wParam, LPARAM lParam)
{
	m_i32Thick = (int32_t)wParam;

	GetActiveView()->PostMessage(GTPPainter_THICKNESS, m_i32Thick);

	return 0;
}

afx_msg LRESULT CChildFrame::OnWinclosing(WPARAM wParam, LPARAM lParam)
{
	if(!m_bModify)
	{
		GetTopLevelFrame()->PostMessage(GTPPainter_WINCLOSING, m_i32ChildNum);
		CFrameWnd::OnClose();
	}
	else
	{
		int32_t i32Result = MessageBox(_T("There is an unsaved document. Do you want to save it?"), _T("Close"), MB_YESNOCANCEL);

		if(i32Result == IDYES)
		{
			if(m_strFileName == "")
			{
				CFileDialog dlgFile(FALSE, _T(".flp"), m_strDefaultName, OFN_OVERWRITEPROMPT, _T("Fourth Logic Paint(*.flp)|*.flp|모든 파일(*.*)|*.*|"), this);
				i32Result = (int32_t)dlgFile.DoModal();

				if(i32Result == IDOK)
				{
					m_strFileName = dlgFile.GetFileName();
					m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

					m_strPath = dlgFile.GetPathName();
					m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));

					GetActiveDocument()->SetPathName(m_strPath);
					GetActiveDocument()->SetTitle(m_strFileName);
					GetActiveView()->PostMessage(GTPPainter_SAVING, -1, true);
				}
			}
			else
			{
				GetActiveDocument()->SetTitle(m_strFileName);
				GetActiveView()->PostMessage(GTPPainter_SAVING, -1, true);
			}
		}
		else if(i32Result == IDNO)
		{
			GetTopLevelFrame()->PostMessage(GTPPainter_WINCLOSING, m_i32ChildNum);
			CFrameWnd::OnClose();
		}
	}

	return 0;
}

void CChildFrame::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CMDIChildWnd::OnWindowPosChanged(lpwndpos);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(lpwndpos->hwndInsertAfter == 0)
	{
		GetTopLevelFrame()->SendMessageW(GTPPainter_FVTOP);
		GetTopLevelFrame()->SendMessageW(GTPPainter_DRAWING, m_bModify);
		GetTopLevelFrame()->SendMessageW(GTPPainter_COLOR, m_sLineColor);
		GetTopLevelFrame()->SendMessageW(GTPPainter_THICKNESS, m_i32Thick);
	}
}

afx_msg LRESULT CChildFrame::OnDefaultn(WPARAM wParam, LPARAM lParam)
{
	CString* pStrTemp = (CString*)lParam;
	m_strDefaultName = pStrTemp->GetString();

	return 0;
}

afx_msg LRESULT CChildFrame::OnData(WPARAM wParam, LPARAM lParam)
{
	CViewData* pVdTemp = (CViewData*)lParam;
	CViewData vdTemp = *pVdTemp;
	vdTemp.m_i32Id = m_i32ChildNum;

	if(m_strFileName == "")
	{
		CString strTemp = GetActiveDocument()->GetTitle();

		if(vdTemp.m_bEdited)
			strTemp = strTemp.Right(strTemp.GetLength() - 1);

		vdTemp.m_strFileName = strTemp;
	}
	else
		vdTemp.m_strFileName = m_strFileName;

	GetTopLevelFrame()->SendMessageW(GTPPainter_DATA, NULL, (LPARAM)&vdTemp);

	return 0;
}


afx_msg LRESULT CChildFrame::OnAddview(WPARAM wParam, LPARAM lParam)
{
	m_i32ChildNum = (int32_t)wParam;

	CViewData vdTemp = CViewData(m_i32ChildNum);

	if(m_strFileName == "")
		vdTemp.m_strFileName = GetActiveDocument()->GetTitle();
	else
		vdTemp.m_strFileName = m_strFileName;

	GetTopLevelFrame()->SendMessageW(GTPPainter_DATA, NULL, (LPARAM)&vdTemp);

	return 0;
}


void CChildFrame::OnSize(UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if(nType == SIZE_MINIMIZED || nType == SIZE_RESTORED)
		GetTopLevelFrame()->PostMessageW(GTPPainter_FVTOP);
	else if(nType == SIZE_MAXIMIZED)
		GetTopLevelFrame()->PostMessageW(GTPPainter_MAXIMIZE);
}


afx_msg LRESULT CChildFrame::OnTopfr(WPARAM wParam, LPARAM lParam)
{
	GetTopLevelFrame()->SendMessageW(GTPPainter_DRAWING, m_bModify);
	GetTopLevelFrame()->SendMessageW(GTPPainter_COLOR, m_sLineColor);
	GetTopLevelFrame()->SendMessageW(GTPPainter_THICKNESS, m_i32Thick);

	return 0;
}
