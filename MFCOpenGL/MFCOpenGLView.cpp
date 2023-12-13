
// MFCOpenGLView.cpp: CMFCOpenGLView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCOpenGL.h"
#endif

#include "MFCOpenGLDoc.h"
#include "MFCOpenGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCOpenGLView

IMPLEMENT_DYNCREATE(CMFCOpenGLView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenGLView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
//	ON_WM_MOUSEHWHEEL()
ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CMFCOpenGLView 构造/析构

CMFCOpenGLView::CMFCOpenGLView() noexcept
{
	// TODO: 在此处添加构造代码
	
}

CMFCOpenGLView::~CMFCOpenGLView()
{
}

BOOL CMFCOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCOpenGLView 绘图

void CMFCOpenGLView::OnDraw(CDC* /*pDC*/)
{
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	opengl.Render();
}


// CMFCOpenGLView 打印

BOOL CMFCOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CMFCOpenGLView 诊断

#ifdef _DEBUG
void CMFCOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenGLDoc)));
	return (CMFCOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CMFCOpenGLView 消息处理程序


int CMFCOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	opengl.SetupPixelFormat(::GetDC(GetSafeHwnd()));
	opengl.Init();
	SetTimer(1, 60, NULL);

	return 0;
}


void CMFCOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	opengl.Reshape(cx, cy);
}


void CMFCOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnLButtonDown(nFlags, point);
}


void CMFCOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnMouseMove(nFlags, point);
}


void CMFCOpenGLView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnTimer(nIDEvent);
	opengl.Render();
}


void CMFCOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
	if (nChar == ' ') {
		opengl.swapMove();
	}
}


//void CMFCOpenGLView::OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt)
//{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	CView::OnMouseHWheel(nFlags, zDelta, pt);
//}


BOOL CMFCOpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (zDelta > 0) opengl.zoom(1);
	else if (zDelta < 0) opengl.zoom(0);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
