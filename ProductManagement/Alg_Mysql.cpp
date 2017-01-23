// MySqlAlg.cpp: implementation of the MySqlAlg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ProductManagement.h"
#include "Alg_Mysql.h"
//#include "AlgMemory.h"
#include ".\\Ctrl\\PublicFunction.h"
#include ".\\Ctrl\\Variable.h"
#include "AlgMemory.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAlg_Mysql Mysql;

MYSQL_RES *pRes;
MYSQL_ROW pRow;
MYSQL_FIELD *pFields;

CAlg_Mysql::CAlg_Mysql()
{
	// Mysql DB 정의
	CString str_tmp;

	int i;
	int nNum;

	for (i=0; i<MAX_NUM; i++)
	{
		m_nMySqlFlag[i]	= NO;
		m_pRes[i]		= NULL;
	}
	// Mysql DB 정의
	// Mysql DB 정의
	m_nWorkTableCnt			= 14;

	m_strWorkTableName[0]	= _T("NO");
	m_strWorkTableName[1]	= _T("DATE");
	m_strWorkTableName[2]	= _T("UHP");
	m_strWorkTableName[3]	= _T("SOCKET");
	m_strWorkTableName[4]	= _T("COK");
	m_strWorkTableName[5]	= _T("RUN");
	m_strWorkTableName[6]	= _T("STOP");
	m_strWorkTableName[7]	= _T("IDLE_1");
	m_strWorkTableName[8]	= _T("IDLE_2");
	m_strWorkTableName[9]	= _T("ALARM");
	m_strWorkTableName[10]	= _T("ALARM_C");
	m_strWorkTableName[11]	= _T("MTBI");
	m_strWorkTableName[12]	= _T("RETEST");
	m_strWorkTableName[13]	= _T("PRIME");

	m_nWorkTableNum[0]		= 5;
	m_nWorkTableNum[1]		= 40;
	m_nWorkTableNum[2]		= 10;
	m_nWorkTableNum[3]		= 5;
	m_nWorkTableNum[4]		= 5;
	m_nWorkTableNum[5]		= 10;
	m_nWorkTableNum[6]		= 10;
	m_nWorkTableNum[7]		= 10;
	m_nWorkTableNum[8]		= 10;
	m_nWorkTableNum[9]		= 10;
	m_nWorkTableNum[10]		= 5;
	m_nWorkTableNum[11]		= 10;
	m_nWorkTableNum[12]		= 5;
	m_nWorkTableNum[13]		= 10;

	m_nWorkTableType[0]		= ID_TYPE;
	m_nWorkTableType[1]		= DATEAUTO_TYPE;
	m_nWorkTableType[2]		= INT_TYPE;
	m_nWorkTableType[3]		= INT_TYPE;
	m_nWorkTableType[4]		= INT_TYPE;
	m_nWorkTableType[5]		= DOUBLE_TYPE;
	m_nWorkTableType[6]		= DOUBLE_TYPE;
	m_nWorkTableType[7]		= DOUBLE_TYPE;
	m_nWorkTableType[8]		= DOUBLE_TYPE;
	m_nWorkTableType[9]		= DOUBLE_TYPE;
	m_nWorkTableType[10]	= INT_TYPE;
	m_nWorkTableType[11]	= DOUBLE_TYPE;
	m_nWorkTableType[12]	= INT_TYPE;
	m_nWorkTableType[13]	= DOUBLE_TYPE;


	m_nCycleTableCnt			= 5;

	m_strCycleTableName[0]	= _T("NO");
	m_strCycleTableName[1]	= _T("DATE");
	m_strCycleTableName[2]	= _T("PART");
	m_strCycleTableName[3]	= _T("CYCLE");
	m_strCycleTableName[4]	= _T("TEST");

	m_nCycleTableNum[0]		= 5;
	m_nCycleTableNum[1]		= 40;
	m_nCycleTableNum[2]		= 40;
	m_nCycleTableNum[3]		= 10;
	m_nCycleTableNum[4]		= 10;

	m_nCycleTableType[0]		= ID_TYPE;
	m_nCycleTableType[1]		= DATEAUTO_TYPE;
	m_nCycleTableType[2]		= TEXT_TYPE;
	m_nCycleTableType[3]	= DOUBLE_TYPE;
	m_nCycleTableType[4]	= DOUBLE_TYPE;

	nNum = MySql_Open(_T("localhost"), _T("root"), _T("1111"), 3306);
	if(nNum > -1)	
	{
		WaitTime(nNum, 0, 1000);
		WaitTime(nNum, 1, 300);	

		MySql_Close(nNum);
	}
}

CAlg_Mysql::~CAlg_Mysql()
{
	int i;

	if(m_strquery != NULL)
	{
		for (i=0; i<MAX_NUM; i++)
		{
			if (m_strquery[i] != NULL)
			{
				delete [] m_strquery[i];
				m_strquery[i] = NULL;
			}
		}
	}
}

int CAlg_Mysql::MySql_Open(CString hostname, CString username,  CString password, int port)
{
	// 데이터 베이스 열기
	int i;
	int nNum = -1;

	char chHost[100];
	char chName[100];
	char chPass[100];
	char chQurey[8192];

	CString strMsg;

	for (i=0; i<MAX_NUM; i++)
	{
		if (m_nMySqlFlag[i] == NO)
		{
			nNum = i;
			break;
		}
	}
	mysql_init(&m_pMySql[nNum]);
	
	clsFunc.OnStringToChar(hostname, chHost);
	clsFunc.OnStringToChar(username, chName);
	clsFunc.OnStringToChar(password, chPass);

	if(!(mysql_real_connect(&m_pMySql[nNum], chHost, chName, chPass, NULL, port, NULL, NULL)))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return -1;
	}

	// 한글 지원하기 위해
	clsFunc.OnStringToChar(_T("SET NAMES euckr"), chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return -1;
	}
	
	m_nMySqlFlag[nNum] = YES;

	return nNum;
}

BOOL CAlg_Mysql::Database_Create(int nNum, CString databasename)
{
	// 데이터 베이스 새로 추가하기 
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("CREATE DATABASE ") + databasename;

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Database_Delete(int nNum, CString databasename)
{
	// 데이터 베이스 삭제하기
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("DROP DATABASE ") + databasename;
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	return TRUE;
}

/*
TYNYINT_TYPE			0
SMALLINT_TYPE			1
MEDIUMINT_TYPE			2
INT_TYPE				3
BIGINT_TYPE				4
FLOAT_TYPE				5
DOUBLE_TYPE				6
DATE_TYPE				7
DATETIME_TYPE			8
TIMESTAMP_TYPE			9 
TIME_TYPE				10
YEAR_TYPE				11
CHAR_TYPE				12
VARCHAR_TYPE			13
TINYBLOB_TYPE			14
TINYTEXT_TYPE			15
BLOB_TYPE				16
TEXT_TYPE				17
MEDIUMBLOB_TYPE			18
MEDIUMTEXT_TYPE			19
LONGBLOB_TYPE			20
LONGTEXT_TYPE			21
NUMBER_TYEP				22
*/

BOOL CAlg_Mysql::Table_Create(int nNum, CString database, CString tablename, int itemnum, CString *itemname, int *itemtype, int *itemhundred)
{
	CString strQuery;
	CString strItem;
	CString strTmp;
	CString strMsg;

	char chQurey[8192];

	int i;

 	strQuery = _T("USE ") + database;

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}


	for(i=0; i<itemnum; i++)
	{
		switch(itemtype[i])
		{
			case ID_TYPE:
				strTmp.Format(_T("%s INT PRIMARY KEY NOT NULL AUTO_INCREMENT"), itemname[i]);
				strItem += strTmp;
				break;
			case INT_TYPE:
				strTmp.Format(_T("%s INT"), itemname[i]);
				strItem += strTmp;
				break;
			case FLOAT_TYPE:
				strTmp.Format(_T("%s FLOAT"), itemname[i]);
				strItem += strTmp;
				break;
			case DOUBLE_TYPE:
				strTmp.Format(_T("%s DOUBLE"), itemname[i]);
				strItem += strTmp;
				break;
			case DATE_TYPE:
				strTmp.Format(_T("%s DATE"), itemname[i]);
				strItem += strTmp;
				break;
			case DATEAUTO_TYPE:
				strTmp.Format(_T("%s TIMESTAMP"), itemname[i]);
				strItem += strTmp;
				break;
			case TEXT_TYPE:
				strTmp.Format(_T("%s CHAR(%d)"), itemname[i], itemhundred[i]);
				strItem += strTmp;
				break;	
		}

		if((itemnum - 1) > i)
		{
			strItem += ",";
		}
	}
	strQuery.Format(_T("CREATE TABLE %s(%s);"), tablename, strItem);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strTmp = mysql_error(&m_pMySql[nNum]);

		if(strTmp == _T("Table 'automation_machine' already exists"))
		{
			return TRUE;
		}
		else
		{
			strMsg	= mysql_error(&m_pMySql[nNum]);
			clsMem.OnNormalMessageWrite(strMsg);

			return FALSE;
		}
	}

	return TRUE;
}

BOOL CAlg_Mysql::Table_Delete(int nNum, CString tablename)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery = _T("DROP TABLE ") + tablename;
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::Record_Add(int nNum, CString tablename, CString *itemdata, BOOL bFlag)
{
	// 레코드 추가 
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strValue;
	CString strItem;
	CString strTmp;
	CString strQuery;
	CString strType;
	CString strMsg;

	char chQurey[8192];

	int i;
	int pos;
	int num;

	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		if (bFlag == FALSE)
		{
			Row = mysql_fetch_row(Res);

			if(itemdata[i] != _T(""))
			{
				strItem += Row[0];
				strTmp = Row[1];
				pos = strTmp.Find(_T("("), 0);
				if(pos < 0)
				{
					pos = strTmp.GetLength();
				}
				strType = strTmp.Mid(0, pos);
				strType.MakeUpper();

				strTmp = _T("");
				if(strType == _T("INT"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("FLOAT"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("DOUBLE"))
				{
					strTmp += itemdata[i];
				}
				else if(strType == _T("DATETIME"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("DATE"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("TIMESTAMP"))
				{

				}
				else if(strType == _T("VARCHAR"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else if(strType == _T("CHAR"))
				{
					strTmp.Format(_T("'%s'"), itemdata[i]);
				}
				else
				{
					strTmp += itemdata[i];
				}

				strValue += strTmp;

				if((num - 1) > i)
				{
					strItem += _T(",");
					strValue += _T(",");
				}
			}
		}
		else if (bFlag == TRUE)
		{
			Row = mysql_fetch_row(Res);

			if (i>0)
			{
				if(itemdata[i-1] != _T(""))
				{
					strItem += Row[0];
					strTmp = Row[1];
					pos = strTmp.Find(_T("("), 0);
					if(pos < 0)
					{
						pos = strTmp.GetLength();
					}
					strType = strTmp.Mid(0, pos);
					strType.MakeUpper();

					strTmp = _T("");
					if(strType == _T("INT"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("FLOAT"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("DOUBLE"))
					{
						strTmp += itemdata[i-1];
					}
					else if(strType == _T("DATETIME"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("DATE"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("TIMESTAMP"))
					{

					}
					else if(strType == _T("VARCHAR"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else if(strType == _T("CHAR"))
					{
						strTmp.Format(_T("'%s'"), itemdata[i-1]);
					}
					else
					{
						strTmp += itemdata[i-1];
					}

					strValue += strTmp;

					if((num - 1) > i)
					{
						strItem += _T(",");
						strValue += _T(",");
					}
				}
			}
		}
	}

	strQuery.Format(_T("INSERT INTO %s(%s) VALUES(%s)"), tablename, strItem, strValue);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	mysql_free_result(Res);
	return TRUE;
}

void CAlg_Mysql::MySql_Close(int nNum)
{
	if (m_strquery[nNum] != NULL)
	{
		delete [] m_strquery[nNum];
		m_strquery[nNum] = NULL;
	}

	mysql_close(&m_pMySql[nNum]);

	m_nMySqlFlag[nNum] = NO;
}

BOOL CAlg_Mysql::Database_Select(int nNum, CString databasename)
{
	// 데이터 베이스 선택하기 
	CString strMsg;

	char chDb[1024];

	clsFunc.OnStringToChar(databasename, chDb);
	if(mysql_select_db(&m_pMySql[nNum], chDb))
	{
		return FALSE;
	}

	return TRUE;
}


BOOL CAlg_Mysql::Record_Updata(int nNum, CString tablename, CString search_item_1, CString search_data_1, CString search_item_2, CString search_data_2, CString change_item, CString data)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(search_item_2 == _T(""))
	{
		strQuery.Format(_T("UPDATE %s SET %s='%s' where %s='%s'"), tablename, change_item, data, search_item_1, search_data_1);
	}
	else
	{
		strQuery.Format(_T("UPDATE %s SET %s='%s' where %s='%s' and %s='%s'"), tablename, change_item, data, search_item_1, search_data_1, search_item_2, search_data_2);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	return TRUE;
}

BOOL CAlg_Mysql::Record_Int_Delete(int nNum, CString tablename, CString itemname, CString range, int value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %d"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %d"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %d AND %s < %d"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %d OR %s > %d"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %d"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Float_Delete(int nNum, CString tablename, CString itemname, CString range, float value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f AND %s < %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f OR %s > %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %f"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Double_Delete(int nNum, CString tablename, CString itemname, CString range, double value[])
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f"), tablename, itemname, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f"), tablename, itemname, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s > %f AND %s < %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s < %f OR %s > %f"), tablename, itemname, value[0], itemname, value[1]);
	}
	else
	{
		strQuery.Format(_T("DELETE FROM %s WHERE %s = %f"), tablename, itemname, value[0]);
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Text_Delete(int nNum, CString tablename, CString itemname, CString value)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	strQuery.Format(_T("DELETE FROM %s WHERE %s = '%s'"), tablename, itemname, value);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

BOOL CAlg_Mysql::Record_Date_Delete(int nNum, CString tablename, CString itemname, CString range, COleDateTime time[])
{
	CString strQuery;
	CString strTmp[2];
	CString strMsg;

	char chQurey[8192];

	if(range == _T("<"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s < '%s'"), tablename, itemname, strTmp[0]);
	}
	else if(range == _T(">"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s > '%s'"), tablename, itemname, strTmp[0]);
	}
	else if(range == _T("<>"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s > '%s' AND %s < '%s'"), tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else if(range == _T("><"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s < '%s' OR %s > '%s'"), tablename, itemname, strTmp[0], itemname, strTmp[1]);
	}
	else
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("DELETE FROM %s WHERE %s = '%s'"), tablename, itemname, strTmp[0]);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	return TRUE;
}

long CAlg_Mysql::Record_Int_Search(int nNum, CString tablename, CString itemsearch, CString range, int value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");

	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}

	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %d"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %d"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %d AND %s < %d"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %d OR %s > %d"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %d"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Float_Search(int nNum, CString tablename, CString itemsearch, CString range, float value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += ",";
		}
	}
	
	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f AND %s < %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f OR %s > %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %f"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Double_Search(int nNum, CString tablename, CString itemsearch, CString range, double value[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	//	SELECT name,age from guest where age <= 25 ;
	if(range == _T("<"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T(">"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < %f"), stritem, tablename, itemsearch, value[0]);
	}
	else if(range == _T("<>"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %f <= %s AND %s <= %f"), stritem, tablename, value[0], itemsearch, itemsearch, value[1]);
	}
	else if(range == _T("><"))
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > %f OR %s < %f"), stritem, tablename, itemsearch, value[0], itemsearch, value[1]);
	}
	else
	{
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = %f"), stritem, tablename, itemsearch, value[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Double_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, double x[], double y[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}
	
	//	SELECT name,age from guest where age <= 25 ;
	
	strQuery.Format(_T("SELECT %s FROM %s WHERE %f <= %s AND %s <= %f AND %f <= %s AND %s <= %f"), stritem, tablename, x[0], itemsearch1, itemsearch1, x[1], y[0], itemsearch2, itemsearch2, y[1]);
	
	
	mysql_free_result(Res);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];
	
	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}
		
		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;
	
	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Text_Search(int nNum, CString tablename, CString itemsearch, CString value)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;

	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	//strQuery.Format(_T("SELECT *FROM %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	strQuery.Format(_T("SELECT *FROM %s WHERE %s = '%s'"), tablename, itemsearch, value);

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Text_Search(int nNum, CString tablename, CString itemsearch1, CString itemsearch2, CString value1, CString value2)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	
	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];
	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}
	
	strQuery.Format(_T("SELECT %s FROM %s WHERE %s = '%s' AND %s = '%s'"), stritem, tablename, itemsearch1, value1, itemsearch2, value2);
	
	mysql_free_result(Res);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];
	
	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);
		
		if(Row == NULL)
		{
			mysql_free_result(Res);
		}
		
		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}
		
		m_strquery[nNum][i] = strData;
	}
	
	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;
	
	return m_nfind_cnt[nNum];
}

long CAlg_Mysql::Record_Date_Search(int nNum, CString tablename, CString itemsearch, CString range, COleDateTime time[])
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	CString strQuery;
	CString stritem;
	CString strTmp[2], strData;
	CString strMsg;

	char chQurey[8192];

	int i, j;
	int num;
	m_nfind_cnt[nNum] = 0;
	
	stritem = _T("");
	strQuery.Format(_T("DESCRIBE %s"), tablename);
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return m_nfind_cnt[nNum];
	}
	
	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	if(range == _T("<"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == _T(">"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());

		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}
	else if(range == _T("<>"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
													      time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s > '%s' AND %s < '%s'"), stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else if(range == _T("><"))
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strTmp[1].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[1].GetYear(), 
														  time[1].GetMonth(),
														  time[1].GetDay(),
														  time[1].GetHour(),
														  time[1].GetMinute(),
														  time[1].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s < '%s' OR %s > '%s'"), stritem, tablename, itemsearch, strTmp[0], itemsearch, strTmp[1]);
	}
	else
	{
		strTmp[0].Format(_T("%04d/%02d/%02d %02d:%02d:%02d"), time[0].GetYear(), 
														  time[0].GetMonth(),
														  time[0].GetDay(),
														  time[0].GetHour(),
														  time[0].GetMinute(),
														  time[0].GetSecond());
		strQuery.Format(_T("SELECT %s FROM %s WHERE %s = '%s'"), stritem, tablename, itemsearch, strTmp[0]);
	}

	mysql_free_result(Res);

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		mysql_free_result(m_pRes[nNum]);
		return m_nfind_cnt[nNum];
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	m_nfind_cnt[nNum] = (long)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	m_strquery[nNum] = new CString[m_nfind_cnt[nNum]];

	for(i=0; i<m_nfind_cnt[nNum]; i++)
	{
		num = mysql_num_fields(Res);
		Row = mysql_fetch_row(Res);

		if(Row == NULL)
		{
			mysql_free_result(Res);
		}

		strData = _T("");
		for(j=0; j<num; j++)
		{
			strData += Row[j];
			strData += _T(",");
		}

		m_strquery[nNum][i] = strData;
	}

	mysql_free_result(Res);
	m_nread_cnt[nNum] = 0;

	return m_nfind_cnt[nNum];
}

BOOL CAlg_Mysql::Table_Select(int nNum, CString databasename, CString tablename)
{
	CString strQuery;
	int num, i;
	CString strTmp;
	CString strMsg;

	char chQurey[8192];

	BOOL find;

	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;

	clsFunc.OnStringToChar(_T("SHOW TABLES"), chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);

		return FALSE;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);
	
	find = FALSE;
	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		strTmp = Row[0];

		strTmp.MakeUpper();
		tablename.MakeUpper();
		
		if(strTmp == tablename)
		{
			find = TRUE;
			break;
		}
	}

	mysql_free_result(Res);

	if(find)
	{
		clsFunc.OnStringToChar(tablename, chQurey);
		if(mysql_select_db(&m_pMySql[nNum], chQurey))
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	
	return FALSE;
}

CString CAlg_Mysql::Record_Read(int nNum)
{
	CString strTmp;

	strTmp = _T("");
	m_nread_cnt[nNum]++;

	strTmp = m_strquery[nNum][m_nread_cnt[nNum]-1];
	if(m_nread_cnt == m_nfind_cnt)
	{
		if (m_strquery[nNum] != NULL)
		{
			delete [] m_strquery[nNum];
			m_strquery[nNum] = NULL;
		}
	}
	return strTmp;
}

BOOL CAlg_Mysql::WaitTime(int nNum, int mode, int time)
{
	CString strQuery;

	char chQurey[8192];

	if(mode == 0)	
	{	
		strQuery.Format(_T("set global max_connections=%d"), time);
	}
	else	
	{	
		strQuery.Format(_T("set wait_timeout=%d"), time);	
	}
	
	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		return FALSE;	
	}
	
	return TRUE;    
}

BOOL CAlg_Mysql::Record_Sort(int nNum, int mode, CString tablename, CString itemsearch)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	if(mode == 0)
	{
		strQuery.Format(_T("SELECT * FROM %s ORDER BY %s ASC"), tablename, itemsearch);
	}
	else
	{
		strQuery.Format(_T("SELECT * FROM %s ORDER BY %s DESC"), tablename, itemsearch);
	}

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		return FALSE;	
	}
	
	return TRUE;
}

/*
SELECT * INTO OUTFILE 'YOUR_FILE.CSV'
 FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '"'
 LINES TERMINATED BY '\n'
 FROM YOUR_TABLE;
*/
BOOL CAlg_Mysql::WriteFile(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];
	//DATA
	strQuery.Format(_T("SELECT * INTO OUTFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM CYCLE_DATA"), strFileName, _T('"'));	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);

		AfxMessageBox(strMsg);
		return FALSE;
	}

	return TRUE;
}


BOOL CAlg_Mysql::WriteFile1(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];
	//DATA
	strQuery.Format(_T("SELECT * INTO OUTFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM PRODUCT_DATA"), strFileName, _T('"'));	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);

		AfxMessageBox(strMsg);
		return FALSE;
	}

	return TRUE;
}
/*
Load Data Local InFile 'FILE_PATH' Into Table androids FIELDS TERMINATED BY ',' ENCLOSED BY '"' LINES TERMINATED BY '\n'
*/

BOOL CAlg_Mysql::ReadFile(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	//DATA
	strQuery.Format(_T("LOAD DATA LOCAL INFILE '%s' INTO TABLE PRODUCT_DATA FIELDS TERMINATED BY ',' ENCLOSED BY '%c' LINES TERMINATED BY '\n'"), strFileName, _T('"'));	
//	strQuery.Format("SELECT * INTO INFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM DATA", strFileName, '"');	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		AfxMessageBox(strMsg);
		
		return FALSE;
	}

	return TRUE;
}

BOOL CAlg_Mysql::ReadFile1(int nNum, CString strFileName)
{
	CString strQuery;
	CString strMsg;

	char chQurey[8192];

	//DATA
	strQuery.Format(_T("LOAD DATA LOCAL INFILE '%s' INTO TABLE CYCLE_DATA FIELDS TERMINATED BY ',' ENCLOSED BY '%c' LINES TERMINATED BY '\n'"), strFileName, _T('"'));	
//	strQuery.Format("SELECT * INTO INFILE '%s' FIELDS TERMINATED BY ',' OPTIONALLY ENCLOSED BY '%c' LINES TERMINATED BY '\r\n' FROM DATA", strFileName, '"');	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		AfxMessageBox(strMsg);
		
		return FALSE;
	}

	return TRUE;
}
// SELECT MAX(article) AS article FROM shop
float CAlg_Mysql::Record_Max(int nNum, CString item)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	int num;

	CString strQuery;
	CString stritem;
	CString strMsg;

	char chQurey[8192];

	int i;

	float dVal = 0.0;

	//DATA
	strQuery.Format(_T("SELECT MAX(%s) AS %s FROM DATA"), item, item);	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		return dVal;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	dVal = (float)_wtof(stritem);

	return dVal;
}

float CAlg_Mysql::Record_Min(int nNum, CString item)
{
	MYSQL_RES *Res;
	MYSQL_ROW Row;
	MYSQL_FIELD *Fields;
	int num;

	CString strQuery;
	CString stritem;
	CString strMsg;

	char chQurey[8192];

	int i;

	float dVal = 0.0;

	//DATA
	strQuery.Format(_T("SELECT MIN(%s) AS %s FROM DATA"), item, item);	

	clsFunc.OnStringToChar(strQuery, chQurey);
	if(mysql_query(&m_pMySql[nNum], chQurey))		
	{
		strMsg	= mysql_error(&m_pMySql[nNum]);
		clsMem.OnNormalMessageWrite(strMsg);
		
		return dVal;
	}

	Res = mysql_store_result(&m_pMySql[nNum]);
	num = (int)mysql_num_rows(Res);
	Fields = mysql_fetch_fields(Res);

	for(i=0; i<num; i++)
	{
		Row = mysql_fetch_row(Res);
		
		stritem += Row[0];
		if((num-1) != i)
		{
			stritem += _T(",");
		}
	}

	dVal = (float)_wtof(stritem);

	return dVal;
}

void CAlg_Mysql::ReadWork(COleDateTime tDate)
{
	int nNum;
	int nCount;
	int i, j, k;
	int nPos[3];
	int nTime;

	CString strTime[24] = {_T("23"), _T("00"), _T("01"), _T("02"), _T("03"),
						   _T("04"), _T("05"), _T("06"), _T("07"), _T("08"),
						   _T("09"), _T("10"), _T("11"), _T("12"), _T("13"),
						   _T("14"), _T("15"), _T("16"), _T("17"), _T("18"),
						   _T("19"), _T("20"), _T("21"), _T("22")};

	CString strTemp;
	CString strData;
	CString strTable[14];

	COleDateTime tTime;
	COleDateTime tDB[2];

	nNum = MySql_Open();

	if (nNum < 0) return;

	tTime = tDate - COleDateTimeSpan(1, 0, 0, 0);
	tDB[0].SetDateTime(tTime.GetYear(), 
		               tTime.GetMonth(),
					   tTime.GetDay(),
					   22,
					   59,
					   0);

	tDB[1].SetDateTime(tDate.GetYear(), 
		               tDate.GetMonth(),
					   tDate.GetDay(),
					   22,
					   1,
					   0);

	st_work_info.nPartNo = 0;
	if (Database_Select(nNum, _T("AMT")))
	{
		if (Table_Select(nNum, _T("AMT"), _T("PRODUCT_DATA")))
		{
			nCount = Record_Date_Search(nNum, _T("PRODUCT_DATA"), _T("DATE"), _T("<>"), tDB);
			if (nCount > 0)
			{
				for (i=0; i<nCount; i++)
				{
					strData = Record_Read(nNum);

					nPos[0] =0;
					for (j=0; j<m_nWorkTableCnt; j++)
					{
						nPos[1]		= strData.Find(_T(","), nPos[0] + 1);
						strTable[j]	= strData.Mid(nPos[0], (nPos[1] - nPos[0]));
						nPos[0]		= nPos[1] + 1;
					}

					strTemp = strTable[1].Mid(11, 2);
					for (k=0; k<24; k++)
					{
						if (strTemp == strTime[k])
						{
							nTime = k;
							break;
						}
					}

					if (strTable[2] != _T("") && strTable[3] != _T("") && strTable[4] != _T("") && strTable[5] != _T("") && strTable[6] != _T("") &&
						strTable[7] != _T("") && strTable[8] != _T("") && strTable[9] != _T("") && strTable[10] != _T("") && strTable[11] != _T("") &&
						strTable[12] != _T("") && strTable[13] != _T(""))
					{
						st_work_info.nUph[nTime]		= _wtoi(strTable[2]);
						st_work_info.nSocketOff[nTime]	= _wtoi(strTable[3]);
						st_work_info.nCok[nTime]		= _wtoi(strTable[4]);
						st_work_info.dRunTime[nTime]	= _wtof(strTable[5]);
						st_work_info.dStopTime[nTime]	= _wtof(strTable[6]);
						st_work_info.dIdleTime[0][nTime]= _wtof(strTable[7]);
						st_work_info.dIdleTime[1][nTime]= _wtof(strTable[8]);
						st_work_info.dAlarmTime[nTime]	= _wtof(strTable[9]);
						st_work_info.nAlarmCount[nTime]	= _wtoi(strTable[10]);
						st_work_info.dMtbi[nTime]		= _wtof(strTable[11]);
						st_work_info.nRetest[nTime]		= _wtoi(strTable[12]);
						st_work_info.dPrime[nTime]		= _wtof(strTable[13]);
					}
					else
					{
						AfxMessageBox(_T("Product Data Check Error"));
					}
				}
			}
		}
	}

	MySql_Close(nNum);
}

void CAlg_Mysql::WriteWork(CString *strData)
{
	int nNum;

	CTime			time;
	COleDateTime	tDB[2];

	time	= CTime::GetCurrentTime();				

	tDB[0].SetDateTime(time.GetYear()-1, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	tDB[1].SetDateTime(0, 0, 0, 0, 0, 0);

	nNum = MySql_Open();
	if (nNum < 0) return;

	if(Mysql.Database_Select(nNum, _T("AMT")))
	{
		if(Table_Select(nNum, _T("AMT"), _T("PRODUCT_DATA")))
		{
			Record_Add(nNum, _T("PRODUCT_DATA"), strData);
		}
		else
		{
			// table이 없으면 새로 table 추가
			if(Table_Create(nNum, _T("AMT"), _T("PRODUCT_DATA"), m_nWorkTableCnt, m_strWorkTableName, m_nWorkTableType, m_nWorkTableNum))
			{
				Record_Add(nNum, _T("PRODUCT_DATA"), strData);
			}
		}
	}
	else
	{
		// amt database가 없으면 database 추가
		if(Database_Create(nNum, _T("AMT")))
		{
			// table이 없으면 새로 table 추가
			if(Table_Create(nNum, _T("AMT"), _T("PRODUCT_DATA"), m_nWorkTableCnt, m_strWorkTableName, m_nWorkTableType, m_nWorkTableNum))
			{
				Record_Add(nNum, _T("PRODUCT_DATA"), strData);
			}
		}
	}

	MySql_Close(nNum);
}

void CAlg_Mysql::ReadCycle(COleDateTime tDate)
{
	int nNum;
	int nCount;
	int i, j, k;
	int nPos[3];
	int nTime;

	double dVal[2];
	
	BOOL bFind;

	CString strTime[24] = {_T("22"), _T("23"), _T("00"), _T("01"), _T("02"),
						   _T("03"), _T("04"), _T("05"), _T("06"), _T("07"),
						   _T("08"), _T("09"), _T("10"), _T("11"), _T("12"),
						   _T("13"), _T("14"), _T("15"), _T("16"), _T("17"),
						   _T("18"), _T("19"), _T("20"), _T("21")};

	CString strTemp;
	CString strData;
	CString strTable[5];

	COleDateTime tTime;
	COleDateTime tDB[2];

	nNum = MySql_Open();

	if (nNum < 0) return;

	tTime = tDate - COleDateTimeSpan(1, 0, 0, 0);
	tDB[0].SetDateTime(tTime.GetYear(), 
		               tTime.GetMonth(),
					   tTime.GetDay(),
					   22,
					   01,
					   0);

	tDB[1].SetDateTime(tDate.GetYear(), 
		               tDate.GetMonth(),
					   tDate.GetDay(),
					   22,
					   1,
					   0);

	st_work_info.nPartNo = 0;
	if (Database_Select(nNum, _T("AMT")))
	{
		if (Table_Select(nNum, _T("AMT"), _T("CYCLE_DATA")))
		{
			nCount = Record_Date_Search(nNum, _T("CYCLE_DATA"), _T("DATE"), _T("<>"), tDB);
			if (nCount > 0)
			{
				for (i=0; i<nCount; i++)
				{
					strData = Record_Read(nNum);

					nPos[0] =0;
					for (j=0; j<m_nCycleTableCnt; j++)
					{
						nPos[1]		= strData.Find(_T(","), nPos[0] + 1);
						strTable[j]	= strData.Mid(nPos[0], (nPos[1] - nPos[0]));
						nPos[0]		= nPos[1] + 1;
					}

					strTemp = strTable[1].Mid(11, 2);
			
					for (k=0; k<24; k++)
					{
						if (strTemp == strTime[k])
						{
							nTime = k;
							break;
						}
					}

					if (strTable[2] != _T("") && strTable[3] != _T("") && strTable[4] != _T(""))
					{
						dVal[0]						= _wtof(strTable[3]);
						dVal[1]						= _wtof(strTable[4]);
						st_work_info.dCycleHour[nTime]	+= dVal[0];
						if (dVal[0] > 0) st_work_info.nCycleHour[nTime]++;

						st_work_info.dTestHour[nTime]	+= dVal[1];
						if (dVal[1] > 0) st_work_info.nTestHour[nTime]++;

						if (st_work_info.nPartNo == 0)
						{
							st_work_info.strPartNo[st_work_info.nPartNo] = strTable[2];

							st_work_info.dCyclePart[nTime][st_work_info.nPartNo]	+= dVal[0];
							st_work_info.dCyclePartTot[st_work_info.nPartNo]		+= dVal[0];
							if (dVal[0] > 0.0f)
							{
								st_work_info.nCyclePart[nTime][st_work_info.nPartNo]++;
								st_work_info.nCyclePartTot[st_work_info.nPartNo]++;
							}

							st_work_info.dTestPart[nTime][st_work_info.nPartNo]		+= dVal[1];
							st_work_info.dTestPartTot[st_work_info.nPartNo]			+= dVal[1];
							if (dVal[1] > 0.0f)
							{
								st_work_info.nTestPart[nTime][st_work_info.nPartNo]++;
								st_work_info.nTestPartTot[st_work_info.nPartNo]++;
							}

							st_work_info.nPartNo++;
						}
						else
						{
							bFind = FALSE;
							for (k=0; k<st_work_info.nPartNo; k++)
							{
								if (st_work_info.strPartNo[k] == strTable[2])
								{
									st_work_info.dCyclePart[nTime][k]	+= dVal[0];
									st_work_info.dCyclePartTot[k]		+= dVal[0];
									if (dVal[0] > 0.0f)
									{
										st_work_info.nCyclePart[nTime][k]++;
										st_work_info.nCyclePartTot[k]++;
									}

									st_work_info.dTestPart[nTime][k]		+= dVal[1];
									st_work_info.dTestPartTot[k]			+= dVal[1];
									if (dVal[1] > 0.0f)
									{
										st_work_info.nTestPart[nTime][k]++;
										st_work_info.nTestPartTot[k]++;
									}

									bFind = TRUE;
									break;
								}
							}

							if (!bFind)
							{
								st_work_info.strPartNo[st_work_info.nPartNo] = strTable[2];

								st_work_info.dCyclePart[nTime][st_work_info.nPartNo]	+= dVal[0];
								st_work_info.dCyclePartTot[st_work_info.nPartNo]		+= dVal[0];
								if (dVal[0] > 0.0f)
								{
									st_work_info.nCyclePart[nTime][st_work_info.nPartNo]++;
									st_work_info.nCyclePartTot[st_work_info.nPartNo]++;
								}

								st_work_info.dTestPart[nTime][st_work_info.nPartNo]		+= dVal[1];
								st_work_info.dTestPartTot[st_work_info.nPartNo]			+= dVal[1];
								if (dVal[1] > 0.0f)
								{
									st_work_info.nTestPart[nTime][st_work_info.nPartNo]++;
									st_work_info.nTestPartTot[st_work_info.nPartNo]++;
								}

								st_work_info.nPartNo++;
							}
						}
					}
					else
					{
						AfxMessageBox(_T("Cycle Data Check Error"));
					}
				}
			}
		}
	}

	MySql_Close(nNum);
}

void CAlg_Mysql::WriteCycle(CString *strData)
{
	int nNum;

	CTime			time;
	COleDateTime	tDB[2];

	time	= CTime::GetCurrentTime();				

	tDB[0].SetDateTime(time.GetYear()-1, time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	tDB[1].SetDateTime(0, 0, 0, 0, 0, 0);

	nNum = MySql_Open();
	if (nNum < 0) return;

	if(Mysql.Database_Select(nNum, _T("AMT")))
	{
		if(Table_Select(nNum, _T("AMT"), _T("CYCLE_DATA")))
		{
			Record_Add(nNum, _T("CYCLE_DATA"), strData);
		}
		else
		{
			// table이 없으면 새로 table 추가
			if(Table_Create(nNum, _T("AMT"), _T("CYCLE_DATA"), m_nCycleTableCnt, m_strCycleTableName, m_nCycleTableType, m_nCycleTableNum))
			{
				Record_Add(nNum, _T("CYCLE_DATA"), strData);
			}
		}
	}
	else
	{
		// amt database가 없으면 database 추가
		if(Database_Create(nNum, _T("AMT")))
		{
			// table이 없으면 새로 table 추가
			if(Table_Create(nNum, _T("AMT"), _T("CYCLE_DATA"), m_nCycleTableCnt, m_strCycleTableName, m_nCycleTableType, m_nCycleTableNum))
			{
				Record_Add(nNum, _T("CYCLE_DATA"), strData);
			}
		}
	}

	MySql_Close(nNum);
}