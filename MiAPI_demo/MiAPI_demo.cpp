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

//#include <afxwin.h>

#include "../LIB/MiAPI.h"

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
}





int _tmain(int argc, _TCHAR* argv[])
{
	int ret ;
	char choosed = 0;
	//-- Start the MiAPI libary
	if( MiAPI_Start() != MiAPI_OK )
	{
		printf("Error: Failed to initialize MiAPI library.\n");
		return MiAPI_INIT_FAIL;
	}
	ret = Do_MiAPI_Version();
	printf("--------------------------------------------------------------\n");

//	AfxMessageBox(_T("Simple message box."));

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

			if (kbhit())
			{
				MiAPI_Exit();
				break;
			}
		}
		return Msg.wParam;
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

