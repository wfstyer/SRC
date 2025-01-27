//-----------------------------------------------------------------------------
//     Author : York Huang
//       Mail : york.huang@mic.com.tw        
//
//    Copyright 2015-2020 MiTAC Computing Technokogy Corp. All rights reserved.
//-----------------------------------------------------------------------------
// MiAPI_demo.cpp : This the console application will demonstrate MiAPI 5 major
// features : GPIO, Hardware monitoring, Display Control, SMBUS, Watchdog. 

#include "stdafx.h"
#include <Windows.h>
#include <process.h>
#include <stdio.h>
#include <conio.h>
#include <iostream>
#include <time.h> 

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <stdlib.h>
#include <sal.h>
#include <tchar.h>
#include <string>
#include <sstream>

//#include "mysql_connection.h" 

#include "../LIB/MiAPI.h"
//#include "../../../../../../../Windows/Microsoft.NET/Framework/v4.0.30319/System.Data.dll"

//#using < mscorlib.dll>
//#using < System.dll>
//#using < System.Data.dll>
//#using < System.Xml.dll>
//
//using namespace System;
//using namespace System::Data;
//using namespace System::Data::SqlClient;

using namespace std;







// Alternatively add the following pragma comment, instead of setting up referrence dependence 
// in compiler environment setting. Be aware to put the same bits version MiAPI.lib in the source
// folder
#pragma comment(lib, "../LIB/MiAPI.lib")

//--Global varibles 
//bool isRefresh = false;     //It indicates if WDT timer has refreshed or not.
//int TimeToCrash = 0;        //It set a time to simulate application crash for WDT demo.

int i = 0;



//-- Functions start ----

int Do_MiAPI_Version(void)
// This feature demonstrates the BIOS version information provided by motherboard SMBIOS. 
// It also fetches the current MiAPI DLL version. Note: If the error code of MiAPI_GetMiAPIVersion
// returns MiAPI_OLD_VERSION (0x05), it might mean the MB's BIOS implement limited features only,
// and might have few compatibility issues on watchdog,SMBUS and GPIO. Please contact vendor for 
// issue report.     
{
	int Major,Minor;
	char BIOSVersion[80];
	char ProductName[80];
	DWORD size;
	int ret = MiAPI_OK;	
	
	printf("*** Show BIOS & MiAPI version ***\n");

	ret = MiAPI_GetProductName(ProductName, &size); 
	printf("Product name : %s\n", ProductName);
	
	
	ret = MiAPI_GetBIOSVersion(BIOSVersion, &size);
	printf("BIOS version : %s\n", BIOSVersion);
	
	
	ret = MiAPI_GetMiAPIVersion(&Major, &Minor);
	printf("MAPI DLL version : %d.%d \n",Major,Minor);
	
	return ret;
}

int Do_MiAPI_GPIO(void)
// This feature demonstrates the GPIO configuration. It uses a C struct (MIAPI_GPIO_STATUS) to 
// hold the GPIO setting(Input/Output) and value(High/Low). 
{
	int ret = MiAPI_OK;
	MIAPI_GPIO_STATUS gpio[10];
	
		
	printf("*** GPIO demo ***\n");
	printf("This demo will set the GPIO 1~5 as input \n  and GPIO 6~10 as output low.");
	
	//--Set GPIO 1~5 to input
	for(int i = 1; i<=5; i++)
	{
       gpio[i-1].Direction = 0x01;     //input = 0x01; output = 0x00 for MiAPI v3.1;
	   gpio[i-1].VoltageLevel = 0x00;  // for input mode, this voltage level is dummy value as no use.
	   ret = MiAPI_GPIO_SetStatus(i, gpio[i-1]);	   
	}	
		
	//--Set GPIO 6~10 to output low
	for(int i = 6; i<=10; i++)
	{
       gpio[i-1].Direction = 0x00;     //input = 0x01; output = 0x00 for MiAPI v3.1;
	   gpio[i-1].VoltageLevel = 0x00;  // Low = 0x00; High = 0x01
	   ret = MiAPI_GPIO_SetStatus(i, gpio[i-1]);	   
	}	
		
	//-- Show  all GPIO status
	for (int i =1; i <= 10; i++ )
	{
    //--Get GPIO status from each GPIO
	    ret = MiAPI_GPIO_GetStatus((BYTE)i, &(gpio[i-1]));	
		printf("  GPIO %02d  :  DIR=%d  LEVEL=%d (Err=%02Xh)\n", i, gpio[i-1].Direction, gpio[i-1].VoltageLevel, ret);
	}

    return ret;	
}

VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{
	printf("timer expired %02d\n", i);
	i++;
	// write to database ***************************************************************
	// write to database ***************************************************************
	// write to database ***************************************************************
	// write to database ***************************************************************
	// write to database ***************************************************************
	// write to database ***************************************************************
	// write to database ***************************************************************
}

int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	char choosed = 0;
	//-- Start the MiAPI libary
	if (MiAPI_Start() != MiAPI_OK)
	{
		printf("Error: Failed to initialize MiAPI library.\n");
		return MiAPI_INIT_FAIL;
	}
	ret = Do_MiAPI_Version();
	printf("--------------------------------------------------------------\n");





	{

#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 2000
		//define handles and variables
		SQLHANDLE sqlConnHandle;
		SQLHANDLE sqlStmtHandle;
		SQLHANDLE sqlEnvHandle;
		SQLWCHAR retconstring[SQL_RETURN_CODE_LEN];
		//initializations
		sqlConnHandle = NULL;
		sqlStmtHandle = NULL;
		//allocations
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
			goto COMPLETED;
		if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
			goto COMPLETED;
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
			goto COMPLETED;
		//output
		cout << "Attempting connection to SQL Server...";
		cout << "\n";
		switch (SQLDriverConnect(sqlConnHandle,
			NULL,
			(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=WK20018;DATABASE=GADATA01",
			SQL_NTS,
			retconstring,
			1024,
			NULL,
			SQL_DRIVER_NOPROMPT)) {
		case SQL_SUCCESS:
			cout << "Successfully connected to SQL Server";
			cout << "\n";
			break;
		case SQL_SUCCESS_WITH_INFO:
			cout << "Successfully connected to SQL Server";
			cout << "\n";
			break;
		case SQL_INVALID_HANDLE:
			cout << "Could not connect to SQL Server";
			cout << "\n";
			goto COMPLETED;
		case SQL_ERROR:
			cout << "Could not connect to SQL Server";
			cout << "\n";
			goto COMPLETED;
		default:
			break;
		}

//#define SQL_QUERY_SIZE      256 // Max. Num characters for SQL Query passed in.

		//std::wstring wszInput = L"SELECT * FROM Workcells";

		//SQLCHAR		wszInput[SQL_QUERY_SIZE]{ "SELECT * FROM Workcells" };

		//if there is a problem connecting then exit application
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
			cout << "\nSome kind of problem....\n";
			goto COMPLETED;
		}
		//output
		//else
		//{

		//char passinput = "SELECT * FROM Workcells";
		//wszInput = passinput;


		//if there is a problem executing the query then exit application
		//else display query result
		//if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, wszInput, SQL_NTS)) {
		//	cout << "Error querying SQL Server";
		//	cout << "\n";
		//	goto COMPLETED;
		//}
		else {
			cout << "\nMade it to the else statement\n";

			//output
			cout << "\n";
			cout << "Executing T-SQL query...";
			cout << "\n";
			//if there is a problem executing the query then exit application
			//else display query result
			if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, (SQLWCHAR*)L"SELECT DESCRIPTION FROM workcenterlist WHERE WCID = 'L14'", SQL_NTS)) {
				cout << "Error querying SQL Server";
				cout << "\n";
				goto COMPLETED;
			}

			//declare output variable and pointer
			SQLCHAR sqlVersion[SQL_RESULT_LEN];
			SQLINTEGER ptrSqlVersion;
			cout << "\nVariables are set.\n";
			while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
				SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);
				//display query result
				cout << "\nAt Least it didn't crash again!\n";
				cout << "\nQuery Result: ";
				cout << sqlVersion << endl;
				cout << "\nAll that from sqlVersion\n\n";
			}
		}


		//close connection and free resources
	COMPLETED:
		SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
		SQLDisconnect(sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
		SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
		//pause the console window - exit when key is pressed
		cout << "\nPress any key to exit...\n";
		_getch();
	}


	// Add a timer
	{
		MSG Msg;
		/* Set up the timer to go off every half second */
		SetTimer(NULL, 0, 500, TimerProc);
		/* Process all messages */
		while (GetMessage(&Msg, NULL, 0, 0) > 0)
		{
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);

			if (_kbhit())
			{
				char ch = _getch();
				if (ch == VK_ESCAPE)
				{
					break;
				}
			}
		}
	}

	//-- Handle MiAPI　functions
	do {
		printf("\n=== Demo Menu ===\n");
		printf("  0 : Exit\n");
		//printf("  1 : Show BIOS & MiAPI version\n");
		//printf("  2 : HW monitoring\n");
		//printf("  3 : GPIO setting\n");
		//printf("  4 : SMBUS scan\n");
		//printf("  5 : Display Control\n");
		//printf("  6 : Watchdog\n");
		//printf("Choose (0 ~ 6) : ");
		choosed = _getche();
		printf("\n\n");
		
	 //--  Call MiAPI functions 
		//switch(choosed)
		//{
		//   case '0' :  //Exit
		//	   printf(" Exit...\n");
		//	   break;

		//   case '1' :
		//	   ret = Do_MiAPI_Version();
		//	   break;

  //         case '2' :
		//	   ret = DO_MiAPI_HWMonitoring();
		//	   break;

  //         case '3' :
		//	   ret = Do_MiAPI_GPIO();
		//	   break;

  //         case '4' :
		//	   ret = Do_MiAPI_SMBUS_SCAN();
		//	   break;

		//   case '5' :
		//	   ret = Do_MiAPI_DisplayControl();
		//	   break;

  //         case '6' :
		//	   ret = Do_MiAPI_WDT();
		//	   break;		   
		//}

	} while (choosed != '0');
			

	//-- It must call MiAPI_Exit() to free the resource when MiAPI exits. 	
	MiAPI_Exit();	

	return 0;
}

