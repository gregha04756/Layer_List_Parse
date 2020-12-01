// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#pragma comment (lib,"strsafe.lib")
#pragma comment(lib, "comctl32.lib")
#pragma comment(lib, "comdlg32.lib")
#pragma comment (lib,"Report_Error_DLL.lib")

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <Windows.h>
#include <Windowsx.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: reference additional headers your program requires here
#include <stdlib.h>
#include <strsafe.h>
#include <math.h>
#include <commctrl.h>
#include <commdlg.h>
#include <Richedit.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <new>
#include "Report_Error_DLL.h"
