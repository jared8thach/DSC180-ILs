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
unsigned long long int counter = 0;
unsigned long long int mouse_x = 0;
unsigned long long int mouse_y = 0;
char* cursor = "null";
CURSORINFO ci;
HCURSOR cursorArray[5];

HWND window;
DWORD thread;

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

	cursorArray[0] = LoadCursor(NULL, IDC_ARROW);
	cursorArray[1] = LoadCursor(NULL, IDC_APPSTARTING);
	cursorArray[2] = LoadCursor(NULL, IDC_HAND);
	cursorArray[3] = LoadCursor(NULL, IDC_IBEAM);
	cursorArray[4] = LoadCursor(NULL, IDC_WAIT);

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

	//-------------------------------------------------------------------------
	// Set input information.
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INPUT_NAME_STRING);
	SET_INPUT_DESCRIPTION(
		MOUSE_X_INPUT_INDEX,
		descriptions[MOUSE_X_INPUT_INDEX]
	);
	SET_INPUT_TYPE(
		MOUSE_X_INPUT_INDEX,
		ULL_COUNTER
	);
	//-------------------------------------------------------------------------
	SET_INPUTS_NAME(INPUT_NAME_STRING);
	SET_INPUT_DESCRIPTION(
		MOUSE_Y_INPUT_INDEX,
		descriptions[MOUSE_Y_INPUT_INDEX]
	);
	SET_INPUT_TYPE(
		MOUSE_Y_INPUT_INDEX,
		ULL_COUNTER
	);
	//-------------------------------------------------------------------------

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

	//-------------------------------------------------------------------------
	// Get cursor position.
	//-------------------------------------------------------------------------
	bret = GetCursorPos(&point);
	if (bret == FALSE) {
		/*goto modeler_read_inputs_error;*/
	}
	mouse_x = (unsigned long long int)point.x;
	mouse_y = (unsigned long long int)point.y;

	//-------------------------------------------------------------------------
	// Get cursor icon.
	//-------------------------------------------------------------------------
	ci.cbSize = sizeof(ci);
	GetCursorInfo(&ci);
	if (cursorArray[0] == ci.hCursor) {
		cursor = "IDC_ARROW";
	}
	else if (cursorArray[1] == ci.hCursor) {
		cursor = "IDC_APPSTARTING";
	}
	else if (cursorArray[2] == ci.hCursor) {
		cursor = "IDC_HAND";
	}
	else if (cursorArray[3] == ci.hCursor) {
		cursor = "IDC_BEAM";
	}
	else if (cursorArray[4] == ci.hCursor) {
		cursor = "IDC_WAIT";
	}
	else {
		cursor = "OTHER";
	}

	//-------------------------------------------------------------------------
	// Set input values.
	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		MOUSE_X_INPUT_INDEX,
		mouse_x
	);
	//-------------------------------------------------------------------------
	SET_INPUT_ULL_VALUE(
		MOUSE_Y_INPUT_INDEX,
		mouse_y
	);
	//-------------------------------------------------------------------------
	//window = GetForeGroundWindow();
	//GetWindowThreadProcessId(&window);
	

	SET_INPUT_STRING_ADDRESS(
		MOUSE_CURSOR_STATE_INDEX,
		cursor
	);

	return(ESRV_SUCCESS);
	/*modeler_read_inputs_error;
		return(ESRV_FAILURE);*/

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
			INPUT_PAUSE_IN_S * 100
		);
		switch (dwret) {
		case WAIT_OBJECT_0:
			//cursor = "WAIT_OBJECT_0";
			goto modeler_listen_inputs_exit; // time to leave!
			break;
		case WAIT_TIMEOUT:
			//cursor = "WAIT_TIMEOUT";
			break; // all good, time to make a measurement
		default:
			goto modeler_listen_inputs_exit; // error condition
		} // switch

		//---------------------------------------------------------------------
		// Generate incrementing input.
		//---------------------------------------------------------------------
		counter += INPUT_EVENT_INCREMENT;

		//---------------------------------------------------------------------
		// Set input values.
		//---------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_INDEX,
			counter
		);

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