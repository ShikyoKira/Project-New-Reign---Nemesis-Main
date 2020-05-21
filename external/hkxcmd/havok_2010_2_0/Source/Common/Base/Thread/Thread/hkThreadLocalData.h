/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HK_THREAD_LOCAL_POINTER_H
#define HKBASE_HK_THREAD_LOCAL_POINTER_H

#include <Common/Base/Config/hkConfigThread.h>
//HK_THREAD_LOCAL - declare a thread local variable
//HK_THREAD_LOCAL_SET(var,value) - assign to tls var
//HK_THREAD_LOCAL_GET(var) - get from tls var

#ifndef HK_XBOX360_EXPLICIT_TLS
#	define HK_XBOX360_EXPLICIT_TLS 0
#endif

#if (HK_CONFIG_THREAD == HK_CONFIG_MULTI_THREADED) && !defined(HK_PLATFORM_SPU)

		#define HK_WIN32_TLS_INVALID ((hkUint32)0xFFFFFFFF)

		extern "C" {
			__declspec(dllimport) unsigned long _stdcall TlsAlloc(void);
			__declspec(dllimport) void* _stdcall TlsGetValue( unsigned long dwTlsIndex );		
			__declspec(dllimport) int _stdcall TlsSetValue( unsigned long dwTlsIndex, void* lpTlsValue);
			__declspec(dllimport) int _stdcall TlsFree( unsigned long dwTlsIndex );
		}


		/// A platform independent wrapper for thread local storage
		/// We assume we always just store a pointer (or data the same size or smaller than a pointer)
		/// i.e.,, sizeof(T) <= sizeof(char*)
		/// In debug it will now stop when platform limits are reached as it will soon cause a crash anyway
		/// such as threadmem being null etc
		template<typename T>
		class hkThreadLocalData
		{
			public:

				hkThreadLocalData() 
				{ 
					m_slotID = TlsAlloc();
					HK_ON_DEBUG( if (m_slotID == HK_WIN32_TLS_INVALID) { HK_BREAKPOINT(0xabba3651); } ) 
				}
				~hkThreadLocalData() 
				{ 
					HK_ON_DEBUG( int r = ) TlsFree(m_slotID);
					HK_ON_DEBUG( if (r == 0) { HK_BREAKPOINT(0xabba3652); } ) 
				}
				HK_FORCE_INLINE T getData() const
				{
					return (T)(hkUlong)TlsGetValue(m_slotID) ;
				}
				HK_FORCE_INLINE void setData(T p) 
				{
					hkUlong v = hkUlong(p);
					HK_ON_DEBUG( int r = ) TlsSetValue( m_slotID, (void*)v );
					HK_ON_DEBUG( if (r == 0) { HK_BREAKPOINT(0xabba3653); } )
				}
				unsigned long m_slotID; 
		};
		
#		define HK_THREAD_LOCAL(TYPE) hkThreadLocalData<TYPE>
#		define HK_THREAD_LOCAL_SET(var,value) var.setData(value)
#		define HK_THREAD_LOCAL_GET(var) var.getData()
#else
#	define HK_THREAD_LOCAL(TYPE) TYPE
#endif

#ifndef HK_THREAD_LOCAL_SET
#	define HK_THREAD_LOCAL_SET(var,value) var = value
#endif
#ifndef HK_THREAD_LOCAL_GET
#	define HK_THREAD_LOCAL_GET(var) var
#endif

#endif // HKBASE_HK_THREAD_LOCAL_POINTER_H

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
