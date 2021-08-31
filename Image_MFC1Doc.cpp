
// Image_MFC1Doc.cpp: CImageMFC1Doc 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Image_MFC1.h"
#endif

#include "Image_MFC1Doc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CImageMFC1Doc

IMPLEMENT_DYNCREATE(CImageMFC1Doc, CDocument)

BEGIN_MESSAGE_MAP(CImageMFC1Doc, CDocument)
END_MESSAGE_MAP()


// CImageMFC1Doc 생성/소멸

CImageMFC1Doc::CImageMFC1Doc() noexcept
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CImageMFC1Doc::~CImageMFC1Doc()
{
}

BOOL CImageMFC1Doc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CImageMFC1Doc serialization

void CImageMFC1Doc::Serialize(CArchive& ar)
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

#ifdef SHARED_HANDLERS

// 축소판 그림을 지원합니다.
void CImageMFC1Doc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 문서의 데이터를 그리려면 이 코드를 수정하십시오.
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 검색 처리기를 지원합니다.
void CImageMFC1Doc::InitializeSearchContent()
{
	CString strSearchContent;
	// 문서의 데이터에서 검색 콘텐츠를 설정합니다.
	// 콘텐츠 부분은 ";"로 구분되어야 합니다.

	// 예: strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CImageMFC1Doc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CImageMFC1Doc 진단

#ifdef _DEBUG
void CImageMFC1Doc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageMFC1Doc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageMFC1Doc 명령


BOOL CImageMFC1Doc::OnOpenDocument(LPCTSTR lpszPathName)
{
if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	CFile rfp;
	rfp.Open(lpszPathName, CFile::modeRead | CFile::typeBinary);
	// 기존입력 메모리 해제
	free2D(m_InputImage, m_inH);
	// 파일 크기 알아내기
	unsigned long fsize = rfp.GetLength();
	// (중요!) 영상의 높이와 폭 계산
	m_inH = m_inW = (int)sqrt((double)fsize);
	// 메모리 할당.	
	m_InputImage = malloc2D(m_inH, m_inW);
	// 파일 --> 메모리로 로딩
	for (int i = 0; i < m_inH; i++)
		rfp.Read(m_InputImage[i],  m_inW);
	rfp.Close();


	return TRUE;
}


void CImageMFC1Doc::free2D(unsigned char** memory, int h)
{
	// TODO: 여기에 구현 코드 추가.
	if (memory != NULL) {
		for (int i = 0; i < h; i++)
			free(memory[i]);
		free(memory);
	}
}


unsigned char** CImageMFC1Doc::malloc2D(int h, int w)
{
	// TODO: 여기에 구현 코드 추가.
	unsigned char** memory;
	memory = (unsigned char**)malloc(h * sizeof(unsigned char*));
	for (int i = 0; i < h; i++)
		memory[i] = (unsigned char*)malloc(w * sizeof(unsigned char));

	// 초기화(클리어)
	for (int i = 0; i < h; i++)
		for (int k = 0; k < w; k++)
			memory[i][k] = 255;

	return memory;
}


void CImageMFC1Doc::OnCloseDocument()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	free2D(m_InputImage, m_inH); // 임의로 만들어둔 2차원 free();
	free2D(m_OutputImage, m_outH);
	CDocument::OnCloseDocument();
}


BOOL CImageMFC1Doc::OnSaveDocument(LPCTSTR lpszPathName)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	CFileDialog SaveDlg(FALSE, L"raw", NULL, OFN_HIDEREADONLY);
	if (SaveDlg.DoModal() != IDOK)
		return FALSE;

	CFile File;
	File.Open(SaveDlg.GetPathName(), CFile::modeCreate | CFile::modeWrite);
	for (int i = 0; i < m_outH; i++)
		File.Write(m_OutputImage[i], m_outW);
	File.Close();

	return CDocument::OnSaveDocument(lpszPathName);
}


void CImageMFC1Doc::OnEqualImage()
{
	// TODO: 여기에 구현 코드 추가.
		// 기존 출력 메모리 해제
	free2D(m_OutputImage, m_outH);
	//(중요!) 출력 영상의 높이와 폭을 결정 --> 알고리즘에 따름
	m_outH = m_inH;
	m_outW = m_inW;
	// 출력 영상 메모리 할당
	m_OutputImage = malloc2D(m_outH, m_outW);

	// 진짜 영상 처리 알고리즘
	for (int i = 0; i < m_inH; i++) {
		for (int k = 0; k < m_inW; k++) {
			m_OutputImage[i][k] = m_InputImage[i][k];
		}
	}
}
