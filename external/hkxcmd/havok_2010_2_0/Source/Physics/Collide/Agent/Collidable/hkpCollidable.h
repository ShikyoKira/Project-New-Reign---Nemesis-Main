/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_COLLIDABLE_H
#define HK_COLLIDE2_COLLIDABLE_H

#include <Physics/Collide/Agent/Collidable/hkpCdBody.h>
#include <Physics/Collide/Dispatch/BroadPhase/hkpTypedBroadPhaseHandle.h>

extern const class hkClass hkpCollidableBoundingVolumeDataClass;

extern const class hkClass hkpCollidableClass;


class hkAabb;
struct hkAabbUint32;
class hkpShape;
class hkTransform;

/// An hkpCollidable can collide with other hkCollidables. It has an hkpShape and a hkMotionState (or hkTransform) for that shape.
/// It also has a void* owner pointer.
/// If you wish to make a collision query, you must create two hkpCollidable objects, and use the hkpCollisionDispatcher
/// to access the correct hkpCollisionAgent for the query.
/// hkpCollidable objects are automatically created by the hkpDynamics system, and the owner points to either the hkpEntity
/// or hkpPhantom object in the world. See these classes for further information.
class hkpCollidable : public hkpCdBody
{



	public:
		// +version(2)

		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpCollidable);

			/// Creates a new hkpCollidable.
			/// Note: this constructor sets the owner to be HK_NULL, call setOwner to specify the owner and type
			/// Note: this collidable can't be used for normal collision detection (processCollision)
		inline hkpCollidable( const hkpShape* shape, const hkTransform* ms, int type = 0);

			/// Creates a new hkpCollidable.
			/// Note: this constructor sets the owner to be HK_NULL, call setOwner to specify the owner and type
		inline hkpCollidable( const hkpShape* shape, const hkMotionState* ms, int type = 0);


		inline ~hkpCollidable();

			/// Gets the hkpShape.
		inline const hkpShape* getShape() const;

			/// Sets the hkpShape. Note: you should be careful about setting the shape of a hkpCollidable at runtime, as if there
			/// are any collision agents that depend on it they will no longer work and crash
		inline void setShape(const hkpShape* shape);

			/// Sets the entity that owns this hkpCollidable.
			/// and the type, see hkpWorldObject::BroadPhaseType for possible types.
		inline void setOwner( void* owner );

			/// Gets the entity that owns this hkpCollidable.
		inline void* getOwner() const
		{
	        return const_cast<char*>( reinterpret_cast<const char*>(this) + m_ownerOffset ); 
	    }

		hkBool checkPerformance();

		//
		// TypedBroadPhase handle i/f:
		//

			/// Gets a const pointer to the embedded hkpTypedBroadPhaseHandle.
		inline const hkpTypedBroadPhaseHandle* getBroadPhaseHandle() const;

			/// Gets a non-const pointer to the embedded hkpTypedBroadPhaseHandle.
		inline       hkpTypedBroadPhaseHandle* getBroadPhaseHandle();


			/// Gets the hkpCollidableQualityType.
		HK_FORCE_INLINE hkpCollidableQualityType getQualityType() const;

			/// Sets the quality type of this collidable.
			///
			/// Note: Having two non-continuously colliding dynamic objects and fixing one of them
			/// doesn't result in continuous collision detection between the objects
			/// if the agent is already created. The current agent will be replaced
			/// by its continuous version only after the bodies separate and lose their broadphase
			/// overlap (when their agent is destroyed) and then come into proximity again
			/// (creating a new agent of type conforming to their current qualityType settings).
		HK_FORCE_INLINE void setQualityType(hkpCollidableQualityType type);

			/// Gets the current allowed penetration depth.
			/// This is a hint to the continuous physics to allow some penetration for this object
			/// to reduce CPU load. Note: this is not a hard limit but more a guideline to the engine.
			/// Depending on the qualityType, this allowed penetration can be breached sooner or later.
			/// See user guide on continuous physics for details.
		HK_FORCE_INLINE hkReal getAllowedPenetrationDepth() const;

			/// Sets the current allowed penetration depth. See getAllowedPenetrationDepth for details.
		HK_FORCE_INLINE void setAllowedPenetrationDepth( hkReal val );


	public:
			///Gets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
		HK_FORCE_INLINE hkUint32 getCollisionFilterInfo() const;

			/// Sets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
		HK_FORCE_INLINE void setCollisionFilterInfo( hkUint32 info );


		hkpCollidable( class hkFinishLoadedObjectFlag flag );



	public:

		///Gets the type. The possible types are defined in hkpWorldObject::BroadPhaseType.
		HK_FORCE_INLINE int getType() const;

	public:

		struct BoundingVolumeData
		{
				HK_DECLARE_REFLECTION();
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpCollidable::BoundingVolumeData );

				BoundingVolumeData();
				
				BoundingVolumeData( class hkFinishLoadedObjectFlag flag );

				HK_FORCE_INLINE bool hasAllocations() const ;
				void deallocate();
				void allocate(int numChildShapes);

				HK_FORCE_INLINE void invalidate() { m_min[0] = 1; m_max[0] = 0; }
				HK_FORCE_INLINE bool isValid() const { return m_min[0] <= m_max[0]; }

			public:
				hkUint32		m_min[3];
				hkUint8			m_expansionMin[3];
				hkUint8			m_expansionShift;
				hkUint32		m_max[3];
				hkUint8			m_expansionMax[3];
				hkUint8			m_padding;
				hkUint16		m_numChildShapeAabbs; //+nosave
				hkUint16		m_capacityChildShapeAabbs; //+nosave
				hkAabbUint32*	m_childShapeAabbs; //+nosave
				hkpShapeKey*	m_childShapeKeys; //+nosave
		};


		enum ForceCollideOntoPpuReasons
		{
			/// Requested by hkpRigidBodyCinfo::m_forceCollideOntoPpu.
			FORCE_PPU_USER_REQUEST = 1,	

			/// Requested by the shape, e.g., hkHeightFieldShapes cannot go to the SPU. See the PlayStation(R)3-specific documentation for what shapes are supported on SPU.
			FORCE_PPU_SHAPE_REQUEST = 2,

			/// Some constraint modifiers require getting collision callbacks on the PPU, therefore forcing the collision detection for the modified objects to stay on the PPU.
			FORCE_PPU_MODIFIER_REQUEST = 4,

			/// Default value of flag.  Means that hkpCollidable's shape hasn't been checked if it can go on the SPU.
			FORCE_PPU_SHAPE_UNCHECKED = 8
		};

	protected:

			/// This is used by hkDynamics to point to the hkpRigidBody.
			/// Check the type before doing casts.
			/// Note: You can use hkpGetRigidBody(hkpCollidable*) to get a type checked owner
			/// (defined in file <hkdynamics/entity/hkpRigidBody.h> )
			/// It is an offset from 'this' to the owner as it assumes that if
			/// a collidable has an owner, the collidable is a member of that owner.
			/// Stored as an offset so that the serialization can handle it 'as is'.
		hkInt8 m_ownerOffset; // +nosave

	public:

			/// PlayStation(R)3 only: If this flag is non zero, all collision agents attached to this body will run on the PPU only
			/// This is a bitfield using ForceCollideOntoPpuReasons.
		hkUint8   m_forceCollideOntoPpu;

			// The shape size to be downloaded to the spu, must be a multiple of 16
		hkUint16 m_shapeSizeOnSpu;			//+nosave

		class hkpTypedBroadPhaseHandle m_broadPhaseHandle;

	public:

		struct BoundingVolumeData m_boundingVolumeData; //+nosave

			// Should be set to the allowed penetration depth
		hkReal m_allowedPenetrationDepth;
};


#	include <Physics/Collide/Agent/Collidable/hkpCollidable.inl>

#endif // HK_COLLIDE2_COLLIDABLE_H

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
