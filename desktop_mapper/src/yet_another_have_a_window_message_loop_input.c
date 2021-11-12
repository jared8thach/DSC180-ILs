/**
*** INTEL CONFIDENTIAL
*** 
*** Copyright (March 2011) (March 2011) Intel Corporation All Rights Reserved. 
*** The source code contained or described herein and all documents related to the
*** source code ("Material") are owned by Intel Corporation or its suppliers or 
*** licensors. Title to the Material remains with Intel Corporation or its 
*** suppliers and licensors. The Material contains trade secrets and proprietary 
*** and confidential information of Intel or its suppliers and licensors. 
*** The Material is protected by worldwide copyright and trade secret laws 
*** and treaty provisions. No part of the Material may be used, copied, 
*** reproduced, modified, published, uploaded, posted, transmitted, distributed,
*** or disclosed in any way without Intel's prior express written permission.
***
*** No license under any patent, copyright, trade secret or other intellectual
*** property right is granted to or conferred upon you by disclosure or delivery
*** of the Materials, either expressly, by implication, inducement, estoppel or
*** otherwise. Any license under such intellectual property rights must be 
*** express and approved by Intel in writing.
**/

//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#if defined (_DEBUG) || defined (__PL_DEBUG__)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG || __PL_DEBUG__

#include <assert.h>
#include <process.h> // for _beginthreadex

#include "desktop_mapper.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global window data.
//-----------------------------------------------------------------------------
HWND hwnd = NULL;
DWORD thread_id = 0;
HANDLE h_thread = NULL;

/*-----------------------------------------------------------------------------
Function: modeler_init_inputs
Purpose : return the inputs count.
In      : pointer to an unsigned integer.
Out     : modified unsigned integer.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int *p, 
	int *pfd, 
	int *pfe, 
	char *po,
	size_t so
) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);
	assert(pfd != NULL);
	assert(pfe != NULL);

	//-------------------------------------------------------------------------
	// Declare our intentions to the framework.
	//-------------------------------------------------------------------------
	SIGNAL_PURE_EVENT_DRIVEN_MODE
	SET_INPUTS_COUNT(INPUTS_COUNT);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: modeler_open_inputs
Purpose : open inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	unsigned int i = 0;

	//-------------------------------------------------------------------------
	// Input descriptions.
	//-------------------------------------------------------------------------
	static char *descriptions[INPUTS_COUNT] = { 
		INPUT_DESCRIPTION_STRINGS 
	};
	static INTEL_MODELER_INPUT_TYPES types[INPUTS_COUNT] = {
		INPUT_TYPES
	};

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	//-------------------------------------------------------------------------
	// Set input information.
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INPUT_NAME_STRING);
	//-------------------------------------------------------------------------
	for( i = 0; i < INPUTS_COUNT; i++) {
		SET_INPUT_DESCRIPTION(
			i, 
			descriptions[i]
		);
		SET_INPUT_TYPE(
			i,
			types[i]
		);
		SET_INPUT_AS_NOT_LOGGED(i);
	} // for i (each input)

	//-------------------------------------------------------------------------
	// Start window manager thread.
	//-------------------------------------------------------------------------
	h_thread = (HANDLE)_beginthreadex(
		NULL,
		ESRV_THREAD_DEFAULT_STACK_SIZE_IN_BYTES,
		window_manager_thread,
		(void *)p,
		ESRV_THREAD_STACK_CREATION_FLAGS,
		(unsigned int *)&thread_id
	);
	if(h_thread == NULL) {
		goto modeler_open_inputs_error;
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

modeler_open_inputs_error:

	return(ESRV_FAILURE);

}

/*-----------------------------------------------------------------------------
Function: modeler_close_inputs
Purpose : close inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL bret = FALSE;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	//-------------------------------------------------------------------------
	// Close window.
	//-------------------------------------------------------------------------
	if(hwnd != NULL) {
		(void)PostMessage(
			hwnd,
			WM_CLOSE ,
			0,
			0
		);
	}

	//-------------------------------------------------------------------------
	// Wait for window thread's end.
	//-------------------------------------------------------------------------
	if(h_thread != NULL) {
		(void)WaitForSingleObject(
			h_thread,
			INFINITE
		);
		h_thread = NULL;
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_read_inputs
Purpose : collect all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_listen_inputs
Purpose : listen for all inputs.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE px) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(px != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_dctl
Purpose : process DCTL commands on DCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

/*-----------------------------------------------------------------------------
Function: modeler_process_lctl
Purpose : process LCTL commands on LCTL interrupt notification.
In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING
	
	assert(p != NULL);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

//-----------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
Function: window_manager_thread
Purpose : launch window
In      : pointer modeler input table (as void).
Out     : none
-----------------------------------------------------------------------------*/
ESRV_API unsigned int __stdcall window_manager_thread(void *px) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL bret = FALSE;
	PINTEL_MODELER_INPUT_TABLE p = NULL;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(px != NULL);
	if(px == NULL) {
		goto window_manager_thread_error;
	}
	p = (PINTEL_MODELER_INPUT_TABLE)px;

	//-------------------------------------------------------------------------
	// Start message loop.
	//-------------------------------------------------------------------------
	hwnd = make_window(p);
	if(hwnd == NULL) {
		goto window_manager_thread_error; 
	}

	//-------------------------------------------------------------------------
	// Note: this is a blocking call!
	//-------------------------------------------------------------------------
	bret = open_window(p);
	if(bret == FALSE) {
		goto window_manager_thread_error; 
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

window_manager_thread_error:

	return(ESRV_FAILURE);

}

/*-----------------------------------------------------------------------------
Function: make_window
Purpose : register window class
In      : pointer to input table data structure
Out     : none
-----------------------------------------------------------------------------*/
HWND make_window(PINTEL_MODELER_INPUT_TABLE p) {
   
	//-------------------------------------------------------------------------
	// Window / class variables.
	//-------------------------------------------------------------------------
	ATOM aret = 0;
	BOOL bret = FALSE;
	HWND window = NULL;
	WNDCLASS window_class = { 0 };
	TCHAR provider_class[] = _T(WINDOW_CLASS);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	//-------------------------------------------------------------------------
	// Create window class.
	//-------------------------------------------------------------------------
	(void)memset(
		&window_class,
		0,
		sizeof(window_class)
	);
	window_class.style = (UINT)(
		CS_HREDRAW | 
		CS_VREDRAW | 
		CS_DBLCLKS
	);
	window_class.lpfnWndProc = (WNDPROC)window_proc;
	window_class.cbClsExtra = 0;
	window_class.cbWndExtra = 0; 
	window_class.hInstance = NULL; 
	window_class.hIcon = LoadIcon(
		NULL, 
		IDI_APPLICATION
	);
	window_class.hCursor = LoadCursor(
		NULL, 
		IDC_ARROW
	); 
	window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); 
	window_class.lpszMenuName = NULL; 
	window_class.lpszClassName = provider_class; 

	//-------------------------------------------------------------------------
	// Register window class.
	//-------------------------------------------------------------------------
	aret = RegisterClass(&window_class);
	if(
		(aret == 0) &&
		(GetLastError() != ERROR_CLASS_ALREADY_EXISTS)
	) { 
		goto make_window_error;
	}

	//-------------------------------------------------------------------------
	// Create window.
	//-------------------------------------------------------------------------
	window = CreateWindowEx(
		(DWORD)WS_DLGFRAME,
		(LPCTSTR)provider_class,
		(LPCTSTR)NULL,
		(DWORD)(
			WS_POPUP |	
			WS_EX_TOPMOST | 
			WS_EX_LAYERED | 
			WS_EX_TRANSPARENT
		),
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		(HWND)NULL,
		(HMENU)NULL,
		(HINSTANCE)NULL,
		(LPVOID)NULL
	);
	if(window == NULL) {
		goto make_window_error;
	}

	return(window);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING_RETURN_VALUE(
		NULL, 
		window
	)

make_window_error:

	return(NULL);

}

/*-----------------------------------------------------------------------------
Function: open_window
Purpose : open window class
In      : pointer to input table data structure
Out     : none
-----------------------------------------------------------------------------*/
BOOL open_window(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Window / message handling variables.
	//-------------------------------------------------------------------------
	MSG msg = { 0 };
	BOOL bret = FALSE;
	LONG_PTR lptr = 0;
	LRESULT lret = 0;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(p != NULL);

	//-------------------------------------------------------------------------
	// Create Window used by the power management thread.
	//-------------------------------------------------------------------------
	hwnd = (HWND)make_window(p);
	if(hwnd == NULL) { 
		goto open_window_error;
	}

	//-------------------------------------------------------------------------
	// Save window handle and notify message loop so it can retrieve it.
	// Note:
	//    If the function succeeds, the return value is the previous value of 
	// the specified offset. If the function fails, the return value is zero.
	// To get extended error information, call GetLastError. If the previous
	// value is zero and the function succeeds, the return value is zero, but
	// the function does not clear the last error information. To determine
	// success or failure, clear the last error information by calling 
	// SetLastError with 0, then call SetWindowLongPtr. Function failure will
	// be indicated by a return value of zero and a GetLastError result that
	// is nonzero.
	// Source: MSDN.
	//-------------------------------------------------------------------------
	SetLastError(0);
	lptr = SetWindowLongPtr(
		hwnd, 
		GWLP_USERDATA, 
		(__int3264)(LONG_PTR)p
	);
	if(
		(lptr == 0) &&
		(GetLastError() != 0)
	) {
		goto open_window_error;
	}
	//-------------------------------------------------------------------------
	lret = SendMessage( // LRET not checked - no error & MSG specific
		hwnd, 
		WM_USER, 
		0, 
		0
	);

	//-------------------------------------------------------------------------
	// Display & update window (actually hidden).
	// Note:
	//    If the window was previously visible, the return value is nonzero. If
	// the window was previously hidden, the return value is zero. So no reason
	// to test bret here.
	//-------------------------------------------------------------------------
	bret = ShowWindow(
		hwnd,
#ifdef _DEBUG
		SW_SHOWNORMAL
#else // _DEBUG
		SW_HIDE
#endif // _DEBUG
	);
	bret = UpdateWindow(hwnd);
	if(bret == FALSE) {
		goto open_window_error;
	}

	//-------------------------------------------------------------------------
	// Message pump.
	//-------------------------------------------------------------------------
	while(
		(STOP_REQUEST == 0) &&
		(
			bret = GetMessage(
				&msg, 
				NULL, 
				0, 
				0
			)
		) != 0
	) { 
		if(bret == -1) {
			goto open_window_error;
		} else {
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
		}
	}

	return(TRUE);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

open_window_error:

	return(FALSE);

}

/*-----------------------------------------------------------------------------
Function: window_proc
Purpose : messages loop function
In      : usual Windows stuff
Out     : none
Return  : status
-----------------------------------------------------------------------------*/
LRESULT CALLBACK window_proc(
	HWND hwnd, 
	UINT message,
	WPARAM wparam,
	LPARAM lparam
) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL f_handled = FALSE;
	LRESULT return_value = FALSE;

	//-------------------------------------------------------------------------
	// Input table pointer.
	//-------------------------------------------------------------------------
	static int f_initialized = 0;
	static PINTEL_MODELER_INPUT_TABLE p = NULL;

	//-------------------------------------------------------------------------
	// Input generation variables.
	//-------------------------------------------------------------------------
	static char click_message[] = "Click!";
	static char settings_message[] = "Settings!";

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	switch(message) {

		//---------------------------------------------------------------------
		// Generate and store metrics.
		// Note: why will this code never execute?
		//---------------------------------------------------------------------
		case WM_LBUTTONDOWN:
			if(f_initialized == 1) {
				SET_INPUT_STRING_ADDRESS(
					INPUT_INDEX,
					click_message
				);
				SET_INPUT_AS_LOGGED(INPUT_INDEX);
				LOG_INPUT_VALUES;
				SET_INPUT_AS_NOT_LOGGED(INPUT_INDEX);
				INPUT_DIAGNOSTIC_HIGHLIGHTED("Click!");
				f_handled = TRUE;
			} else {
				f_handled = FALSE;
			}
			break;

		//---------------------------------------------------------------------
		// Generate and store metrics.
		// Note: use the Performance Options utility and change the shadow
		// under the mous setting to trigger.
		//---------------------------------------------------------------------
		case WM_SETTINGCHANGE:
			if(f_initialized == 1) {
				SET_INPUT_STRING_ADDRESS(
					INPUT_INDEX,
					settings_message
				);
				SET_INPUT_AS_LOGGED(INPUT_INDEX);
				LOG_INPUT_VALUES;
				SET_INPUT_AS_NOT_LOGGED(INPUT_INDEX);
				INPUT_DIAGNOSTIC_HIGHLIGHTED("Settings!");
				f_handled = TRUE;
			} else {
				f_handled = FALSE;
			}
			break;

		//---------------------------------------------------------------------
		// Retrieve the PILT.
		//---------------------------------------------------------------------
		case WM_USER:
			p = (PINTEL_MODELER_INPUT_TABLE)GetWindowLongPtr(
				hwnd, 
				GWLP_USERDATA
			);
			assert(p != NULL);
			f_initialized = 1;
			f_handled = TRUE;
			break;

		//---------------------------------------------------------------------
		// Close window.
		//---------------------------------------------------------------------
		case WM_CLOSE:
			f_handled = TRUE;
			break;

		//---------------------------------------------------------------------
		// Quit.
		//---------------------------------------------------------------------
		case WM_DESTROY:
			PostQuitMessage(0);
			f_handled = TRUE;
			break;

		default:
			f_handled = FALSE;
			break;

	} // switch message

	//-------------------------------------------------------------------------
	// Pass un-processed messages to Window's default message handler.
	//-------------------------------------------------------------------------
	if(f_handled == FALSE) {
		return(
			DefWindowProc(
				hwnd, 
				message, 
				wparam,  
				lparam
			)
		);
	} else {
		return(return_value);
	}

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}
