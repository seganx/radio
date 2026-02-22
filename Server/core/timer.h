/********************************************************************
	created:	2017/05/22
	filename: 	Time.h
	Author:		Sajad Beigjani
	eMail:		sajad.b@gmail.com
	Site:		www.SeganX.com
	Desc:		This file contain a simple class and functions to sx_handle times
*********************************************************************/
#ifndef DEFINED_TIME
#define DEFINED_TIME

#include "def.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

SEGAN_LIB_API sx_ulong sx_time_now();
SEGAN_LIB_API sx_ulong sx_time_diff(const sx_ulong t1, const sx_ulong t2);
SEGAN_LIB_API void sx_time_print(char* dest, const sx_uint destsize, const sx_ulong timeval);
SEGAN_LIB_API sx_int sx_localtime(struct tm* out, const time_t* t);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif	//	DEFINED_TIME