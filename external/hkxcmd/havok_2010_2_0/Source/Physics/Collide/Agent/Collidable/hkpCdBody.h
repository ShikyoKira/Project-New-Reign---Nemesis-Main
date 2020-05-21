/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CD_BODY_H
#define HK_COLLIDE2_CD_BODY_H

#include <Physics/Collide/Shape/hkpShape.h>

extern const class hkClass hkpCdBodyClass;


class hkpShape;
class hkMotionState;
class hkpCollidable;
	
	/// The hkpCdBody class is a helper class, which is used to traverse
	/// the shape hierarchy. It is used for all narrowphase collision detection queries.
	/// It contains a shape, and an associated transform. Given two hkpCdBody classes, you
	/// can query for the closest distance using the hkCollisionAgentInterface.
	/// Note: You do not instantiate this class directly. Use the hkpCollidable, which inherits from
	/// hkpCdBody instead.
	///
	/// This class is also created temporarily by the collision detector during queries.
	/// For example, when querying a shape against a landscape, hkpCdBody classes are created
	/// for each sub-shape in the landscape. When hkpCdBody classes are referenced in callbacks,
	/// their data may only be temporary, and you should not hold references to them.
	///
	/// This class holds either a pointer to a transform or a motionState.
	/// The motion state pointer is used only for the internal processCollision call hierarchy
	/// The transform pointer is used for all other collision detection queries
class hkpCdBody
{
	//+version(1)

	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCdBody);	

			// ( CdBody is auto reflected, but the XML has some extra settings in it too. )
		HK_DECLARE_REFLECTION();

		// Empty constructor; handle with care;
		HK_FORCE_INLINE hkpCdBody(  ){}

		friend class hkpCollidable;

		HK_FORCE_INLINE hkpCdBody( const hkpShape* shape, const hkMotionState* motionState );

		HK_FORCE_INLINE hkpCdBody( const hkpShape* shape, const hkTransform* t );

		HK_FORCE_INLINE hkpCdBody( const hkpCdBody& body ){}	// private copy constructor


			///  Get the transform for the current shape.
			///  Note: this transform might be temporary, do not store pointers to this transform
		HK_FORCE_INLINE const hkTransform& getTransform() const;
		
			/// Get the current shape
			/// Note: this pointer might be temporary, do not store pointers to this
		HK_FORCE_INLINE const hkpShape* getShape() const;

			/// Get the root collidable. This is the root of the hkpCdBody tree.
			/// You can use this call in a callback to get back the collidable which was used to originate the collision query
			/// Note: collidables are persistent.
		HK_FORCE_INLINE const hkpCollidable* getRootCollidable() const;

			/// Returns the shape key of the current shape with respect to the parent shape.
			/// I.e. if it is not HK_INVALID_SHAPE_KEY then the hkpCdBody's parent implements hkpShapeContainer and
			/// this->getParent->getContainer()->getChildShape(key) will return the same shape as
			/// this->getShape() (possibly at a different address for temporary shapes).
		HK_FORCE_INLINE hkpShapeKey getShapeKey() const ;

			/// Returns the shape key of the topmost child (non-root) in the hierarchy.
		HK_FORCE_INLINE hkpShapeKey getTopLevelShapeKey() const;
	
			/// Return the parent hkpCdBody
		HK_FORCE_INLINE const hkpCdBody* getParent() const;


		//
		//	Internal public section
		//
	public:
			// Constructor which copies parent and motionstate, does not set m_shape and m_shapeKey values
			// This function should only be called internally by a collision agent
		explicit HK_FORCE_INLINE hkpCdBody( const hkpCdBody* parent );


			// This constructor is used by collision agents to create temporary hkpCdBody objects
			// This function should only be called internally by a collision agent
		HK_FORCE_INLINE hkpCdBody( const hkpCdBody* parent, const hkMotionState* ms );


			// This constructor is used by collision agents to create temporary hkpCdBody objects
			// This function should only be called internally by a collision agent
		HK_FORCE_INLINE hkpCdBody( const hkpCdBody* parent, const hkTransform* t );

			// sets the shape and shapeKey 
			// IMPORTANT: Do not call this function directly if this collidable is in the physics world, 
			// because it is unsafe.
			// Use hkpRigidBody or hkpPhantom setShape() methods instead.
			// This call is only allowed if m_parent->getShape() implements hkpShapeContainer.
			// This function should only be called internally by a collision agent.
		HK_FORCE_INLINE void setShape( const hkpShape* shape, hkpShapeKey key );

			// Gets the motion state
		HK_FORCE_INLINE const hkMotionState* getMotionState() const;

			// Sets the motion state
		HK_FORCE_INLINE void setMotionState( const hkMotionState* state );

			// Sets the transform
		HK_FORCE_INLINE void setTransform( const hkTransform* t );


	public:

		const hkpShape*       m_shape;
		hkpShapeKey           m_shapeKey; //+overridetype(hkUint32)

			// This is either a hkTransform, or a hkMotionState, and is only accessible through the public get and set methods.
			// This is not serialized, but set upon addition to world etc., as sometimes
			// it is just an offset to a transform (offsets == int == could be serialized) 
			// but sometimes it is a hkMotionState that needs to be a typed ptr, so the two
			// can't coexist in a serialization friendly way. 
		const void* m_motion; //+nosave

		const hkpCdBody*		 m_parent; // +serialized(false)
};


#	include <Physics/Collide/Agent/Collidable/hkpCdBody.inl>

#endif // HK_COLLIDE2_CD_BODY_H

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
