/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#include <Common/Base/hkBase.h>
#include <Common/Base/Container/ObjectPackUtility/hkObjectPackUtility.h>


template <class ContainingObject>
hkObjectPackUtility<ContainingObject>::hkObjectPackUtility(ContainingObject& object) : m_copyFromObject(&object), m_newObject(HK_NULL)
{
	HK_ASSERT2(0x2be1f83a, &object, "hkObjectPackUtility object is NULL");
}


template <class ContainingObject>
template<typename ArrayType>
hkResult hkObjectPackUtility<ContainingObject>::addPackedArray(const hkArray<ArrayType>& array)
{
	ArrayMember localMember;
	localMember.m_pointer = (void*)&array;
	localMember.m_objectSize = hkSizeOf(ArrayType);
	m_arrayMembers.pushBack(localMember);
	return HK_SUCCESS;
}

template <class ContainingObject>
template<typename ArrayType>
hkResult hkObjectPackUtility<ContainingObject>::addArrayNotOwned(const hkArray<ArrayType>& array)
{
	HK_ASSERT2(0x47be0cf1, m_newObject, "markArrayNotOwned can only be called after getFinishedObject");

	const int arrayOffset = reinterpret_cast<const char*>(&array) - reinterpret_cast<const char*>(m_copyFromObject);
	hkArray<ArrayType>& newArray = *reinterpret_cast<hkArray<ArrayType>*>(reinterpret_cast<char *>(m_newObject) + arrayOffset);

	newArray._setDataUnchecked(newArray.begin(), newArray.getSize(), newArray.getCapacityAndFlags() | array.DONT_DEALLOCATE_FLAG);
	return HK_SUCCESS;
}

template <class ContainingObject>
template<typename ArrayType>
hkResult hkObjectPackUtility<ContainingObject>::stealArrayContents(hkArray<ArrayType>& array)
{
	HK_ASSERT2(0x227d92d7, m_newObject, "stealArrayContents can only be called after getFinishedObject");
	const int arrayOffset = int(reinterpret_cast<const char*>(&array) - reinterpret_cast<const char*>(m_copyFromObject));
	hkArray<ArrayType>& newArray = *reinterpret_cast<hkArray<ArrayType>*>(reinterpret_cast<char *>(m_newObject) + arrayOffset);

	if(array.getCapacityAndFlags() & array.DONT_DEALLOCATE_FLAG)
	{
		// Do a simple copy
		newArray = array;
	}
	else
	{
		newArray._setDataUnchecked(HK_NULL, 0, 0);
		newArray.swap(array);
	}
	return HK_SUCCESS;
}
template <class ContainingObject>
template<typename ArrayType>
bool hkObjectPackUtility<ContainingObject>::isAlreadyPacked(hkArray<ArrayType>& array)
{
	const int arrayOffset = int(reinterpret_cast<const char*>(array.begin()) - reinterpret_cast<const char*>(m_copyFromObject));
	return (arrayOffset > 0) && (arrayOffset < m_copyFromObject->getAllocatedSize());
}

template <class ContainingObject>
ContainingObject* hkObjectPackUtility<ContainingObject>::getFinishedObject()
{
	HK_ASSERT2(0x25b7f32f, m_newObject == HK_NULL, "getFinishedObject() can only be called once");
	int totalExtraSize = 0;
	for(int i=0;i<m_arrayMembers.getSize();i++)
	{
		hkArray<char>* array = (hkArray<char>*)m_arrayMembers[i].m_pointer;
		totalExtraSize += HK_NEXT_MULTIPLE_OF(16, m_arrayMembers[i].m_objectSize * array->getSize());
	}

	const int allocatedSize = HK_NEXT_MULTIPLE_OF(16, hkSizeOf(ContainingObject)) + totalExtraSize;
	HK_ASSERT2(0x1fac4447, (allocatedSize >= 0) && (allocatedSize <= 65536), "Invalid size");
	m_newObject = reinterpret_cast<ContainingObject*>(hkMemHeapBlockAlloc<char>(allocatedSize));
	// Need to set up vtable
	
	new(m_newObject) ContainingObject();

	
	
	
	hkString::memCpy(m_newObject, m_copyFromObject, hkSizeOf(ContainingObject));
	
	m_newObject->m_memSizeAndFlags = hkUint16(allocatedSize);
	m_newObject->m_referenceCount = 1;
	HK_MEMORY_TRACKER_NEW_SIZED_REFOBJECT(ContainingObject, m_newObject, allocatedSize);

	//m_offsetSoFar(0), m_totalExtraSize(HK_NEXT_MULTIPLE_OF(16, totalExtraSize))
	// COPY
	int offsetSoFar = 0;
	for(int i=0;i<m_arrayMembers.getSize();i++)
	{
		hkArray<char>* array = (hkArray<char>*)m_arrayMembers[i].m_pointer;
		HK_ASSERT2(0x3cd21013, (offsetSoFar >= 0) && ((offsetSoFar & 0x0f) == 0), "Invalid internal offset");
		const int sizeToCopy = m_arrayMembers[i].m_objectSize * array->getSize();
		HK_ASSERT2(0x191060bb, (offsetSoFar + sizeToCopy) < 65536, "Array too large or invalid offset");
		hkString::memCpy(reinterpret_cast<char*>(m_newObject) + HK_NEXT_MULTIPLE_OF(16, hkSizeOf(ContainingObject)) + offsetSoFar, array->begin(), sizeToCopy);

		const int arrayOffset = int(reinterpret_cast<const char*>(array) - reinterpret_cast<const char*>(m_copyFromObject));
		hkArray<char>& newArray = *reinterpret_cast<hkArray<char>*>(reinterpret_cast<char *>(m_newObject) + arrayOffset);

		HK_ASSERT2(0x27fec3e6, (offsetSoFar & 0x0f) == 0, "Invalid internal offset");
		newArray._setDataUnchecked(reinterpret_cast<char*>(m_newObject) + hkSizeOf(ContainingObject) + offsetSoFar, array->getSize(), array->getCapacityAndFlags() | array->DONT_DEALLOCATE_FLAG);

		offsetSoFar = HK_NEXT_MULTIPLE_OF(16, offsetSoFar + sizeToCopy);
		HK_ASSERT2(0x5752e30c, offsetSoFar <= totalExtraSize, "Array overflows allocated object size");
	}

	// FINISH
	return m_newObject;
}


hkObjectUnpackUtility::hkObjectUnpackUtility(void* object, void* oldPointer, int totalExtraSize) :
		m_newObject(object), m_oldPointer(hkUlong(oldPointer))
{
}

template<typename ArrayType>
hkResult hkObjectUnpackUtility::patchArray(hkArray<ArrayType>& array)
{
	int arrayOffset = reinterpret_cast<hkUlong>(array.begin()) - m_oldPointer;
	HK_ASSERT2(0x404702e2, (arrayOffset >= 0) && (arrayOffset <= 0xffff), "Invalid array for unpack");
	array._setDataUnchecked(reinterpret_cast<ArrayType*>((char*)m_newObject + arrayOffset), array.getSize(), array.getCapacity() | array.DONT_DEALLOCATE_FLAG);
	HK_ASSERT2(0x171cafac, (hkUlong((char*)m_newObject + arrayOffset) & 0x0f) == 0, "Incorrect alignment");
	return HK_SUCCESS;
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
