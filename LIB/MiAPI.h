#ifndef _MiAPI_H
#define _MiAPI_H

enum MiAPI_RETURN_CODE
{
    MiAPI_OK = 0,
    MiAPI_INIT_FAIL,
    MiAPI_NOT_SUPPORT,
    MiAPI_UNLOAD_FAIL,
    MiAPI_READ_FAIL,
	MiAPI_OLD_VERSION,

    MiAPI_PCI_READ_FAIL = 0x11,
    MiAPI_PCI_WRITE_FAIL,
    MiAPI_IO_READ_FAIL,
    MiAPI_IO_WRITE_FAIL,
    MiAPI_MEMORY_MAP_FAIL,
    MiAPI_MEMORY_READ_FAIL,
    MiAPI_MEMORY_WRITE_FAIL,

    MiAPI_WDT_GET_FAIL = 0x21,
    MiAPI_WDT_SET_FAIL,

    MiAPI_GPIO_QUERY_FAIL = 0x31,
    MiAPI_GPIO_MUX_FAIL,
    MiAPI_GPIO_SETDIR_FAIL,
    MiAPI_GPIO_GETSTATUS_FAIL,
    MiAPI_GPIO_SETSTATUS_FAIL,

    MiAPI_SMBUS_DEVICE_UNAVAILABLE = 0x41,
    MiAPI_SMBUS_READBYTE_FAIL,
    MiAPI_SMBUS_WRITEBYTE_FAIL,
    MiAPI_SMBUS_READBLOCK_FAIL,
    MiAPI_SMBUS_WRITEBLOCK_FAIL,

    MiAPI_VGA_INVALID_RANGE = 0x51,
    MiAPI_VGA_INIT_FAIL,
    MiAPI_VGA_QUERY_FAIL,
    MiAPI_VGA_GET_AMOUNT_OF_MONITORS_FAIL,
    MiAPI_VGA_GET_BRIGHTNESS_FAIL,
    MiAPI_VGA_SET_BRIGHTNESS_FAIL,
    MiAPI_VGA_GET_CONTRAST_FAIL,
    MiAPI_VGA_SET_CONTRAST_FAIL,
    MiAPI_VGA_SET_ORIENTATION_FAIL,
    MiAPI_VGA_DDCCI_UNSUPPORTED,

    MiAPI_FANSPEED_GET_FAIL = 0x61,
    MiAPI_FANSPEED_SET_FAIL,
    MiAPI_VOLTAGE_GET_FAIL,
    MiAPI_TEMPERATURE_GET_FAIL,

    MiAPI_WIRELESS_INIT_FAIL = 0x71,
    MiAPI_WIRELESS_QUERY_FAIL,
    MiAPI_WIRELESS_GET_AMOUNT_OF_ACCESS_POINT_FAIL,
};

enum FAN_TYPE
{
    NONEFAN = 0,
    CPUFAN,
    SYSFAN,
    AUXFAN
};

typedef struct _MIAPI_GPIO_STATUS
{
    BYTE Direction;
    BYTE VoltageLevel;
} MIAPI_GPIO_STATUS;

typedef struct _MIAPI_MONITOR_INFO
{
    WORD Orientation;
    DWORD DeviceIndex;
    WCHAR FriendlyDeviceName[64];
    DWORD WMITotalBrightnessLevel;
} MIAPI_MONITOR_INFO;

typedef struct _MIAPI_WIFI_INFO
{
    int DeviceIndex;
    UCHAR SSID[32];
    DWORD SSIDLength;
    int RSSI_dBm;
    int RSSI_Percentage;
} MIAPI_WIFI_INFO;

typedef struct _MIAPI_BRIGHTNESS
{
    DWORD MinimumBrightness;
    DWORD MaximumBrightness;
    DWORD CurrentBrightness;
} MIAPI_BRIGHTNESS;

typedef struct _MIAPI_CONTRAST
{
    DWORD MinimumContrast;
    DWORD MaximumContrast;
    DWORD CurrentContrast;
} MIAPI_CONTRAST;

#ifndef MIAPI_EXPORTS
#define DLL_API __declspec(dllimport)
#else
#define DLL_API __declspec(dllexport)
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    DLL_API int _stdcall MiAPI_Start(void);
    DLL_API void _stdcall MiAPI_Exit(void);
    DLL_API int _stdcall MiAPI_GetMiAPIVersion(int *Major, int *Minor);
    DLL_API int _stdcall MiAPI_GetBIOSVersion(char *BIOSVersion, DWORD *Size);
    DLL_API int _stdcall MiAPI_GetProductName(char *ProductName, DWORD *Size);
	
    DLL_API int _stdcall MiAPI_Watchdog_Start(void);
    DLL_API int _stdcall MiAPI_Watchdog_GetRange(DWORD *Minimum, DWORD *Maximum, DWORD *Current);
    DLL_API int _stdcall MiAPI_Watchdog_SetConfig(DWORD Timeout, bool Reboot);
    DLL_API int _stdcall MiAPI_Watchdog_Refresh(void);
    DLL_API int _stdcall MiAPI_Watchdog_Disable(void);

    
    DLL_API int _stdcall MiAPI_GPIO_GetStatus(BYTE PinNum, MIAPI_GPIO_STATUS* MiAPI_GPIOStatus);
    DLL_API int _stdcall MiAPI_GPIO_SetStatus(BYTE PinNum, MIAPI_GPIO_STATUS MiAPI_GPIOStatus);
	    
    //DLL_API int _stdcall MiAPI_SMBus_Read(BYTE SlaveAddress, WORD RegisterOffset, BYTE *Result);
    //DLL_API int _stdcall MiAPI_SMBus_Write(BYTE SlaveAddress, WORD RegisterOffset, BYTE Result);
	DLL_API int _stdcall MiAPI_SMBusReadQuick(BYTE SlaveAddress);
	DLL_API int _stdcall MiAPI_SMBusWriteQuick(BYTE SlaveAddress);
	DLL_API int _stdcall MiAPI_SMBusReceiveByte(BYTE SlaveAddress, BYTE *Result);
	DLL_API int _stdcall MiAPI_SMBusSendByte(BYTE SlaveAddress, BYTE Result);
	DLL_API int _stdcall MiAPI_SMBusReadByte(BYTE SlaveAddress, BYTE RegisterOffset, BYTE *Result);
	DLL_API int _stdcall MiAPI_SMBusWriteByte(BYTE SlaveAddress, BYTE RegisterOffset, BYTE Result);
	DLL_API int _stdcall MiAPI_SMBusReadWord(BYTE SlaveAddress, BYTE RegisterOffset, WORD *Result);
	DLL_API int _stdcall MiAPI_SMBusWriteWord(BYTE SlaveAddress, BYTE RegisterOffset, WORD Result);
	DLL_API int _stdcall MiAPI_SMBusReadBlock(BYTE SlaveAddress, BYTE RegisterOffset, BYTE *Result, DWORD *Count);
	DLL_API int _stdcall MiAPI_SMBusWriteBlock(BYTE SlaveAddress, BYTE RegisterOffset, BYTE *Result, DWORD Count);

    DLL_API int _stdcall MiAPI_Display_GetAmountOfMonitors(int *AmountOfMonitors);
    DLL_API int _stdcall MiAPI_Display_GetMonitorInfo(MIAPI_MONITOR_INFO *MiAPI_MonitorInfo, DWORD Index);
    //DLL_API int _stdcall MiAPI_Display_GetMonitorCap(DWORD *MonitorCapabilities, DWORD Index);
    DLL_API int _stdcall MiAPI_Display_Rescan();
    DLL_API int _stdcall MiAPI_Display_GetBrightness(MIAPI_BRIGHTNESS *MiAPI_Brightness, DWORD Index);
    DLL_API int _stdcall MiAPI_Display_SetBrightness(DWORD NewBrightness, DWORD Index);
    DLL_API int _stdcall MiAPI_Display_GetContrast(MIAPI_CONTRAST *MiAPI_Contrast, DWORD Index);
    DLL_API int _stdcall MiAPI_Display_SetContrast(DWORD NewContrast, DWORD Index);
    DLL_API int _stdcall MiAPI_Display_SetOrientation(short Orientation, DWORD Index);
	DLL_API int _stdcall MiAPI_Display_On(void);
	DLL_API int _stdcall MiAPI_Display_Off(void);

    DLL_API int _stdcall MiAPI_GetFanSpeed(WORD FanType, WORD *Retval, WORD *TypeSupport);
    DLL_API int _stdcall MiAPI_SetFanSpeed(WORD FanType, WORD SetVal, WORD *TypeSupport);
    DLL_API int _stdcall MiAPI_GetTemperature(WORD TempType, WORD *RetVal, WORD *TypeSupport);
    DLL_API int _stdcall MiAPI_GetVoltage(WORD VoltType, WORD *RetVal, WORD *TypeSupport);

    DLL_API int _stdcall MiAPI_Wireless_GetAmountOfAccessPoint(int *AmountOfAccessPoint);
    DLL_API int _stdcall MiAPI_Wireless_GetWiFiStatus(MIAPI_WIFI_INFO *MiAPI_WiFiInfo, DWORD Index);
    DLL_API int _stdcall MiAPI_Wireless_RequeryWiFiStatus();
#ifdef __cplusplus
}
#endif

#endif  //  _MiAPI_H