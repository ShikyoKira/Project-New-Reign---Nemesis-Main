// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>
#include <Common/Base/System/StackTracer/hkStackTracer.h>

hkStackTracer::hkStackTracer()
{
}

hkStackTracer::~hkStackTracer()
{
}

void hkStackTracer::dumpStackTrace( const hkUlong* trace, int numtrace, printFunc pfunc, void* context ) const
{
	for( int i = 0; i < numtrace; ++i )
	{
		char buf[256];
		hkString::snprintf(buf, sizeof(buf), "0x%x\n", trace[i] );
		pfunc(buf, context);
	}
}

int hkStackTracer::getStackTrace( hkUlong* trace, int maxtrace )
{
	struct frame
	{
		frame* next;
		hkUlong retaddr;
	};

	frame* stackPtr;
	frame* framePtr;

	__asm
	{
		mov stackPtr, esp
		mov framePtr, ebp
	}

	int count = 0;
	if( stackPtr && framePtr )
	{
		frame* current = framePtr;

		while( (count < maxtrace)
			&& (stackPtr < current)
			&& (current != 0)
			&& (current->retaddr != 0) )
		{
			trace[count++] = current->retaddr;
			current = current->next;
		}
	}

	return count;
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
