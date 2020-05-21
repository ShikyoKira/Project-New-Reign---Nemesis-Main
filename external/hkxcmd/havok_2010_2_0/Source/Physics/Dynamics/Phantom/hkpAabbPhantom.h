/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_AABB_PHANTOM_H
#define HK_DYNAMICS2_AABB_PHANTOM_H

#include <Physics/Dynamics/Phantom/hkpPhantom.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

struct hkpWorldRayCastInput;
struct hkpWorldRayCastOutput;
class hkpRayHitCollector;
struct hkpLinearCastInput;
class hkpCdPointCollector;
class hkpCdPointCollector;
class hkpCollidable;


extern const hkClass hkpAabbPhantomClass;

/// This phantom simply maintains a list of hkpCollidable objects that overlap with the phantom's AABB
/// hkAabbPhantoms are very fast as they use only broadphase collision detection. They are recommended
///  - for any triggers / regions you need for game logic for which a detailed shape is not necessary (i.e you can
///    use an AABB). This phantom has no hkpShape associated with it. If you call getShape() on the hkpCollidable
///    stored in this phantom, it will return HK_NULL.
///  - to speed up short-ray casts, which are repeated every frame at roughly the same position
///    (e.g the wheels of the car in the Havok vehicle SDK).
///    See hkpAabbPhantom::castRay() for details.
///
/// Note: This is highly not recommended if you have long or non frame coherent raycasts.
class hkpAabbPhantom : public hkpPhantom
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// To add a phantom to the world, you use hkpWorld::addEntity( myPhantom ).
		hkpAabbPhantom( const hkAabb& aabb, hkUint32 collisionFilterInfo = 0 );

		~hkpAabbPhantom();

			/// Gets the hkpPhantom type. For this class the type is HK_PHANTOM_AABB
		virtual hkpPhantomType getType() const;

			/// Gets the list of collidables that are currently overlapping with the phantom.
			/// Although unlikely, it is possible that the order of elements in the array is
			/// nondeterministic. To ensure determinism, proceed the call by a call to
			/// ensureDeterministicOrder().
		inline hkArray<hkpCollidable*>& getOverlappingCollidables();

			/// Casts a ray specified by "input". For each hit found the "collector" object receives
			/// a callback. See the hkpRayHitCollector for details.
			/// Note: You must make sure that the hkAabb for the phantom contains the ray being cast.
			/// call setAabb() with an AABB of the correct size in the correct place if this is not the case.
			/// The phantom castRay collects up all the collidables overlapping with the AABB of the ray, and
			/// calls castRay against these collidables. This is usually much fast than calling hkpWorld::castRay().
			/// If you are calling a long ray cast, you should use
			/// hkpWorld::castRay instead.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RO] );
		void castRay( const hkpWorldRayCastInput& input, hkpRayHitCollector& collector ) const;

			/// Casts a ray specified by "input". This fills out the output structure with the closest
			/// ray hit.
			/// Note: You must make sure that the hkAabb for the phantom contains the ray being cast.
			/// call setAabb() with an AABB of the correct size in the correct place if this is not the case.
			/// The phantom castRay collects up all the collidables overlapping with the AABB of the ray, and
			/// calls castRay against these collidables. If you are calling a long ray cast, you should use
			/// hkpWorld::castRay instead.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RO] );
		void castRay( const hkpWorldRayCastInput& input, hkpWorldRayCastOutput& output ) const;

			/// Casts the collidable "toBeCast" against each of this phantom's overlapping
			/// collidables.
			/// The collision filter of the given collidable is tested against the
			/// collision filter of any collidables overlapping with the phantom, using the
			/// world's collision filter.
			/// This is a convenience function for performing asynchronous queries like
			/// hkpAabbPhantom::castRay, and does not modify the phantom or the collidable
			/// which is cast.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RO] );
		void linearCast(const hkpCollidable* const toBeCast, const hkpLinearCastInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector ) const;


		//
		// hkpPhantom interface
		//

			// hkpPhantom interface implementation
		virtual void calcAabb( hkAabb& aabb );

			// hkpPhantom interface implementation
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		virtual void addOverlappingCollidable( hkpCollidable* collidable );

			// hkpPhantom interface implementation
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		virtual hkBool isOverlappingCollidableAdded( const hkpCollidable* collidable );

			// hkpPhantom interface implementation
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		virtual void removeOverlappingCollidable( hkpCollidable* collidable );
			
			/// hkpPhantom clone interface
		virtual hkpPhantom* clone() const; 

			/// Sets the AABB.
			/// This will also update the broadphase, which may trigger the callbacks addOverlappingCollidable and removeOverlappingCollidable
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		void setAabb(const hkAabb& newAabb);

			/// Get the AABB of the phantom.
		inline const hkAabb& getAabb( ) const;

			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RO] );
		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

	protected:
		virtual hkMotionState* getMotionState(){ return HK_NULL; }

			/// Phantom batch move utility needs to access the AABB.
		friend class hkpPhantomBatchMoveUtil;

		class hkAabb m_aabb;

		hkArray<hkpCollidable*> m_overlappingCollidables; //+nosave

	public:

		hkpAabbPhantom( class hkFinishLoadedObjectFlag flag );

		//
		// INTERNAL USE ONLY
		//

			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		virtual void deallocateInternalArrays();

	public:
			/// Ensures that collidables returned by getOverlappingCollidables are in deterministic
			/// order.
		virtual void ensureDeterministicOrder();

	public:
			/// False if the array of collidables has changed since we last sorted it.
		hkBool m_orderDirty; //+nosave

			/// Order relation on m_overlappingCollidables.
		class OrderByUid;
};

#include <Physics/Dynamics/Phantom/hkpAabbPhantom.inl>

#endif	// HK_DYNAMICS2_AABB_PHANTOM_H

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
