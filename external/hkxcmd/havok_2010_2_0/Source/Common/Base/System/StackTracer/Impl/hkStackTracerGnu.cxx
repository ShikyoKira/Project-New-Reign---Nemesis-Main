// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#if defined(HK_PLATFORM_UNIX)
#include <Common/Base/hkBase.h>
#	include <Common/Base/Fwd/hkcmalloc.h>
#	include <Common/Base/Fwd/hkcstdio.h>
#	include <Common/Base/Fwd/hkcstring.h>
#	include <execinfo.h>
#	define backtrace_symbols_free(syms) free(syms)
#elif defined( HK_PLATFORM_PSP )
#	include <Common/Base/System/StackTracer/Impl/hkBacktrace.cxx>
#else
#	error "Dont know where to find 'backtrace'"
#endif

// 
// Be careful not to call any havok functions which may
// result in a memory allocation as these methods are
// called from the debug memory destructor
//

hkStackTracer::hkStackTracer()
{
}

hkStackTracer::~hkStackTracer()
{
}

static const int IGNORE_FRAMES = 2;

#if 0 // Slow backtrace from gdb

extern char* program_invocation_name;

// This is slow compared to simply calling backtrace_symbols, but gives it
// gives the file:line information.
void hkStackTracer::dumpStackTrace( const hkUlong* trace, int numtrace, printFunc pfunc, void* context ) const
{
    char cmd[1024];
    int cur = snprintf( cmd, sizeof(cmd), "addr2line -C -f -e %s", program_invocation_name);
    for( int i = IGNORE_FRAMES; i < numtrace; ++i )
    {
        cur += snprintf( cmd+cur, sizeof(cmd)-cur, " %p", (void*)trace[i] );
        if( cur > (int)sizeof(cmd) ) return;
    }
    FILE* fin = popen(cmd, "r");
    while( feof(fin) == 0 )
    {
        char func[1024];
        char file[1024];
        if( fgets(func, sizeof(func), fin) )
        {
            if( fgets(file, sizeof(file), fin) )
            {
                int l = strlen(file);
                file[ l > 0 ? l-1 : 0 ] = 0;
                pfunc(file, context);
                pfunc(":", context);
                pfunc(func, context);
            }
        }
    }
    pclose(fin);
}

#else

void hkStackTracer::dumpStackTrace( const hkUlong* trace, int numtrace, printFunc pfunc, void* context ) const
{

    char** strings = backtrace_symbols( (void* const*)trace, numtrace);
    if(strings)
    {
        for( int i = IGNORE_FRAMES; i < numtrace; ++i )
        {
            const char* loc = hkString::strStr(strings[i], "(");
			char buf[1024];
			hkString::sprintf( buf, "%s\n", loc ? loc : strings[i] );
            pfunc( buf, context);
        }
        backtrace_symbols_free(strings);
    }
    else
    {
        char buf[100];
        for( int i = IGNORE_FRAMES; i < numtrace; ++i )
        {
            hkString::snprintf(buf, 100, "%.8x (no symbols)\n", trace[i]);
            pfunc(buf, context);
        }
    }
}

#endif

int hkStackTracer::getStackTrace( hkUlong* trace, int maxtrace )
{
	int depth = backtrace( (void**)trace, maxtrace);
#ifdef HK_PLATFORM_PSP
	extern char _etext[];

	for(int i = 0; i < depth; i++)
	{
		trace[i] = ((char*)trace[i] - _etext);
	}
#endif
	return depth;  
}

void hkStackTracer::refreshSymbols()
{

}

void hkStackTracer::getModuleInfo( printFunc pfunc, void* context ) const
{

}

/*
* Havok SDK - NO SOURCE PC DOWNLOAD, BUILD(#20101115)
* 
* Confidential Information of Havok.  (C) Copyright 1999-2010
* Telekinesys Research Limited t/a Havok. All Rights Reserved. The Havok
* Logo, and the Havok buzzsaw logo are trademarks of Havok.  Title, ownership
* rights, and intellectual property rights in the Havok software remain in
* Havok and/or its suppliers.
* 
* Use of this software for evaluation purposes is subject to and indicates
* acceptance of the End User licence Agreement for this product. A copy of
* the license is included with this software and is also available at www.havok.com/tryhavok.
* 
*/
