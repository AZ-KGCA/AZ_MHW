#include "Odbc.h"

void Odbc::Load()
{
	m_dbDataList.clear();
	m_ColumnList.clear();

	// ��ü ���ڵ� ��ȸ, �߰�, ����, ����
	/*TCHAR sql[] = L"select * from tblCigar";
	SQLRETURN hr = SQLExecDirect(g_odbc.g_hStmt, sql, SQL_NTS);*/
	SQLRETURN hr = SQLExecute(g_hSelectAllStmt);
	if (hr != SQL_SUCCESS)
	{
		ErrorMsg(g_hSelectAllStmt);
		return;
	}
	SQLLEN  count; // update, insert, delete ��밡���Դ�.
	SQLRETURN ret = SQLRowCount(g_hSelectAllStmt, &count);

	SQLSMALLINT colCount;
	SQLNumResultCols(g_hSelectAllStmt, &colCount);

	TColDescription col;

	for (int iCol = 1; iCol < colCount + 1; iCol++)
	{
		col.icol = iCol;
		hr = SQLDescribeCol(g_hSelectAllStmt,
			col.icol,
			col.szColName,
			sizeof(col.szColName),
			&col.pcchColName,
			&col.pfSqlType,
			&col.pcbColDef,
			&col.pibScale,
			&col.pfNullable);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg(g_hSelectAllStmt);
			break;
		}
		m_ColumnList.push_back(col);
	}


	while (SQLFetch(g_hSelectAllStmt) != SQL_NO_DATA)
	{
		RECORD record;
		dbitem dtItem;
		for (int iCol = 0; iCol < m_ColumnList.size(); iCol++)
		{
			/* SQLBindCol ��ü�Ѵ�.
			   �������� ������� ��� ���� ��Ʈ������ �ްڴ�.*/
			hr = SQLGetData(g_hSelectAllStmt, m_ColumnList[iCol].icol,
				SQL_WCHAR, m_ColumnList[iCol].bindData,
				sizeof(m_ColumnList[iCol].bindData), NULL);
			if (hr == SQL_SUCCESS)
			{
				record.push_back(m_ColumnList[iCol].bindData);
			}
		}
		if (hr == SQL_SUCCESS)
		{
			m_dbDataList.push_back(record);
		}
	}
	if (g_hSelectAllStmt) SQLCloseCursor(g_hSelectAllStmt);

	SQLFreeStmt(g_hSelectAllStmt, SQL_CLOSE);
} // End Load

void Odbc::Init()
{
	// ȯ���ڵ�(g_hEnv), �����ڵ�(g_hDbc), ����ڵ�(g_hStmt)

	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ� 
	SQLRETURN hr = SQLAllocHandle(SQL_HANDLE_ENV, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		SQL_NULL_HANDLE, // ������ �θ� �ڵ� ����
		&g_hEnv // ������ �ڵ��� �ּ�
	);
	if (hr != SQL_SUCCESS)
	{
		return;
	}

	// SQLSetEnvAttr : ȯ���ڵ� �Ӽ� ����
	if (SQLSetEnvAttr(g_hEnv, // ȯ��Ӽ� �ڵ�
		SQL_ATTR_ODBC_VERSION, // �Ӽ��� ���� ����
		(SQLPOINTER)SQL_OV_ODBC3_80, //�Ӽ� �� ����
		SQL_IS_INTEGER) != SQL_SUCCESS // �Ӽ� ���� ����
		)
	{
		ErrorMsg(NULL);
		return;
	}

	if (hr == SQL_SUCCESS)
	{
		//�����ڵ�(g_hDbc)
		// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
		hr = SQLAllocHandle(SQL_HANDLE_DBC, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
			g_hEnv, // ������ �θ� �ڵ� ����
			&g_hDbc // ������ �ڵ��� �ּ�
		);
		if (hr != SQL_SUCCESS)
		{
			ErrorMsg(NULL);
			return;
		}
	}
} // End Init

bool Odbc::CreatePrepare()
{
	// Select All
	//SQLRETURN ret;
	CreateUserAllSelect();

	// Select where
	CreateSelectWhereName();

	// Insert
	//CreateInsertAccount();
	Signup();

	// Update
	CreateUpdate();

	// Delete
	DeleteLoingInfo();

	// Login
	LoginCheck();

	return true;
} // End CreatePrepare

void Odbc::ErrorMsg(SQLHSTMT  stmt)
{
	int value = -1;
	SQLTCHAR sqlState[10] = { 0, };
	SQLTCHAR msg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLTCHAR errorMsg[SQL_MAX_MESSAGE_LENGTH + 1] = { 0, };
	SQLSMALLINT msgLen;
	SQLINTEGER nativeError = 0;

	//SQLRETURN hr;
	// ���տ���
	/*while (hr = SQLGetDiagRec(SQL_HANDLE_STMT, g_hStmt, value, sqlState, &nativeError, msg,
		_countof(msg), &msgLen) != SQL_NO_DATA)*/
		//�ܼ�����
	SQLError(g_hEnv, g_hDbc, g_hStmt,
		sqlState, &nativeError, msg, SQL_MAX_MESSAGE_LENGTH + 1, &msgLen);
	{
		//_stprintf(errorMsg, L"SQLSTATE:%s, ��������:%s, �����ڵ�:%d", sqlState, msg, nativeError);
		UE_LOG(LogTemp, Warning, TEXT("SQLSTATE:%s, state info : %s, error code : %d\n"), sqlState, msg, nativeError);
		::MessageBox(NULL, errorMsg, L"state info", 0);
	}
} // End ErrorMsg

void Odbc::Connect(std::wstring dbName)
{
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	//_stprintf(inConnect, _T("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());
	UE_LOG(LogTemp, Warning, TEXT("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=%s;"), dbName.c_str());

	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, inConnect, _countof(inConnect),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);
	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(NULL);
		return;
	}


	//����ڵ�(g_hStmt)
	// SQLSetEnvAttr : ȯ���ڵ� �Ӽ� ����
	SQLAllocHandle(SQL_HANDLE_STMT, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hStmt // ������ �ڵ��� �ּ�
	);

	CreatePrepare();
} // End Connect

void Odbc::ConnetMssql(std::wstring dbName)
{
	UE_LOG(LogTemp, Warning, TEXT("odbc connect"));
	TCHAR  inConnect[255] = { 0, };
	TCHAR  outConnect[255] = { 0, };
	TCHAR  dir[MAX_PATH] = { 0, };
	GetCurrentDirectory(MAX_PATH, dir);

	TCHAR dsn[] = L"Driver={SQL Server};Server=shader.kr;Address=127.0.01,1433;Network=dbmssocn;Database=test_DB;Uid=sa;Pwd=dpsjwl2012;";
	SQLSMALLINT  cbOutCon;
	SQLRETURN hr = SQLDriverConnect(g_hDbc, NULL, dsn, _countof(dsn),
		outConnect, _countof(outConnect),
		&cbOutCon, SQL_DRIVER_NOPROMPT);

	if (hr != SQL_SUCCESS && hr != SQL_SUCCESS_WITH_INFO)
	{
		// �α��� ����
		UE_LOG(LogTemp, Warning, TEXT("sql fail / hr : %d"), hr);
		ErrorMsg(NULL);
		return;
	}
	else {
		// �α��� ����
		UE_LOG(LogTemp, Warning, TEXT("sql seccess / hr : %d"), hr);
	}

	//����ڵ�(g_hStmt)
	SQLAllocHandle(SQL_HANDLE_STMT, g_hDbc, &g_hStmt);
	CreatePrepare();
} // End ConnetMssql

void Odbc::DisConnect()
{
	if (g_hStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hStmt);
	if (g_hSelectAllStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hSelectAllStmt);
	if (g_hInsertStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hInsertStmt);
	if (g_hUpdateStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hUpdateStmt);
	if (g_hDeleteStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hDeleteStmt);
	if (g_hLoginCheckStmt) SQLFreeHandle(SQL_HANDLE_STMT, g_hLoginCheckStmt);

	if (g_hDbc) SQLFreeHandle(SQL_HANDLE_DBC, g_hDbc);
	if (g_hEnv) SQLFreeHandle(SQL_HANDLE_ENV, g_hEnv);
} // End DisConnect

//bool Odbc::AddSQL(dbitem& record)
//{
//	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
//	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));
//	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
//	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));
//
//	//m_iSelectLevel = record.level;
//
//	// SQLExecute : �غ�� �Ķ���͸� ��ü�ؼ� �ٷ� �����Ѵ�
//	SQLRETURN hr = SQLExecute(g_hInsertStmt);
//	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
//	{
//		ErrorMsg(g_hInsertStmt);
//		return false;
//	}
//	if (g_hInsertStmt) SQLCloseCursor(g_hInsertStmt);
//	
//	// SQLFreeStmt : �ڵ�� ���õ� ��� �ڿ��� ����
//	SQLFreeStmt(g_hInsertStmt, SQL_CLOSE);
//
//	return true;
//} // End AddSQL

bool Odbc::AddSQL(dbitem& record)
{
	ZeroMemory(m_szInsertName, sizeof(m_szInsertName));
	CopyMemory(m_szInsertName, record.name.c_str(), record.name.size() * sizeof(TCHAR));
	ZeroMemory(m_szInsertPass, sizeof(m_szInsertPass));
	CopyMemory(m_szInsertPass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));

	//m_iSelectLevel = record.level;

	// SQLExecute : �غ�� �Ķ���͸� ��ü�ؼ� �ٷ� �����Ѵ�
	SQLRETURN hr = SQLExecute(g_hSignupStmt);
	if (hr != SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		ErrorMsg(g_hSignupStmt);
		return false;
	}
	if (g_hSignupStmt) SQLCloseCursor(g_hSignupStmt);

	// SQLFreeStmt : �ڵ�� ���õ� ��� �ڿ��� ����
	SQLFreeStmt(g_hSignupStmt, SQL_CLOSE);

	return true;
} // End AddSQL

bool Odbc::UpdateSQL(dbitem& record, std::wstring selectName)
{
	// ������ ���̰� NCHAR(10) ���� ũ�� �ȵȴ�. 
	ZeroMemory(m_szSelectName, sizeof(TCHAR) * 10);
	CopyMemory(m_szSelectName, selectName.c_str(), selectName.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdateName, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdateName, record.name.c_str(), record.name.size() * sizeof(TCHAR));

	ZeroMemory(m_szUpdatePass, sizeof(TCHAR) * 10);
	CopyMemory(m_szUpdatePass, record.pass.c_str(), record.pass.size() * sizeof(TCHAR));


	m_iUpdateLevel = record.level;
	m_iUpdateSex = record.sex;

	m_iDataLength = sizeof(m_szUpdateName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));
	SWORD sReturn;
	SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sReturn,
		0, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);
	SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);

	std::time_t now = std::time(NULL); // 1970,01,01, 0��
	std::tm* ptm = std::localtime(&now);

	m_ts.year = ptm->tm_year + 1900;
	m_ts.month = ptm->tm_mon + 1;
	m_ts.day = ptm->tm_mday;
	m_ts.hour = ptm->tm_hour;
	m_ts.minute = ptm->tm_min;
	m_ts.second = ptm->tm_sec;
	m_ts.fraction = 0;

	SQLRETURN hr = SQLExecute(g_hUpdateStmt);
	if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
	{
		if (SQLFetch(g_hUpdateStmt) != SQL_NO_DATA)
		{
			if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
			SQLFreeStmt(g_hUpdateStmt, SQL_RESET_PARAMS);
			SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
			return true;
		}
	}
	ErrorMsg(g_hUpdateStmt);
	if (g_hUpdateStmt) SQLCloseCursor(g_hUpdateStmt);
	SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
	SQLFreeStmt(g_hUpdateStmt, SQL_CLOSE);
	return true;
} // End UpdateSQL

bool Odbc::ReadRecord(std::wstring selectName)
{
	ZeroMemory(retName, sizeof(TCHAR) * 10);
	ZeroMemory(retPass, sizeof(TCHAR) * 10);
	// ���� : sizeof(TCHAR)*10 �� ������ �߿���.
	SQLRETURN hr1 = SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, sizeof(TCHAR) * 10, &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, sizeof(TCHAR) * 10, &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);
	SQLBindCol(g_hReadStmt, 8, SQL_TYPE_TIMESTAMP, &logoutTS, sizeof(logoutTS), &llogout);
	m_iDataLength = sizeof(m_szReadName);
	m_cbColumn = SQL_NTS;


	SQLRETURN ret = SQLBindParameter(g_hReadStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_UNICODE,
		m_iDataLength, 0, m_szReadName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	if (!selectName.empty())
	{
		ZeroMemory(m_szReadName, sizeof(TCHAR) * 10);
		CopyMemory(m_szReadName, selectName.c_str(), selectName.size() * sizeof(TCHAR));
		SQLRETURN hr = SQLExecute(g_hReadStmt);
		if (hr == SQL_SUCCESS || hr == SQL_SUCCESS_WITH_INFO)
		{
			if (SQLFetch(g_hReadStmt) != SQL_NO_DATA)
			{
				if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
				SQLFreeStmt(g_hReadStmt, SQL_RESET_PARAMS);
				SQLFreeStmt(g_hReadStmt, SQL_UNBIND);
				SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
				return true;
			}
		}
	}
	ErrorMsg(g_hReadStmt);
	if (g_hReadStmt) SQLCloseCursor(g_hReadStmt);
	SQLFreeStmt(g_hReadStmt, SQL_CLOSE);
	return false;
}

bool Odbc::DeleteSQL(const TCHAR* szName)
{
	if (szName != nullptr)
	{
		ZeroMemory(m_szDeleteName, sizeof(m_szDeleteName));
		// TODO szName �� 8����Ʈ�� �������� Ȯ���ؾ��� �ӽ÷� m_szDeleteName ������ �޴°ɷ� ����
		CopyMemory(m_szDeleteName, szName, sizeof(m_szDeleteName));
		//CopyMemory(m_szDeleteName, szName, sizeof(szName));

		// SQLExecute : �غ�� �Ķ���͸� ��ü�ؼ� �ٷ� �����Ѵ�
		SQLRETURN hr = SQLExecute(g_hDeleteStmt);

		if (hr == SQL_SUCCESS)
		{
			// SQLFetch ��� ���տ��� ���ε��� �����͸� ��ȯ
			if (SQLFetch(g_hDeleteStmt) != SQL_NO_DATA)
			{
				if (g_hDeleteStmt) SQLCloseCursor(g_hDeleteStmt);
				SQLFreeStmt(g_hDeleteStmt, SQL_CLOSE);
				return true;
			}
		}
	}

	ErrorMsg(g_hDeleteStmt);
	if (g_hDeleteStmt) SQLCloseCursor(g_hDeleteStmt);
	SQLFreeStmt(g_hDeleteStmt, SQL_CLOSE);
	return false;

}

bool Odbc::Signup()
{
	// Login �α��� üũ
	SQLRETURN ret;
	//insert into dbo.user_table(Name, Pass) values('kim', 'dd');
	std::wstring sql7 = L"insert into user_table (Name, Pass) values(?,?)";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hSignupStmt // ������ �ڵ��� �ּ�
	);

	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hSignupStmt, // Handle
		(SQLTCHAR*)sql7.c_str(), // ������ sql ��
		SQL_NTS // TextLength?
	);

	m_iLoginIdDataLength = sizeof(m_szInsertName);
	m_iLoginPwDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hSignupStmt, // ����ڵ�
		1, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT, // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szInsertName, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hSignupStmt, // ����ڵ�
		2, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT, // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iLoginPwDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szInsertPass, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hSignupStmt);
		return false;
	}


	return true;
}

bool Odbc::LoginCheckSQL(const TCHAR* szName, const TCHAR* szPw)
{
	UE_LOG(LogTemp, Warning, TEXT("LoginCheckSQL"));
	if (szName != nullptr && szPw != nullptr)
	{
		ZeroMemory(m_szLoginId, sizeof(m_szLoginId));
		CopyMemory(m_szLoginId, szName, sizeof(m_szLoginId));
		ZeroMemory(m_szLoginPw, sizeof(m_szLoginPw));
		CopyMemory(m_szLoginPw, szPw, sizeof(m_szLoginPw));

		// SQLExecute : �غ�� �Ķ���͸� ��ü�ؼ� �ٷ� �����Ѵ�
		SQLRETURN hr = SQLExecute(g_hLoginCheckStmt);

		if (hr == SQL_SUCCESS)
		{
			// SQLFetch ��� ���տ��� ���ε��� �����͸� ��ȯ
			if (SQLFetch(g_hLoginCheckStmt) != SQL_NO_DATA)
			{
				if (g_hLoginCheckStmt) SQLCloseCursor(g_hLoginCheckStmt);
				SQLFreeStmt(g_hLoginCheckStmt, SQL_CLOSE);
				return true;
			}
		}
	}

	ErrorMsg(g_hLoginCheckStmt);
	if (g_hLoginCheckStmt) SQLCloseCursor(g_hLoginCheckStmt);
	SQLFreeStmt(g_hLoginCheckStmt, SQL_CLOSE);
	return false;
}

bool Odbc::CreateUserAllSelect()
{
	// select * from [table]
	SQLRETURN ret;
	std::wstring sql = L"{CALL usp_User_AllSelect}";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hSelectAllStmt // ������ �ڵ��� �ּ�
	);
	ret = SQLPrepare(g_hSelectAllStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hSelectAllStmt);
		return false;
	}
	return true;
}

bool Odbc::CreateSelectWhereName()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{ ?=CALL usp_Select_WhereName(?)}";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hReadStmt // ������ �ڵ��� �ּ�
	);
	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hReadStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szSelectName);
	m_cbColumn = SQL_NTS;

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hReadStmt, // ����ڵ�
		1, // �Ķ���� ��ȣ
		SQL_PARAM_OUTPUT, // �Ķ���� �뵵
		SQL_C_SHORT, // �Ķ���� ������ Ÿ��(C��)
		SQL_INTEGER, // �Ķ���� ������ Ÿ��(SQL��)
		0, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		&sRet, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		0, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	ret = SQLBindParameter(g_hReadStmt, // ����ڵ�
		2, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT,  // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szSelectName, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hReadStmt);
		return false;
	}

	SQLBindCol(g_hReadStmt, 1, SQL_INTEGER, &retID, 0, &lID);
	SQLBindCol(g_hReadStmt, 2, SQL_UNICODE, retName, _countof(retName), &lName);
	SQLBindCol(g_hReadStmt, 3, SQL_UNICODE, retPass, _countof(retPass), &lPass);
	SQLBindCol(g_hReadStmt, 4, SQL_INTEGER, &retLevel, 0, &lLevel);
	SQLBindCol(g_hReadStmt, 5, SQL_INTEGER, &retSex, 0, &lSex);
	SQLBindCol(g_hReadStmt, 6, SQL_TYPE_TIMESTAMP, &accountTS, sizeof(accountTS), &lAccount);
	SQLBindCol(g_hReadStmt, 7, SQL_TYPE_TIMESTAMP, &loginTS, sizeof(loginTS), &llogin);

	return true;
}

bool Odbc::CreateInsertAccount()
{
	SQLRETURN ret;
	std::wstring sql;
	sql = L"{?=CALL usp_InsertAccount(?,?)}";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT, // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hInsertStmt // ������ �ڵ��� �ּ�
	);
	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hInsertStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hInsertStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_INTEGER,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}
	ret = SQLBindParameter(g_hInsertStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szInsertPass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hInsertStmt);
		return false;
	}

	return true;
}

bool Odbc::CreateUpdate()
{
	SQLRETURN ret;
	std::wstring sql = L"{?=CALL usp_UpdateUserInfo(?,?,?,?,?)}";

	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hUpdateStmt // ������ �ڵ��� �ּ�
	);
	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hUpdateStmt, (SQLTCHAR*)sql.c_str(), SQL_NTS);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	m_iDataLength = sizeof(m_szInsertName);
	m_cbColumn = SQL_NTS;
	::ZeroMemory(&m_ts, sizeof(m_ts));

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hUpdateStmt, 1, SQL_PARAM_OUTPUT, SQL_C_SHORT, SQL_SMALLINT,
		0, 0, &sRet,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 2, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szSelectName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 3, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdateName,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 4, SQL_PARAM_INPUT, SQL_UNICODE, SQL_CHAR,
		m_iDataLength, 0, m_szUpdatePass,
		m_iDataLength, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}
	ret = SQLBindParameter(g_hUpdateStmt, 5, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateLevel,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	ret = SQLBindParameter(g_hUpdateStmt, 6, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER,
		0, 0, &m_iUpdateSex,
		0, &m_cbColumn);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hUpdateStmt);
		return false;
	}

	return true;
}

bool Odbc::DeleteLoingInfo()
{
	// Delete
	SQLRETURN ret;
	std::wstring sql5 = L"delete from user_table where Name=?";
	//std::wstring sql5 = L"select * from user_table where Name =?";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hDeleteStmt // ������ �ڵ��� �ּ�
	);

	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hDeleteStmt, // Handle
		(SQLTCHAR*)sql5.c_str(), // ������ sql ��
		SQL_NTS // TextLength?
	);

	m_iDataLength = sizeof(m_szDeleteName);
	m_cbColumn = SQL_NTS;

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hDeleteStmt, // ����ڵ�
		1, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT, // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szDeleteName, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);
	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hDeleteStmt);
		return false;
	}
	return true;
}

bool Odbc::LoginCheck()
{
	// Login �α��� üũ
	SQLRETURN ret;

	std::wstring sql6 = L"select * from user_table where Name =? and Pass=?";
	//std::wstring sql6 = L"select * from user_table where Name =?";
	// SQLAllocHandle : �ڵ��� �Ҵ� �Լ�
	ret = SQLAllocHandle(SQL_HANDLE_STMT,  // �Ҵ��ϰ����ϴ� �ڵ� Ÿ��
		g_hDbc, // ������ �θ� �ڵ� ����
		&g_hLoginCheckStmt // ������ �ڵ��� �ּ�
	);

	// SQLPrepare �Ķ���͸� �ٲ㼭 sql ����
	ret = SQLPrepare(g_hLoginCheckStmt, // Handle
		(SQLTCHAR*)sql6.c_str(), // ������ sql ��
		SQL_NTS // TextLength?
	);

	m_iLoginIdDataLength = sizeof(m_szLoginId);
	m_iLoginPwDataLength = sizeof(m_szLoginPw);
	m_cbColumn = SQL_NTS;

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hLoginCheckStmt, // ����ڵ�
		1, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT, // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szLoginId, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	// ?�� �Ķ���� ��Ŀ��� �ϸ�, ������ ���ε��� ���������� ��ü ? ��ŭ SQLBindParameter �Ҵ� �ؾ� �Ѵ�.
	ret = SQLBindParameter(g_hLoginCheckStmt, // ����ڵ�
		2, // �Ķ���� ��ȣ
		SQL_PARAM_INPUT, // �Ķ���� �뵵
		SQL_UNICODE, // �Ķ���� ������ Ÿ��(C��)
		SQL_CHAR, // �Ķ���� ������ Ÿ��(SQL��)
		m_iLoginPwDataLength, // �Ķ���� ������ ũ��
		0, // �Ķ���� ������ �ڸ���
		m_szLoginPw, // ���� �Ķ���Ϳ� ����� ������ �ּ�
		m_iDataLength, // �Ķ������ ���ڿ��̳� �������϶� ������ ũ��
		&m_cbColumn // ���̿� ���� �μ�
	);

	if (ret != SQL_SUCCESS)
	{
		ErrorMsg(g_hLoginCheckStmt);
		return false;
	}


	return true;
}
