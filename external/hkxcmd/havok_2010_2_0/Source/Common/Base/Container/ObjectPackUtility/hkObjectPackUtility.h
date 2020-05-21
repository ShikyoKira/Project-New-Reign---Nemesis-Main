/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PACKOBJECT_H
#define HK_PACKOBJECT_H


template <class ContainingObject>
class hkObjectPackUtility
{
	public:
			/// Create an object packer for the given object. Note that members are memcpy()'ed,
			/// Make sure to call addReference() for any contained hkReferencedObjects and to
			/// manage any shared memory appropriately.
			///
			/// totalExtraSize is the total extra size to allocate. Each arrays is padded out to
			/// 16 bytes, this padding must be taken into account in this size
		hkObjectPackUtility(ContainingObject& object);

			/// Add an array into the packed memory area. The memory allocated must be sufficient to
			/// store it
		template<typename ArrayType>
		hkResult addPackedArray(const hkArray<ArrayType>& array);

			/// Mark array data as not owned. Whichever object does own the data must
			/// keep it allocated for the lifetime of the packed object
		template<typename ArrayType>
		hkResult addArrayNotOwned(const hkArray<ArrayType>& array);
		
			/// Steal the data from the original object into the copied
			/// object. The array in the original object is emptied
		template<typename ArrayType>
		hkResult stealArrayContents(hkArray<ArrayType>& array);

			/// Return the allocated object. The old object is memcpy()'ed to the new -- reference counts etc,
			/// are not treated specially
		ContainingObject* getFinishedObject();

			/// Is this member already packed?
		template<typename ArrayType>
		bool isAlreadyPacked(hkArray<ArrayType>& array);

	private:

		struct ArrayMember
		{
			void* m_pointer;
			int m_objectSize;
		};
		
		hkArray<ArrayMember> m_arrayMembers;
		ContainingObject* m_copyFromObject;
		ContainingObject* m_newObject;
};


/// Rebase the pointers in a packed object. This is only necessary if the object has been
/// memcpy()'ed or DMAd (e.g., to an SPU)
class hkObjectUnpackUtility
{
	public:
			/// Unpack an object that has been packed. totalExtraSize is for checking only
		HK_FORCE_INLINE hkObjectUnpackUtility(void* object, void* oldPointer, int totalExtraSize = 0xFFFF);

			/// Patch an array member. Members must be patched in the same order in which
			/// they were added.
		template<typename ArrayType>
		HK_FORCE_INLINE hkResult patchArray(hkArray<ArrayType>& array);

	private:
		void* m_newObject;
		hkUlong m_oldPointer;
};

#include <Common/Base/Container/ObjectPackUtility/hkObjectPackUtility.inl>

#endif // HK_PACKOBJECT_H

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
