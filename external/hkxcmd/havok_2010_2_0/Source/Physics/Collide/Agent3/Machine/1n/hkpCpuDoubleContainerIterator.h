/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_DOUBLE_BATCH_ITERATOR
#define HK_DEFAULT_DOUBLE_BATCH_ITERATOR

#include <Physics/Collide/Shape/hkpShape.h>
#include <Physics/Collide/Shape/hkpShapeContainer.h>
#include <Physics/Collide/Filter/hkpCollisionFilter.h>

class hkpCpuDoubleContainerIterator
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCpuDoubleContainerIterator );

		HK_FORCE_INLINE hkpCpuDoubleContainerIterator( const HK_SHAPE_CONTAINER* containerA, const HK_SHAPE_CONTAINER* containerB, const hkpShapeKeyPair* shapeKeys );

			/// prepare iterator for being queried for getShapes and getShapeKeys
		HK_FORCE_INLINE void update();

			/// Return the current shape key.
		HK_FORCE_INLINE hkpShapeKeyPair getShapeKeyPair();

			/// Get the collision filter infos for the two child shapes.
		HK_FORCE_INLINE hkBool isCollisionEnabled( const hkpProcessCollisionInput* input, const hkpCdBody* collectionBodyA, const hkpCdBody* collectionBodyB );

			/// Set the body to have the current shape and shape key.
		HK_FORCE_INLINE void setShapes( hkpCdBody& bodyA, hkpCdBody& bodyB );

			/// Next time the update is called it should advance.
		HK_FORCE_INLINE void advance();

	public:
		hkpShapeBuffer m_shapeBufferA;

		hkpShapeBuffer m_shapeBufferB;

		const HK_SHAPE_CONTAINER* m_containerA;

		const HK_SHAPE_CONTAINER* m_containerB;

		const hkpShapeKeyPair* m_shapeKeys;

		hkpShapeKeyPair m_lastShapeKeyPair;

		hkpShapeKey m_extractedShapeKeyA;

		hkpShapeKey m_extractedShapeKeyB;

		const hkpShape* m_extractedShapeA;

		const hkpShape* m_extractedShapeB;
};

#include<Physics/Collide/Agent3/Machine/1n/hkpCpuDoubleContainerIterator.inl>

#endif // HK_DEFAULT_DOUBLE_BATCH_ITERATOR

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
