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
#include <fstream>
#include <time.h> 

#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <stdlib.h>
#include <sal.h>
#include <tchar.h>
#include <wchar.h>
#include <string>
#include <sstream>
#include <cwchar>
//#include "tinyxml2.h"

#include "../LIB/MiAPI.h"

using namespace std;

// Alternatively add the following pragma comment, instead of setting up referrence dependence 
// in compiler environment setting. Be aware to put the same bits version MiAPI.lib in the source
// folder
#pragma comment(lib, "../LIB/MiAPI.lib")

//--Global varibles 
//bool isRefresh = false;     //It indicates if WDT timer has refreshed or not.
//int TimeToCrash = 0;        //It set a time to simulate application crash for WDT demo.

int i = 0;
bool estopmem = true;
//char querytext;

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
	// ******************** remember it didn't have {} originally ********************
	MIAPI_GPIO_STATUS gpio[10]{};
	
	//printf("*** GPIO demo ***\n");
	//printf("This demo will set the GPIO 5~10 as input \n  and GPIO 1~4 as output low.");
	
	//--Set GPIO 5~10 to input
	for(int i = 5; i<=10; i++)
	{
       gpio[i-1].Direction = 0x01;     //input = 0x01; output = 0x00 for MiAPI v3.1;
	   gpio[i-1].VoltageLevel = 0x00;  // for input mode, this voltage level is dummy value as no use.
	   ret = MiAPI_GPIO_SetStatus(i, gpio[i-1]);	   
	}	
		
	//--Set GPIO 1~4 to output low
	for(int i = 1; i<=4; i++)
	{
       gpio[i-1].Direction = 0x00;     //input = 0x01; output = 0x00 for MiAPI v3.1;
	   gpio[i-1].VoltageLevel = 0x00;  // Low = 0x00; High = 0x01
	   ret = MiAPI_GPIO_SetStatus(i, gpio[i-1]);	   
	}	
		

	////-- Show  all GPIO status
	//for (int i =1; i <= 10; i++ )
	//{
 //   //--Get GPIO status from each GPIO
	//    ret = MiAPI_GPIO_GetStatus((BYTE)i, &(gpio[i-1]));	
	//	printf("  GPIO %02d  :  DIR=%d  LEVEL=%d (Err=%02Xh)\n", i, gpio[i-1].Direction, gpio[i-1].VoltageLevel, ret);
	//}

    return ret;	
}

int Read_MiAPI_GPIO(int gpioaddress)
{
	int ret = MiAPI_OK;
	MIAPI_GPIO_STATUS gpio[10]{};
	if (gpioaddress >= 5)
		if (gpioaddress <= 10)
			{
				ret = MiAPI_GPIO_GetStatus((BYTE)gpioaddress, &(gpio[gpioaddress-1]));
				return(gpio[gpioaddress - 1].VoltageLevel);
			}
		else
		{
			return ret;
		}
}

int Write_MiAPI_GPIO(int gpioaddress, bool gpiohighlow)
{
	int ret = MiAPI_OK;
	MIAPI_GPIO_STATUS gpio[10]{};
	if (gpioaddress >= 1)
		if (gpioaddress <= 4)
		{
			gpio[i - 1].Direction = 0x00;     //input = 0x01; output = 0x00 for MiAPI v3.1;
			if (gpiohighlow)
			{
				gpio[gpioaddress - 1].VoltageLevel = 0x01;  // Low = 0x00; High = 0x01
			}
			else
			{
				gpio[gpioaddress - 1].VoltageLevel = 0x00;  // Low = 0x00; High = 0x01
			}
			ret = MiAPI_GPIO_SetStatus(gpioaddress, gpio[gpioaddress - 1]);
		}
	return ret;
}

int Do_SQL_Query(PWCHAR ptrsqlquerytext)
//Attempt to move SQL query to it's own little space
{
#define SQL_RESULT_LEN 240
#define SQL_RETURN_CODE_LEN 2000
		//define handles and variables
	SQLHANDLE	sqlConnHandle;
	SQLHANDLE	sqlStmtHandle;
	SQLHANDLE	sqlEnvHandle;
	SQLWCHAR	retconstring[SQL_RETURN_CODE_LEN];

	int ret = 0;

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
	printf("Attempting connection to SQL Server...");
	printf("\n");
	switch (SQLDriverConnect(sqlConnHandle,
		NULL,
		(SQLWCHAR*)L"DRIVER={SQL Server};SERVER=192.168.9.82;DATABASE=GADATA01",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_NOPROMPT)) {
	case SQL_SUCCESS:
		printf("Successfully connected to SQL Server");
		printf("\n");
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

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle)) {
		cout << "\nSome kind of problem....\n";
		goto COMPLETED;
	}
	else {

		//output
		cout << "\n";
		cout << "Executing T-SQL query...";
		cout << "\n";
		//if there is a problem executing the query then exit application
		//else display query result
		if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, ptrsqlquerytext, SQL_NTS)) {
			cout << "Error querying SQL Server";
			cout << "\n";
			goto COMPLETED;
		}

		//declare output variable and pointer
		SQLCHAR sqlcelltext[SQL_RESULT_LEN];
		SQLINTEGER ptrsqlcelltext;
		while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {
			SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlcelltext, SQL_RESULT_LEN, &ptrsqlcelltext);
			//display query result
			cout << "\nQuery Result: ";
			cout << sqlcelltext << endl;
			ret = std::stoi(reinterpret_cast<const char*>(sqlcelltext));
		}
	}


	//close connection and free resources
COMPLETED:
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);
	//pause the console window - exit when key is pressed
	//cout << "\nPress ESC key to exit...\n";
	//char ch = _getch();
	//if (ch == VK_ESCAPE);
	//{
	//	cout << "\nGraceful exit\n";
	//}
	return(ret);
}


VOID CALLBACK TimerProc(HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime)
{

	int ret{};
	ret == 0;
	int statusvariable;

	std::ifstream inFile("machinestatus.txt");
	std::string line;

	if (inFile.is_open())
	{
		std::getline(inFile, line);
		cout << line << "\n";
		std::getline(inFile, line);
		cout << line << "\n";
		std::getline(inFile, line);
		cout << line << "\n";
		std::getline(inFile, line);
		cout << line << "\n\n";

		ret = std::stoi(line);

		inFile.close();
	}
	else
	{
		std::cerr << "unable to open file";
	}

	// -- check EStop status in database -- set/reset EStop relay
	//wchar_t checkEStop[1024] = L"SELECT EStop FROM TestStandX1 WHERE ID = 1";
	//ret = Do_SQL_Query(checkEStop);

	
	// ******************** flipping back and forth on estop ??? **************************************************


	// ** check estop here **

	cout << "\n" << estopmem << "++\n";

	if (ret != 0)
	{
		cout << ret << "Run condition - close EStop relay - pull down to ground ****\n";
		if (estopmem)
		{
			// nothing
		}
		else
		{
			ret = Write_MiAPI_GPIO(1, 0); // close EStop relay - pull down to ground
			estopmem = true;
		}
	}
	else
	{
		cout << ret << "EStop condition - open EStop relay ****\n";
		if (estopmem)
		{
			ret = Write_MiAPI_GPIO(1, 1); // open EStop relay
			estopmem = false;
		}
		else
		{
			// nothing
		}
	}

	// -- check GPIO for input status

	cout << "\n" << estopmem << "++\n";


	std::ofstream myFile;
	myFile.open("machinestatus.txt");




	//wchar_t querybody[1024] = L"UPDATE TestStandX1 SET ";
	//wchar_t* queryvariable;
	//wchar_t* querysuffix = L" WHERE ID = 1";



	for (int i = 5; i <= 8; i++)
	{
		ret = Read_MiAPI_GPIO(i);
		if (ret >=0 )
			if (ret <= 1)
			{
				switch (i)
				{
				case 5:
					if (ret)
					{
						//queryvariable = L"PumpOn = 0"; // if circuit closed then pump is off
						statusvariable = 0;
						break;
					}
					else
					{
						//queryvariable = L"PumpOn = 1"; // if circuit open then pump is on
						statusvariable = 1;
						break;
					}
				case 6:
					if (ret)
					{
						//queryvariable = L"Filter_1 = 0"; // if circuit open then filter is dirty
						if (estopmem)
						{
							ret = Write_MiAPI_GPIO(1, 1); // open EStop relay
							estopmem = false;
						}
						else
						{
							// nothing
						}
						statusvariable = 0;
						break;
					}
					else
					{
						//queryvariable = L"Filter_1 = 1"; // if circuit closed then filter is clean
						estopmem = true;
						statusvariable = 1;
						break;
					}
				case 7:
					if (ret)
					{
						//queryvariable = L"Filter_1 = 0"; // if circuit open then filter is dirty
						if (estopmem)
						{
							ret = Write_MiAPI_GPIO(1, 1); // open EStop relay
							estopmem = false;
						}
						else
						{
							// nothing
						}
						statusvariable = 0;
						break;
					}
					else
					{
						//queryvariable = L"Filter_1 = 1"; // if circuit closed then filter is clean
						estopmem = true;
						statusvariable = 1;
						break;
					}
				case 8:
					if (ret)
					{
						//queryvariable = L"EStop = 1"; // if EStop circuit high then not EStop
						if (estopmem)
						{
							// nothing
						}
						else
						{
							ret = Write_MiAPI_GPIO(1, 0); // close EStop relay - pull down to ground - pull down to ground
							estopmem = true;
						}
						statusvariable = 1;
						break;
					}
					else
					{
						//queryvariable = L"EStop = 0"; // if EStop circuit low then EStop
						if (estopmem)
						{
							ret = Write_MiAPI_GPIO(1, 1); // open EStop relay
							estopmem = false;
						}
						else
						{
							// nothing
						}
						statusvariable = 0;
						break;
					}
				default:
					break;
				}

				myFile << statusvariable << "\n";
				cout << i << "-" << statusvariable << "****\n";
				cout << "\n" << estopmem << "++\n";



				// - comment out SQL query for the moment
				//wcscat(querybody, queryvariable);
				//wcscat(querybody, querysuffix);
				//ret = Do_SQL_Query(querybody);



			}
	}

	cout << "\n";
	myFile.close();

	//printf("timer tick %02d\n", i);
	//i++;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int ret;
	char choosed = 0;
	//wchar_t *queryvariable;
	//wchar_t* partnumber = L"29";

	//-- Start the MiAPI libary
	if (MiAPI_Start() != MiAPI_OK)
	{
		//printf("Error: Failed to initialize MiAPI library.\n");
		return MiAPI_INIT_FAIL;
	}
	else
	{
		// -- Initialize GPIO inputs/outputs
		ret = Do_MiAPI_GPIO();
	}

	//do {
	//	printf("\n=== PM Check ===\n");
	//	printf("  1 : Enter 1 to Confirm PM is done\n");
	//	choosed = _getche();
	//	printf("\n\n");
	//} while (choosed != '1');

	//ret = Do_MiAPI_Version();
	//printf("--------------------------------------------------------------\n");

	// ********************************************* SQL Query call ************************************************

	//wchar_t querybody[1024] = L"SELECT Descript FROM Partmap WHERE PartNum = 38";

	//wchar_t queryvariable = std::to_wstring(partnumber);
	//queryvariable = partnumber;
	//wcscat(querybody, queryvariable);

	//cout << "\nQuery Body: ";
	//cout << &querybody;
	//cout << "\n";

	//ret = Do_SQL_Query(querybody);

	//cout << ret;

	// ********************************************* SQL Query call ************************************************


	// Add a timer
	{
		MSG Msg;
		/* Set up the timer to go off every three seconds */
		SetTimer(NULL, 0, 3000, TimerProc);
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
		printf("\n=== Exit Menu ===\n");
		printf("  0 : Exit\n");
		choosed = _getche();
		printf("\n\n");
	} while (choosed != '0');

	//-- It must call MiAPI_Exit() to free the resource when MiAPI exits. 	
	MiAPI_Exit();	

	return 0;
}

