/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_MEMORY_SNAPSHOT_VIEWER_H
#define HK_UTILITIES2_MEMORY_SNAPSHOT_VIEWER_H

#include <Common/Visualize/hkProcess.h>
#include <Common/Visualize/hkProcessContext.h>

/// Sends a memory snapshot to the VDB clients.
/// Does not a consume command, but rather sends the packet when stepped for the
/// first time and then deletes itself.
class hkMemorySnapshotProcess : public hkReferencedObject, public hkProcess
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

			/// Creates a hkMemorySnapshotProcess.
		static hkProcess* HK_CALL create( const hkArray<hkProcessContext*>& contexts );
		
			/// Registers the hkMemorySnapshotProcess with the hkViewerFactory.
		static void HK_CALL registerProcess();

		static inline const char* HK_CALL getName() { return "* Grab Memory Snapshot"; }

		// hkProcess implementation.
		virtual int getProcessTag() { return m_tag; }
		virtual void step( hkReal frameTimeInMs );

	protected:

		hkMemorySnapshotProcess( const hkArray<hkProcessContext*>& contexts );

	protected:
			
		static int m_tag;
};

#endif	// HK_UTILITIES2_MEMORY_SNAPSHOT_VIEWER_H

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
