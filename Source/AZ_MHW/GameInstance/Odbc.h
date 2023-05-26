#pragma once
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include <sqltypes.h>
#include <sql.h>
#include <sqlext.h>
#include <tchar.h>
#include <string>
#include <time.h>
#include <ctime>
#include <vector>
#include <string>
#include <iostream>

//#pragma comment(lib, "legacy_stdio_definitions.lib")
//참고 Roaming::RoamingNotInitializedException C++ 예외 발생시 accessdatabaseengine_X64.exe 설치
using RECORD = std::vector<std::wstring>;

struct dbitem
{
	std::wstring name;
	std::wstring pass;
	int			level;
	int			sex;
};

struct dbitem2
{
	std::wstring col1;
	std::wstring col2;
	std::wstring name;
	int			 count;
};

// 1개의 필드 정보
struct TColDescription
{
	SQLUSMALLINT    icol;
	SQLWCHAR		szColName[80];
	SQLSMALLINT     cchColNameMax;
	SQLSMALLINT     pcchColName;
	SQLSMALLINT     pfSqlType;
	SQLULEN			pcbColDef;
	SQLSMALLINT     pibScale;
	SQLSMALLINT     pfNullable;
	SQLWCHAR		bindData[80];
	SQLINTEGER      bindValue;
	SQLLEN			byteSize;
};

class Odbc
{
public:
	SWORD sRet;
	SQLINTEGER cbRet;

	SQLHENV   g_hEnv; // 환경핸들
	SQLHDBC	  g_hDbc; // 연결핸들
	SQLHSTMT  g_hStmt; // 명령핸들
	SQLHSTMT  g_hReadStmt; // SQLExecute 핸들
	SQLHSTMT  g_hDeleteStmt; // SQLPrepare Delete 핸들
	SQLHSTMT  g_hLoginCheckStmt; // SQLPrepare LoginCheck 핸들
	SQLHSTMT  g_hSignupStmt; // SQLPrepare LoginCheck 핸들

	std::vector<TColDescription>	m_ColumnList;
	std::vector<RECORD>				m_dbDataList;
	std::vector<dbitem>				m_dbList;
	TCHAR m_szSelectName[10] = { 0, };

	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT    g_hSelectAllStmt;
	TCHAR		m_szReadName[10] = { 0, };
	SQLINTEGER  m_iDataLength;
	SQLLEN      m_cbColumn;
	void Load();
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hInsertStmt; // SQLPrepare Insert 핸들
	TCHAR	   m_szInsertName[20] = { 0, };
	TCHAR	   m_szInsertPass[20] = { 0, };
	SQLINTEGER m_iSelectLevel;
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hUpdateStmt; // SQLPrepare Update 핸들
	TCHAR	   m_szUpdateName[10] = { 0, };
	TCHAR	   m_szUpdatePass[10] = { 0, };
	SQLINTEGER m_iUpdateLevel;
	SQLINTEGER m_iUpdateSex;
	TIMESTAMP_STRUCT m_ts;
	/// <summary>
	/// 
	/// </summary>
	SQLHSTMT  g_hPassStmt;
	TCHAR	  m_szDeleteName[64] = { 0, };
	// LoginCheck
	TCHAR	   m_szLoginId[20] = { 0, };
	TCHAR	   m_szLoginPw[20] = { 0, };
	SQLINTEGER  m_iLoginIdDataLength;
	SQLINTEGER  m_iLoginPwDataLength;

	/////////////////////
	// inventory table //
	/////////////////////
	SQLHSTMT  g_hInventoryAllStmt; // SQLPrepare SelectAll 핸들
	TCHAR     m_InventoryName[64] = { 0, };
	SQLINTEGER m_InventoryLength;
	SQLLEN m_InventoryColumn;

	std::vector<TColDescription>	inventory_column_list_;
	std::vector<RECORD>				inventory_dbData_List;
	std::vector<dbitem>				inventory_db_List;

	RECORD inventory_data;

public:
	void Init();
	bool CreatePrepare();
	void ErrorMsg(SQLHSTMT  stmt);
	void Connect(std::wstring dbName);
	void ConnetMssql(std::wstring dbName);
	void DisConnect();
	bool AddSQL(dbitem& record);
	bool UpdateSQL(dbitem& record, std::wstring selectName);
	bool ReadRecord(std::wstring selectName);
	bool DeleteSQL(const TCHAR* szName);
	bool LoginCheckSQL(const TCHAR* szName, const TCHAR* szPw);
	RECORD Inventory_Open(const TCHAR* szName);
	bool GetItem(dbitem2& record);
	bool GetItemCount(dbitem2& record);
	bool UseItem(dbitem2& record);
	bool UseItemCount(dbitem2& record);
	bool InventoryToStorage(dbitem2& record);
	bool Item_Mount(dbitem2& record);
	bool Item_Unmount(dbitem2& record);

private:
	// Prepare
	bool CreateUserAllSelect();
	bool CreateSelectWhereName();
	bool CreateInsertAccount();
	bool CreateUpdate();
	bool DeleteLoingInfo();
	bool LoginCheck();
	bool Signup();
	bool InventoryOpenSql();

public:
	int    retID;					SQLLEN  lID;
	TCHAR  retName[25] = { 0, };	SQLLEN  lName;
	TCHAR  retPass[25] = { 0, };	SQLLEN  lPass;
	int    retLevel;				SQLLEN  lLevel;
	int    retSex;					SQLLEN  lSex;
	TIMESTAMP_STRUCT accountTS;		SQLLEN  lAccount;
	TIMESTAMP_STRUCT loginTS;		SQLLEN  llogin;
	TIMESTAMP_STRUCT logoutTS;		SQLLEN  llogout;
};