/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TYPED_BROAD_PHASE_HANDLE
#define HK_TYPED_BROAD_PHASE_HANDLE

#include <Physics/Collide/BroadPhase/hkpBroadPhaseHandle.h>
#include <Physics/Collide/Agent/Collidable/hkpCollidableQualityType.h>

extern const class hkClass hkpTypedBroadPhaseHandleClass;

/// An hkpBroadPhaseHandle with a type. hkpCollidable has a member of type hkpTypedBroadPhaseHandle.
/// If you use the Havok dynamics lib then
/// type can be hkpWorldObject::BROAD_PHASE_ENTITY for entities or hkpWorldObject::BROAD_PHASE_PHANTOM for phantoms.
/// Also you can use
///   - hkpRigidBody* hkpGetRigidBody( hkpCollidable* collidable )
///   - hkpPhantom* hkpGetPhantom(hkpCollidable* collidable)
class hkpTypedBroadPhaseHandle : public hkpBroadPhaseHandle
{
	public:
		HK_DECLARE_REFLECTION();
		
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpTypedBroadPhaseHandle);

			///Creates a new hkpTypedBroadPhaseHandle of the specified type
			///Make sure to call setowner before using this handle.
		HK_FORCE_INLINE hkpTypedBroadPhaseHandle( int type );

			///Creates a new hkpTypedBroadPhaseHandle of the specified type
		HK_FORCE_INLINE hkpTypedBroadPhaseHandle( void* owner, int type );
		
			///Gets the type. The possible types are defined in hkpWorldObject::BroadPhaseType.
		HK_FORCE_INLINE int getType() const;
		
			///Gets the owner of this handle
		HK_FORCE_INLINE void setOwner(void* o);

			///Gets the owner of this handle
		HK_FORCE_INLINE void* getOwner() const;

			///Gets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
		HK_FORCE_INLINE hkUint32 getCollisionFilterInfo() const;

			///Sets the collision filter info. This is an identifying value used by collision filters
			/// - for example, if a group filter is used, this value would encode a collision group and a system group
		HK_FORCE_INLINE void setCollisionFilterInfo( hkUint32 info );

		hkpTypedBroadPhaseHandle( class hkFinishLoadedObjectFlag flag ) : hkpBroadPhaseHandle(flag) {}
		
	protected:

		enum { OFFSET_INVALID = 127 };
		inline void setType( int type );
		friend class hkpBroadPhaseBorder; // it overrides the type of its owned phantoms

		hkInt8 m_type;
		 // would have padding of 8 here anyway, so keep at 8 and assert if owner more than +/-128 bytes away
		hkInt8 m_ownerOffset; // +nosave

	public:

			/// The quality of the object,
			/// You should use the hkpCollisionDispatcher to get the hkpCollisionQualityInfo
		hkInt8 m_objectQualityType;
		hkUint32 m_collisionFilterInfo;

};

#	include <Physics/Collide/Dispatch/BroadPhase/hkpTypedBroadPhaseHandle.inl>

#endif // HK_TYPED_BROAD_PHASE_HANDLE

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
