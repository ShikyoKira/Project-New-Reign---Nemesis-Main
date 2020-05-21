/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_OBJECT_UPDATE_TRACKER_H
#define HK_SERIALIZE_OBJECT_UPDATE_TRACKER_H

/// Interface used by the versioning system.
/// The hkObjectUpdateTracker is used to track memory allocations,
/// and addition/deletion/replacement of objects when versioning
/// is performed.
class hkObjectUpdateTracker : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			//
			// Memory
			//

			/// An allocation has been made. Track it with the rest of the allocations.
		virtual void addAllocation(void* p) = 0;

			/// An chunk allocation has been made. Track it with the rest of the allocations.
		virtual void addChunk(void* p, int n, HK_MEMORY_CLASS c) = 0;

			//
			// Inter object
			//

			/// Notify that there is a pointer at fromWhere which points to the given object.
			/// The 'fromWhere' pointer is patched to point to the new object.
		virtual void objectPointedBy( void* object, void* fromWhere ) = 0;

			/// Notify that an object is to be replaced.
			/// All pointers to the old object are patched to point to the new object.
			/// If newObject has a vtable, a finish function is added.
		virtual void replaceObject( void* oldObject, void* newObject, const hkClass* newClass ) = 0;

			//
			// Finish functions
			//

			/// Notify that the object requires a finish function.
		virtual void addFinish( void* newObject, const char* className ) = 0;

			/// Remove a finish function from an object.
			/// Typically because it has been copied by the version function.
		virtual void removeFinish( void* oldObject ) = 0;
};

#endif // HK_SERIALIZE_OBJECT_UPDATE_TRACKER_H

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
