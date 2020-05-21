/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_TRACE_STREAM_H
#define HKBASE_TRACE_STREAM_H

#include <Common/Base/System/Io/OStream/hkOStream.h>

extern const class hkClass hkTraceStreamTitleClass;

extern const class hkClass hkTraceStreamClass;

	/// This class is used to debug the internal core of Havok.
	/// This class in not intended to be used by clients yet.
class hkTraceStream: public hkReferencedObject, public hkSingleton<hkTraceStream>
{
	//+serializable(false)  
	//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

		struct Title
		{
			//+serializable(false)  
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkTraceStream::Title );
            HK_DECLARE_REFLECTION();

			Title() { m_value[0] = 0; }
			Title(const char* value)
			{
				HK_ASSERT2(0xf0ff0090, hkString::strLen(value) < 32, "Error in debugging utils: title cannot exceed 31 characters.");
				hkString::strNcpy(m_value, value, 31);
			}

			char m_value[32];
		};


			// disable all printf with a given title.
		void disableTitle( const char* title ) { m_titles.pushBack( Title(title) ); }

			// enable all printf with a given title.
		//void enableTitle( const char* title )  { }

			// reset the traceStream
		void reset();

			// print a debug line with a given title
		void printf( const char* title, const char* fmt, ... );

			// internal dummy function
		void dontPrintf( const char* title, const char* fmt, ... );

	public:
		hkTraceStream(){ m_stream = HK_NULL; m_counter = 0; }
			/// Needed to create a new vtbl
		//virtual ~hkTraceStream() {}

	
		hkTraceStream( hkFinishLoadedObjectFlag flag ):m_titles(flag) {}

	public:
		hkOstream* m_stream;		
		int		   m_counter;

        hkArray<struct Title> m_titles;

};

#ifdef HK_DEBUG
//#	define HK_DEBUG_TOI "debugToi.txt"
#endif

#if defined HK_DEBUG_TOI
#	define hkToiPrintf hkTraceStream::getInstance().printf
#	define hkDebugToi 1
#else
#	define hkToiPrintf if(0) hkTraceStream::getInstance().dontPrintf
#	define hkDebugToi 0
#endif

#define HK_IS_TRACE_ENABLED(A, B) true


#endif // HKBASE_TRACE_STREAM_H

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
