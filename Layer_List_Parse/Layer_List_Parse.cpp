// Layer_List_Parse.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Layer_List_Parse.h"

using namespace std;

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);


BOOL Is_a_SKU_line(std::string& someline, DWORDLONG& some_SKU, int& nSKU_len)
{
	std::string chk_str;
	std::string::size_type n;
	BOOL b_r = TRUE;
	nSKU_len = (int)0;
	n = someline.find("_");
	b_r = !(std::string::npos == n);
	if (b_r)
	{
		chk_str = someline.substr(0, n).c_str();
		some_SKU = std::strtoll(someline.substr(0, n).c_str(), NULL, 10);
		nSKU_len = (int)n * (int)(!(0 == some_SKU));
		b_r = !(0 == some_SKU);
	}
	return b_r;
}



#if 0
void ReportError(DWORD dwError)
{
	TCHAR* lpErrMsg = NULL;
	TCHAR szErrMsg[1024];
	DWORD dwResult = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwError,
		MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),
		(LPTSTR)&lpErrMsg,
		MAX_LOADSTRING,
		NULL);
	HRESULT hRes = StringCchPrintf(szErrMsg, sizeof(szErrMsg)/sizeof(TCHAR), _T("Error %d: %s"),dwError,lpErrMsg);
	int nResult = ::MessageBox(NULL,(LPWSTR)szErrMsg,szTitle,MB_OK);
	if ( lpErrMsg )
	{
		::LocalFree(lpErrMsg);
		lpErrMsg = NULL;
	}
}
void ReportError(const char* pchError)
{
	TCHAR* lpErrMsg = NULL;
	TCHAR szErrMsg[1024];
	PVOID pvTemp = ::SecureZeroMemory((PVOID)szErrMsg,sizeof(szErrMsg));
	size_t nLen = ::strnlen_s(pchError,sizeof(szErrMsg)/sizeof(TCHAR));
	int nResult = ::MultiByteToWideChar(CP_ACP,
		MB_PRECOMPOSED,
		pchError,
		nLen,
		szErrMsg,
		sizeof(szErrMsg)/sizeof(TCHAR));
//	HRESULT hRes = StringCchPrintf(szErrMsg, sizeof(szErrMsg)/sizeof(TCHAR), _T("Error %d: %s"),dwError,lpErrMsg);
	nResult = ::MessageBox(NULL,(LPWSTR)szErrMsg,_T("Trash35"),MB_OK);
	if ( lpErrMsg )
	{
		::LocalFree(lpErrMsg);
		lpErrMsg = NULL;
	}
}
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPTSTR lpCmdLine,int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	BOOL b_result;
	DWORD dw_error;
	INITCOMMONCONTROLSEX iccex = {0};
	INT_PTR ip_result;

 	// TODO: Place code here.

	hInst = hInstance; // Store instance handle in our global variable

	iccex.dwSize = sizeof(iccex);
	iccex.dwICC = ICC_STANDARD_CLASSES|ICC_BAR_CLASSES;
	b_result = ::InitCommonControlsEx(&iccex);
	dw_error = ::GetLastError();
	if ( !b_result )
	{
		if ( ERROR_SUCCESS != dw_error )
		{
			ReportError(dw_error);
			return 0;
		}
	}

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LAYER_LIST_PARSE, szWindowClass, MAX_LOADSTRING);

	
	return (int)ip_result = ::DialogBox(hInst,MAKEINTRESOURCE(IDD_ABOUTBOX),NULL,About);
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	BOOL b_result;
	BOOL b_match_pn;
	static BOOL b_success;
	static Layer_IDs selected_layer;
	DWORD dw_error;
	HRESULT hres_result;
	int int_result;
	static OPENFILENAME ofn;
	static TCHAR szOpenDlgFileName[MAX_PATH];
	std::wstring wstr_File_Results;
	static int int_strlen;
	PVOID pv_temp;
	static std::wstring wstr_Title;
	static std::string str_In_Name;
	static std::string str_Out_Name;
	static std::string ifline;
	static std::string ofline;
	static std::ifstream* lpinfile;
	static std::ofstream* lpoutfile;
	static char* lp_ch_filename;
	static std::list<DWORDLONG>* lp_li_pn;
	static std::list<DWORDLONG>::iterator it_pn;
	DWORDLONG li_this_pn;
	int n_SKU_len;

	switch (message)
	{
	case WM_INITDIALOG:
		selected_layer = Layer_IDs::SKUS;
		Button_SetCheck(GetDlgItem(hDlg,IDC_BTN_SKUS),BST_CHECKED);
		b_success = true;
		int_strlen = 0;
		lpinfile = NULL;
		lpoutfile = NULL;
		pv_temp = SecureZeroMemory((PVOID)szOpenDlgFileName,sizeof(szOpenDlgFileName));
		return (INT_PTR)TRUE;
	case WM_COMMAND:
		if ( (LOWORD(wParam) == IDC_BTN_SKUS) && (HIWORD(wParam) == BN_CLICKED) )
		{
			selected_layer = Layer_IDs::SKUS;
		}
		if ( (LOWORD(wParam) == IDC_BTN_NONSKUS) && (HIWORD(wParam) == BN_CLICKED) )
		{
			selected_layer = Layer_IDs::NONSKUS;
		}
		if ( (LOWORD(wParam) == IDC_BTNBROWSE) && (HIWORD(wParam) == BN_CLICKED) )
		{
			b_success = true;
			pv_temp = ::SecureZeroMemory((PVOID)szOpenDlgFileName,sizeof(szOpenDlgFileName));
			pv_temp = ::SecureZeroMemory((PVOID)&ofn,sizeof(ofn));
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hDlg;
			ofn.lpstrFile = (LPWSTR)szOpenDlgFileName;
//
// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
// use the contents of szFile to initialize itself.
//
			ofn.lpstrFile[0] = '\0';
			ofn.nMaxFile = sizeof(szOpenDlgFileName)/sizeof(TCHAR);
			ofn.lpstrFilter = _T("Txt files\0*.txt\0");
			ofn.nFilterIndex = 1;
			ofn.lpstrFileTitle = NULL;
			ofn.nMaxFileTitle = 0;
			ofn.lpstrInitialDir = NULL;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

// Display the Open dialog box. 
			b_result = ::GetOpenFileName(&ofn);
			dw_error = ::GetLastError();
			if ( !b_result )
			{
				if ( ERROR_SUCCESS != dw_error )
				{
					ReportError(dw_error);
				}
				return (INT_PTR)TRUE;
			}
			if ( NULL != lp_li_pn )
			{
				delete lp_li_pn;
				lp_li_pn = NULL;
			}
			try
			{
				lp_li_pn = (std::list<DWORDLONG>*)new std::list<DWORDLONG>; 
			}
			catch (std::bad_alloc &ba)
			{
				ReportError(ba.what());
				b_success = false;
				return (INT_PTR)TRUE;
			}
			int_strlen = 0;
			hres_result = ::StringCchLength(szOpenDlgFileName,sizeof(szOpenDlgFileName)/sizeof(TCHAR),(size_t*)&int_strlen);
#if _DEBUG
			if ( int_strlen )
			{
				int_strlen = int_strlen;
			}
#endif
			if ( int_strlen )
			{
				wstr_Title.assign(szTitle);
				if ( NULL != ofn.lpstrFile )
				{
					wstr_Title.append(_T(" - "));
					wstr_Title.append(ofn.lpstrFile);
				}
				b_result = ::SetWindowText(hDlg,wstr_Title.c_str());
			}
			try
			{
				lpinfile = (std::ifstream*)new ifstream();
			}
			catch (std::bad_alloc ba)
			{
				::ReportError(::GetLastError());
				b_success = false;
				return (INT_PTR)TRUE;
			}
			if ( b_success )
			{
				lpinfile->exceptions(ifstream::failbit|ifstream::badbit);
				hres_result = ::StringCchLength(szOpenDlgFileName,(size_t)MAX_PATH,(size_t*)&int_strlen);
				lp_ch_filename = (char*)new char[int_strlen+1];
				pv_temp = ::SecureZeroMemory((PVOID)lp_ch_filename,(SIZE_T)int_strlen+1);
				int_result = ::WideCharToMultiByte(CP_ACP,
					0,
					szOpenDlgFileName,
					int_strlen,
					lp_ch_filename,
					int_strlen,
					NULL,
					NULL);
				str_In_Name.assign(lp_ch_filename);
				try
				{
					lpinfile->open(str_In_Name,ifstream::in);
				}
				catch (ifstream::failure fof)
				{
					ReportError(::GetLastError());
					b_success = false;
					return (INT_PTR)TRUE;
				}
			}
			if ( b_success )
			{
				str_Out_Name.assign(str_In_Name);
				str_Out_Name.insert(str_In_Name.find(".txt"),(Layer_IDs::SKUS==selected_layer)?"-parsed_SKUS":"-parsed_NONSKUS");
				try
				{
					lpoutfile = (std::ofstream*)new ofstream();
				}
				catch (std::bad_alloc ba)
				{
					::ReportError(::GetLastError());
					b_success = false;
					return (INT_PTR)TRUE;
				}
				lpoutfile->exceptions(ofstream::failbit|ofstream::badbit);
				try
				{
					lpoutfile->open(str_Out_Name,ofstream::out);
				}
				catch (ofstream::failure &fof)
				{
					ReportError(::GetLastError());
					ReportError(fof.what());
					b_success = false;
					return (INT_PTR)TRUE;
				}
			}
			while ( b_success && !lpinfile->eof() )
			{
				try
				{
					getline(*lpinfile,ifline);
				}
				catch (std::exception &e)
				{
					if ( lpinfile->eof() )
					{
						break;
					}
					ReportError(::GetLastError());
					ReportError(e.what());
					b_success = false;
					return (INT_PTR)TRUE;
				}
#if 0
				while ( ifline.find('"') != string::npos )
				{
					int_result = ifline.find('"');
					ifline.erase(int_result,1);
				}
#endif
				if (ifline.find('"') != string::npos)
				{
					int_result = ifline.find('"');
					ifline.erase(int_result,1);  /* Get rid of starting " character */
				}
				if (ifline.find('"') != string::npos)
				{
					int_result = ifline.find('"');
					ifline.erase(int_result,string::npos);  /* Get rid of everything after the second " character inclusive*/
				}

/*
See if there is a SKU at beginning of line, numerical value followed by an underscore '_'. Don't repeat lines with the same SKU.
Furthermore only save lines that contain a SKU number.
*/
				if (Layer_IDs::SKUS == selected_layer && Is_a_SKU_line(ifline, li_this_pn, n_SKU_len))
				{
/*					if (0L != (li_this_pn = strtol(ifline.substr(0, 5).c_str(), NULL, 10))) */
					b_match_pn = false;
					for ( it_pn = lp_li_pn->begin(); (0 < n_SKU_len) && (it_pn != lp_li_pn->end()); it_pn++ )
					{
						if ( *it_pn == li_this_pn )
						{
							b_match_pn = true;
							break;
						}
					}
					if ( !b_match_pn )
					{
						lp_li_pn->push_back(li_this_pn);
						std::size_t found = ifline.find("_");
						while ( found != string::npos && ifline.find("_",found+1) != string::npos )
						{
							ifline.replace(ifline.find("_",found+1),1," ");
						}
						if ( ifline.find("|") != string::npos )
						{
							ofline.assign(ifline.substr(0,ifline.find("|")));
							ofline.append("\n");
							try
							{
								lpoutfile->write(ofline.c_str(),ofline.length());
							}
							catch (ofstream::failure &fof)
							{
								ReportError(::GetLastError());
								ReportError(fof.what());
								b_success = false;
								return (INT_PTR)TRUE;
							}
						}
						else if ( ifline.length() > 0 )
						{
							try
							{
								ifline.append("\n");
								lpoutfile->write(ifline.c_str(),ifline.length());
							}
							catch (ofstream::failure &fof)
							{
								ReportError(::GetLastError());
								ReportError(fof.what());
								b_success = false;
								return (INT_PTR)TRUE;
							}
						}
						else
						{
#if _DEBUG
							b_success = b_success;
#endif
						}
					}
				}
				if ( (Layer_IDs::NONSKUS == selected_layer) && !Is_a_SKU_line(ifline, li_this_pn, n_SKU_len))
				{
					if ( ifline.length() > 0 )
					{
						try
						{
							ifline.append("\n");
							lpoutfile->write(ifline.c_str(),ifline.length());
						}
						catch (ofstream::failure &fof)
						{
							ReportError(::GetLastError());
							ReportError(fof.what());
							b_success = false;
							return (INT_PTR)TRUE;
						}
					}
				}
			} /* End of while (...) */
			hres_result = ::StringCchLength(szOpenDlgFileName,(size_t)MAX_PATH,(size_t*)&int_strlen);
			if ( int_strlen && lpinfile->is_open() && lpoutfile->is_open() )
			{
				if ( !b_success )
				{
					b_result = ::SetDlgItemText(hDlg,IDC_TXTPROCESSRESULT,_T("File processing failed"));
				}
				if ( b_success )
				{
					b_result = ::SetDlgItemText(hDlg,IDC_TXTFILESTATUS,szOpenDlgFileName);
					wstr_File_Results.assign(_T("File processed ok. "));
					int_result = (int)lp_li_pn->size();
					wstr_File_Results += std::to_wstring((DWORDLONG)int_result);
					wstr_File_Results += _T(" SKU numbers found");
					b_result = ::SetDlgItemText(hDlg,IDC_TXTPROCESSRESULT,wstr_File_Results.c_str());
				}
			}
			else
			{
				b_result = ::SetDlgItemText(hDlg,IDC_TXTPROCESSRESULT,_T("No file processed"));
			}
			if ( NULL != lp_li_pn )
			{
				delete lp_li_pn;
				lp_li_pn = NULL;
			}
			if ( NULL != lp_ch_filename )
			{
				delete[] lp_ch_filename;
				lp_ch_filename = NULL;
			}
			if ( NULL != lpinfile )
			{
				lpinfile->close();
				delete lpinfile;
				lpinfile = NULL;
			}
			if ( NULL != lpoutfile )
			{
				lpoutfile->close();
				delete lpoutfile;
				lpoutfile = NULL;
			}
			return (INT_PTR)TRUE;
		} /* End of if( (LOWORD(wParam) == IDC_BTNBROWSE) && (HIWORD(wParam) == BN_CLICKED) ) */
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			if ( NULL != lp_ch_filename )
			{
				delete[] lp_ch_filename;
				lp_ch_filename = NULL;
			}
			if ( NULL != lpoutfile )
			{
				if ( lpoutfile->is_open() )
				{
					lpoutfile->close();
					delete lpoutfile;
					lpoutfile = NULL;
				}
			}
			if ( NULL != lpinfile )
			{
				if ( lpinfile->is_open() )
				{
					lpinfile->close();
					delete lpinfile;
					lpinfile = NULL;
				}
			}
			if ( NULL != lp_li_pn )
			{
				delete lp_li_pn;
				lp_li_pn = NULL;
			}
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
