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

#ifndef __INCLUDE_INCREMENTING_INPUT__
#define __INCLUDE_INCREMENTING_INPUT__

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
#define INPUTS_COUNT 1
#define INPUT_INDEX 0
#define INPUT_NAME_STRING "INC"
#define INPUT_DESCRIPTION_STRINGS \
	"Simple incrementing counter."
#define INPUT_TYPES \
	ULL_COUNTER

//-----------------------------------------------------------------------------
// +----------------------------+---------------------+-----------------------+
// | DCTL NAME                  | DCTL Arg            | Answer to DCTL        |
// +----------------------------+---------------------+-----------------------+ 
// | RESET                      | None                | Rester the input to 0 |
// +----------------------------+---------------------+-----------------------+ 
// | MULTIPLY                   | None                | Multiply the input by |
// |                            |                     | a constant (10).      |
// +----------------------------+---------------------+-----------------------+ 
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DCTL Defines.
//-----------------------------------------------------------------------------
#define INPUT_RESET_COMMAND '!'
#define INPUT_RESET_TOKEN "RESET"
#define INPUT_RESET_DCTL_NAME "INC-RESET"

#define INPUT_MULTIPLY_COMMAND '*'
#define INPUT_MULTIPLY_TOKEN "MULTIPLY"
#define INPUT_MULTIPLY_TOKEN "MULTIPLY"
#define INPUT_MULTIPLY_DCTL_NAME "INC-MULTIPLY"
#define INPUT_MULTIPLY_DEFAULT_MULTIPLIER 10

#define INPUT_RESET_ARGUMENTS_SIZE 20

//-----------------------------------------------------------------------------
// Function prototypes.
//-----------------------------------------------------------------------------
	ESRV_API ESRV_STATUS modeler_init_inputs(
		unsigned int*,
		int*,
		int*,
		char*,
		size_t
	);
	ESRV_API ESRV_STATUS modeler_open_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_API ESRV_STATUS modeler_close_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_read_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE);
	ESRV_STATUS modeler_process_lctl(PINTEL_MODELER_INPUT_TABLE);
	/*--------------------------------------------------------------------------*/
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // __INCLUDE_INCREMENTING_INPUT__
