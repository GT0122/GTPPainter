// GTPPainterFormDoc.cpp: 구현 파일
//

#include "pch.h"
#include "GTPPainter.h"
#include "GTPPainterFormDoc.h"


// CGTPPainterFormDoc

IMPLEMENT_DYNCREATE(CGTPPainterFormDoc, CDocument)

CGTPPainterFormDoc::CGTPPainterFormDoc()
{
}

BOOL CGTPPainterFormDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CGTPPainterFormDoc::~CGTPPainterFormDoc()
{
}


BEGIN_MESSAGE_MAP(CGTPPainterFormDoc, CDocument)
END_MESSAGE_MAP()


// CGTPPainterFormDoc 진단

#ifdef _DEBUG
void CGTPPainterFormDoc::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CGTPPainterFormDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CGTPPainterFormDoc serialization

void CGTPPainterFormDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}
#endif


// CGTPPainterFormDoc 명령
