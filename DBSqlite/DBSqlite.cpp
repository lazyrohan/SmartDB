// DBSqlite.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "Sqlite\sqlite3.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	sqlite3* pDBCon = NULL; //sqilte connection object
	sqlite3_stmt* pExecute = NULL;

	//Create or open database
	char dbFileName[] = "SmartDB";
	int iResult = sqlite3_open_v2(dbFileName, &pDBCon, SQLITE_OPEN_CREATE | SQLITE_OPEN_READWRITE, NULL);
	cout << "Database open result: " << iResult << endl;
	//cout << "SQLITE_OK= " << SQLITE_OK << endl;

	if (iResult != SQLITE_OK)
	{
		cout << "Can`t open or create database file: " << dbFileName << " !" << endl;
		sqlite3_close(pDBCon);
		return false;
	}

	//Create table if not exist
	//Create table SQL instruction
	const char cCreateTableSQL[] = "CREATE TABLE IF NOT EXISTS ItemInfo(ItemID INTEGER PRIMARY KEY, ItemName TEXT, ItemDesc TEXT)";
	sqlite3_stmt* stmt = NULL;
	int sqlLen = strlen(cCreateTableSQL);

	if (sqlite3_prepare(pDBCon, cCreateTableSQL, sqlLen, &stmt, NULL) != SQLITE_OK)
	{
		if (stmt)
			sqlite3_finalize(stmt);
		sqlite3_close(pDBCon);
		cout << "SQL instruction for create table doesn't work." << endl;
		return false;
	}

	if (sqlite3_step(stmt) != SQLITE_DONE)
	{
		sqlite3_finalize(stmt);
		sqlite3_close(pDBCon);
		cout << "Failed to execute SQL instruction." << endl;
		return false;
	}

	//Release create table resource
	sqlite3_finalize(stmt);
	cout << "Create table succeed." << endl;

	//start a transaction
	sqlite3_stmt* stmtBegin = NULL;
	const char SQLBegin[] = "BEGIN TRANSACTION";
	if (sqlite3_prepare(pDBCon, SQLBegin, strlen(SQLBegin), &stmtBegin, NULL) != SQLITE_OK)
	{
		if (stmtBegin)
			sqlite3_finalize(stmtBegin);
		sqlite3_close(pDBCon);
		cout << "Start transaction failed." << endl;
		//return false;
	}
	if (sqlite3_step(stmtBegin) != SQLITE_DONE)
	{
		sqlite3_finalize(stmtBegin);
		sqlite3_close(pDBCon);
		cout << "Execute tansaction begin failed." << endl;
		//return false;
	}
	sqlite3_finalize(stmtBegin);
	cout << "Begin transaction succeed." << endl;

	// Insert data
	const char SQLInsert[] = "INSERT INTO ItemInfo VALUES(?,?,?)";
	sqlite3_stmt* stmtInsert = NULL;
	if (sqlite3_prepare(pDBCon,SQLInsert,strlen(SQLInsert),&stmtInsert,NULL) != SQLITE_OK)
	{
		if (stmtInsert)
			sqlite3_finalize(stmtInsert);
		sqlite3_close(pDBCon);
		cout << "Insert SQL instruction failed." << endl;
		//return false;
	}
	//sqlite3_finalize(stmtInsert);
	int insertColNo = 10;
	const char tstItem[] = "TstItem";

	for (int i = 0; i < insertColNo;i++)
	{
		//Bind ItemID
		//sqlite3_bind_int(stmtInsert,1,i);
		//Bind ItemName
		sqlite3_bind_text(stmtInsert,2,tstItem,strlen(tstItem),SQLITE_TRANSIENT);
		//Bind ItemDesc
		sqlite3_bind_text(stmtInsert,3,tstItem,strlen(tstItem),SQLITE_TRANSIENT);

		if (sqlite3_step(stmtInsert) != SQLITE_DONE)
		{
			sqlite3_finalize(stmtInsert);
			sqlite3_close(pDBCon);
			cout << "Excute insert failed." << endl;
			//return false;
		}

		sqlite3_reset(stmtInsert);
		cout << "Insert column "<< i<<" succeed."<< endl;
	}
	sqlite3_finalize(stmtInsert);
	
	//Commit transaction
	char SQLCommit[] = "COMMIT";
	sqlite3_stmt* stmtCommit = NULL;
	if (sqlite3_prepare(pDBCon,SQLCommit,strlen(SQLCommit),&stmtCommit,NULL) != SQLITE_OK)
	{
		if (stmtCommit)
			sqlite3_finalize(stmtCommit);
		sqlite3_close(pDBCon);
		cout << "Prepare commit instruction failed." << endl;
		//return false;
	}
	if (sqlite3_step(stmtCommit) != SQLITE_DONE)
	{
		sqlite3_finalize(stmtCommit);
		sqlite3_close(pDBCon);
		cout << "Execute commit failed." << endl;
		//return false;
	}
	sqlite3_finalize(stmtCommit);
	cout << "Insert "<<insertColNo<<" rows succeed." << endl;

	//Query database
	const char SQLSelect[] = "SELECT ItemID FROM ItemInfo";
	sqlite3_stmt* stmtSelect = NULL;
	if (sqlite3_prepare(pDBCon,SQLSelect,strlen(SQLSelect),&stmtSelect,NULL) !=SQLITE_OK)
	{
		if (stmtSelect)
			sqlite3_finalize(stmtSelect);
		sqlite3_close(pDBCon);
		cout << "Prepare select instruction failed." << endl;
		//return false;
	}

	int colCount = sqlite3_column_count(stmtSelect);
	int rowCount = sqlite3_data_count(stmtSelect);
	cout << "Found "<<colCount<<" columns "<<rowCount<<" datas at database." << endl;

	

	//Close database
	sqlite3_close(pDBCon);

	//For debug
	system("pause");

	return 0;
}

