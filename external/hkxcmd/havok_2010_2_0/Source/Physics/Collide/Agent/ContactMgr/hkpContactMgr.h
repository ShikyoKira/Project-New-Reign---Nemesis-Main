/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONTACT_MGR_H
#define HK_COLLIDE2_CONTACT_MGR_H

struct hkpProcessCdPoint;
struct hkpProcessCollisionData;
class hkpCdBody;
struct hkpProcessCollisionInput;
struct hkpProcessCollisionOutput;
class hkpCollidable;
class hkpGskCache;

#ifndef hkCollisionConstraintOwner
class hkpConstraintOwner;
#	define hkCollisionConstraintOwner hkpConstraintOwner
#endif

#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>
#include <Physics/ConstraintSolver/Constraint/Contact/hkpContactPointProperties.h>

	/// The hkpContactMgr is an interface used by a collision agent to report its results.
	/// This class is used internally by hkDynamics
class hkpContactMgr: public hkReferencedObject	
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONTACT );

		enum Type
		{
			TYPE_SIMPLE_CONSTRAINT_CONTACT_MGR,
			TYPE_REPORT_CONTACT_MGR,
			TYPE_CONVEX_LIST_CONTACT_MGR,
			TYPE_NULL_CONTACT_MGR,
			TYPE_USER_CONTACT_MGR,
			TYPE_MAX
		};

		enum ToiAccept
		{
			TOI_ACCEPT = 0,
			TOI_REJECT = 1
		};

		hkpContactMgr( Type type ): m_type(type){}

			/// Add a new contact point. This should return an unique and persistent id for that point
			/// See hkpCollisionListener for further details.
		HK_FORCE_INLINE hkContactPointId addContactPoint( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp );

			/// reserve a number of contact points, if you want to release those reserve
			/// contact points, just call this function with a negative parameter
			/// Note: if this function fails, no contact points have been reserved.
			/// See hkpCollisionListener for further details.
		HK_FORCE_INLINE hkResult reserveContactPoints( int numPoints );

			/// Removes a contact point with a given id (given by addContactPoint)
			/// See hkpCollisionListener for further details.
		HK_FORCE_INLINE void removeContactPoint( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner );

			/// After all agents between a given pair of hkCollidables are called, all information
			/// is passed to process contact. See hkpCollisionListener for further details.
		HK_FORCE_INLINE void processContact( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData );


			/// Adds a potential TOI and returns what to do with this TOI.
			/// This is the only reasonable place to reject TOIs
			/// See hkpCollisionListener for further details.
		HK_FORCE_INLINE ToiAccept addToi( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& propertiesOut );

			/// Fire callbacks about a removed TOI.
		HK_FORCE_INLINE void removeToi( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties );


			//
			//	Hand coded vtable for spu
			//
	public:

			// we cannot define those functions to be virtual on an spu because:
			// - when we build the jump table, we need the address to the actual function implementation and not to the vtable call
			// - sadly, the compiler always takes the address to the vtable call as soon as the function is virtual (so we have make those functions non virtual)
		virtual hkContactPointId addContactPointImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp ) = 0;
		virtual hkResult reserveContactPointsImpl( int numPoints ) = 0;
		virtual void removeContactPointImpl( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner ) = 0;
		virtual void processContactImpl( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData ) = 0;
		virtual ToiAccept addToiImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& propertiesOut ) = 0;
		virtual void removeToiImpl( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties ) = 0;

		/// delete yourself
		virtual void cleanup(){}

	public:
		Type m_type;
};

	// on spus we have to include this inl file after we include hkSimpleContactConstraintMgr.h
#	include <Physics/Collide/Agent/ContactMgr/hkpContactMgr.inl>

#endif // HK_COLLIDE2_CONTACT_MGR_H

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
