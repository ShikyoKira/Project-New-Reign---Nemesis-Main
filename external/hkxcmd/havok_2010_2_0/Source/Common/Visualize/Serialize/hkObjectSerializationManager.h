/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_OBJECT_SERIALIZATION_MANAGER_H
#define HK_OBJECT_SERIALIZATION_MANAGER_H

#include <Common/Base/Reflection/hkClassMember.h>
#include <Common/Visualize/Serialize/hkObjectSerialize.h>
#include <Common/Visualize/Serialize/hkObjectSerializeRegistry.h>

	/// Maintains a set of objects that are being read from a stream. Nested objects may not necessarily be
	/// present, in which case you can request them.
class hkObjectSerializationManager
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkObjectSerializationManager);
		hkObjectSerializationManager( hkStreamWriter* streamWriter );
		~hkObjectSerializationManager();

			/// Set the ID of the hkClass that describes itself on the other machine.
		void setClassClassId( hkUint64 classId ) { m_classClassId = classId; }

			/// Get the ID of the hkClass that describes itself on the other machine.
		hkUint64 getClassClassId() const { return m_classClassId; }

		void setSpecialClassClassIds(hkArray<hkUint64>& specialClassClassIds) { m_dataReg.setSpecialClassClassIds(specialClassClassIds); }

			/// Sets the target rules for the other machine.
		void setTargetRules(const hkStructureLayout::LayoutRules& rules );

			/// read an object from the stream (and pass the data on to the callbacks)
			/// returns the object read.
		void readObject( hkStreamReader* s, void*& data, hkUint64& dataId, hkUint64& klassId );

			/// If get object returns HK_NULL, you can request it (if network connection is up)
		void* getObject( hkUint64 id ) { return m_dataReg.getObjectData(id); }

			/// Send an updated data packet. Will send in the target layout. Assumes the new data is in local layout.
		void sendUpdatedMember(  hkUint64 id, const hkArray<hkInt16>& memberPath, hkClassMember::Type type, const void* ptr );

			/// If get object returns HK_NULL, you can request it (if network connection is up)
		hkUint64 getObjectType( hkUint64 objectId ) { return m_ptrToTypeMap.getWithDefault(objectId, 0); }

			/// Request the data for a given object. Callback will be raised on read.
		void requestObject( hkUint64 id, bool recur, bool autoUpdate, bool forceRequest); // class or data ptr. This ptr must have been seen in a read already or else its class will not be known.

			/// Throw away an object and deallocate the memory.
		void discardObject( hkUint64 id );

			/// Make a given object 'live', i.e., chase down all global references and make them live too
			/// It will return false if it could not resolve all the references (if it has
			/// not seen those object yet). It will only do an object at most once. Normally
			/// class data is made live to aid in walking the members.
		bool makeLive( hkUint64 ptrId, bool forceUpdate = false );
		bool isLive( hkUint64 ptrId );

			/// reverse lookup (slow).
		hkUint64 getID( const void* data );

			/// internal really:
		void addDataType( hkUint64 dataId, hkUint64 classId, hkBool overwrite );

	protected:

		hkStreamWriter* m_streamWriter;

			/// the type that represents a hkClass type itself on the server (sent at the start)
		hkUint64 m_classClassId;
		
			/// The layout reported by the target (server) so that we can send back data packets
			/// in its own format to it.
		hkStructureLayout m_targetLayout;
		
		typedef hkPointerMap< hkUint64, hkUint64 >::Iterator  TypeMapIterator;
		hkPointerMap< hkUint64, hkUint64 > m_ptrToTypeMap;
		hkObjectSerializeRegistry m_dataReg; // dataPtr -> void*, managed

		typedef hkPointerMap< hkUint64, hkUint64 >::Iterator  PendingReqIterator;
		hkPointerMap< hkUint64, int> m_pendingRequests;

		hkPointerMap< hkUint64, hkArray<hkObjectSerialize::GlobalFixup>*> m_idToFixups;
		hkArray< hkUint64> m_liveIds;
};

#endif

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
