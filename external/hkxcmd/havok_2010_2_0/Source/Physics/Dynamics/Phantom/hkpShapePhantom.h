/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_SHAPE_PHANTOM_H
#define HK_DYNAMICS2_SHAPE_PHANTOM_H

#include <Physics/Dynamics/Phantom/hkpPhantom.h>
#include <Common/Base/Types/Physics/MotionState/hkMotionState.h>

class hkCollisionEnvironment;
class hkpCollisionDispatcher;
class hkpCollidable;
class hkpShape;
class hkpCollisionAgent;
struct hkpLinearCastInput;
struct hkpCollisionInput;
class hkpCdPointCollector;
class hkpCdBodyPairCollector;

extern const hkClass hkpShapePhantomClass;

/// This class represents a phantom with an arbitrary shape and transform for query purposes.
///
/// Please read the hkpPhantom documentation first.
///
/// It can be used to:
///     - Get the closest points between the shape and all other nearby objects
///     - Linear cast the shape against all other objects
///     - Get the penetrations between the shape and its neighbours.
///     - For each of the three functions, all the shapes in the world whose broadphase AABBs overlap
///       with the hkpShapePhantom's broadphase AABB will be tested.
///
/// This phantom has no physical presence in the simulation.
///
/// There are two implementations of this class:
///     - the hkpSimpleShapePhantom which does not do any narrowphase caching
///     - the hkpCachingShapePhantom which caches the narrowphase by creating a
///       hkpCollisionAgent for every overlapping pair.
///
/// In order to decide which version to use, you should first read the hkpGskBaseAgent documentation.
/// Some rules when to use hkpCachingShapePhantom:
///     - Your shape is not a sphere and not a capsule (e.g., box, convexVertices...)
///     - CPU is more important than memory
///     - The position of your phantom moves with a reasonable velocity
///     - The length of the path in a linearcast is short
class hkpShapePhantom : public hkpPhantom 
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Constructor
		hkpShapePhantom( const hkpShape* shape, const hkTransform& transform );


			/// This function sets the position of the shape phantom and performs a linear cast from the new position of the shape
			/// phantom to the target position specified in hkpLinearCastInput.
			/// \note This will update the broadphase AABB
			/// \note This will assume that the rotation is already set
			///
			/// The reason that these two operations are performed together is for performance reasons
			/// (so that only one call to update the broadphase AABB is needed)
			/// This version of setPositionAndLinearCast creates an AABB big enough to hold the entire cast of the shape, and
			/// calls the broadphase, then calls linearCast on all the overlapping objects the broadphase returns.
			/// If you are performing a long linear cast you should NOT use this method. Use hkpWorld::linearCast() instead.
			/// See comments for hkpWorld::linearCast() for more details.
			/// The [optional] startPointCollector returns all the closest points
			/// at the start point of the linear cast. If you do not want this functionality, pass HK_NULL as the
			/// "startCollector".
		virtual void setPositionAndLinearCast( const hkVector4& position, const hkpLinearCastInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector ) = 0;

			/// This is analogical to setPositionAndLinearCast(), but it allows you to set the full initial transform.
		virtual void setTransformAndLinearCast( const hkTransform& transform, const hkpLinearCastInput& input, hkpCdPointCollector& castCollector, hkpCdPointCollector* startCollector ) = 0;

			/// You can call this in order to collect the closest points between the collidable and the objects
			/// at the phantoms current position. For each shape within collision tolerance of the shape in the phantom, the "collector"
			/// will receive a callback. See hkpCdPointCollector for more details.
		virtual void getClosestPoints( hkpCdPointCollector& collector, const hkpCollisionInput* input = HK_NULL ) = 0;

			/// This can be called in order to collect all penetrating shapes
			/// at the phantoms current position. For each shape which overlaps with the phantom's shape, the "collector" receives a callback.
			/// See hkpCdBodyPairCollector for more details.
			/// Although unlikely, it is possible that callbacks are issued in a nondeterministic order. To guarantee that
			/// the order is deterministic, proceed this call by a call to ensureDeterministicOrder().
		virtual void getPenetrations( hkpCdBodyPairCollector& collector, const hkpCollisionInput* input = HK_NULL ) = 0;

			/// Read access to the transform.
		inline const hkTransform& getTransform() const;

			/// Sets the transform and updates the AABB.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		void setTransform( const hkTransform& transform);

			/// A faster way to move the phantom, setting the translation only.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RW] [this,HK_ACCESS_RW] );
		void setPosition( const hkVector4& position, hkReal extraTolerance = 0.0f );

			// Interface implementation
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_RO] [this,HK_ACCESS_RW] );
		hkWorldOperation::Result setShape( const hkpShape* shape );


	public:

		//
		// hkpPhantom interface
		//
			/// Get the current AABB.
			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		virtual void calcAabb( hkAabb& aabb );


			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RO] );
		virtual hkMotionState* getMotionState();
		class hkMotionState m_motionState;


		hkpShapePhantom( class hkFinishLoadedObjectFlag flag ) : hkpPhantom( flag ) {}

		//
		// INTERNAL USE ONLY
		//

			/// ###ACCESS_CHECKS###( [m_world,HK_ACCESS_IGNORE] [this,HK_ACCESS_RW] );
		virtual void deallocateInternalArrays();
};

#include <Physics/Dynamics/Phantom/hkpShapePhantom.inl>


#endif //HK_DYNAMICS2_SHAPE_PHANTOM_H

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
