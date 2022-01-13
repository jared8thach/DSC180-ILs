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

#ifndef __INCLUDE_HOOK_INPUT__
#define __INCLUDE_HOOK_INPUT__

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
#define PATH_SEPARATOR_CHAR '\\'
#define STRING_BUFFERS_SIZE 1024

#define INPUT_COUNT 9
	#define INPUT_CLICK_X_POSITION_INDEX 0
	#define INPUT_CLICK_Y_POSITION_INDEX 1
	#define INPUT_CLICKED_UI_OBJECT_ID_INDEX 2
	#define INPUT_CLICKED_UI_OBJECT_ROOT_ID_INDEX 3
	#define INPUT_CLICKED_UI_OBJECT_CLASS_NAME_INDEX 4
	#define INPUT_CLICKED_UI_OBJECT_STYLE_INDEX 5
	#define INPUT_CLICKED_UI_OBJECT_EXTENDED_STYLE_INDEX 6
	#define INPUT_CLICKED_UI_OBJECT_NAME_INDEX 7
	#define INPUT_CLICKED_UI_OBJECT_OWNING_PROCESS_IMAGE_INDEX 8

#define INPUT_NAME_STRING "MOUSE-HOOK"

#define INPUT_DESCRIPTION_STRINGS \
	"Click X Position (in pixels).", \
	"Click Y Position (in pixels).", \
	"Clicked UI Object ID (no unit).", \
	"Clicked UI Object Root ID (no unit).", \
	"Clicked UI Object Class Name (no unit).", \
	"Clicked UI Object Style (no unit).", \
	"Clicked UI Object Extended Style (no unit).", \
	"Clicked UI Object Name (no unit).", \
	"Clicked UI Object Owning Process Image (no unit)."

#define INPUT_TYPES \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	STRING_COUNTER, \
	ULL_COUNTER, \
	ULL_COUNTER, \
	STRING_COUNTER, \
	STRING_COUNTER

#define WAIT_EVENTS_COUNT (2)
	#define STOP_EVENT_INDEX (0)
	#define CLICK_EVENT_INDEX (1)

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
ESRV_API unsigned int __stdcall custom_event_listner_thread(void *);
//-----------------------------------------------------------------------------
unsigned int __stdcall get_object_info(void *);
unsigned int __stdcall mouse_messages_loop(void *);
LRESULT CALLBACK process_mouse_messages(int, WPARAM, LPARAM);
/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCLUDE_HOOK_INPUT__
