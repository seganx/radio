/********************************************************************
	created:	2012/03/21
	filename: 	Def.h
	Author:		Sajad Beigjani
	eMail:		sajad.b@gmail.com
	Site:		www.seganx.com
	Desc:		Perprocess defines will be here
*********************************************************************/
#ifndef DEFINED_def
#define DEFINED_def

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>


//////////////////////////////////////////////////////////////////////////
//  basic type definitions
//////////////////////////////////////////////////////////////////////////

#define sx_sbyte 	int8_t
#define sx_byte		uint8_t
#define sx_short    int16_t
#define sx_ushort   uint16_t
#define sx_wchar   	uint16_t
#define sx_int		int32_t
#define sx_uint		uint32_t
#define sx_dword	uint32_t
#define sx_int64	int64_t
#define sx_ulong	uint64_t
#define sx_uint64 	uint64_t
#define sx_handle	void*
#define sx_hresult 	int



#if __cplusplus
#else
#ifdef bool
#undef bool
#endif
#ifdef true
#undef true
#endif
#ifdef false
#undef false
#endif
typedef unsigned char bool;
#define true 1
#define false 0
#endif


//////////////////////////////////////////////////////////////////////////
//  functions type and classes type preprocessors
//////////////////////////////////////////////////////////////////////////
#define NOMINMAX

#ifdef IN
#undef IN
#endif
#define IN

#ifdef OUT
#undef OUT
#endif
#define OUT

#ifdef IN_OUT
#undef IN_OUT
#endif
#define IN_OUT

#ifdef null
#undef null
#endif
#define null    NULL

#ifdef init
#undef init
#endif
#define init    {0}

//////////////////////////////////////////////////////////////////////////
//!!!    CHANGE THESE PREPROCESSORS TO CHANGE COMPILER BEHAVIOR      !!!//
//////////////////////////////////////////////////////////////////////////
#if defined(_WIN32)
#if defined( SEGAN_IMPORT )
	#define SEGAN_LIB_API					//__declspec(dllimport)
#else
	#define SEGAN_LIB_API					//__declspec(dllexport)
#endif
#else
	#define SEGAN_LIB_API
#endif

#if defined(_WIN32)
#define SEGAN_LIB_INLINE					//__forceinline
#else
#define SEGAN_LIB_INLINE					//static inline
#endif

#define SEGAN_INLINE						//inline

#if defined(_WIN32)
	#define SEGAN_ALIGN_16					__declspec(align(16))
#else
	#define SEGAN_ALIGN_16
#endif

#define SEGANX_TRACE_ASSERT					1		//	check and log some special events on containers

#define SEGANX_TRACE_MEMORY					1		//	use second version of memory leak detector

#define SEGANX_TRACE_CRASHRPT               1

#define SEGANX_TRACE_CALLSTACK              1

#define SEGANX_TRACE_PROFILER               0


#define SEGAN_LIB_MULTI_THREADED			0		//	enable core library multi-threaded safe 

#define SX_LIB_SINGLETON                    0



//////////////////////////////////////////////////////////////////////////
//!!!  DO NOT CHANGE THIS AREA ANY MORE	 !!!//
//////////////////////////////////////////////////////////////////////////

// release preprocessors
#define sx_release(Obj)						{ if (Obj) { Obj->Release(); } }
#define sx_release_and_null(Obj)			{ if (Obj) { Obj->Release(); Obj = null; } }

// some useful macros for flags
#define sx_flag_has(set, flag)				( set & flag )
#define sx_flag_hasnt(set, flag)			( !( set & flag ) )
#define sx_flag_add(set, flag)				( set |= flag )
#define sx_flag_rem(set, flag)				( set &= ~flag )

// some useful functions for sx_byte operations
#define sx_byte_of(var, index)				( ( (sx_byte*)(&var) )[index] )
#define sx_1th_byte_of(var)					( ( (sx_byte*)(&var) )[0] )
#define sx_2th_byte_of(var)					( ( (sx_byte*)(&var) )[1] )
#define sx_3th_byte_of(var)					( ( (sx_byte*)(&var) )[2] )
#define sx_4th_byte_of(var)					( ( (sx_byte*)(&var) )[3] )

#define sx_1th_word_of(var)					( ( (word*)(&var) )[0] )
#define sx_2th_word_of(var)					( ( (word*)(&var) )[1] )

#define sx_fourcc(ch0, ch1, ch2, ch3)		( (sx_dword)(sx_byte)(ch0) | ((sx_dword)(sx_byte)(ch1) << 8) | ((sx_dword)(sx_byte)(ch2) << 16) | ((sx_dword)(sx_byte)(ch3) << 24 ) )

//!	takes a structure name s, and a field name f in s
#define sx_offset_of(s, f)		        	( (sx_dword)&( ((s *)0)->f ) )

//! return number of items in a static array
#define sx_array_count(x)					( sizeof(x) / sizeof(x[0]) )

//! avoid class from copy constructor and assign operator
#define sx_sterile_class(classname)		    private: classname(classname& obj); void operator= (classname& obj)

//	some crazy macro to define unique names
#define PP_CAT(a, b) PP_CAT_I(a, b)
#define PP_CAT_I(a, b) PP_CAT_II(~, a ## b)
#define PP_CAT_II(p, res) res
#define sx_unique_name(base) PP_CAT(base, __COUNTER__)


//! string conversions to support multiplatforms
#if	defined(_WIN32)
	#define	PATH_PART	'/'
	#define sx_snprintf(buf, size, fmt, ...)       _snprintf_s((buf), (size), _TRUNCATE, (fmt), ##__VA_ARGS__)
#elif defined(_MAC)
	//#define	PATH_PART
#else
	//#define	PATH_PART
	#define sx_snprintf(buf, size, fmt, ...)       snprintf((buf), (size), (fmt), ##__VA_ARGS__)
#endif


//	use debug output window in IDE
#if defined(_DEBUG)
#if defined(_MSC_VER)
#if ( _MSC_VER >= 1400 )
#define DEBUG_OUTPUT_WINDOW
#endif
#endif
#endif

#if defined(_WIN32)
//! disable container warnings
#pragma warning(disable:4251)
#pragma warning(disable:4275)
#endif



#endif	//	DEFINED_def
