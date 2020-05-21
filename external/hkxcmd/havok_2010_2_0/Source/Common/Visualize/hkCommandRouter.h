/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_COMMAND_ROUTER
#define HK_VISUALIZE_COMMAND_ROUTER

#include <Common/Base/Container/PointerMultiMap/hkPointerMultiMap.h>
class hkDisplaySerializeIStream;
class hkProcess;

/// A class to deserialize chunks of data that have come from the client to the server.
/// For example the creation and deletion of viewers.
class hkCommandRouter : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

			/// Register handlers so that the stream can be understood
			/// it is not reference counted (handler (process) is just an interface)
			/// so should persist as long as this object or be removed before deleting itself
		void registerProcess(hkProcess* handler);
		void unregisterProcess(hkProcess* handler);

			/// Consume the commands until an ACK is reached or the stream is not OK anymore
		hkBool consumeCommands(hkDisplaySerializeIStream* stream);

	protected:
		
			/// Commands to processes that understand them.
		hkPointerMultiMap< hkUint8, hkProcess* > m_commandMap;

			/// Whether we have received version information from the client or not.
		bool m_haveClientVersion;
};

#endif // HK_VISUALIZE_COMMAND_ROUTER

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
