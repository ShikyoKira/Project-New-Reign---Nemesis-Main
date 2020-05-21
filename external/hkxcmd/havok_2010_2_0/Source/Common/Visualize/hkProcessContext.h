/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_PROCESS_CONTEXT_H
#define HK_VISUALIZE_PROCESS_CONTEXT_H

class hkVisualDebugger;
class hkJobThreadPool;

	/// hkProcessContext provides a way to get information to Visual Debugger processes e.g., viewers, in a
	/// non physics-specific way. Note that the hkProcessContext itself is not Havok memory managed. It is just
	/// an interface so that you can use your own container classes for whatever data you want to expose to
	/// processes you create and use with the Visual Debugger.
class hkProcessContext
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkProcessContext);

		hkProcessContext() : m_owner(HK_NULL) { }

		virtual ~hkProcessContext() {}

			/// Get the type of context. Common ones are 'Physics' or 'Animation'. Leave it open as a generic
			/// string so that users can make their own contexts for Processes.
		virtual const char* getType() = 0; 

			/// Set the owner.
		virtual void setOwner(hkVisualDebugger* vdb) { m_owner = vdb; }

			/// This function must be called once per frame
			/// It prepares timer data for sending to the VDB. If you are using a thread pool you can optionally pass it in here.
		void syncTimers( hkJobThreadPool* threadPool = HK_NULL );

			/// Add timers from an additional thread pool
		void addThreadPoolTimers( hkJobThreadPool* threadPool );

	public:

			/// hkProcessContext owner.
		hkVisualDebugger* m_owner;

			/// Used by the stats viewer, the current start and end of the threads being timed.
			/// If they are empty arrays, will use the static hkMonitorStream::begin() and
			/// hkMonitorStream::end() pointers directly instead.
		hkInplaceArray<const char*, 6> m_monitorStreamBegins;
			/// See m_monitorStreamBegins comment.
		hkInplaceArray<const char*, 6> m_monitorStreamEnds;
};

#endif // HK_VISUALIZE_PROCESS_CONTEXT_H

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
