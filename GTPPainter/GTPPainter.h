
// GTPPainter.h: GTPPainter 애플리케이션의 기본 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#define GTPPainter_SAVING      WM_USER + 120
#define GTPPainter_SAVING_AS   WM_USER + 130
#define GTPPainter_LOADING     WM_USER + 140
#define GTPPainter_THICKNESS   WM_USER + 200
#define GTPPainter_COLOR       WM_USER + 210
#define GTPPainter_DEFAULTN    WM_USER + 220
#define GTPPainter_DRAWING     WM_USER + 300
#define GTPPainter_NEWDRAW     WM_USER + 310
#define GTPPainter_POPUP       WM_USER + 330
#define GTPPainter_FVTOP       WM_USER + 340
#define GTPPainter_TOPFR       WM_USER + 350
#define GTPPainter_MAXIMIZE    WM_USER + 360
#define GTPPainter_CLOSING     WM_USER + 400
#define GTPPainter_OPENING     WM_USER + 410
#define GTPPainter_WINCLOSING  WM_USER + 420
#define GTPPainter_FVCLOSING   WM_USER + 430
#define GTPPainter_DATA        WM_USER + 500
#define GTPPainter_FORM        WM_USER + 510
#define GTPPainter_OFFSET      WM_USER + 520
#define GTPPainter_SCALE       WM_USER + 530
#define GTPPainter_ALLDATA     WM_USER + 540
#define GTPPainter_ADDVIEW     WM_USER + 550
#define GTPPainter_DELVIEW     WM_USER + 560

// CGTPPainterApp:
// 이 클래스의 구현에 대해서는 GTPPainter.cpp을(를) 참조하세요.
//

class CGTPPainterApp : public CWinApp
{
public:
	CGTPPainterApp() noexcept;


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGTPPainterApp theApp;
