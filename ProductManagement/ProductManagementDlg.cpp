
// ProductManagementDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ProductManagement.h"
#include "ProductManagementDlg.h"
#include "afxdialogex.h"
#include ".\\Ctrl\\Variable.h"
#include ".\\Ctrl\\PublicFunction.h"
#include ".\\Ctrl\\ChartCtrl\\ChartDateTimeAxis.h"
#include ".\\Ctrl\\ChartCtrl\\ChartLineSerie.h"
#include ".\\Ctrl\\ChartCtrl\\ChartPointsSerie.h"
#include "Alg_Mysql.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ID_OBJECT_GRAPH_8			40007
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
tagWORK_INFO			st_work_info;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CProductManagementDlg 대화 상자




CProductManagementDlg::CProductManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProductManagementDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	st_work_info.nPartNo = 0;
}

void CProductManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_DATA,				m_pGridData);
	DDX_Control(pDX, IDC_CUSTOM_CHART,				m_pChart);
	DDX_Control(pDX, IDC_GROUP_DATA,				m_groupData);
	DDX_Control(pDX, IDC_GROUP_DATE,				m_groupDate);
	DDX_Control(pDX, IDC_BTN_SEARCH,				m_btnSearch);
	DDX_Control(pDX, IDC_BTN_SAVE,					m_btnSave);
	DDX_Control(pDX, IDC_BTN_EXIT,					m_btnExit);
	DDX_Control(pDX, IDC_MONTHCALENDAR_DATE, m_mcDate);
}

BEGIN_MESSAGE_MAP(CProductManagementDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, &CProductManagementDlg::OnBnClickedBtnExit)
	ON_COMMAND(IDC_RADIO_UPH, &CProductManagementDlg::OnRadioUph)
	ON_COMMAND(IDC_RADIO_MTBI, &CProductManagementDlg::OnRadioMtbi)
	ON_COMMAND(IDC_RADIO_ALARM, &CProductManagementDlg::OnRadioAlarm)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CProductManagementDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_SAVE, &CProductManagementDlg::OnBnClickedBtnSave)
	ON_BN_CLICKED(IDC_BTN_DB_SAVE, &CProductManagementDlg::OnBnClickedBtnDbSave)
	ON_BN_CLICKED(IDC_BTN_DB_SAVE2, &CProductManagementDlg::OnBnClickedBtnDbSave2)
END_MESSAGE_MAP()


// CProductManagementDlg 메시지 처리기

BOOL CProductManagementDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	OnInitButton();
	OnInitDigit();
	OnInitLabel();
	OnInitGroupBox();
	OnInitGridData();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CProductManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CProductManagementDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CProductManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CProductManagementDlg::OnInitGridData()
{
	int   i, j;
	int	  max_row, max_col;
	int   row;
	int	  nVal = 0;
	int   nCount;
	int   nShift;
	int   nShiftCount;
	int	  nA, nB, nC;
	int	  nAlarmA, nAlarmB, nAlarmC;

	double dRunTimeA, dRunTimeB, dRunTimeC;
	double dShiftAve;
	double dA, dB, dC;
	double dVal = 0.0f;
	double dAve;

	CString strTemp;
	CRect rect;

	double dRunTime = 0.0f;
	int nAlarm = 0;

	max_row = 17 + (st_work_info.nPartNo * 2); // unloader tray y count
	max_col = 30;

	m_pGridData.SetFrameFocusCell(FALSE);
	m_pGridData.SetTrackFocusCell(FALSE);
	m_pGridData.EnableSelection(FALSE);

	m_pGridData.SetGridLineColor(BLACK_C);
	m_pGridData.SetGridLines(1);

	m_pGridData.SetRowCount(max_row);
	m_pGridData.SetColumnCount(max_col);

	m_pGridData.SetFixedRowCount(2);
	m_pGridData.SetFixedColumnCount(2);
	m_pGridData.SetFixedBkColor(RGB(0,0,200));
	m_pGridData.SetFixedBkColor(RGB(200,200,255));
	m_pGridData.SetTextBkColor(RGB(150,150,200));

	for (i=0; i<max_row; i++) 
	{
		m_pGridData.SetRowHeight(i, 22);

		for (j=0; j<max_col; j++) 
		{
			if (j == 0 || j == 1)
			{
				m_pGridData.SetColumnWidth(j, 80);
			}
			else m_pGridData.SetColumnWidth(j, 60);
				
			m_pGridData.SetItemFormat(i, j, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
			m_pGridData.SetItemState(i, j, GVIS_READONLY);
			m_pGridData.SetItemFont(i, j, &clsFunc.OnLogFont(14));
			m_pGridData.SetItemBkColour(i, j, WHITE_C, BLACK_C);
		}
	}

	row = 0;
	m_pGridData.MergeCells(row, 0, row, 5);
	m_pGridData.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("-"));

	m_pGridData.MergeCells(row, 6, row, 13);
	m_pGridData.SetItemBkColour(row, 6, GREEN_L, BLACK_C);
	m_pGridData.SetItemFont(row, 6, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 6, _T("GY"));

	m_pGridData.MergeCells(row, 14, row, 21);
	m_pGridData.SetItemBkColour(row, 14, RED_L, BLACK_C);
	m_pGridData.SetItemFont(row, 14, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 14, _T("DAY"));

	m_pGridData.MergeCells(row, 22, row, 29);
	m_pGridData.SetItemBkColour(row, 22, GREEN_L, BLACK_C);
	m_pGridData.SetItemFont(row, 22, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 22, _T("SW"));

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("ITEM"));

	m_pGridData.SetItemBkColour(row, 2, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 2, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 2, _T("TOTAL"));

	m_pGridData.SetItemBkColour(row, 3, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 3, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 3, _T("GY"));

	m_pGridData.SetItemBkColour(row, 4, RED_L, BLACK_C);
	m_pGridData.SetItemFont(row, 4, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 4, _T("DAY"));

	m_pGridData.SetItemBkColour(row, 5, GREEN_L, BLACK_C);
	m_pGridData.SetItemFont(row, 5, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 5, _T("SW"));

	m_pGridData.SetItemBkColour(row, 6, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 6, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 6, _T("23"));

	m_pGridData.SetItemBkColour(row, 7, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 7, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 7, _T("24"));

	m_pGridData.SetItemBkColour(row, 8, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 8, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 8, _T("1"));

	m_pGridData.SetItemBkColour(row, 9, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 9, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 9, _T("2"));

	m_pGridData.SetItemBkColour(row, 10, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 10, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 10, _T("3"));

	m_pGridData.SetItemBkColour(row, 11, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 11, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 11, _T("4"));

	m_pGridData.SetItemBkColour(row, 12, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 12, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 12, _T("5"));

	m_pGridData.SetItemBkColour(row, 13, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 13, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 13, _T("6"));

	m_pGridData.SetItemBkColour(row, 14, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 14, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 14, _T("7"));

	m_pGridData.SetItemBkColour(row, 15, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 15, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 15, _T("8"));

	m_pGridData.SetItemBkColour(row, 16, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 16, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 16, _T("9"));

	m_pGridData.SetItemBkColour(row, 17, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 17, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 17, _T("10"));

	m_pGridData.SetItemBkColour(row, 18, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 18, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 18, _T("11"));

	m_pGridData.SetItemBkColour(row, 19, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 19, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 19, _T("12"));

	m_pGridData.SetItemBkColour(row, 20, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 20, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 20, _T("13"));

	m_pGridData.SetItemBkColour(row, 21, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 21, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 21, _T("14"));

	m_pGridData.SetItemBkColour(row, 22, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 22, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 22, _T("15"));

	m_pGridData.SetItemBkColour(row, 23, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 23, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 23, _T("16"));

	m_pGridData.SetItemBkColour(row, 24, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 24, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 24, _T("17"));

	m_pGridData.SetItemBkColour(row, 25, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 25, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 25, _T("18"));

	m_pGridData.SetItemBkColour(row, 26, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 26, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 26, _T("19"));

	m_pGridData.SetItemBkColour(row, 27, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 27, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 27, _T("20"));

	m_pGridData.SetItemBkColour(row, 28, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 28, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 28, _T("21"));

	m_pGridData.SetItemBkColour(row, 29, BLUE_D, WHITE_C);
	m_pGridData.SetItemFont(row, 29, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 29, _T("22"));

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("PRODUCT"));

	nVal		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		nVal += st_work_info.nUph[i];

		nShift += st_work_info.nUph[i];
		if (i == 7)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%d"), nVal);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%d"), nVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("UPH (EA)"));

	nVal		= 0;
	nCount		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		nVal += st_work_info.nUph[i];
		if (st_work_info.nUph[i] > 0) nCount++;

		nShift += st_work_info.nUph[i];
		if (st_work_info.nUph[i] > 0)  nShiftCount++;
		if (i == 7)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)nShiftCount);
			else strTemp.Format(_T("%.1f"), 0.0f);
			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
			nShiftCount	= 0;
		}
		else if (i == 15)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)nShiftCount);
			else strTemp.Format(_T("%.1f"), 0.0f);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
			nShiftCount	= 0;
		}
		else if (i == 23)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)nShiftCount);
			else strTemp.Format(_T("%.1f"), 0.0f);
			m_pGridData.SetItemText(row, 5, strTemp);
		}
		strTemp.Format(_T("%d"), st_work_info.nUph[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nCount > 0) strTemp.Format(_T("%.1f"), (double)nVal / (double)nCount);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("RUN TIME (M)"));
	
	dVal		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dRunTime[i];

		dShiftAve += st_work_info.dRunTime[i];
		if (i == 7)
		{
			dRunTimeA = dShiftAve * (double)60;
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			dRunTimeB = dShiftAve * (double)60;
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			dRunTimeC = dShiftAve * (double)60;
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%.1f"), st_work_info.dRunTime[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	dRunTime = dVal * (double)60;

	strTemp.Format(_T("%.1f"), dVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, GREEN_C, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("CYCLE TIME ALL (S)"));

	dVal		= 0.0f;
	dAve		= 0.0f;
	nCount		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	nA = nB = nC = 0;
	dA = dB = dC = 0.0f;
	for (i=0; i<24; i++)
	{
		if (st_work_info.nCycleHour[i] > 0)
		{
			dAve = st_work_info.dCycleHour[i] / (double) st_work_info.nCycleHour[i];
			nCount++;
		}
		else dAve = 0.0f;
		
		dVal += dAve;

		dShiftAve += dAve;
		if (st_work_info.nCycleHour[i] > 0) nShiftCount++;
		if (i >= 0 && i < 8)
		{
			if (nShiftCount <= 0) dA += 0.0f;
			else
			{
				dA += dShiftAve / (double)nShiftCount;
				nA++;
			}
			dShiftAve		= 0;
			nShiftCount		= 0;
		}
		else if (i >= 8 && i < 16)
		{
			if (nShiftCount <= 0) dB += 0.0f;
			else
			{
				dB += dShiftAve / (double)nShiftCount;
				nB++;
			}
			dShiftAve		= 0;
			nShiftCount		= 0;
		}
		else
		{
			if (nShiftCount <= 0) dC += 0.0f;
			else
			{
				dC += dShiftAve / (double)nShiftCount;
				nC++;
			}
		}
		strTemp.Format(_T("%.1f"), dAve);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nCount > 0)
	{
		dAve = dVal / (double) nCount;
	}
	else dAve = 0.0f;
	strTemp.Format(_T("%.1f"), dAve);
	m_pGridData.SetItemText(row, 2, strTemp);

	if (nA > 0) strTemp.Format(_T("%.1f"), dA / (double)nA);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 3, strTemp);

	if (nB > 0) strTemp.Format(_T("%.1f"), dB / (double)nB);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 4, strTemp);

	if (nC > 0) strTemp.Format(_T("%.1f"), dC / (double)nC);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 5, strTemp);

	for (i=0; i<st_work_info.nPartNo; i++)
	{
		row++;
		m_pGridData.MergeCells(row, 0, row, 1);
		m_pGridData.SetItemBkColour(row, 0, GREEN_C, BLACK_C);
		m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(14));
		m_pGridData.SetItemText(row, 0, st_work_info.strPartNo[i]);
			
		dVal		= 0.0f;
		dAve		= 0.0f;
		nCount		= 0;
		nShift		= 0;
		nShiftCount	= 0;
		dShiftAve	= 0.0f;
		nA = nB = nC = 0;
		dA = dB = dC = 0.0f;
		for (j=0; j<24; j++)
		{
			if (st_work_info.nCyclePart[j][i] > 0)
			{
				dAve = st_work_info.dCyclePart[j][i] / (double) st_work_info.nCyclePart[j][i];

				nCount++;
			}
			else dAve = 0.0f;
			dVal += dAve;

			dShiftAve += dAve;
			if (st_work_info.nCyclePart[j][i] > 0) nShiftCount++;
			if (j >= 0 && j < 8)
			{
				if (nShiftCount <= 0) dA += 0.0f;
				else
				{
					dA += dShiftAve / (double)nShiftCount;
					nA++;
				}

				dShiftAve		= 0;
				nShiftCount		= 0;
			}
			else if (j >= 8 && j < 16)
			{
				if (nShiftCount <= 0) dB += 0.0f;
				else
				{
					dB += dShiftAve / (double)nShiftCount;
					nB++;
				}

				dShiftAve		= 0;
				nShiftCount		= 0;
			}
			else
			{
				if (nShiftCount <= 0) dC += 0.0f;
				else
				{
					dC += dShiftAve / (double)nShiftCount;
					nC++;
				}
			}
			strTemp.Format(_T("%.1f"), dAve);
			m_pGridData.SetItemText(row, j + 6, strTemp);
		}

		if (nCount > 0)
		{
			dAve = dVal / (double) nCount;
		}
		else dAve = 0.0f;
		strTemp.Format(_T("%.1f"), dAve);
		m_pGridData.SetItemText(row, 2, strTemp);

		if (nA > 0) strTemp.Format(_T("%.1f"), dA / (double)nA);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 3, strTemp);

		if (nB > 0) strTemp.Format(_T("%.1f"), dB / (double)nB);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 4, strTemp);

		if (nC > 0) strTemp.Format(_T("%.1f"), dC / (double)nC);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 5, strTemp);
	}

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, GREEN_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("TEST TIME ALL (S)"));

	dVal		= 0.0f;
	dAve		= 0.0f;
	nCount		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	nA = nB = nC = 0;
	dA = dB = dC = 0.0f;
	for (i=0; i<24; i++)
	{
		if (st_work_info.nTestHour[i] > 0)
		{
			dAve = st_work_info.dTestHour[i] / (double) st_work_info.nTestHour[i];

			nCount++;
		}
		else dAve = 0.0f;
		
		dVal += dAve;

		dShiftAve += dAve;
		if (st_work_info.nTestHour[i] > 0) nShiftCount++;
		if (i >= 0 && i < 8)
		{
			if (nShiftCount <= 0) dA += 0.0f;
			else
			{
				dA += dShiftAve / (double)nShiftCount;
				nA++;
			}
			dShiftAve		= 0;
			nShiftCount		= 0;
		}
		else if (i >= 8 && i < 16)
		{
			if (nShiftCount <= 0) dB += 0.0f;
			else
			{
				dB += dShiftAve / (double)nShiftCount;
				nB++;
			}
			dShiftAve		= 0;
			nShiftCount		= 0;
		}
		else
		{
			if (nShiftCount <= 0) dC += 0.0f;
			else
			{
				dC += dShiftAve / (double)nShiftCount;
				nC++;
			}
		}
		strTemp.Format(_T("%.1f"), dAve);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nCount > 0)
	{
		dAve = dVal / (double) nCount;
	}
	else dAve = 0.0f;
	strTemp.Format(_T("%.1f"), dAve);
	m_pGridData.SetItemText(row, 2, strTemp);

	if (nA > 0) strTemp.Format(_T("%.1f"), dA / (double)nA);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 3, strTemp);

	if (nB > 0) strTemp.Format(_T("%.1f"), dB / (double)nB);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 4, strTemp);

	if (nC > 0) strTemp.Format(_T("%.1f"), dC / (double)nC);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 5, strTemp);

	for (i=0; i<st_work_info.nPartNo; i++)
	{
		row++;
		m_pGridData.MergeCells(row, 0, row, 1);
		m_pGridData.SetItemBkColour(row, 0, GREEN_L, BLACK_C);
		m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(14));
		m_pGridData.SetItemText(row, 0, st_work_info.strPartNo[i]);

		dVal		= 0.0f;
		dAve		= 0.0f;
		nCount		= 0;
		nShift		= 0;
		nShiftCount	= 0;
		dShiftAve	= 0.0f;
		nA = nB = nC = 0;
		dA = dB = dC = 0.0f;
		for (j=0; j<24; j++)
		{
			if (st_work_info.nTestPart[j][i] > 0)
			{
				dAve = st_work_info.dTestPart[j][i] / (double) st_work_info.nTestPart[j][i];

				nCount++;
			}
			else dAve = 0.0f;
			dVal += dAve;

			dShiftAve += dAve;
			if (st_work_info.nTestPart[j][i] > 0) nShiftCount++;
			if (j >= 0 && j < 8)
			{
				if (nShiftCount <= 0) dA += 0.0f;
				else
				{
					dA += dShiftAve / (double)nShiftCount;
					nA++;
				}

				dShiftAve		= 0;
				nShiftCount		= 0;
			}
			else if (j >= 8 && j < 16)
			{
				if (nShiftCount <= 0) dB += 0.0f;
				else
				{
					dB += dShiftAve / (double)nShiftCount;
					nB++;
				}

				dShiftAve		= 0;
				nShiftCount		= 0;
			}
			else
			{
				if (nShiftCount <= 0) dC += 0.0f;
				else
				{
					dC += dShiftAve / (double)nShiftCount;
					nC++;
				}
			}

			strTemp.Format(_T("%.1f"), dAve);
			m_pGridData.SetItemText(row, j + 6, strTemp);
		}

		if (nCount > 0)
		{
			dAve = dVal / (double) nCount;
		}
		else dAve = 0.0f;

		strTemp.Format(_T("%.1f"), dAve);
		m_pGridData.SetItemText(row, 2, strTemp);

		if (nA > 0) strTemp.Format(_T("%.1f"), dA / (double)nA);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 3, strTemp);

		if (nB > 0) strTemp.Format(_T("%.1f"), dB / (double)nB);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 4, strTemp);

		if (nC > 0) strTemp.Format(_T("%.1f"), dC / (double)nC);
		else strTemp.Format(_T("%.1f"), 0.0f);
		m_pGridData.SetItemText(row, 5, strTemp);
	}

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("SOCKET OFF (%)"));

	nVal		= 0;
	nCount		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		nVal += st_work_info.nSocketOff[i];
		nCount++;

		nShift += st_work_info.nSocketOff[i];
		nShiftCount++;
		if (i == 7)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)64 / (double) 8 * (double)100);
			else strTemp.Format(_T("%.1f"), 0.0f);

			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
			nShiftCount	= 0;
		}
		else if (i == 15)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)64 / (double) 8 * (double)100);
			else strTemp.Format(_T("%.1f"), 0.0f);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
			nShiftCount	= 0;
		}
		else if (i == 23)
		{
			if (nShiftCount > 0) strTemp.Format(_T("%.1f"), (double)nShift / (double)64 / (double) 8 * (double)100);
			else strTemp.Format(_T("%.1f"), 0.0f);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%d"), st_work_info.nSocketOff[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nCount > 0) strTemp.Format(_T("%.1f"), (double)nVal / (double)64 / (double)24 * (double)100);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 2, strTemp);
	
	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("PRIME YEILD (%)"));

	dVal		= 0;
	nCount		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;

	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dPrime[i];
		if (st_work_info.dPrime[i] > 0.0f) nCount++;

		dShiftAve += st_work_info.dPrime[i];
		if (st_work_info.dPrime[i] > 0.0f) nShiftCount++;
		if (i == 7)
		{
			if (nShiftCount > 0)
			{
				strTemp.Format(_T("%.1f"), dShiftAve / (double)nShiftCount);
				m_pGridData.SetItemText(row, 3, strTemp);
			}
			else
			{
				m_pGridData.SetItemText(row, 3, _T("0.0"));
			}

			dShiftAve		= 0.0f;
			nShiftCount		= 0;
		}
		else if (i == 15)
		{
			if (nShiftCount > 0)
			{
				strTemp.Format(_T("%.1f"), dShiftAve / (double)nShiftCount);
				m_pGridData.SetItemText(row, 4, strTemp);
			}
			else
			{
				m_pGridData.SetItemText(row, 4, _T("0.0"));
			}

			dShiftAve		= 0.0f;
			nShiftCount		= 0;
		}
		else if (i == 23)
		{
			if (nShiftCount > 0)
			{
				strTemp.Format(_T("%.1f"), dShiftAve / (double)nShiftCount);
				m_pGridData.SetItemText(row, 5, strTemp);
			}
			else
			{
				m_pGridData.SetItemText(row, 5, _T("0.0"));
			}
		}

		strTemp.Format(_T("%.1f"), st_work_info.dPrime[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nCount > 0) strTemp.Format(_T("%.1f"), dVal / (double)nCount);
	else strTemp.Format(_T("%.1f"), 0.0f);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("RETEST (EA)"));

	nVal		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		nVal += st_work_info.nRetest[i];

		nShift += st_work_info.nRetest[i];
		if (i == 7)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%d"), st_work_info.nRetest[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%d"), nVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, RED_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("ALARM COUNT (EA)"));

	nVal		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		nVal += st_work_info.nAlarmCount[i];

		nShift += st_work_info.nAlarmCount[i];
		if (i == 7)
		{
			nAlarmA = nShift;
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
		}
		else if (i == 15)
		{
			nAlarmB = nShift;
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
		}
		else if (i == 23)
		{
			nAlarmC = nShift;
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%d"), st_work_info.nAlarmCount[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}

	nAlarm = nVal;
	strTemp.Format(_T("%d"), nVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, RED_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("ALARM TIME  (M)"));

	dVal		= 0.0f;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dAlarmTime[i];

		dShiftAve += st_work_info.dAlarmTime[i];
		if (i == 7)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%.1f"), st_work_info.dAlarmTime[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%.1f"), dVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("MTBI (M)"));

	dVal		= 0.0f;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dMtbi[i];

		dShiftAve += st_work_info.dMtbi[i];
		if (i == 7)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%.1f"), st_work_info.dMtbi[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	if (nAlarm > 0)
	{
		dAve = (dRunTime / (double)nAlarm) / (double)60;
		strTemp.Format(_T("%.1f"), dAve);
	}
	else strTemp.Format(_T("%1.f"), 0.0f);
	m_pGridData.SetItemText(row, 2, strTemp);

	if (nAlarmA > 0)
	{
		dAve = (dRunTimeA / (double)nAlarmA) / (double)60;
		strTemp.Format(_T("%.1f"), dAve);
	}
	else strTemp.Format(_T("%1.f"), 0.0f);
	m_pGridData.SetItemText(row, 3, strTemp);

	if (nAlarmB > 0)
	{
		dAve = (dRunTimeB / (double)nAlarmB) / (double)60;
		strTemp.Format(_T("%.1f"), dAve);
	}
	else strTemp.Format(_T("%1.f"), 0.0f);
	m_pGridData.SetItemText(row, 4, strTemp);

	if (nAlarmC > 0)
	{
		dAve = (dRunTimeC / (double)nAlarmC) / (double)60;
		strTemp.Format(_T("%.1f"), dAve);
	}
	else strTemp.Format(_T("%1.f"), 0.0f);
	m_pGridData.SetItemText(row, 5, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	//m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemBkColour(row, 0, BLUE_D, BLUE_D);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("COK CHANGE (EA)"));

	nVal		= 0;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		m_pGridData.SetItemBkColour(row, i, BLUE_D, BLUE_D);//2016.1122

		nVal += st_work_info.nCok[i];

		nShift += st_work_info.nCok[i];
		if (i == 7)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 3, strTemp);

			nShift		= 0;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 4, strTemp);

			nShift		= 0;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%d"), nShift);
			m_pGridData.SetItemText(row, 5, strTemp);
		}

		strTemp.Format(_T("%d"), st_work_info.nCok[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%d"), nVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("IDLE TIME (M)"));

	dVal		= 0.0f;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dIdleTime[0][i];

		dShiftAve += st_work_info.dIdleTime[0][i];
		if (i == 7)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}
		strTemp.Format(_T("%.1f"), st_work_info.dIdleTime[0][i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%.1f"), dVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	//m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemBkColour(row, 0, BLUE_D, BLUE_D);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("LABEL IDLE TIME (M)"));

	dVal		= 0.0f;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		m_pGridData.SetItemBkColour(row, i, BLUE_D, BLUE_D);//2016.1122

		dVal += st_work_info.dIdleTime[1][i];

		dShiftAve += st_work_info.dIdleTime[1][i];
		if (i == 7)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}
		strTemp.Format(_T("%.1f"), st_work_info.dIdleTime[1][i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%.1f"), dVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	row++;
	m_pGridData.MergeCells(row, 0, row, 1);
	m_pGridData.SetItemBkColour(row, 0, YELLOW_L, BLACK_C);
	m_pGridData.SetItemFont(row, 0, &clsFunc.OnLogFont(16));
	m_pGridData.SetItemText(row, 0, _T("STOP TIME (M)"));

	dVal		= 0.0f;
	nShift		= 0;
	nShiftCount	= 0;
	dShiftAve	= 0.0f;
	for (i=0; i<24; i++)
	{
		dVal += st_work_info.dStopTime[i];
		dShiftAve += st_work_info.dStopTime[i];
		if (i == 7)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 3, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 15)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 4, strTemp);

			dShiftAve		= 0.0f;
		}
		else if (i == 23)
		{
			strTemp.Format(_T("%.1f"), dShiftAve);
			m_pGridData.SetItemText(row, 5, strTemp);
		}
		strTemp.Format(_T("%.1f"), st_work_info.dStopTime[i]);
		m_pGridData.SetItemText(row, i + 6, strTemp);
	}
	strTemp.Format(_T("%.1f"), dVal);
	m_pGridData.SetItemText(row, 2, strTemp);

	m_pGridData.Invalidate(FALSE);
}

void CProductManagementDlg::OnInitButton()
{
	m_btnExit.SetBitmaps(IDC_BTN_EXIT, IDB_BITMAP_EXIT_DN, WINDOW_DN, IDB_BITMAP_EXIT_UP, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnExit.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnExit.SetFont(clsFunc.m_pFont[3]);
	m_btnExit.SetTooltipText(_T("Program Exit"));
//	m_btnMainData.EnableWindow(FALSE);

	m_btnSearch.SetBitmaps(IDC_BTN_SEARCH, IDB_BITMAP_SEARCH_DN, WINDOW_DN, IDB_BITMAP_SEARCH_UP, WINDOW_UP);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSearch.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSearch.SetFont(clsFunc.m_pFont[3]);
	m_btnSearch.SetTooltipText(_T("Data Search"));

	m_btnSave.SetBitmaps(IDC_BTN_SAVE, IDB_BITMAP_SAVE_DN, WINDOW_DN, IDB_BITMAP_SAVE_UP, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_IN, WINDOW_DN);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_OUT, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_BK_FOCUS, WINDOW_UP);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_IN, BLACK_C);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_OUT, BLUE_C);
	m_btnSave.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, BLUE_C);
	m_btnSave.SetFont(clsFunc.m_pFont[3]);
	m_btnSave.SetTooltipText(_T("Data Save"));
}

void CProductManagementDlg::OnInitDigit()
{

}


void CProductManagementDlg::OnInitLabel()
{

}

void CProductManagementDlg::OnInitGroupBox()
{
	m_groupDate.SetFont(clsFunc.OnLogFont(14));
	m_groupDate.SetCatptionTextColor(BLUE_C);
	m_groupDate.SetBorderColor(ORANGE_C);
	m_groupDate.SetFontBold(TRUE);
	//m_groupDate.SetBackgroundColor(WINDOW_UP);

	m_groupData.SetFont(clsFunc.OnLogFont(14));
	m_groupData.SetCatptionTextColor(BLUE_C);
	m_groupData.SetBorderColor(ORANGE_C);
	m_groupData.SetFontBold(TRUE);
	//m_groupData.SetBackgroundColor(WINDOW_UP);
}

void CProductManagementDlg::OnBnClickedBtnExit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_pChart.RemoveAllSeries();

	CDialog::OnOK();
}

void CProductManagementDlg::ChartDisplay(int nMode)
{
	CChartStandardAxis	*pBottomAxis;
	CChartStandardAxis	*pLeftAxis;
	CChartStandardAxis	*pRightAxis;
	
	CChartXYSerie		*pSeriesL[3];
	CChartXYSerie		*pSeriesP[3];
	CChartLineSerie		*pLineSeries[3];
	CChartPointsSerie	*pPointsSeries[3];

	double XValues[25];
	double YValues[25];

	int i;
	int nVal;

	pSeriesL[0] = NULL;
	pSeriesL[1] = NULL;
	pSeriesP[0] = NULL;
	pSeriesP[1] = NULL;
/*
	nVal =  m_pChart.GetSeriesCount();
	for (i=0; i<nVal; i++)
	{
		m_pChart.RemoveAllSeries();
	}*/
	m_pChart.RemoveAllSeries();

	pBottomAxis = m_pChart.CreateStandardAxis(CChartCtrl::BottomAxis);
	pBottomAxis->SetMinMax(0, 24); 

	pLeftAxis = m_pChart.CreateStandardAxis(CChartCtrl::LeftAxis);
	pLeftAxis->SetMinMax(0, 1500);

	pRightAxis = m_pChart.CreateStandardAxis(CChartCtrl::RightAxis);
	pRightAxis->SetMinMax(0, 25000);
	
	// 900 기준 Line 그리기
	for (i=0; i<25; i++)
	{
		XValues[i] = i;
		YValues[i] = 900;
	}
	//										  false(하단기준) true(상단기준), false(좌측기준) true(우측기준)
	pLineSeries[0] = m_pChart.CreateLineSerie(false, false);
	pSeriesL[0] = pLineSeries[0];

	pSeriesL[0]->SetName(_T("Purpose"));
	pLineSeries[0]->SetWidth(1);
	pLineSeries[0]->SetPenStyle(1);
	
	pSeriesL[0]->SetColor(RED_C);
	pSeriesL[0]->SetPoints(XValues,YValues,25);

	// uph 생산량 그리기
	for (i=0; i<24; i++)
	{
		XValues[i] = i + 1;
		if (st_work_info.nUph[i] < 0) st_work_info.nUph[i] = 0;
		YValues[i] = st_work_info.nUph[i];
	}
	//                                            하단기준, 좌측기준
	pPointsSeries[1] = m_pChart.CreatePointsSerie(false, false);
	pSeriesP[1] = pPointsSeries[1];
	pSeriesP[1]->SetName(_T("UPH"));
	pPointsSeries[1]->SetPointType(CChartPointsSerie::ptRectangle);
	pPointsSeries[1]->SetPointSize(6, 6);
	pSeriesP[1]->SetColor(BLUE_C);
	pSeriesP[1]->SetPoints(XValues,YValues,24);
	
	pLineSeries[1] = m_pChart.CreateLineSerie(false, false);
	pSeriesL[1] = pLineSeries[1];
	pLineSeries[1]->SetWidth(2);
	pLineSeries[1]->SetPenStyle(0);
	pSeriesL[1]->SetColor(BLUE_C);
	pSeriesL[1]->SetPoints(XValues,YValues,24);

	//
	nVal = 0;
	for (i=0; i<24; i++)
	{
		XValues[i]	= i + 1;
		YValues[i]	= nVal + st_work_info.nUph[i];
		nVal		+= st_work_info.nUph[i];
	}
	
	pPointsSeries[2] = m_pChart.CreatePointsSerie(false, true);
	pSeriesP[2] = pPointsSeries[2];
	pSeriesP[2]->SetName(_T("Total"));
	pPointsSeries[2]->SetPointType(CChartPointsSerie::ptRectangle);
	pPointsSeries[2]->SetPointSize(6, 6);
	pSeriesP[2]->SetColor(YELLOW_C);
	pSeriesP[2]->SetPoints(XValues,YValues,24);

	pLineSeries[2] = m_pChart.CreateLineSerie(false, true);
	pSeriesL[2] = pLineSeries[2];
	pLineSeries[2]->SetWidth(2);
	pLineSeries[2]->SetPenStyle(0);
	pSeriesL[2]->SetColor(YELLOW_C);
	pSeriesL[2]->SetPoints(XValues,YValues,24);

	m_pChart.GetLegend()->SetVisible(true);
	m_pChart.RefreshCtrl();
}

void CProductManagementDlg::OnRadioUph()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nChartMode == 0) return;

	m_nChartMode = 0;
	ChartDisplay(m_nChartMode);
}


void CProductManagementDlg::OnRadioMtbi()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nChartMode == 1) return;

	m_nChartMode = 1;
	ChartDisplay(m_nChartMode);
}


void CProductManagementDlg::OnRadioAlarm()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	if (m_nChartMode == 2) return;

	m_nChartMode = 2;
	ChartDisplay(m_nChartMode);
}


void CProductManagementDlg::OnBnClickedBtnSearch()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime time;
	COleDateTime tDB;

	CString strTemp;

	int i, j;

	st_work_info.nPartNo = 0;

	for (i=0; i<24; i++)
	{
		st_work_info.nUph[i]			= 0;			
		st_work_info.nSocketOff[i]		= 0;		
		st_work_info.nCok[i]			= 0;			
		st_work_info.dRunTime[i]		= 0.0f;		
		st_work_info.dStopTime[i]		= 0.0f;		
		st_work_info.dIdleTime[0][i]	= 0.0f;	
		st_work_info.dIdleTime[1][i]	= 0.0f;	
		st_work_info.dAlarmTime[i]		= 0.0f;		
		st_work_info.nAlarmCount[i]		= 0;	
		st_work_info.nRetest[i]			= 0;		
		st_work_info.dMtbi[i]			= 0.0f;	

		st_work_info.dPrime[i]			= 0.0f;
		st_work_info.dCycleHour[i]		= 0.0f;
		st_work_info.dTestHour[i]		= 0.0f;

		st_work_info.nCycleHour[i]		= 0;
		st_work_info.nTestHour[i]		= 0;

		for (j=0; j<100; j++)
		{
			st_work_info.dCyclePart[i][j]	= 0.0f;
			st_work_info.dCycleAve[i][j]	= 0.0f;
			st_work_info.nCyclePart[i][j]	= 0;
		
			st_work_info.dTestPart[i][j]	= 0.0f;
			st_work_info.dTestAve[i][j]		= 0.0f;
			st_work_info.nTestPart[i][j]		= 0;
		}
	}

	for (i=0; i<100; i++)
	{
		st_work_info.strPartNo[i] = _T("");
		
		st_work_info.dCyclePartTot[i]	= 0.0f;
		st_work_info.dTestPartTot[i]	= 0.0f;

		st_work_info.nCyclePartTot[i]	= 0;
		st_work_info.nTestPartTot[i]	= 0;
	}

	//m_dtDate.GetTime(time);
	m_mcDate.GetCurSel(time);
	strTemp.Format(_T("%04d%02d%02d_%02d:%02d:%02d"), time.GetYear(),
													  time.GetMonth(),
													  time.GetDay(),
													  time.GetHour(),
													  time.GetMinute(),
													  time.GetSecond());

	tDB.SetDateTime(time.GetYear(),
					time.GetMonth(),
					time.GetDay(),
					0,
					0,
					0);

	Mysql.ReadWork(tDB);
	Mysql.ReadCycle(tDB);

	OnInitGridData();

	ChartDisplay(0);
}


void CProductManagementDlg::OnBnClickedBtnSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strFileName[3];

	CFileDialog dlgFileOpen(FALSE, 
						    _T("*.*"), 
							NULL, 
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							_T("All Files(*.*)|*.*||"),
							NULL );
	
	if( dlgFileOpen.DoModal()==IDOK ) 
	{
		strFileName[0] = dlgFileOpen.GetFileTitle();
		strFileName[1] = dlgFileOpen.GetFolderPath();
		strFileName[2].Format(_T("%s\\%s.csv"), strFileName[1], strFileName[0]);
		m_pGridData.Save((LPCTSTR)strFileName[2]);
	}
}


void CProductManagementDlg::OnBnClickedBtnDbSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nNum;
	
	CString strTime[2];

	CTime time = CTime::GetCurrentTime();

	nNum = Mysql.MySql_Open();

	if (nNum < 0) return;
/*
	strTime[0] = _T("d:\C_DB_AMT8562201605171546_W.TXT");
	if (Mysql.Database_Select(nNum, _T("AMT")))
	{
		Mysql.WriteFile(nNum, strTime[0]);
	}
	Mysql.MySql_Close(nNum);
*/
	strTime[0] = _T("D:\\DB_AMT8562_201605181401_P.TXT");
	if(Mysql.Database_Select(nNum, _T("AMT")))
	{
		if(Mysql.Table_Create(nNum, _T("AMT"), _T("PRODUCT_DATA"), Mysql.m_nWorkTableCnt, Mysql.m_strWorkTableName, Mysql.m_nWorkTableType, Mysql.m_nWorkTableNum))
		{
			Mysql.ReadFile(nNum, strTime[0]);
		}
	}
	else
	{
		if(Mysql.Database_Create(nNum, _T("AMT")))
		{
			if(Mysql.Table_Create(nNum, _T("AMT"), _T("PRODUCT_DATA"), Mysql.m_nWorkTableCnt, Mysql.m_strWorkTableName, Mysql.m_nWorkTableType, Mysql.m_nWorkTableNum))
			{
				Mysql.ReadFile(nNum, strTime[0]);
			}
		}
	}

	Mysql.MySql_Close(nNum);

	nNum = Mysql.MySql_Open();

	if (nNum < 0) return;

	strTime[1] = _T("d:\\DB_AMT8562_201605181401_C.TXT");
	if(Mysql.Database_Select(nNum, _T("AMT")))
	{
		if(Mysql.Table_Create(nNum, _T("AMT"), _T("CYCLE_DATA"), Mysql.m_nCycleTableCnt, Mysql.m_strCycleTableName, Mysql.m_nCycleTableType, Mysql.m_nCycleTableNum))
		{
			Mysql.ReadFile1(nNum, strTime[1]);
		}
	}
	else
	{
		if(Mysql.Database_Create(nNum, _T("AMT")))
		{
			if(Mysql.Table_Create(nNum, _T("AMT"), _T("CYCLE_DATA"), Mysql.m_nCycleTableCnt, Mysql.m_strCycleTableName, Mysql.m_nCycleTableType, Mysql.m_nCycleTableNum))
			{
				Mysql.ReadFile1(nNum, strTime[1]);
			}
		}
	}

	/*if (Mysql.Database_Select(nNum, _T("AMT")))
	{
		Mysql.WriteFile1(nNum, strTime[1]);
	}*/
	Mysql.MySql_Close(nNum);
}


void CProductManagementDlg::OnBnClickedBtnDbSave2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strTime[2];
	int nNum;

	CTime time = CTime::GetCurrentTime();

	nNum = Mysql.MySql_Open();

	if (nNum < 0) return;

	strTime[0].Format(_T("d:\\DB_AMT8562_%04d%02d%02d%02d%02d_P.TXT"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	if (Mysql.Database_Select(nNum, _T("AMT")))
	{
		Mysql.WriteFile1(nNum, strTime[0]);
	}
	Mysql.MySql_Close(nNum);
	
	nNum = Mysql.MySql_Open();

	if (nNum < 0) return;

	strTime[1].Format(_T("d:\\DB_AMT8562_%04d%02d%02d%02d%02d_C.TXT"), time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute());
	if (Mysql.Database_Select(nNum, _T("AMT")))
	{
		Mysql.WriteFile(nNum, strTime[1]);
	}
	Mysql.MySql_Close(nNum);
}
