/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_SERIALIZE_DISPLAYRBTRANS_H
#define HK_UTILITIES2_SERIALIZE_DISPLAYRBTRANS_H

#include <Physics/Dynamics/Entity/hkpRigidBody.h>

extern const class hkClass hkpSerializedDisplayRbTransformsDisplayTransformPairClass;

extern const class hkClass hkpSerializedDisplayRbTransformsClass;

/// This is a handy, simple, serializable that stores a
/// list of rigidbodies and their original display transform.
/// This transform info can be lost when transforms are collapsed
/// in Havok2 tools for optimization of collision detection. This
/// change that happens during the collapse is stored here if desired.
/// Typically used by the hke2xml converter etc.
class hkpSerializedDisplayRbTransforms : public hkReferencedObject
{
public:

	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_UTILITIES);
	HK_DECLARE_REFLECTION();

	inline hkpSerializedDisplayRbTransforms();
	
	virtual ~hkpSerializedDisplayRbTransforms();

		/// Get the number of bodies mapped by this class.
	inline int getNumPairs() const;
	
		/// Returns -1 if not present.
	inline int findBodyIndex(const hkpRigidBody* b) const;

		/// Make sure that pi is with range.
	inline hkTransform& getTransform(int pi);

		/// Make sure that pi is with range.
	inline hkpRigidBody* getBody(int pi);

		/// Will reference count rb
	inline void addPair( hkpRigidBody* rb, const hkTransform& t);

public:
	
	struct DisplayTransformPair
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpSerializedDisplayRbTransforms::DisplayTransformPair );
		HK_DECLARE_REFLECTION();

		hkpRigidBody* m_rb;
		hkTransform  m_localToDisplay; // difference between the shifted RigidBody (due to collapse transforms perhaps) to the original (display) transform.
	};

	hkArray<struct DisplayTransformPair> m_transforms;
	
	hkpSerializedDisplayRbTransforms(hkFinishLoadedObjectFlag flag) :
		hkReferencedObject(flag), m_transforms(flag) { }
};

#include <Physics/Utilities/Serialize/Display/hkpSerializedDisplayRbTransforms.inl>

#endif //HK_UTILITIES2_SERIALIZE_DISPLAYRBTRANS_H

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
