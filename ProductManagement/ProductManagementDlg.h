
// ProductManagementDlg.h : ��� ����
//

#pragma once

#include ".\\Ctrl\\XPGroupBox.h"
#include ".\\Ctrl\\Grid\\GridCtrl.h"
#include ".\\Ctrl\\Grid\\GridURLCell.h"
#include ".\\Ctrl\\Grid\\GridCellCombo.h"
#include ".\\Ctrl\\Grid\\GridCellCheck.h"
#include ".\\Ctrl\\Grid\\GridCellNumeric.h"
#include ".\\Ctrl\\Grid\\GridCellDateTime.h"
#include ".\\Ctrl\\BtnST.h"
#include ".\\Ctrl\\GradientStatic.h"
#include ".\\Ctrl\\ChartCtrl\\ChartCtrl.h"
// CProductManagementDlg ��ȭ ����
class CProductManagementDlg : public CDialogEx
{
public:
	int							m_nChartMode;

	CXPGroupBox					m_groupDate;
	CXPGroupBox					m_groupData;

	CGridCtrl					m_pGridData;

	CButtonST					m_btnSearch;
	CButtonST					m_btnSave;
	CButtonST					m_btnExit;

	CChartCtrl					m_pChart;

	void						OnInitButton();
	void						OnInitGroupBox();
	void						OnInitLabel();
	void						OnInitDigit();
	void						OnInitGridData();
	void						ChartDisplay(int nMode);
// �����Դϴ�.
public:
	CProductManagementDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PRODUCTMANAGEMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExit();
	afx_msg void OnRadioUph();
	afx_msg void OnRadioMtbi();
	afx_msg void OnRadioAlarm();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnSave();
	CMonthCalCtrl m_mcDate;
	afx_msg void OnBnClickedBtnDbSave();
	afx_msg void OnBnClickedBtnDbSave2();
};
