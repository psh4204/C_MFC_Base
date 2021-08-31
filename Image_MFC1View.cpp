
// Image_MFC1View.cpp: CImageMFC1View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Image_MFC1.h"
#endif

#include "Image_MFC1Doc.h"
#include "Image_MFC1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageMFC1View

IMPLEMENT_DYNCREATE(CImageMFC1View, CView)

BEGIN_MESSAGE_MAP(CImageMFC1View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(IDM_EQUAL_IMAGE, &CImageMFC1View::OnEqualImage)
END_MESSAGE_MAP()

// CImageMFC1View 생성/소멸

CImageMFC1View::CImageMFC1View() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CImageMFC1View::~CImageMFC1View()
{
}

BOOL CImageMFC1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CImageMFC1View 그리기

void CImageMFC1View::OnDraw(CDC* pDC)
{
	CImageMFC1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	int px;
	for (int i = 0; i < pDoc->m_inH; i++) {
		for (int k = 0; k < pDoc->m_inW; k++) {
			px = pDoc->m_InputImage[i][k];
			pDC->SetPixel(k + 5, i + 5, RGB(px, px, px));
		}
	}
	for (int i = 0; i < pDoc->m_outH; i++) {
		for (int k = 0; k < pDoc->m_outW; k++) {
			px = pDoc->m_OutputImage[i][k];
			pDC->SetPixel(pDoc->m_inW + k + 20, i + 5, RGB(px, px, px));
		}
	}
}


// CImageMFC1View 인쇄

BOOL CImageMFC1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CImageMFC1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CImageMFC1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CImageMFC1View 진단

#ifdef _DEBUG
void CImageMFC1View::AssertValid() const
{
	CView::AssertValid();
}

void CImageMFC1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CImageMFC1Doc* CImageMFC1View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageMFC1Doc)));
	return (CImageMFC1Doc*)m_pDocument;
}
#endif //_DEBUG


// CImageMFC1View 메시지 처리기


void CImageMFC1View::OnEqualImage()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CImageMFC1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	pDoc->OnEqualImage();
	Invalidate(TRUE); // 화면 리프레쉬
}
