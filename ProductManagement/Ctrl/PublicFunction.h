#pragma once
class CPublicFunction
{
public:
	CFont			*m_pFont[20];

	LOGFONT			OnLogFont(int nFontSize);

	CString			OnCharToString(char chData[], int nLength);
	void			OnStringToChar(CString strData, char chData[]);

	CPublicFunction(void);
	~CPublicFunction(void);
};

extern CPublicFunction clsFunc;