/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DATA_OBJECT_UTIL_H
#define HK_DATA_OBJECT_UTIL_H

#include <Common/Serialize/Data/hkDataObject.h>
#include <Common/Base/Reflection/hkClassMember.h>

class hkResource;
class hkObjectResource;
class hkClassNameRegistry;
class hkTypeInfoRegistry;

	/// Utilities for copying between hkDataObject and native types.
namespace hkDataObjectUtil
{
		/// Recursively copy the object into an hkResource.
		/// This function calls hkDataObjectUtil::toResourceWithRegistry using hkBuiltinTypeRegistry.
	hkResource* HK_CALL toResource( const hkDataObject& src);

		/// Recursively copy the object into an hkResource using classes from the supplied
		/// 'classReg' registry. If class is not found then the object is not copied.
		/// Usually you should use hkDataObjectUtil::toResource() instead of this function.
	hkResource* HK_CALL toResourceWithRegistry(const hkDataObject& src, const hkClassNameRegistry* classReg);

		/// Recursively copy the object into an hkObjectResource.
		/// This function calls hkDataObjectUtil::toObjectWithRegistry() using hkBuiltinTypeRegistry.
	hkObjectResource* HK_CALL toObject( const hkDataObject& src);

		/// Recursively copy the object into an hkObjectResource using classes and type infos from the supplied
		/// 'classReg' and 'typeReg' registries correspondingly.
		/// Note: If class is not found or class has a member of the c-string, array of c-strings, hkVariant,
		/// array of hkVariants, simple array or homogeneous array then the object is not copied and
		/// this function returns HK_NULL.
		/// Usually you should use hkDataObjectUtil::toObject() instead of this function.
	hkObjectResource* HK_CALL toObjectWithRegistry(const hkDataObject& src, const hkClassNameRegistry* classReg, const hkTypeInfoRegistry* typeReg);

		/// Convert hkClassMember::Type to hkDataObject::Type.
	hkDataObject::Type HK_CALL getTypeFromMemberType(hkClassMember::Type mtype, hkClassMember::Type stype, const hkClass* klass, int count);

		/// Copy the contents of one world into another.
	hkResult HK_CALL deepCopyWorld( hkDataWorld& dst, const hkDataWorld& src );

		/// Calculate the extra storage (above the normal hkClass size) required to create
		/// a native version of the given object. Normally this is 0
	int HK_CALL getExtraStorageSize(const hkClass* klass, const hkDataObject& obj);
}

#endif // HK_DATA_OBJECT_UTIL_H

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
