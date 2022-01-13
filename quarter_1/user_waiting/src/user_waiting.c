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
#include <assert.h>
#include "user_waiting.h"
#include "service.h"
#include <Windows.h>

//-----------------------------------------------------------------------------
// Global counter variable.
//-----------------------------------------------------------------------------
char* cursor = "null"; // name of cursor icon to return
char* prevCursor = "null"; // name of previous cursor icon
CURSORINFO ci = { 0 }; // cursor info structure
HCURSOR cursorHandles[15] = { 0 }; // array of cursor icon handles
char* cursorStrings[15] = { '\0' }; // array cursor icon string names
DWORD thread_id = 0;
HANDLE h_thread = NULL;


/*-----------------------------------------------------------------------------
Function: modeler_init_inputs
Purpose : return the inputs count.
In      : pointers defined by the interface.
Out     : modified unsigned integer.
Return  : status.
-----------------------------------------------------------------------------*/
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int* p,
	int* pfd,
	int* pfe,
	char* po,
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

	cursorHandles[0] = LoadCursor(NULL, IDC_APPSTARTING);
	cursorHandles[1] = LoadCursor(NULL, IDC_ARROW);
	cursorHandles[2] = LoadCursor(NULL, IDC_CROSS);
	cursorHandles[3] = LoadCursor(NULL, IDC_HAND);
	cursorHandles[4] = LoadCursor(NULL, IDC_HELP);
	cursorHandles[5] = LoadCursor(NULL, IDC_IBEAM);
	cursorHandles[6] = LoadCursor(NULL, IDC_ICON);
	cursorHandles[7] = LoadCursor(NULL, IDC_NO);
	cursorHandles[8] = LoadCursor(NULL, IDC_SIZEALL);
	cursorHandles[9] = LoadCursor(NULL, IDC_SIZENESW);
	cursorHandles[10] = LoadCursor(NULL, IDC_SIZENS);
	cursorHandles[11] = LoadCursor(NULL, IDC_SIZENWSE);
	cursorHandles[12] = LoadCursor(NULL, IDC_SIZEWE);
	cursorHandles[13] = LoadCursor(NULL, IDC_UPARROW);
	cursorHandles[14] = LoadCursor(NULL, IDC_WAIT);

	cursorStrings[0] = "IDC_APPSTARTING";
	cursorStrings[1] = "IDC_ARROW";
	cursorStrings[2] = "IDC_CROSS";
	cursorStrings[3] = "IDC_HAND";
	cursorStrings[4] = "IDC_HELP";
	cursorStrings[5] = "IDC_IBEAM";
	cursorStrings[6] = "IDC_ICON";
	cursorStrings[7] = "IDC_NO";
	cursorStrings[8] = "IDC_SIZEALL";
	cursorStrings[9] = "IDC_SIZENESW";
	cursorStrings[10] = "IDC_SIZENS";
	cursorStrings[11] = "IDC_SIZENWSE";
	cursorStrings[12] = "IDC_SIZEWE";
	cursorStrings[13] = "IDC_UPARROW";
	cursorStrings[14] = "IDC_WAIT";

	SIGNAL_EVENT_DRIVEN_MODE;
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
	static char* descriptions[INPUTS_COUNT] = {
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

	h_thread = (HANDLE)_beginthreadex(
		NULL,
		0,
		custom_event_listener_thread,
		(void *)p,
		0,
		(unsigned int *)&thread_id
	);
	if (h_thread == NULL) {
		goto modeler_open_inputs_error;
	}


	//-------------------------------------------------------------------------
	// Set input information.
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INPUT_NAME_STRING);
	SET_INPUT_DESCRIPTION(
		MOUSE_CURSOR_STATE_INDEX,
		descriptions[MOUSE_CURSOR_STATE_INDEX]
	);
	SET_INPUT_TYPE(
		MOUSE_CURSOR_STATE_INDEX,
		STRING_COUNTER
	);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

modeler_open_inputs_error:

	return (ESRV_FAILURE);

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

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	BOOL bret = FALSE;
	POINT point = { 0 };

	assert(p != NULL);

	////-------------------------------------------------------------------------
	//// Get cursor icon.
	////-------------------------------------------------------------------------
	//ci.cbSize = sizeof(ci);
	//GetCursorInfo(&ci);

	////-------------------------------------------------------------------------
	//// Comparing cursor to cursorHandles array.
	////-------------------------------------------------------------------------
	//for (int i = 0; i < 15; i = i + 1) {
	//	if (cursorHandles[i] == ci.hCursor) {
	//		cursor = cursorStrings[i];
	//		break;
	//	}
	//}

	////-------------------------------------------------------------------------
	//// Set input values.
	////-------------------------------------------------------------------------
	//SET_INPUT_STRING_ADDRESS(
	//	MOUSE_CURSOR_STATE_INDEX,
	//	cursor
	//);

	return(ESRV_SUCCESS);

//modeler_read_inputs_error;

	return(ESRV_FAILURE);

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
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE p) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	DWORD dwret = 0;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

		assert(p != NULL);

	while (STOP_REQUEST == 0) {

		//---------------------------------------------------------------------
		// Pause to simulate event triggering.
		// Note:
		//    Rather than using a sleep, which would lock the event listener 
		//    thread, we recommend using the method shown below. In general
		//    developers of event-driven input libraries should add into the
		//    end condition the event / semaphore via the STOP_SIGNAL macro 
		//    (also - but not instead - use the STOP_REQUEST macro).
		//---------------------------------------------------------------------
		dwret = WaitForSingleObject(
			STOP_SIGNAL,
			INPUT_PAUSE_IN_S * 1000
		);
		switch (dwret) {
		case WAIT_OBJECT_0:
			goto modeler_listen_inputs_exit; // time to leave!
			break;
		case WAIT_TIMEOUT:
			break; // all good, time to make a measurement
		default:
			goto modeler_listen_inputs_exit; // error condition
		} // switch

		//---------------------------------------------------------------------
		// Trigger asynchronous logging.
		//---------------------------------------------------------------------
		LOG_INPUT_VALUES;

	}
modeler_listen_inputs_exit:

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)

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

ESRV_API unsigned int __stdcall custom_event_listener_thread(void* px) {

	//-------------------------------------------------------------------------
	// Generic variables.
	//-------------------------------------------------------------------------
	DWORD dwret = 0;

	//-------------------------------------------------------------------------
	// Watchdog variables.
	//-------------------------------------------------------------------------
	WATCHDOG_VARIABLES

	//-------------------------------------------------------------------------
	// Access helper variables.
	//-------------------------------------------------------------------------
	PINTEL_MODELER_INPUT_TABLE p = NULL;

	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	// Exception handling section begin.
	//-------------------------------------------------------------------------
	INPUT_BEGIN_EXCEPTIONS_HANDLING

	assert(px != NULL);
	p = (PINTEL_MODELER_INPUT_TABLE)px;

	//-------------------------------------------------------------------------
	// Name this thread (for debug mode only).
	//-------------------------------------------------------------------------
	INPUT_DIAGNOSTIC_NAME_THIS_THREAD(
		p,
		"custom_event_listener_thread"
	);

	//-------------------------------------------------------------------------
	// Register this thread with watchdog.
	//-------------------------------------------------------------------------
	INPUT_REGISTER_EVENT_LOCKED_THREAD_WITH_WATCHDOG(
		p,
		"custom_event_listener_thread",
		h_thread,
		thread_id,
		STOP_SIGNAL,
		custom_event_listener_thread_exit
	);

	while (STOP_REQUEST == 0) {
		//---------------------------------------------------------------------
			// Pause to simulate event triggering.
			// Note:
			//    Rather than using a sleep, which would lock the event listener 
			//    thread, we recommend using the method shown below. In general
			//    developers of event-driven input libraries should add into the
			//    end condition the event / semaphore via the STOP_SIGNAL macro 
			//    (also - but not instead - use the STOP_REQUEST macro).
			//---------------------------------------------------------------------
		dwret = WaitForSingleObject(
			STOP_SIGNAL,
			INPUT_PAUSE_IN_S * 33 // times milliseconds (10 * 0.033 second)
		);
		switch (dwret) {
		case WAIT_OBJECT_0:
				goto custom_event_listener_thread_exit; // time to leave!
				break;
			case WAIT_TIMEOUT:
				//-------------------------------------------------------------------------
				// Get cursor icon.
				//-------------------------------------------------------------------------
				ci.cbSize = sizeof(ci);
				GetCursorInfo(&ci);

				//-------------------------------------------------------------------------
				// Comparing cursor to cursorHandles array.
				//-------------------------------------------------------------------------
				for (int i = 0; i < 15; i = i + 1) {
					if (cursorHandles[i] == ci.hCursor) {
						cursor = cursorStrings[i];
						break;
					}
				}

				// if cursor icon state did not change, do not log
				if (prevCursor == cursor) {
					break;
				}

				//-------------------------------------------------------------------------
				// Set input values.
				//-------------------------------------------------------------------------
				SET_INPUT_STRING_ADDRESS(
					MOUSE_CURSOR_STATE_INDEX,
					cursor
				);

				// setting previous cursor value equal to current cursor value
				prevCursor = cursor;

				break; // all good, time to make a measurement
			default:
				goto custom_event_listener_thread_exit; // error condition
		} // switch

		//---------------------------------------------------------------------
		// Trigger logging.
		//---------------------------------------------------------------------
		LOG_INPUT_VALUES;
	} // while

	//-------------------------------------------------------------------------
	// Un-register this thread with watchdog.
	//-------------------------------------------------------------------------
	INPUT_UNREGISTER_THREAD_WITH_WATCHDOG(
		p,
		h_thread,
		thread_id
	);

custom_event_listener_thread_exit:
	return(ESRV_FAILURE);

	return(ESRV_SUCCESS);

	//-------------------------------------------------------------------------
	// Exception handling section end.
	//-------------------------------------------------------------------------
	INPUT_END_EXCEPTIONS_HANDLING(p)
}