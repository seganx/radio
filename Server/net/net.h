/********************************************************************
	created:	2012/04/11
	filename: 	Net.h
	Author:		Sajad Beigjani
	eMail:		sajad.b@gmail.com
	Site:		www.SeganX.com
	Desc:		This file contain some basic parts of network system
				these parts are include some functions to initialize &
				finalize				
*********************************************************************/
#pragma once

#include "../core/def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//! initialize network system
SEGAN_LIB_API bool sx_net_initialize(void);

//! finalize network system
SEGAN_LIB_API void sx_net_finalize(void);

//! legacy: return a short name for an explicit error code
SEGAN_LIB_API char* sx_net_error_string(const sx_int code);

//! unified: get last platform error code (WSAGetLastError / errno)
SEGAN_LIB_API sx_int sx_net_last_error_code(void);

//! unified: get string for last error (code==0) or a given code
SEGAN_LIB_API const char* sx_net_last_error_string(sx_int code);

#ifdef __cplusplus
}
#endif // __cplusplus
