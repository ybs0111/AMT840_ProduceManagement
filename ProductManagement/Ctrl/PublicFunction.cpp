#include "StdAfx.h"
#include "PublicFunction.h"

CPublicFunction clsFunc;

CPublicFunction::CPublicFunction(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		m_pFont[i]	= NULL;
		m_pFont[i]	= new CFont;
		m_pFont[i]->CreateFont(i + 10,0,0,0,900,0,0,0,0,0,0,ANTIALIASED_QUALITY,0, _T("MS Sans Serif"));		//Bitstream Vera Sans Mono
	}
}


CPublicFunction::~CPublicFunction(void)
{
	int i;
	
	for(i=0; i<20; i++)
	{
		if (m_pFont[i] != NULL)
		{
			delete m_pFont[i];
			m_pFont[i] = NULL;
		}
	}
}

LOGFONT CPublicFunction::OnLogFont(int nFontSize)
{
	LOGFONT lf;
	::ZeroMemory(&lf,sizeof(LOGFONT));
	lf.lfHeight = nFontSize;
	lf.lfWeight = 10;
	lf.lfWeight = 10;
	lf.lfItalic = false;
	lf.lfUnderline = false;
	lf.lfStrikeOut = false;
	lf.lfCharSet=VIETNAMESE_CHARSET;

	return lf;
}

CString	CPublicFunction::OnCharToString(char chData[], int nLength)
{
	CString strTmp;
	BSTR	buf;
	int		nCnt;

	nCnt	= MultiByteToWideChar(CP_ACP, 0, chData, nLength, NULL, NULL);
	buf		= SysAllocStringLen(NULL, nCnt);
	MultiByteToWideChar(CP_ACP, 0, chData, nLength, buf, nCnt);
	strTmp.Format(_T("%s"), buf);

	SysFreeString(buf);

	return strTmp;
}


void CPublicFunction::OnStringToChar(CString strData, char chData[])
{
	int nCnt;
	wchar_t *chBuf;

	chBuf = strData.GetBuffer(strData.GetLength());
	nCnt  = WideCharToMultiByte(CP_ACP, 0, chBuf, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, chBuf, -1, chData, nCnt, 0, 0);
}