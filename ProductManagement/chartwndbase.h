/************************************************************************************************
// $Header: /Surveyor/GuiLib/ChartWndBase.h 4     10/22/02 2:01p Scott.pelger $
//***********************************************************************************************
/************************************************************************************************/
/*                                                                                              */
/* File    : ChartWndBase.h                                                                     */
/*                                                                                              */
/* Purpose : base class for the four main windows contained within the CChartCtrl               */
/*                                                                                              */
/* Author  : Scott Pelger                                             Date Created: 10JUN02     */
/*                                                                                              */
/* Revisions                                                                                    */
/*                                                                                              */
/* Engineer              Date        Description                                                */
/*                                                                                              */
/* Scott Pelger          10JUN02     initial version                                            */
/*                                                                                              */
/************************************************************************************************/
#if !defined(AFX_CHARTWNDBASE_H__E4D51A34_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)
#define AFX_CHARTWNDBASE_H__E4D51A34_CD70_11D5_8037_8C7A05B7325A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChartConstants.h"
#include "MyMemDC.h"

/////////////////////////////////////////////////////////////////////////////
// CChartWndBase window

class CChartWndBase : public CWnd
{
// Construction
public:
	CChartWndBase();

// Attributes
public:
protected:
	CRect       m_rcClientRect;
    COLORREF    m_crWindowColor;
    COLORREF    m_crTextColor;
    CFont*      m_pFont;
    BYTE        m_yFontPointSize;
    CString     m_FontName;
    BYTE        m_yFontStyle;

    CMyMemDC*   m_pMemDC;
    CMyMemDC*   m_pPrintDC;

// Operations
public:
    virtual void SetPrinting(CMyMemDC* pPrintDC=NULL) {m_pPrintDC = pPrintDC;};
    virtual void SetWindowRect(CRect* pRect) {m_rcClientRect = *pRect;};
    virtual BOOL SetFont(BYTE byPointSize, char* psFontName, BYTE byStyle=0);
    virtual COLORREF SetTextColor(COLORREF crColor) {COLORREF crTempColor = m_crTextColor; m_crTextColor = crColor; return crTempColor;};
    virtual COLORREF GetTextColor(void) {return m_crTextColor;};
    virtual COLORREF SetWindowColor(COLORREF crColor) {COLORREF crTempColor = m_crWindowColor; m_crWindowColor = crColor; return crTempColor;};
    virtual COLORREF GetWindowColor(void) {return m_crWindowColor;};

    virtual void Draw(CMyMemDC* pMemDC) {ASSERT(FALSE);};
    virtual CFont* GetFont(void) {return m_pFont;};
    virtual int CalculateFontHeight();
    virtual BYTE GetFontPointSize(void) {return m_yFontPointSize;};
    virtual CString GetFontName(void) {return m_FontName;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChartWndBase)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CChartWndBase();

	// Generated message map functions
protected:
	//{{AFX_MSG(CChartWndBase)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC) {return TRUE;};
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARTWNDBASE_H__E4D51A34_CD70_11D5_8037_8C7A05B7325A__INCLUDED_)


//***********************************************************************************************
// END OF FILE
// $Log: /Surveyor/GuiLib/ChartWndBase.h $
// 
// 4     10/22/02 2:01p Scott.pelger
// added OnMouseMove() and CalculateFontHeight(). also added three members
// that store the font point size, style, and name for retrieval
// 
// 3     9/27/02 4:07p Scott.pelger
// in process of adding printing functionality
// 
// 2     9/25/02 8:55a Scott.pelger
// updated chart code to provide image exportation as bmp file
// 
// 1     8/30/02 3:06p Scott.pelger
// initial version
//***********************************************************************************************
