#include "pch.h"

#ifdef _DEBUG
wstring SOLUTION_DIR = L"C:\\Work\\ue5-shooter-project";
wstring SERVER_DIR = SOLUTION_DIR + L"\\Binaries\\WindowsServer\\ShooterServer.exe";
#else
wstring SERVER_DIR = L"WindowsServer\\ShooterServer.exe";
#endif

string IP = "127.0.0.1";
uint16 MAIN_SERVER_PORT = 6122;
uint16 DEDI_SERVER_PORT = 7122;