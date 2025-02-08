
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "GTPPainter.h"
#include "MainFrm.h"
#include "ViewData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_COLOR_CUSTOM, &CMainFrame::OnColorCustom)
	ON_COMMAND(ID_BLACK, &CMainFrame::OnBlack)
	ON_COMMAND(ID_LRED, &CMainFrame::OnLred)
	ON_COMMAND(ID_LIME, &CMainFrame::OnLime)
	ON_COMMAND(ID_BLUE, &CMainFrame::OnBlue)
	ON_UPDATE_COMMAND_UI(ID_BLACK, &CMainFrame::OnUpdateBlack)
	ON_UPDATE_COMMAND_UI(ID_LRED, &CMainFrame::OnUpdateLred)
	ON_UPDATE_COMMAND_UI(ID_LIME, &CMainFrame::OnUpdateLime)
	ON_UPDATE_COMMAND_UI(ID_BLUE, &CMainFrame::OnUpdateBlue)
	ON_UPDATE_COMMAND_UI(ID_COLOR_CUSTOM, &CMainFrame::OnUpdateColorCustom)
	ON_COMMAND(ID_THICK_1, &CMainFrame::OnThick1)
	ON_COMMAND(ID_THICK_3, &CMainFrame::OnThick3)
	ON_COMMAND(ID_THICK_5, &CMainFrame::OnThick5)
	ON_UPDATE_COMMAND_UI(ID_THICK_1, &CMainFrame::OnUpdateThick1)
	ON_UPDATE_COMMAND_UI(ID_THICK_3, &CMainFrame::OnUpdateThick3)
	ON_UPDATE_COMMAND_UI(ID_THICK_5, &CMainFrame::OnUpdateThick5)
	ON_COMMAND(ID_NEW_FILE, &CMainFrame::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN_NEW, &CMainFrame::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE_NEW, &CMainFrame::OnFileSaveNew)
	ON_COMMAND(ID_FILE_SAVE_AS_NEW, &CMainFrame::OnFileSaveAsNew)
	ON_COMMAND(ID_FILE_CLOSE_NEW, &CMainFrame::OnCloseNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_NEW, &CMainFrame::OnUpdateFileSaveNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE_NEW, &CMainFrame::OnUpdateClose)
	ON_MESSAGE(GTPPainter_DRAWING, &CMainFrame::OnDrawing)
	ON_WM_CONTEXTMENU()
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_EXIT, &CMainFrame::OnClose)
	ON_CBN_SELCHANGE(ID_THICKNESS, &CMainFrame::OnCbThicknessChange)
	ON_CBN_SELCHANGE(ID_COLOR, &CMainFrame::OnCbColorChange)
	ON_MESSAGE(GTPPainter_CLOSING, &CMainFrame::OnClosing)
	ON_MESSAGE(GTPPainter_COLOR, &CMainFrame::OnColor)
	ON_MESSAGE(GTPPainter_THICKNESS, &CMainFrame::OnThickness)
	ON_MESSAGE(GTPPainter_WINCLOSING, &CMainFrame::OnWinclosing)
	ON_MESSAGE(GTPPainter_DATA, &CMainFrame::OnData)
	ON_MESSAGE(GTPPainter_FORM, &CMainFrame::OnForm)
	ON_COMMAND(ID_NEW_INFORM, &CMainFrame::OnNewInform)
	ON_UPDATE_COMMAND_UI(ID_NEW_INFORM, &CMainFrame::OnUpdateNewInform)
	ON_MESSAGE(GTPPainter_FVCLOSING, &CMainFrame::OnFvclosing)
	ON_MESSAGE(GTPPainter_FVTOP, &CMainFrame::OnFvtop)
	ON_WM_SHOWWINDOW()
	ON_NOTIFY(TBN_DROPDOWN, AFX_IDW_TOOLBAR, &CMainFrame::OnNewFiles)
	ON_MESSAGE(GTPPainter_MAXIMIZE, &CMainFrame::OnMaximize)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
	m_bAutoMenuEnable = FALSE;
	m_i32Thick = 1;
	m_sLineColor = RGB(0, 0, 0);
	m_bModify = false;
	m_strTitle.LoadStringW(AFX_IDS_APP_TITLE);
	m_strFileName = "";
	m_i32ChildIndex = 1;
	m_i32ChildId = 1;
	m_i32ChildNum = 0;
	m_pViewInform = nullptr;
	m_bInform = false;
	m_bInformFront = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if(!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	   !m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if(!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators) / sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	CImageList imgToolBar;
	CBitmap bmpToolBar;

	m_wndToolBar.SetHeight(25);
	m_wndToolBar.GetToolBarCtrl().SetButtonWidth(50, 500);
	m_wndToolBar.GetToolBarCtrl().SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);

	bmpToolBar.LoadBitmap(IDB_TOOLBAR_NEW);
	imgToolBar.Create(16, 15, ILC_COLOR24 | ILC_MASK, 1, 0);
	imgToolBar.Add(&bmpToolBar, RGB(192, 192, 192));

	m_wndToolBar.GetToolBarCtrl().SetImageList(&imgToolBar);

	imgToolBar.Detach();

	m_wndToolBar.ModifyStyle(0, TBSTYLE_FLAT);

	m_wndToolBar.SetButtons(NULL, 8);

	m_wndToolBar.SetButtonInfo(0, ID_NEW_FILE, TBSTYLE_DROPDOWN, 0);
	m_wndToolBar.SetButtonInfo(1, ID_SEP_1, TBBS_SEPARATOR, NULL);
	m_wndToolBar.SetButtonInfo(2, ID_FILE_OPEN_NEW, TBSTYLE_BUTTON, 1);
	m_wndToolBar.SetButtonInfo(3, ID_FILE_SAVE_NEW, TBSTYLE_BUTTON, 2);
	m_wndToolBar.SetButtonInfo(4, ID_FILE_CLOSE_NEW, TBSTYLE_BUTTON, 3);
	m_wndToolBar.SetButtonInfo(5, ID_SEP_2, TBBS_SEPARATOR, NULL);
	m_wndToolBar.SetButtonInfo(6, ID_SEP_3, TBBS_SEPARATOR, NULL);
	m_wndToolBar.SetButtonInfo(7, ID_SEP_4, TBBS_SEPARATOR, NULL);

	CRect rButton;

	m_wndToolBar.GetItemRect(6, &rButton);
	rButton.left += 0;
	rButton.top += 0;
	rButton.right += 50;
	rButton.bottom += 100;

	m_cbThick.Create(CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE, rButton, &m_wndToolBar, ID_THICKNESS);
	m_cbThick.AddString(_T("1 px"));
	m_cbThick.AddString(_T("3 px"));
	m_cbThick.AddString(_T("5 px"));
	m_cbThick.SetCurSel(0);

	m_cbThick.SetItemData(0, 1);
	m_cbThick.SetItemData(1, 3);
	m_cbThick.SetItemData(2, 5);

	m_wndToolBar.GetItemRect(7, &rButton);
	rButton.left += 50;
	rButton.top += 0;
	rButton.right += 150;
	rButton.bottom += 100;

	m_cbColor.Create(CBS_DROPDOWNLIST | WS_CHILD | WS_VSCROLL | WS_TABSTOP | WS_VISIBLE, rButton, &m_wndToolBar, ID_COLOR);
	m_cbColor.AddString(_T("BLACK"));
	m_cbColor.AddString(_T("LIGHTRED"));
	m_cbColor.AddString(_T("LIME"));
	m_cbColor.AddString(_T("BLUE"));
	m_cbColor.AddString(_T("User defined"));
	m_cbColor.SetCurSel(0);

	CMenu* pMenuMain = GetMenu();
	pMenuMain->ModifyMenuW(1, MF_BYPOSITION, ID_LINE, L"도구(&T)");

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CMDIFrameWnd::PreCreateWindow(cs))
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기

void CMainFrame::OnColorCustom()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CColorDialog dlgColor(m_sLineColor);

	if(dlgColor.DoModal() == IDOK)
	{
		m_sLineColor = dlgColor.GetColor();

		GetActiveFrame()->PostMessageW(GTPPainter_COLOR, m_sLineColor);
	}

	if(m_sLineColor == RGB(0, 0, 0))
		m_cbColor.SetCurSel(0);
	else if(m_sLineColor == RGB(255, 0, 0))
		m_cbColor.SetCurSel(1);
	else if(m_sLineColor == RGB(0, 255, 0))
		m_cbColor.SetCurSel(2);
	else if(m_sLineColor == RGB(0, 0, 255))
		m_cbColor.SetCurSel(3);
	else
		m_cbColor.SetCurSel(4);
}

void CMainFrame::OnBlack()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_sLineColor = RGB(0, 0, 0);
	m_cbColor.SetCurSel(0);

	GetActiveFrame()->PostMessageW(GTPPainter_COLOR, m_sLineColor);
}

void CMainFrame::OnLred()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_sLineColor = RGB(255, 0, 0);
	m_cbColor.SetCurSel(1);

	GetActiveFrame()->PostMessageW(GTPPainter_COLOR, m_sLineColor);
}

void CMainFrame::OnLime()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_sLineColor = RGB(0, 255, 0);
	m_cbColor.SetCurSel(2);

	GetActiveFrame()->PostMessageW(GTPPainter_COLOR, m_sLineColor);
}

void CMainFrame::OnBlue()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_sLineColor = RGB(0, 0, 255);
	m_cbColor.SetCurSel(3);

	GetActiveFrame()->PostMessage(GTPPainter_COLOR, m_sLineColor);
}

void CMainFrame::OnUpdateBlack(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_sLineColor == RGB(0, 0, 0))
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateLred(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_sLineColor == RGB(255, 0, 0))
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateLime(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_sLineColor == RGB(0, 255, 0))
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateBlue(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_sLineColor == RGB(0, 0, 255))
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateColorCustom(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.

	if(m_sLineColor != RGB(0, 0, 0) && m_sLineColor != RGB(255, 0, 0) && m_sLineColor != RGB(0, 255, 0) && m_sLineColor != RGB(0, 0, 255))
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnThick1()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_i32Thick = 1;
	m_cbThick.SetCurSel(0);

	GetActiveFrame()->PostMessage(GTPPainter_THICKNESS, m_i32Thick);
}

void CMainFrame::OnThick3()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_i32Thick = 3;
	m_cbThick.SetCurSel(1);

	GetActiveFrame()->PostMessage(GTPPainter_THICKNESS, m_i32Thick);
}

void CMainFrame::OnThick5()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_i32Thick = 5;
	m_cbThick.SetCurSel(2);

	GetActiveFrame()->PostMessage(GTPPainter_THICKNESS, m_i32Thick);
}

void CMainFrame::OnUpdateThick1(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_i32Thick == 1)
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateThick3(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_i32Thick == 3)
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnUpdateThick5(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_i32Thick == 5)
		pCmdUI->SetCheck(MF_CHECKED);
	else
		pCmdUI->SetCheck(MF_UNCHECKED);
}

void CMainFrame::OnFileNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	POSITION posFirst = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = nullptr;
	pTemplate = AfxGetApp()->GetNextDocTemplate(posFirst);
	pTemplate->OpenDocumentFile(NULL);

	if(m_i32ChildIndex == 1)
	{
		m_strDefaultName = GetActiveFrame()->GetActiveDocument()->GetTitle();

		TCHAR arrChar[256];

		GetModuleFileName(NULL, arrChar, 256);
		m_strDefaultPath = arrChar;

		int32_t i32Index;

		for(int32_t i = 0; i < 3; i++)
		{
			i32Index = m_strDefaultPath.ReverseFind('\\');
			m_strDefaultPath = m_strDefaultPath.Left(i32Index);
		}
	}

	CString strChildNum;
	strChildNum.Format(_T("%d"), m_i32ChildIndex);
	GetActiveFrame()->GetActiveDocument()->SetPathName(m_strDefaultPath);
	GetActiveFrame()->GetActiveDocument()->SetTitle(m_strDefaultName + strChildNum);
	SetWindowText(m_strTitle + " - " + m_strDefaultName + strChildNum);

	CMenu* pMenuMain = GetMenu();
	CMenu* pMenuSub = pMenuMain->GetSubMenu(0);

	if(pMenuSub->GetMenuItemCount() < 8)
	{
		pMenuSub->InsertMenuW(3, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_NEW, L"저장(&S)\tCtrl+S");
		pMenuSub->InsertMenuW(4, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_AS_NEW, L"다른 이름으로 저장(&A)");
		pMenuSub->InsertMenuW(5, MF_BYPOSITION | MF_STRING, ID_FILE_CLOSE_NEW, L"닫기(&C)");
	}

	if(pMenuMain->GetMenuItemCount() < 3)
	{
		CMenu menuLine, menuThick, menuColor;
		menuThick.CreatePopupMenu();
		menuThick.AppendMenuW(MF_STRING, ID_THICK_1, L"&1 px\tCTRL+1");
		menuThick.AppendMenuW(MF_STRING, ID_THICK_3, L"&3 px\tCTRL+2");
		menuThick.AppendMenuW(MF_STRING, ID_THICK_5, L"&5 px\tCTRL+3");

		menuColor.CreatePopupMenu();
		menuColor.AppendMenuW(MF_STRING, ID_BLACK, L"BLAC&K\tCTRL+SHIFT+1");
		menuColor.AppendMenuW(MF_STRING, ID_LRED, L"LIGHT&RED\tCTRL+SHIFT+2");
		menuColor.AppendMenuW(MF_STRING, ID_LIME, L"&LIME\tCTRL+SHIFT+3");
		menuColor.AppendMenuW(MF_STRING, ID_BLUE, L"&BLUE\tCTRL+SHIFT+4");
		menuColor.AppendMenuW(MF_STRING, ID_COLOR_CUSTOM, L"&User defined");

		menuLine.CreatePopupMenu();
		menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuThick.Detach(), L"선 두께(&T)");
		menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuColor.Detach(), L"선 색(&R)");

		pMenuMain->InsertMenuW(1, MF_BYPOSITION | MF_POPUP, (uintptr_t)menuLine.Detach(), L"도구(&T)");
		pMenuMain->ModifyMenuW(1, MF_BYPOSITION, ID_LINE, L"도구(&T)");
	}

	pMenuSub->Detach();
	pMenuMain->Detach();

	m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, FALSE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, FALSE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, FALSE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, FALSE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, FALSE);

	m_cbThick.ShowWindow(true);
	m_cbColor.ShowWindow(true);

	DrawMenuBar();
	
	CString* pStrTemp = new CString(m_strDefaultName);
	GetActiveFrame()->SendMessageW(GTPPainter_DEFAULTN, NULL, (LPARAM)pStrTemp);

	delete pStrTemp;
	pStrTemp = nullptr;

	if(m_bInform)
		m_pViewInform->SendMessageW(GTPPainter_ADDVIEW, m_i32ChildId);

	GetActiveFrame()->SendMessageW(GTPPainter_ADDVIEW, m_i32ChildId);

	m_i32ChildIndex++;
	m_i32ChildId++;
	m_i32ChildNum++;

	DrawMenuBar();
}

void CMainFrame::OnFileOpen()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CFileDialog dlgLoad(TRUE, _T(".flp"), m_strDefaultName, OFN_OVERWRITEPROMPT | OFN_FILEMUSTEXIST, _T("Fourth Logic Paint(*.flp)|*.flp|모든 파일(*.*)|*.*|"), this);
	int32_t i32Result = (int32_t)dlgLoad.DoModal();

	if(i32Result == IDOK)
	{
		m_strFileName = dlgLoad.GetFileName();
		m_strFileName = m_strFileName.Left(m_strFileName.GetLength() - 4);

		m_strPath = dlgLoad.GetPathName();
		m_strPath = m_strPath.Left(m_strPath.ReverseFind('\\'));

		if(CheckCrc32(m_strFileName, m_strPath))
		{
			POSITION posFirst = AfxGetApp()->GetFirstDocTemplatePosition();
			CDocTemplate* pTemplate = nullptr;
			pTemplate = AfxGetApp()->GetNextDocTemplate(posFirst);
			pTemplate->OpenDocumentFile(NULL);

			GetActiveFrame()->GetActiveDocument()->SetPathName(m_strPath);
			GetActiveFrame()->GetActiveDocument()->SetTitle(m_strFileName);

			GetActiveFrame()->SendMessageW(GTPPainter_OPENING);

			if(m_bInform)
				m_pViewInform->SendMessageW(GTPPainter_ADDVIEW, m_i32ChildId);

			GetActiveFrame()->SendMessageW(GTPPainter_ADDVIEW, m_i32ChildId);

			CMenu* pMenuMain = GetMenu();
			CMenu* pMenuSub = pMenuMain->GetSubMenu(0);

			if(pMenuSub->GetMenuItemCount() < 8)
			{
				pMenuSub->InsertMenuW(3, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_NEW, L"저장(&S)\tCtrl+S");
				pMenuSub->InsertMenuW(4, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_AS_NEW, L"다른 이름으로 저장(&A)");
				pMenuSub->InsertMenuW(5, MF_BYPOSITION | MF_STRING, ID_FILE_CLOSE_NEW, L"닫기(&C)");
			}

			if(pMenuMain->GetMenuItemCount() < 3)
			{
				CMenu menuLine, menuThick, menuColor;
				menuThick.CreatePopupMenu();
				menuThick.AppendMenuW(MF_STRING, ID_THICK_1, L"&1 px\tCTRL+1");
				menuThick.AppendMenuW(MF_STRING, ID_THICK_3, L"&3 px\tCTRL+2");
				menuThick.AppendMenuW(MF_STRING, ID_THICK_5, L"&5 px\tCTRL+3");

				menuColor.CreatePopupMenu();
				menuColor.AppendMenuW(MF_STRING, ID_BLACK, L"BLAC&K\tCTRL+SHIFT+1");
				menuColor.AppendMenuW(MF_STRING, ID_LRED, L"LIGHT&RED\tCTRL+SHIFT+2");
				menuColor.AppendMenuW(MF_STRING, ID_LIME, L"&LIME\tCTRL+SHIFT+3");
				menuColor.AppendMenuW(MF_STRING, ID_BLUE, L"&BLUE\tCTRL+SHIFT+4");
				menuColor.AppendMenuW(MF_STRING, ID_COLOR_CUSTOM, L"&User defined");

				menuLine.CreatePopupMenu();
				menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuThick.Detach(), L"선 두께(&T)");
				menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuColor.Detach(), L"선 색(&R)");

				pMenuMain->InsertMenuW(1, MF_BYPOSITION | MF_POPUP, (uintptr_t)menuLine.Detach(), L"도구(&T)");
				pMenuMain->ModifyMenuW(1, MF_BYPOSITION, ID_LINE, L"도구(&T)");
			}

			pMenuSub->Detach();
			pMenuMain->Detach();

			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, FALSE);

			m_cbThick.ShowWindow(true);
			m_cbColor.ShowWindow(true);
			
			m_i32ChildId++;
			m_i32ChildNum++;

			DrawMenuBar();
		}
	}
}

void CMainFrame::OnFileSaveNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	GetActiveFrame()->PostMessage(GTPPainter_SAVING);
}

void CMainFrame::OnFileSaveAsNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다
	GetActiveFrame()->PostMessage(GTPPainter_SAVING_AS);
}

void CMainFrame::OnCloseNew()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	GetActiveFrame()->PostMessage(GTPPainter_CLOSING, false);
}

void CMainFrame::OnUpdateFileSaveNew(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_bModify)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateClose(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_i32ChildNum > 0)
		pCmdUI->Enable(true);
	else
		pCmdUI->Enable(false);
}

afx_msg LRESULT CMainFrame::OnDrawing(WPARAM wParam, LPARAM lParam)
{
	m_bModify = (bool)wParam;

	return 0;
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(pMsg->message == WM_KEYDOWN)
	{
		if(GetKeyState(VK_CONTROL) & 0x8000)
		{
			if(GetKeyState(VK_SHIFT) & 0x8000)
			{
				if(pMsg->wParam == 49)
					OnBlack();
				else if(pMsg->wParam == 50)
					OnLred();
				else if(pMsg->wParam == 51)
					OnLime();
				else if(pMsg->wParam == 52)
					OnBlue();
			}
			else if(pMsg->wParam == 49)
				OnThick1();
			else if(pMsg->wParam == 50)
				OnThick3();
			else if(pMsg->wParam == 51)
				OnThick5();
			else if(pMsg->wParam == 83)
				OnFileSaveNew();
			else if(pMsg->wParam == 78)
				OnFileNew();
			else if(pMsg->wParam == 79)
				OnFileOpen();
		}
	}

	return false;
}

void CMainFrame::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu menuPopup;

	menuPopup.LoadMenuW(IDR_POPUP);
	CMenu* pMenuSubPopup = menuPopup.GetSubMenu(1);

	pMenuSubPopup->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, AfxGetMainWnd());
}

void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	int32_t i32Result = MessageBox(_T("Do you really want to quit the program?"), _T("Exit"), MB_YESNO);

	if(i32Result == IDYES)
		GetActiveFrame()->PostMessageW(GTPPainter_WINCLOSING);
}

void CMainFrame::OnCbThicknessChange()
{
	int32_t i32Thick = (int32_t)m_cbThick.GetItemData(m_cbThick.GetCurSel());

	if(i32Thick == 1)
		OnThick1();
	else if(i32Thick == 3)
		OnThick3();
	else
		OnThick5();
}

void CMainFrame::OnCbColorChange()
{
	int32_t i32Color = m_cbColor.GetCurSel();

	if(i32Color == 0)
		OnBlack();
	else if(i32Color == 1)
		OnLred();
	else if(i32Color == 2)
		OnLime();
	else if(i32Color == 3)
		OnBlue();
	else
		OnColorCustom();
}

afx_msg LRESULT CMainFrame::OnClosing(WPARAM wParam, LPARAM lParam)
{
	int32_t i32Child = (int32_t)wParam;
	m_i32ChildNum--;

	if(m_i32ChildNum == 0)
	{
		m_bModify = false;

		CMenu* pMenuMain = GetMenu();
		CMenu* pMenuSub = pMenuMain->GetSubMenu(0);
		pMenuSub->DeleteMenu(ID_FILE_SAVE_NEW, MF_BYCOMMAND);
		pMenuSub->DeleteMenu(ID_FILE_SAVE_AS_NEW, MF_BYCOMMAND);
		pMenuSub->DeleteMenu(ID_FILE_CLOSE_NEW, MF_BYCOMMAND);

		pMenuMain->DeleteMenu(ID_LINE, MF_BYCOMMAND);

		pMenuSub->Detach();
		pMenuMain->Detach();

		m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, TRUE);
		m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, TRUE);
		
		m_cbThick.ShowWindow(false);
		m_cbColor.ShowWindow(false);

		DrawMenuBar();
	}
	else
		GetActiveFrame()->PostMessageW(GTPPainter_DRAWING, NULL, 1);

	std::vector<CViewData>::iterator vctVdIter;

	for(vctVdIter = m_vctVdViews.begin(); vctVdIter < m_vctVdViews.end(); vctVdIter++)
	{
		if(vctVdIter->m_i32Id == i32Child)
		{
			m_vctVdViews.erase(vctVdIter);

			break;
		}
	}

	if(m_bInform)
		m_pViewInform->PostMessageW(GTPPainter_DELVIEW, i32Child);

	GetActiveFrame()->PostMessageW(GTPPainter_TOPFR);

	return 0;
}

afx_msg LRESULT CMainFrame::OnColor(WPARAM wParam, LPARAM lParam)
{
	m_sLineColor = (COLORREF)wParam;
	int32_t i32Index = m_cbColor.GetCurSel();

	if(m_sLineColor == RGB(0, 0, 0))
	{
		if(i32Index != 0)
			m_cbColor.SetCurSel(0);
	}
	else if(m_sLineColor == RGB(255, 0, 0))
	{
		if(i32Index != 1)
			m_cbColor.SetCurSel(1);
	}
	else if(m_sLineColor == RGB(0, 255, 0))
	{
		if(i32Index != 2)
			m_cbColor.SetCurSel(2);
	}
	else if(m_sLineColor == RGB(0, 0, 255))
	{
		if(i32Index != 3)
			m_cbColor.SetCurSel(3);
	}
	else
	{
		if(i32Index != 4)
			m_cbColor.SetCurSel(4);
	}
	
	return 0;
}

afx_msg LRESULT CMainFrame::OnThickness(WPARAM wParam, LPARAM lParam)
{
	m_i32Thick = (int32_t)wParam;
	int32_t i32Index = m_cbThick.GetCurSel();

	if(m_i32Thick == 1)
	{
		if(i32Index != 0)
			m_cbThick.SetCurSel(0);
	}
	else if(m_i32Thick == 3)
	{
		if(i32Index != 1)
			m_cbThick.SetCurSel(1);
	}
	else
	{
		if(i32Index != 2)
			m_cbThick.SetCurSel(2);
	}
	
	return 0;
}

afx_msg LRESULT CMainFrame::OnWinclosing(WPARAM wParam, LPARAM lParam)
{
	int32_t i32Child = (int32_t)wParam;

	if(m_i32ChildNum == 0)
		CMDIFrameWnd::OnClose();
	else
	{
		if((int32_t)wParam == -1)
		{
			GetActiveFrame()->PostMessageW(GTPPainter_TOPFR);
			GetActiveFrame()->PostMessageW(GTPPainter_WINCLOSING); 
			m_i32ChildNum--;
		}
		else
		{
			if(m_bInform)
				m_pViewInform->SendMessageW(GTPPainter_DELVIEW, i32Child);

			std::vector<CViewData>::iterator vctVdIter;

			for(vctVdIter = m_vctVdViews.begin(); vctVdIter < m_vctVdViews.end(); vctVdIter++)
			{
				if(vctVdIter->m_i32Id == i32Child)
				{
					m_vctVdViews.erase(vctVdIter);
					break;
				}
			}

			GetActiveFrame()->PostMessageW(GTPPainter_TOPFR);
			GetActiveFrame()->PostMessageW(GTPPainter_WINCLOSING);
			m_i32ChildNum--;
		}
	}

	return 0;
}

uint32_t CMainFrame::ConvertToInt(int8_t* pI8Char)
{
	uint32_t u32One, u32Two, u32Three, u32Four;
	u32One = pI8Char[0] & 255;
	u32Two = pI8Char[1] & 255;
	u32Three = pI8Char[2] & 255;
	u32Four = pI8Char[3] & 255;

	return ((int32_t)u32Four << 24) + ((int32_t)u32Three << 16) + ((int32_t)u32Two << 8) + u32One;
}

bool CMainFrame::CheckCrc32(CString strFile, CString strPath)
{
	bool bCheck = false;

	do
	{
		int8_t arrI8FL[11];
		FILE* pFLoad = nullptr;

		uint32_t u32Total, u32Line;

		int8_t arrI8Int[sizeof(uint32_t)] = { 0 };
		int8_t arrI8Double[sizeof(double)] = { 0 };
		uint32_t u32Crc32save = 0;
		uint32_t u32Crc32val = 0;
		uint32_t u32Crc32header = 0;
		u32Crc32val ^= 0xFFFFFFFF;
		u32Crc32header ^= 0xFFFFFFFF;

		fopen_s(&pFLoad, strPath + '/' + strFile + ".flp", "rb");

		if(pFLoad)
		{
			fread(&arrI8FL, sizeof(int8_t), 11, pFLoad);
			CalcCrc32(&u32Crc32val, arrI8FL, sizeof(int8_t) * 11);
			CalcCrc32(&u32Crc32header, (int8_t*)"FourthLogic", sizeof(int8_t) * 11);

			if(u32Crc32val == u32Crc32header)
			{
				fread(&arrI8Int, sizeof(uint32_t), 1, pFLoad);
				CalcCrc32(&u32Crc32val, arrI8Int, sizeof(uint32_t));

				fread(&arrI8Int, sizeof(uint32_t), 1, pFLoad);
				CalcCrc32(&u32Crc32val, arrI8Int, sizeof(uint32_t));
				u32Total = ConvertToInt(arrI8Int);

				for(uint32_t i = 0; i < u32Total; i++)
				{
					fread(&arrI8Int, sizeof(uint32_t), 1, pFLoad);
					CalcCrc32(&u32Crc32val, arrI8Int, sizeof(uint32_t));
					u32Line = ConvertToInt(arrI8Int);

					for(uint32_t j = 0; j < u32Line; j++)
					{
						fread(&arrI8Double, sizeof(double), 1, pFLoad);
						CalcCrc32(&u32Crc32val, arrI8Double, sizeof(double));

						fread(&arrI8Double, sizeof(double), 1, pFLoad);
						CalcCrc32(&u32Crc32val, arrI8Double, sizeof(double));

						fread(&arrI8Int, sizeof(uint32_t), 1, pFLoad);
						CalcCrc32(&u32Crc32val, arrI8Int, sizeof(uint32_t));

						fread(&arrI8Int, sizeof(uint32_t), 1, pFLoad);
						CalcCrc32(&u32Crc32val, arrI8Int, sizeof(uint32_t));
					}
				}

				fread(&u32Crc32save, sizeof(uint32_t), 1, pFLoad);
				u32Crc32val = u32Crc32val ^ 0xFFFFFFFF;

				fclose(pFLoad);

				if(u32Crc32save != u32Crc32val)
				{
					MessageBoxW(_T("Failed to load file."), MB_OK);

					break;
				}
			}
			else
			{
				fclose(pFLoad);

				MessageBoxW(_T("Failed to load file."), MB_OK);

				break;
			}
		}
		else
		{
			MessageBoxW(_T("Failed to load file."), MB_OK);

			break;
		}

		bCheck = true;
	}
	while(false);

	return bCheck;
}
		
static const uint32_t g_u32Crc32Tab[] =
{
	0x00000000L, 0x77073096L, 0xee0e612cL, 0x990951baL, 0x076dc419L,
	0x706af48fL, 0xe963a535L, 0x9e6495a3L, 0x0edb8832L, 0x79dcb8a4L,
	0xe0d5e91eL, 0x97d2d988L, 0x09b64c2bL, 0x7eb17cbdL, 0xe7b82d07L,
	0x90bf1d91L, 0x1db71064L, 0x6ab020f2L, 0xf3b97148L, 0x84be41deL,
	0x1adad47dL, 0x6ddde4ebL, 0xf4d4b551L, 0x83d385c7L, 0x136c9856L,
	0x646ba8c0L, 0xfd62f97aL, 0x8a65c9ecL, 0x14015c4fL, 0x63066cd9L,
	0xfa0f3d63L, 0x8d080df5L, 0x3b6e20c8L, 0x4c69105eL, 0xd56041e4L,
	0xa2677172L, 0x3c03e4d1L, 0x4b04d447L, 0xd20d85fdL, 0xa50ab56bL,
	0x35b5a8faL, 0x42b2986cL, 0xdbbbc9d6L, 0xacbcf940L, 0x32d86ce3L,
	0x45df5c75L, 0xdcd60dcfL, 0xabd13d59L, 0x26d930acL, 0x51de003aL,
	0xc8d75180L, 0xbfd06116L, 0x21b4f4b5L, 0x56b3c423L, 0xcfba9599L,
	0xb8bda50fL, 0x2802b89eL, 0x5f058808L, 0xc60cd9b2L, 0xb10be924L,
	0x2f6f7c87L, 0x58684c11L, 0xc1611dabL, 0xb6662d3dL, 0x76dc4190L,
	0x01db7106L, 0x98d220bcL, 0xefd5102aL, 0x71b18589L, 0x06b6b51fL,
	0x9fbfe4a5L, 0xe8b8d433L, 0x7807c9a2L, 0x0f00f934L, 0x9609a88eL,
	0xe10e9818L, 0x7f6a0dbbL, 0x086d3d2dL, 0x91646c97L, 0xe6635c01L,
	0x6b6b51f4L, 0x1c6c6162L, 0x856530d8L, 0xf262004eL, 0x6c0695edL,
	0x1b01a57bL, 0x8208f4c1L, 0xf50fc457L, 0x65b0d9c6L, 0x12b7e950L,
	0x8bbeb8eaL, 0xfcb9887cL, 0x62dd1ddfL, 0x15da2d49L, 0x8cd37cf3L,
	0xfbd44c65L, 0x4db26158L, 0x3ab551ceL, 0xa3bc0074L, 0xd4bb30e2L,
	0x4adfa541L, 0x3dd895d7L, 0xa4d1c46dL, 0xd3d6f4fbL, 0x4369e96aL,
	0x346ed9fcL, 0xad678846L, 0xda60b8d0L, 0x44042d73L, 0x33031de5L,
	0xaa0a4c5fL, 0xdd0d7cc9L, 0x5005713cL, 0x270241aaL, 0xbe0b1010L,
	0xc90c2086L, 0x5768b525L, 0x206f85b3L, 0xb966d409L, 0xce61e49fL,
	0x5edef90eL, 0x29d9c998L, 0xb0d09822L, 0xc7d7a8b4L, 0x59b33d17L,
	0x2eb40d81L, 0xb7bd5c3bL, 0xc0ba6cadL, 0xedb88320L, 0x9abfb3b6L,
	0x03b6e20cL, 0x74b1d29aL, 0xead54739L, 0x9dd277afL, 0x04db2615L,
	0x73dc1683L, 0xe3630b12L, 0x94643b84L, 0x0d6d6a3eL, 0x7a6a5aa8L,
	0xe40ecf0bL, 0x9309ff9dL, 0x0a00ae27L, 0x7d079eb1L, 0xf00f9344L,
	0x8708a3d2L, 0x1e01f268L, 0x6906c2feL, 0xf762575dL, 0x806567cbL,
	0x196c3671L, 0x6e6b06e7L, 0xfed41b76L, 0x89d32be0L, 0x10da7a5aL,
	0x67dd4accL, 0xf9b9df6fL, 0x8ebeeff9L, 0x17b7be43L, 0x60b08ed5L,
	0xd6d6a3e8L, 0xa1d1937eL, 0x38d8c2c4L, 0x4fdff252L, 0xd1bb67f1L,
	0xa6bc5767L, 0x3fb506ddL, 0x48b2364bL, 0xd80d2bdaL, 0xaf0a1b4cL,
	0x36034af6L, 0x41047a60L, 0xdf60efc3L, 0xa867df55L, 0x316e8eefL,
	0x4669be79L, 0xcb61b38cL, 0xbc66831aL, 0x256fd2a0L, 0x5268e236L,
	0xcc0c7795L, 0xbb0b4703L, 0x220216b9L, 0x5505262fL, 0xc5ba3bbeL,
	0xb2bd0b28L, 0x2bb45a92L, 0x5cb36a04L, 0xc2d7ffa7L, 0xb5d0cf31L,
	0x2cd99e8bL, 0x5bdeae1dL, 0x9b64c2b0L, 0xec63f226L, 0x756aa39cL,
	0x026d930aL, 0x9c0906a9L, 0xeb0e363fL, 0x72076785L, 0x05005713L,
	0x95bf4a82L, 0xe2b87a14L, 0x7bb12baeL, 0x0cb61b38L, 0x92d28e9bL,
	0xe5d5be0dL, 0x7cdcefb7L, 0x0bdbdf21L, 0x86d3d2d4L, 0xf1d4e242L,
	0x68ddb3f8L, 0x1fda836eL, 0x81be16cdL, 0xf6b9265bL, 0x6fb077e1L,
	0x18b74777L, 0x88085ae6L, 0xff0f6a70L, 0x66063bcaL, 0x11010b5cL,
	0x8f659effL, 0xf862ae69L, 0x616bffd3L, 0x166ccf45L, 0xa00ae278L,
	0xd70dd2eeL, 0x4e048354L, 0x3903b3c2L, 0xa7672661L, 0xd06016f7L,
	0x4969474dL, 0x3e6e77dbL, 0xaed16a4aL, 0xd9d65adcL, 0x40df0b66L,
	0x37d83bf0L, 0xa9bcae53L, 0xdebb9ec5L, 0x47b2cf7fL, 0x30b5ffe9L,
	0xbdbdf21cL, 0xcabac28aL, 0x53b39330L, 0x24b4a3a6L, 0xbad03605L,
	0xcdd70693L, 0x54de5729L, 0x23d967bfL, 0xb3667a2eL, 0xc4614ab8L,
	0x5d681b02L, 0x2a6f2b94L, 0xb40bbe37L, 0xc30c8ea1L, 0x5a05df1bL,
	0x2d02ef8dL
};

void CMainFrame::CalcCrc32(uint32_t* pU32Crc32val, int8_t* pI8Char, const int32_t i32Size)
{
	for(int32_t i = 0; i < i32Size; i++)
		*pU32Crc32val = g_u32Crc32Tab[(*pU32Crc32val ^ pI8Char[i]) & 0xFF] ^ ((*pU32Crc32val >> 8) & 0x00FFFFFF);
}

afx_msg LRESULT CMainFrame::OnData(WPARAM wParam, LPARAM lParam)
{
	CViewData* pVdTemp = (CViewData*)lParam;
	CViewData vdTemp = *pVdTemp;

	bool bHad = false;
	std::vector<CViewData>::iterator vctVdIter;

	for(vctVdIter = m_vctVdViews.begin(); vctVdIter < m_vctVdViews.end(); vctVdIter++)
	{
		if(vctVdIter->m_i32Id == vdTemp.m_i32Id)
		{
			vctVdIter->m_strFileName = vdTemp.m_strFileName;
			vctVdIter->m_f64OffsetX = vdTemp.m_f64OffsetX;
			vctVdIter->m_f64OffsetY = vdTemp.m_f64OffsetY;
			vctVdIter->m_f64Scale = vdTemp.m_f64Scale;
			vctVdIter->m_bEdited = vdTemp.m_bEdited;

			bHad = true;
			break;
		}
	}

	if(!bHad)
		m_vctVdViews.push_back(*pVdTemp);

	if(m_bInform)
		m_pViewInform->SendMessageW(GTPPainter_DATA, NULL, (LPARAM)&vdTemp);

	return 0;
}

afx_msg LRESULT CMainFrame::OnForm(WPARAM wParam, LPARAM lParam)
{
	m_pViewInform = (CView*)wParam;

	m_pViewInform->PostMessageW(GTPPainter_ALLDATA, NULL, (LPARAM)&m_vctVdViews);

	return 0;
}

void CMainFrame::OnNewInform()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	POSITION posFirst = AfxGetApp()->GetFirstDocTemplatePosition();
	CDocTemplate* pTemplate = nullptr;

	pTemplate = AfxGetApp()->GetNextDocTemplate(posFirst);
	pTemplate = AfxGetApp()->GetNextDocTemplate(posFirst);
	pTemplate->OpenDocumentFile(NULL);

	GetActiveFrame()->GetActiveDocument()->SetTitle(_T("Information"));

	m_bInform = true;
}

void CMainFrame::OnUpdateNewInform(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	if(m_bInform)
		pCmdUI->Enable(false);
	else
		pCmdUI->Enable(true);
}

afx_msg LRESULT CMainFrame::OnFvclosing(WPARAM wParam, LPARAM lParam)
{
	m_bInform = false;

	return 0;
}

afx_msg LRESULT CMainFrame::OnFvtop(WPARAM wParam, LPARAM lParam)
{
	tagWINDOWPLACEMENT twpWindow;
	{
		twpWindow.length = sizeof(WINDOWPLACEMENT);
	}

	GetActiveFrame()->GetWindowPlacement(&twpWindow);

	if(!m_bInformFront)
	{
		if(twpWindow.showCmd == 2 || GetActiveFrame()->GetActiveView() == m_pViewInform || GetActiveFrame() == this)
		{
			CMenu* pMenu = GetMenu();
			CMenu* pSub = pMenu->GetSubMenu(0);
			pSub->DeleteMenu(ID_FILE_SAVE_NEW, MF_BYCOMMAND);
			pSub->DeleteMenu(ID_FILE_SAVE_AS_NEW, MF_BYCOMMAND);
			pSub->DeleteMenu(ID_FILE_CLOSE_NEW, MF_BYCOMMAND);

			pMenu->DeleteMenu(ID_LINE, MF_BYCOMMAND);

			pSub->Detach();
			pMenu->Detach();

			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, TRUE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, TRUE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, TRUE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, TRUE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, TRUE);

			m_cbThick.ShowWindow(false);
			m_cbColor.ShowWindow(false);

			m_bInformFront = true;
		
			DrawMenuBar();
		}
	}
	else
	{
		if(twpWindow.showCmd != 2 && GetActiveFrame()->GetActiveView() != m_pViewInform)
		{
			CMenu* pMenuMain = GetMenu();
			CMenu* pMenuSub = pMenuMain->GetSubMenu(0);

			if(pMenuSub->GetMenuItemCount() < 8)
			{
				pMenuSub->InsertMenuW(3, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_NEW, L"저장(&S)\tCtrl+S");
				pMenuSub->InsertMenuW(4, MF_BYPOSITION | MF_STRING, ID_FILE_SAVE_AS_NEW, L"다른 이름으로 저장(&A)");
				pMenuSub->InsertMenuW(5, MF_BYPOSITION | MF_STRING, ID_FILE_CLOSE_NEW, L"닫기(&C)");
			}

			if(pMenuMain->GetMenuItemCount() < 3)
			{
				CMenu menuLine, menuThick, menuColor;
				menuThick.CreatePopupMenu();
				menuThick.AppendMenuW(MF_STRING, ID_THICK_1, L"&1 px\tCTRL+1");
				menuThick.AppendMenuW(MF_STRING, ID_THICK_3, L"&3 px\tCTRL+2");
				menuThick.AppendMenuW(MF_STRING, ID_THICK_5, L"&5 px\tCTRL+3");

				menuColor.CreatePopupMenu();
				menuColor.AppendMenuW(MF_STRING, ID_BLACK, L"BLAC&K\tCTRL+SHIFT+1");
				menuColor.AppendMenuW(MF_STRING, ID_LRED, L"LIGHT&RED\tCTRL+SHIFT+2");
				menuColor.AppendMenuW(MF_STRING, ID_LIME, L"&LIME\tCTRL+SHIFT+3");
				menuColor.AppendMenuW(MF_STRING, ID_BLUE, L"&BLUE\tCTRL+SHIFT+4");
				menuColor.AppendMenuW(MF_STRING, ID_COLOR_CUSTOM, L"&User defined");

				menuLine.CreatePopupMenu();
				menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuThick.Detach(), L"선 두께(&T)");
				menuLine.AppendMenuW(MF_POPUP, (uintptr_t)menuColor.Detach(), L"선 색(&R)");

				pMenuMain->InsertMenuW(1, MF_BYPOSITION | MF_POPUP, (uintptr_t)menuLine.Detach(), L"도구(&T)");
				pMenuMain->ModifyMenuW(1, MF_BYPOSITION, ID_LINE, L"도구(&T)");
			}

			pMenuSub->Detach();
			pMenuMain->Detach();
			
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, FALSE);
			m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, FALSE);

			m_cbThick.ShowWindow(true);
			m_cbColor.ShowWindow(true);

			m_bInformFront = false;

			DrawMenuBar();
		}
	}

	GetActiveFrame()->PostMessageW(GTPPainter_TOPFR);
	
	return 0;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CMDIFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CMenu* pMenuMain = GetMenu();
	CMenu* pMenuSub = pMenuMain->GetSubMenu(0);
	pMenuSub->DeleteMenu(ID_FILE_SAVE_NEW, MF_BYCOMMAND);
	pMenuSub->DeleteMenu(ID_FILE_SAVE_AS_NEW, MF_BYCOMMAND);
	pMenuSub->DeleteMenu(ID_FILE_CLOSE_NEW, MF_BYCOMMAND);

	pMenuMain->DeleteMenu(ID_LINE, MF_BYCOMMAND);

	pMenuSub->Detach();
	pMenuMain->Detach();
	
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_SAVE_NEW, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_FILE_CLOSE_NEW, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_2, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_3, TRUE);
	m_wndToolBar.GetToolBarCtrl().HideButton(ID_SEP_4, TRUE);

	m_cbThick.ShowWindow(false);
	m_cbColor.ShowWindow(false);

	DrawMenuBar();
}

void CMainFrame::OnNewFiles(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	LPNMTOOLBAR pToolBar = reinterpret_cast<LPNMTOOLBAR>(pNMHDR);
	ClientToScreen(&(pToolBar->rcButton));

	CMenu menuPopup;

	VERIFY(menuPopup.LoadMenu(IDR_POPUP));
	CMenu* pMenuSubPopup = menuPopup.GetSubMenu(2);

	pMenuSubPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pToolBar->rcButton.left + 16, pToolBar->rcButton.bottom, this);

	*pResult = 0;
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(WM_SETTEXT == message)
		return CWnd::DefWindowProc(message, wParam, lParam);

	return CMDIFrameWnd::DefWindowProc(message, wParam, lParam);
}


afx_msg LRESULT CMainFrame::OnMaximize(WPARAM wParam, LPARAM lParam)
{
	CString strTitle = m_strTitle;
	strTitle += L" - ";
	strTitle += GetActiveFrame()->GetActiveDocument()->GetTitle();
	SetWindowTextW(strTitle);

	return 0;
}