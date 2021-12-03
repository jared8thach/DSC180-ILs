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
//#define _MEMORY_DEBUG 1
//#define __PL_WINDOWS__

#if defined (_DEBUG) || defined (__PL_DEBUG__)
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
#endif // _DEBUG || __PL_DEBUG__

#include <assert.h>
#include <process.h> // for _beginthreadex
#include <Windows.h>
#include <Psapi.h>
#include <string.h>

#include "desktop_mapper.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global mouse hook data.
//-----------------------------------------------------------------------------
HHOOK h_mouse_hook = NULL;
HANDLE h_click_detected = NULL;
//HWND h_window_detected = NULL;


//-----------------------------------------------------------------------------
// Global mouse hook data.
//-----------------------------------------------------------------------------
HWND h_window = NULL; // window handle
DWORD thread_id = 0; // thread id
DWORD process_id = 0; // process id
HANDLE h_process = NULL; // process handle
TCHAR process_path[MAX_PATH] = { '\0' }; // process path
wchar_t* prevToken = 0; // previous .exe process
BOOL isHung = FALSE;
BOOL isImmersive = FALSE;
RECT window_rect = { 0 }; // windows rect object


//-------------------------------------------------------------------------
// Data structures.
//-------------------------------------------------------------------------
typedef struct _windows_structure {
	HWND h_window;
	DWORD process_id; // not logged
	HANDLE h_process; // not logged
	TCHAR process_path[MAX_PATH]; // not logged
	wchar_t* curToken;
	wchar_t* prevToken; // not logged
	BOOL isHung;
	BOOL isImmersive;
	int upper_left_x;
	int upper_left_y;
	int bottom_right_x;
	int bottom_right_y;
} WINDOWS_STRUCTURE, * PWINDOWS_STRUCTURE;

typedef struct _samples_structure {
	unsigned entry_count;
	PWINDOWS_STRUCTURE windows_entry;
} SAMPLES_STRUCTURE, * PSAMPLES_STRUCTURE;

WINDOWS_STRUCTURE windows_struct = { 0 };
SAMPLES_STRUCTURE samples_struct = { 0 };

// defining structure and callback function
typedef struct {
	DWORD parent_process_id;
	DWORD child_process_id;
} parent_child_info;

BOOL CALLBACK EnumChildWindowsCallback(HWND hWnd, LPARAM lp) {
	parent_child_info* info = (parent_child_info*)lp;
	DWORD pid = 0;
	GetWindowThreadProcessId(hWnd, &pid);
	if (pid != info->parent_process_id) info->child_process_id = pid;
	return TRUE;
}


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
	SIGNAL_EVENT_DRIVEN_MODE;
	//SIGNAL_PURE_EVENT_DRIVEN_MODE;
	//SIGNAL_MULTIPLEXED_LOG_MODE;
	
		// LOGIC
		// take time stamp of data, 
		// put into input,
		// set value of input,
		// for private data set Z value,
		// log this input
		// REPEAT FOR NEXT WINDOW
		// however, set private data to "1" using Multplex Logger Thread
		// SIGNAL_MULTIPLEX_LOG_MOD in modeler_init

		// INPUTTING VAUES
		// done in generate_metrics()
		// first check if logger is capable of handling Multiplex logging
		// lret = LOG_INPUT_VALUES --> to check if Multiplex logging is available
		// switch(lret)
			// case MULTIPLEX_LOG_OK

		// LOGGING VALUES
		// log after each interation during the Z axis

		// OTHER
		// keep in mind UNMASKED_ALL_WINDOW_INPUTS (similar to SET_INPUT_AS_LOGGED)
		// and MASKED_ALL_WINDOW_INPUTS (similar to SET_INPUT_AS_NOT_LOGGED)
		

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
	// Initializing structures.
	//-------------------------------------------------------------------------
	samples_struct.windows_entry = (PWINDOWS_STRUCTURE)malloc(sizeof(WINDOWS_STRUCTURE) * 16); // keeping track of a maximum of 16 windows
	if (samples_struct.windows_entry == NULL) {
		goto modeler_open_inputs_error;
	}
	INPUT_DIAGNOSTIC_MALLOC(samples_struct.windows_entry, sizeof(WINDOWS_STRUCTURE));
	memset(samples_struct.windows_entry, 0, sizeof(WINDOWS_STRUCTURE));
	samples_struct.entry_count = 1;


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
	// Remove mouse hook -- if not removed yet.
	//-------------------------------------------------------------------------
	if(h_mouse_hook != NULL) {
		bret = UnhookWindowsHookEx(h_mouse_hook);
		if(bret == FALSE) {
			// Should push an error (read chapter 8)
			goto modeler_close_inputs_error;
		}
		h_mouse_hook = NULL;
	}

	//-------------------------------------------------------------------------
	// Free dynamic memory of data structures.
	//-------------------------------------------------------------------------
	if (samples_struct.windows_entry != NULL) {
		INPUT_DIAGNOSTIC_FREE(samples_struct.windows_entry);
		free(samples_struct.windows_entry);
		samples_struct.windows_entry = NULL;
		samples_struct.entry_count = 0;
	}

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

modeler_close_inputs_error:

	return(ESRV_FAILURE);

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
	// Hook thread variables.
	//-------------------------------------------------------------------------
    DWORD msg_loop_thread_id = 0;
    DWORD collector_thread_id = 0;
	HANDLE h_msg_loop_thread = NULL;
	HANDLE h_collector_thread = NULL;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(px != NULL);

	//-------------------------------------------------------------------------
	// Setup threads and synch data.
	//-------------------------------------------------------------------------
	h_click_detected = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL
	);
	if(h_click_detected == NULL) {
		goto custom_event_listner_thread_exit;
	}
	//h_window_detected = CreateEvent(
	//	NULL,
	//	FALSE,
	//	FALSE,
	//	NULL
	//);
	//if (h_window_detected == NULL) {
	//	goto custom_event_listner_thread_exit;
	//}
	//-------------------------------------------------------------------------
	h_collector_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		generate_metrics,
		px,
		0,
		(unsigned int *)&collector_thread_id
	);
	if(h_collector_thread == NULL) {
		goto custom_event_listner_thread_exit;
	}
	//-------------------------------------------------------------------------
	h_msg_loop_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		mouse_messages_loop,
		NULL,
		0,
		(unsigned int *)&msg_loop_thread_id
	);
	if(h_msg_loop_thread == NULL) {
		goto custom_event_listner_thread_exit;
	}

	//-------------------------------------------------------------------------
	// Run the message loop!
	//-------------------------------------------------------------------------
    if(h_msg_loop_thread != NULL) {
		WaitForSingleObject(
			h_msg_loop_thread,
			INFINITE
		);
	}

custom_event_listner_thread_exit:

	//-------------------------------------------------------------------------
	// Free resources.
	//-------------------------------------------------------------------------
	if(h_click_detected != NULL) {
		CloseHandle(h_click_detected);
		h_click_detected = NULL;
	}

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

	// RECEIVES DCTL FROM FOREGROUND WINDOW
	// REGISTER TO RECEIVE

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
Function: mouse_messages_loop
Purpose : mouse message hook function.
In      : pointer - as void - to a message structure.
Out     : updated input data.
Return  : status.
-----------------------------------------------------------------------------*/
unsigned int __stdcall mouse_messages_loop(void *pv) {

	//-------------------------------------------------------------------------
	// Message handling variables.
	//-------------------------------------------------------------------------
	MSG message = { 0 };
	HINSTANCE h_instance = GetModuleHandle(NULL);

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	//-------------------------------------------------------------------------
	// Setup mouse hook.
	//-------------------------------------------------------------------------
    h_mouse_hook = SetWindowsHookEx(
		WH_MOUSE_LL, 
		process_mouse_messages, 
		h_instance, 
		0
	);
	if(h_mouse_hook == NULL) {
		goto mouse_messages_loop_error;
	}
	//-------------------------------------------------------------------------
    while(
		GetMessage(
			&message,
			NULL,
			0,
			0
		)
	) {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }

	//-------------------------------------------------------------------------
	// Remove mouse hook.
	//-------------------------------------------------------------------------
	if(h_mouse_hook != NULL) {
		(void)UnhookWindowsHookEx(h_mouse_hook);
		h_mouse_hook = NULL;
	}
	
	return((int)ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

mouse_messages_loop_error:

	return((int)ESRV_FAILURE);

}

/*-----------------------------------------------------------------------------
Function: process_mouse_messages
Purpose : mouse event handler.
In      : mouse hook message data.
Out     : updated click position data.
Return  : status.
-----------------------------------------------------------------------------*/
LRESULT CALLBACK process_mouse_messages(
	int code, 
	WPARAM wparam, 
	LPARAM lparam
) {

	//-------------------------------------------------------------------------
	// Message handling variables.
	//-------------------------------------------------------------------------
    MOUSEHOOKSTRUCT *p_mouse_struct = (MOUSEHOOKSTRUCT *)lparam;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	//-------------------------------------------------------------------------
	// Do as MSDN says!
	//-------------------------------------------------------------------------
	if(code < 0) {
		goto process_mouse_messages_exit;
	}

	//-------------------------------------------------------------------------
	// Get click.
	//-------------------------------------------------------------------------
    if(
		(p_mouse_struct != NULL) &&
		(
			(wparam == WM_LBUTTONDOWN) ||
			(wparam == WM_RBUTTONDOWN)
		)
	) {
		if(h_click_detected != NULL) {
			(void)SetEvent(h_click_detected);
		}
    }

process_mouse_messages_exit:

	//-------------------------------------------------------------------------
	// Call next hook in line.
	//-------------------------------------------------------------------------
	return(
		CallNextHookEx(
			h_mouse_hook, 
			code, 
			wparam, 
			lparam
		)
	);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(NULL)

}

/*-----------------------------------------------------------------------------
Function: generate_metrics
Purpose : measure and store input data.
In      : none.
Out     : updated input data.
Return  : status.
-----------------------------------------------------------------------------*/
unsigned int __stdcall generate_metrics(void *pv) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	DWORD dwret = 0;

	//-------------------------------------------------------------------------
	// Ease access variables.
	//-------------------------------------------------------------------------
	PINTEL_MODELER_INPUT_TABLE p = NULL;

	//-------------------------------------------------------------------------
	// Wait variables.
	//-------------------------------------------------------------------------
	HANDLE wait_events[WAIT_EVENTS_COUNT] = { NULL, NULL };

	//-------------------------------------------------------------------------
	// Input generation variables.
	//-------------------------------------------------------------------------
	
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	//-------------------------------------------------------------------------
	// Get PILT pointer.
	//-------------------------------------------------------------------------
	assert(pv != NULL);
	if(pv == NULL) {
		goto generate_metrics_exit;
	}
	p = (PINTEL_MODELER_INPUT_TABLE)pv;

	//-------------------------------------------------------------------------
	// Setup wait variables.
	//-------------------------------------------------------------------------
	wait_events[STOP_EVENT_INDEX] = STOP_SIGNAL;
	wait_events[CLICK_EVENT_INDEX] = h_click_detected;
	//wait_events[WINDOW_EVENT_INDEX] = h_window_detected;
	assert(wait_events[STOP_EVENT_INDEX] != NULL);
	assert(wait_events[CLICK_EVENT_INDEX] != NULL);
	//assert(wait_events[WINDOW_EVENT_INDEX] != NULL);

	//-------------------------------------------------------------------------
	// Waiting for the end of run.
	//-------------------------------------------------------------------------
	while(STOP_REQUEST == 0) {

		//---------------------------------------------------------------------
		// Waiting for mouse event thread's signal (a click).
		//---------------------------------------------------------------------
		dwret = WaitForMultipleObjects(
			WAIT_EVENTS_COUNT,
			wait_events,
			FALSE,
			INFINITE // yes, infinite!
		);
		switch(dwret) {
			case STOP_EVENT_INDEX:
				goto generate_metrics_exit; // time to leave!
				break;
			case CLICK_EVENT_INDEX:

				// sets inputs for window 1
				h_window = GetForegroundWindow();
				if (h_window != NULL) {
					get_window_info(p, h_window, 0);
				}

				// sets inputs for window 2
				h_window = GetNextWindow(h_window, GW_HWNDNEXT);
				if (h_window != NULL) {
					get_window_info(p, h_window, 1);
				}
			
				SET_INPUT_ULL_VALUE(
					INPUT_999_INDEX,
					999
				);

				break;

			default:
				goto generate_metrics_exit; // error condition
		} // switch

		//---------------------------------------------------------------------
		// Generate and store metrics.
		//---------------------------------------------------------------------

		for (int i = 0; i < INPUTS_COUNT; i = i + 1) {
			SET_INPUT_AS_LOGGED(i);
		}

		LOG_INPUT_VALUES;

		for (int i = 0; i < INPUTS_COUNT; i = i + 1) {
			SET_INPUT_AS_NOT_LOGGED(i);
		}

		INPUT_DIAGNOSTIC_HIGHLIGHTED("Click!");

	} // while need to run

	// SET_INPUT_PRIVATE_DWORD(index, value) per window

generate_metrics_exit:

	return(0);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}

void get_window_info(PINTEL_MODELER_INPUT_TABLE p, HWND hWnd, int index) {

	thread_id = 0; // thread id
	process_id = 0; // process id
	h_process = NULL; // process handle
	//process_path[MAX_PATH] = { '\0' }; // process path
	prevToken = 0; // previous .exe process

	// getting thread id and process_id
	(void)GetWindowThreadProcessId(hWnd, &process_id);

	// using parent/child structure and callback function to get .exe
	parent_child_info info = { 0 };
	(void)GetWindowThreadProcessId(hWnd, &info.parent_process_id);
	info.child_process_id = info.parent_process_id;
	(void)EnumChildWindows(hWnd, EnumChildWindowsCallback, (LPARAM)&info);

	// allowing access and getting process handle
	h_process = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, info.child_process_id);

	// getting path to process if h_process is not null
	if (h_process != NULL) {
		(void)GetProcessImageFileName(h_process, process_path, MAX_PATH);

		// intializing tokens to retrieve .exe
		wchar_t* curToken = 0;
		wchar_t* tempToken = 0;

		// getting first token
		curToken = _tcstok(process_path, L"\\");

		// iterating through all tokens
		while (curToken != NULL) {

			tempToken = _tcstok(NULL, L"\\");

			// if current token is not null, assign to token as output
			if (tempToken != NULL) {
				curToken = tempToken;
			}

			// break if we are at end of file path
			else {
				break;
			}
		}


		// checking if window is hung
		isHung = IsHungAppWindow(hWnd);
		// checking if process is immersive
		isImmersive = IsImmersiveProcess(h_process);

		SET_INPUT_ULL_VALUE(
			index * 8,
			hWnd
		);

		SET_INPUT_UNICODE_STRING_ADDRESS(
			index * 8 + 1,
			curToken
		);

		SET_INPUT_ULL_VALUE(
			index * 8 + 2,
			isHung
		);

		SET_INPUT_ULL_VALUE(
			index * 8 + 3,
			isImmersive
		);
	}
	
	// getting window rectangle information
	GetWindowRect(hWnd, &window_rect);
	if (&window_rect != NULL) {

		SET_INPUT_ULL_VALUE(
			index * 8 + 4,
			window_rect.left
		);

		SET_INPUT_ULL_VALUE(
			index * 8 + 5,
			window_rect.top
		);

		SET_INPUT_ULL_VALUE(
			index * 8 + 6,
			window_rect.right
		);

		SET_INPUT_ULL_VALUE(
			index * 8 + 7,
			window_rect.bottom
		);
	}
}
