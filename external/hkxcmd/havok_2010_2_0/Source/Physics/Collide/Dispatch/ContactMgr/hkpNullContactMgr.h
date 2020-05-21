/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_NULL_CONTACT_MGR_H
#define HK_COLLIDE2_NULL_CONTACT_MGR_H

#include <Physics/Collide/Agent/ContactMgr/hkpContactMgr.h>

class hkpCollidable;
struct hkpCollisionInput;
struct hkpProcessCdPoint;


	/// An hkpContactMgr that doesn't add any contact points.
class hkpNullContactMgr: public hkpContactMgr
{
	//+vtable(true)
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkpNullContactMgr() : hkpContactMgr( hkpContactMgr::TYPE_NULL_CONTACT_MGR ){}

			/// No contact points are added in this implementation, however HK_NULL is a valid contact point id (!= HK_INVALID_CONTACT_POINT )
		hkContactPointId addContactPointImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& env, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp)
		{ return 0; }

		void removeContactPointImpl( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner )
		{}

		void processContactImpl( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData )
		{}

		ToiAccept addToiImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& properties )
		{
			return TOI_ACCEPT;	// needed for unit tests
		}

		void removeToiImpl( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties )
		{}


		hkResult reserveContactPointsImpl( int numPoints )
		{
			return HK_SUCCESS;
		}

		void cleanup()
		{}

};

#endif // HK_COLLIDE2_NULL_CONTACT_MGR_H

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
