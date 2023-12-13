
// MFCOpenGLView.h: CMFCOpenGLView 类的接口
//

#pragma once
#include "MyOpenGL.h"

class CMFCOpenGLView : public CView
{
protected: // 仅从序列化创建
	CMFCOpenGLView() noexcept;
	DECLARE_DYNCREATE(CMFCOpenGLView)

// 特性
public:
	CMFCOpenGLDoc* GetDocument() const;
	MyOpenGL opengl;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
//	afx_msg void OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // MFCOpenGLView.cpp 中的调试版本
inline CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLDoc*>(m_pDocument); }
#endif

