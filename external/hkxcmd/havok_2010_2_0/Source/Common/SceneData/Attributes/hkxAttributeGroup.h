/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCENEDATA_HKX_ATTRIBUTE_GROUP_H
#define HK_SCENEDATA_HKX_ATTRIBUTE_GROUP_H

#include <Common/SceneData/Attributes/hkxAttribute.h>

#include <Common/SceneData/Attributes/hkxSparselyAnimatedBool.h>
#include <Common/SceneData/Attributes/hkxSparselyAnimatedEnum.h>
#include <Common/SceneData/Attributes/hkxSparselyAnimatedInt.h>
#include <Common/SceneData/Attributes/hkxSparselyAnimatedString.h>
#include <Common/SceneData/Attributes/hkxAnimatedFloat.h>
#include <Common/SceneData/Attributes/hkxAnimatedVector.h>
#include <Common/SceneData/Attributes/hkxAnimatedQuaternion.h>
#include <Common/SceneData/Attributes/hkxAnimatedMatrix.h>

/// hkxAttributeGroup meta information
extern const class hkClass hkxAttributeGroupClass;

	/// Attribute groups are associated to hkxNodes and contain an array of hkxAttribute under a common heading.
	/// They usually represent a set of data associated with a node as a unit. For example, rigid body data.
struct hkxAttributeGroup
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkxAttributeGroup );
	HK_DECLARE_REFLECTION();

	hkxAttributeGroup() {}
	hkxAttributeGroup(hkFinishLoadedObjectFlag f) : m_name(f), m_attributes(f) {}
	hkxAttributeGroup( const hkxAttributeGroup& other ) { *this = other; }

	hkxAttributeGroup& operator=( const hkxAttributeGroup& other );

		/// The name of the attribute group - for example "hkpRigidBody".
	hkStringPtr m_name;

		/// The array of individual attributes in the group.
	hkArray<struct hkxAttribute> m_attributes;

		
	/*
	** Get attribute value. Assumes non-animated attributes (takes first value)
	** Use find...() methods to get full access to attribute animation
	*/

		/// Retrieve the bool value of an attribute (case insensitive). Returns HK_FAILURE if not found (supports Bool, Int).
	hkResult getBoolValue (const char* name, bool warnIfNotFound, hkBool& boolOut) const;

		/// Retrieve the int value of an attribute (case insensitive). Returns HK_FAILURE if not found (supports Int, Enum, Bool).
	hkResult getIntValue (const char* name, bool warnIfNotFound, int& intOut) const;

		/// Retrieve the (unsigned) int value of an attribute (case insensitive). Returns HK_FAILURE if not found (supports Int, Enum, Bool).
	hkResult getIntValue (const char* name, bool warnIfNotFound, hkUint32& intOut) const;

		/// Retrieve the string value an attribute (case insensitive). Returns HK_FAILURE if not found (supports String, Enum).
	hkResult getStringValue (const char* name, bool warnIfNotFound, const char*& stringOut) const;

		/// Retrieve the value of a float attribute (case insensitive). Returns HK_FAILURE if not found (or not the right type).
	hkResult getFloatValue (const char* name, bool warnIfNotFound, float& floatOut) const;

		/// Retrieve the value of a vector attribute (case insensitive). Returns HK_FAILURE if not found (or not the right type).
	hkResult getVectorValue (const char* name, bool warnIfNotFound, hkVector4& vectorOut) const;

		/// Retrieve the value of a quaternion attribute (case insensitive). Returns HK_FAILURE if not found (or not the right type).
	hkResult getQuaternionValue (const char* name, bool warnIfNotFound, hkQuaternion& quaternionOut) const;

		/// Retrieve the value of a matrix attribute (case insensitive). Returns HK_FAILURE if not found (or not the right type).
	hkResult getMatrixValue (const char* name, bool warnIfNotFound, hkMatrix4& matrixOut) const;

	/*
	** Search for attribute by name and type
	*/

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxSparselyAnimatedBool* findBoolAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxSparselyAnimatedInt* findIntAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxSparselyAnimatedEnum* findEnumAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxSparselyAnimatedString* findStringAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxAnimatedFloat* findFloatAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxAnimatedVector* findVectorAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxAnimatedQuaternion* findQuaternionAttributeByName (const char* name) const;

		/// Utility method (case insensitive), returns NULL if attribute wasn't found or it's the wrong type.
	hkxAnimatedMatrix* findMatrixAttributeByName (const char* name) const;

	/*
	** Generic attribute search
	*/

		/// Finds an attribute by name (case insensitive), returns index.
	int findAttributeIndexByName ( const char* name ) const;

		/// Finds an attribute by name (case insensitive), returns variant.
	hkRefVariant findAttributeVariantByName ( const char* name ) const;

		/// Finds an attribute by name (case insensitive), returns object (value). Can (optionally) check the type.
	hkReferencedObject* findAttributeObjectByName( const char* name, const hkClass* type = HK_NULL ) const;

};


#endif //HK_SCENEDATA_HKX_ATTRIBUTE_GROUP_H

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
