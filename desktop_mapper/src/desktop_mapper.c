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

//-------------------------------------------------------------------------
// Data structures.
//-------------------------------------------------------------------------
typedef struct _windows_structure {
	unsigned n_windows;
	HWND windows[100];
	LONG top_corners[100];
} WINDOWS_STRUCTURE, * PWINDOWS_STRUCTURE;

typedef struct _samples_structure {
	unsigned entry_count;
	PWINDOWS_STRUCTURE windows_entry;
} SAMPLES_STRUCTURE, * PSAMPLES_STRUCTURE;

WINDOWS_STRUCTURE windows_struct = { 0 };
SAMPLES_STRUCTURE samples_struct = { 0 };

// temp variables
HWND h_window_2 = NULL;
HWND h_window_3 = NULL;
LPRECT window_rect = { 0 };

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
	SET_INPUTS_COUNT(INPUTS_COUNT);

	////-------------------------------------------------------------------------
	//// Initializing structures.
	////-------------------------------------------------------------------------
	//samples_struct.windows_entry = (PWINDOWS_STRUCTURE)malloc(sizeof(WINDOWS_STRUCTURE));
	//assert(samples_struct.windows_entry != NULL);
	//INPUT_DIAGNOSTIC_MALLOC(samples_struct.windows_entry, sizeof(WINDOWS_STRUCTURE));
	//memset(samples_struct.windows_entry, 0, sizeof(WINDOWS_STRUCTURE));
	//samples_struct.entry_count = 1;


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

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

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

	////-------------------------------------------------------------------------
	//// Free dynamic memory of data structures.
	////-------------------------------------------------------------------------
	//if (samples_struct.windows_entry != NULL) {
	//	INPUT_DIAGNOSTIC_FREE(samples_struct.windows_entry);
	//	free(samples_struct.windows_entry);
	//	samples_struct.windows_entry = NULL;
	//	samples_struct.entry_count = 0;
	//}

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

				//h_window = GetForegroundWindow();
				/*h_window_2 = GetNextWindow(h_window, GW_HWNDNEXT);*/

				//GetWindowRect(
				//	h_window,
				//	window_rect
				//);

				h_window = NULL; // window handle
				thread_id = 0; // thread id
				process_id = 0; // process id
				h_process = NULL; // process handle

				// getting foreground window handle and setting isImmersive and isHung to default FALSE
				h_window = GetForegroundWindow();
				BOOL isImmersive = FALSE;
				BOOL isHung = FALSE;

				if (h_window != NULL) {

					// getting thread id and process_id
					(void)GetWindowThreadProcessId(h_window, &process_id);

					// using parent/child structure and callback function to get .exe
					parent_child_info info = { 0 };
					(void)GetWindowThreadProcessId(h_window, &info.parent_process_id);
					info.child_process_id = info.parent_process_id;
					(void)EnumChildWindows(h_window, EnumChildWindowsCallback, (LPARAM)&info);

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

						// checking if process is immersive
						isImmersive = IsImmersiveProcess(h_process);
						// checking if window is hung
						isHung = IsHungAppWindow(h_window);

						SET_INPUT_ULL_VALUE(
							INPUT_GET_FOREGROUND_WINDOW_HANDLE_INDEX,
							h_window // 5439954, 1902670
						);

						SET_INPUT_UNICODE_STRING_ADDRESS(
							INPUT_GET_FOREGROUND_WINDOW_EXE_INDEX,
							curToken
						);
					}
				}

				thread_id = 0; // thread id
				process_id = 0; // process id
				h_process = NULL; // process handle

				// getting foreground window handle and setting isImmersive and isHung to default FALSE
				h_window = GetNextWindow(h_window, GW_HWNDNEXT);
				isImmersive = FALSE;
				isHung = FALSE;

				if (h_window != NULL) {

					// getting thread id and process_id
					(void)GetWindowThreadProcessId(h_window, &process_id);

					// using parent/child structure and callback function to get .exe
					parent_child_info info = { 0 };
					(void)GetWindowThreadProcessId(h_window, &info.parent_process_id);
					info.child_process_id = info.parent_process_id;
					(void)EnumChildWindows(h_window, EnumChildWindowsCallback, (LPARAM)&info);

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

						// checking if process is immersive
						isImmersive = IsImmersiveProcess(h_process);
						// checking if window is hung
						isHung = IsHungAppWindow(h_window);

						SET_INPUT_ULL_VALUE(
							INPUT_GET_NEXT_WINDOW_HANDLE_INDEX,
							h_window
						);

						//prints(curToken.GetType());
						//printf("\n");
						//printf("%d", *curToken);
						//printf("\n");
						//printf("%s", "conhost.exe");


						//if ((wchar_t *)curToken == (wchar_t*)"conhost.exe") {
						//	break;
						//}

						SET_INPUT_UNICODE_STRING_ADDRESS(
							INPUT_GET_NEXT_WINDOW_EXE_INDEX,
							curToken
						);
					}
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

generate_metrics_exit:

	return(0);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

}
