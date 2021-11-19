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

#ifndef __INCLUDE_HAVE_A_WINDOW_MESSAGE_LOOP_INPUT__
#define __INCLUDE_HAVE_A_WINDOW_MESSAGE_LOOP_INPUT__

//-----------------------------------------------------------------------------
// Headers inclusions.
//-----------------------------------------------------------------------------
#include <windows.h>
#include "pub_intel_modeler.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
/*--------------------------------------------------------------------------*/

//-----------------------------------------------------------------------------
// Defines.
//-----------------------------------------------------------------------------
#define INPUTS_COUNT 17

#define INPUT_WINDOW_1_HANDLE_INDEX 0
#define INPUT_WINDOW_1_EXE_INDEX 1
#define INPUT_WINDOW_1_IS_HUNG 2
#define INPUT_WINDOW_1_IS_IMMERSIVE 3
#define INPUT_WINDOW_1_UPPER_LEFT_X_INDEX 4
#define INPUT_WINDOW_1_UPPER_LEFT_Y_INDEX 5
#define INPUT_WINDOW_1_BOTTOM_RIGHT_X_INDEX 6
#define INPUT_WINDOW_1_BOTTOM_RIGHT_Y_INDEX 7

#define INPUT_WINDOW_2_HANDLE_INDEX 8
#define INPUT_WINDOW_2_EXE_INDEX 9
#define INPUT_WINDOW_2_IS_HUNG 10
#define INPUT_WINDOW_2_IS_IMMERSIVE 11
#define INPUT_WINDOW_2_UPPER_LEFT_X_INDEX 12
#define INPUT_WINDOW_2_UPPER_LEFT_Y_INDEX 13
#define INPUT_WINDOW_2_BOTTOM_RIGHT_X_INDEX 14
#define INPUT_WINDOW_2_BOTTOM_RIGHT_Y_INDEX 15

#define INPUT_999_INDEX 16

#define INPUT_NAME_STRING "DSKTP"
#define INPUT_DESCRIPTION_STRINGS \
	"Window 1 HWND.", \
	"Window 1 .exe.", \
	"Window 1 is hung.", \
	"Window 1 is immersive.", \
	"Window 1 upper left x coordinate.", \
	"Window 1 upper left y coordinate.", \
	"Window 1 bottom right x coordinate.", \
	"Window 1 bottom right y coordinate.", \
	"Window 1 HWND.", \
	"Window 1 .exe.", \
	"Window 1 is hung.", \
	"Window 1 is immersive.", \
	"Window 1 upper left x coordinate.", \
	"Window 1 upper left y coordinate.", \
	"Window 1 bottom right x coordinate.", \
	"Window 1 bottom right y coordinate.", \
	"999"
#define INPUT_TYPES \
	ULL_COUNTER, \
	STRING_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	STRING_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER

//-----------------------------------------------------------------------------
// Mouse click defines.
//-----------------------------------------------------------------------------
#define SLEEP_EVENTS_COUNT \
	(1)
#define WAIT_EVENTS_COUNT \
	(2)
#define STOP_EVENT_INDEX \
		(0)
#define CLICK_EVENT_INDEX \
		(1)
#define WINDOW_EVENT_INDEX \
		(2)

//-----------------------------------------------------------------------------
// Function prototypes.
//-----------------------------------------------------------------------------
ESRV_API ESRV_STATUS modeler_init_inputs(
	unsigned int *, 
	int *, 
	int *, 
	char *,
	size_t
);
ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE);
ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE);
//-----------------------------------------------------------------------------
unsigned int __stdcall generate_metrics(void *);
unsigned int __stdcall mouse_messages_loop(void *);
LRESULT CALLBACK process_mouse_messages(int, WPARAM, LPARAM);

void get_window_info(PINTEL_MODELER_INPUT_TABLE, HWND, int);
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCLUDE_HAVE_A_WINDOW_MESSAGE_LOOP_INPUT__
