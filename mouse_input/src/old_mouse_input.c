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

#include "mouse_input.h"

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Global counter.
//-----------------------------------------------------------------------------
unsigned long long int counter = 0;

//-----------------------------------------------------------------------------
// Global data to process DCTL.
//-----------------------------------------------------------------------------
DCTL_COMMAND dctl_command;
DCTL_COMMAND_DATA dctl_command_argument[
	INPUT_RESET_ARGUMENTS_SIZE
] = { 0 };

	/*-----------------------------------------------------------------------------
	Function: modeler_init_inputs
	Purpose : return the inputs count.
	In      : pointer to an unsigned integer.
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
		//-------------------------------------------------------------------------
		for (i = 0; i < INPUTS_COUNT; i++) {
			SET_INPUT_DESCRIPTION(
				i,
				descriptions[i]
			);
			SET_INPUT_TYPE(
				i,
				types[i]
			);
		} // for i (each input)

		//-------------------------------------------------------------------------
		// Register DCTLs.
		//-------------------------------------------------------------------------
		dctl_command = INPUT_RESET_COMMAND;
		//-------------------------------------------------------------------------
		REGISTER_INPUT_LIBRARY_DCTL(
			INPUT_RESET_DCTL_NAME,
			1,
			INPUT_RESET_TOKEN,
			dctl_command,
			dctl_command_argument
		);
		//-------------------------------------------------------------------------
		dctl_command = INPUT_MULTIPLY_COMMAND;
		REGISTER_INPUT_LIBRARY_DCTL(
			INPUT_MULTIPLY_DCTL_NAME,
			1,
			INPUT_MULTIPLY_TOKEN,
			dctl_command,
			dctl_command_argument
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
		// Generate incrementing input.
		//-------------------------------------------------------------------------
		counter++;

		//-------------------------------------------------------------------------
		// Set input values.
		//-------------------------------------------------------------------------
		SET_INPUT_ULL_VALUE(
			INPUT_INDEX,
			counter
		);

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
	ESRV_STATUS modeler_listen_inputs(PINTEL_MODELER_INPUT_TABLE p) {

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
	Function: modeler_process_dctl
	Purpose : process DCTL commands on DCTL interrupt notification.
	In      : pointer to PINTEL_MODELER_INPUT_TABLE data structure.
	Out     : modified PINTEL_MODELER_INPUT_TABLE data structure.
	Return  : status.
	-----------------------------------------------------------------------------*/
	ESRV_STATUS modeler_process_dctl(PINTEL_MODELER_INPUT_TABLE p) {

		//-------------------------------------------------------------------------
		// Generic variables.
		//-------------------------------------------------------------------------
		size_t l = 0;
		size_t ll = 0;

		//-------------------------------------------------------------------------
		// Variables used to extract multiplier -- if any.
		//-------------------------------------------------------------------------
		int x = 0;
		char* px = NULL;
		unsigned long long int multiplier =
			INPUT_MULTIPLY_DEFAULT_MULTIPLIER
			;

		//-------------------------------------------------------------------------

		//-------------------------------------------------------------------------
		// Exception handling section begin.
		//-------------------------------------------------------------------------
		INPUT_BEGIN_EXCEPTIONS_HANDLING

			assert(p != NULL);

		switch (dctl_command) {

		case INPUT_RESET_COMMAND:

			//-----------------------------------------------------------------
			// Reset the counter.
			//-----------------------------------------------------------------
			counter = 0;
			break;

		case INPUT_MULTIPLY_COMMAND:

			//-----------------------------------------------------------------
			// Sear for a multiplier in the user message.
			//-----------------------------------------------------------------
			ll = strlen(INPUT_MULTIPLY_TOKEN);
			l = strlen(dctl_command_argument);
			if (l > ll) {
				px = &dctl_command_argument[ll];
				x = atoi(px);
				if (x != 0) {
					multiplier = (unsigned long long int)x;
				}
			}

			//-----------------------------------------------------------------
			// Multiply the counter.
			//-----------------------------------------------------------------
			counter *= multiplier;
			break;

		} // switch

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
