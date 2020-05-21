/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CONVEX_LIST_SHARED_HELPERS_H
#define HK_COLLIDE2_CONVEX_LIST_SHARED_HELPERS_H


#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/Deprecated/ConvexList/hkpConvexListShape.h>

// File info: this file is shared by the hkpConvexListAgent & hkConvexListAgent3

// C4701: local variable 'output' may be used without having been initialized
#	pragma warning(push)
#	pragma warning(disable: 4701)


struct hkpProcessCollisionOutputBackup
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpProcessCollisionOutputBackup );

	hkpProcessCollisionOutputBackup( const hkpProcessCollisionOutput& output )
	{
		m_firstPoint = output.m_firstFreeContactPoint;

		m_toi = output.m_toi;
#if defined(HK_1N_MACHINE_SUPPORTS_WELDING)
		if ( output.m_potentialContacts )
		{
			m_weldingInformation = *output.m_potentialContacts;
		}
#endif
	}

	inline void rollbackOutput( const hkpCdBody& bodyA,	const hkpCdBody& bodyB, hkpProcessCollisionOutput& output, hkpContactMgr* mgr )
	{
		if ( output.m_toi.m_time != m_toi.m_time )
		{
			mgr->removeToi( *output.m_constraintOwner.val(), output.m_toi.m_properties );
			output.m_toi = m_toi;
		}

		output.m_firstFreeContactPoint = m_firstPoint;
#if defined(HK_1N_MACHINE_SUPPORTS_WELDING)
		if ( output.m_potentialContacts )
		{
			*output.m_potentialContacts = m_weldingInformation;
		}
#endif
	}

	hkpProcessCdPoint* m_firstPoint;
	hkpProcessCollisionOutput::PotentialInfo m_weldingInformation;
	hkpProcessCollisionOutput::ToiInfo m_toi;
};


class hkpMapPointsToSubShapeContactMgr : public hkpContactMgr
{
public:
HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	hkpMapPointsToSubShapeContactMgr( hkpContactMgr* mgr ): hkpContactMgr( hkpContactMgr::TYPE_CONVEX_LIST_CONTACT_MGR )
	{
		m_contactMgr = mgr;
		m_invalidPointHit = false;
	}

	hkContactPointId addContactPointImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, const hkpGskCache* contactCache, hkContactPoint& cp )
	{
		const hkpCdBody* aMod = &a;
		const hkpCdBody* bMod = &b;
		hkpCdBody aTemp;
		hkpCdBody bTemp;
		if (a.getShape()->getType() == HK_SHAPE_CONVEX_LIST )
		{
			int shapeKey = ( 0xff00 & contactCache->m_vertices[0] ) >> 8;

			for (int i = 1; i < contactCache->m_dimA; ++i)
			{
				int shapeKey2 = ( 0xff00 & contactCache->m_vertices[i] ) >> 8;
				if ( shapeKey2 != shapeKey )
				{
					m_invalidPointHit = true;
					return HK_INVALID_CONTACT_POINT;
				}
			}
			hkpShapeBuffer buffer;
			const hkpShape* childShape = static_cast<const hkpConvexListShape*>(a.getShape())->getChildShape(shapeKey, buffer);
			new (&aTemp) hkpCdBody(&a);
			aTemp.setShape( childShape, shapeKey );
			aMod = &aTemp;
		}

		if (b.getShape()->getType() == HK_SHAPE_CONVEX_LIST )
		{
			int shapeKey = (0xff00 & contactCache->m_vertices[contactCache->m_dimA]) >> 8;

			for (int i = contactCache->m_dimA + 1; i < contactCache->m_dimA + contactCache->m_dimB; ++i)
			{
				int shapeKey2 = (0xff00 & contactCache->m_vertices[i]) >> 8;
				if ( shapeKey2 != shapeKey )
				{
					m_invalidPointHit = true;
					return HK_INVALID_CONTACT_POINT;
				}
			}
			hkpShapeBuffer buffer;
			const hkpShape* childShape = static_cast<const hkpConvexListShape*>(b.getShape())->getChildShape(shapeKey, buffer);
			new (&bTemp) hkpCdBody(&b);
			bTemp.setShape( childShape, shapeKey );
			bMod = &bTemp;
		}
		return m_contactMgr->addContactPoint( *aMod, *bMod, input, output, contactCache, cp );
	}

	hkResult reserveContactPointsImpl( int numPoints )
	{
		return m_contactMgr->reserveContactPoints( numPoints );
	}

	void removeContactPointImpl( hkContactPointId cpId, hkCollisionConstraintOwner& constraintOwner )
	{
		m_contactMgr->removeContactPoint( cpId, constraintOwner );
	}

	void processContactImpl( const hkpCollidable& a, const hkpCollidable& b, const hkpProcessCollisionInput& input, hkpProcessCollisionData& collisionData )
	{
		m_contactMgr->processContact( a, b, input, collisionData );
	}

	void cleanup()
	{
		m_contactMgr->cleanup();
	}

	ToiAccept addToiImpl( const hkpCdBody& a, const hkpCdBody& b, const hkpProcessCollisionInput& input, hkpProcessCollisionOutput& output, hkTime toi, hkContactPoint& cp, const hkpGskCache* gskCache, hkReal& projectedVelocity, hkpContactPointProperties& propertiesOut )
	{
		const hkpCdBody* aMod = &a;
		const hkpCdBody* bMod = &b;
		hkpCdBody aTemp;
		hkpCdBody bTemp;
		if (a.getShape()->getType() == HK_SHAPE_CONVEX_LIST )
		{
			int shapeKey = ( 0xff00 & gskCache->m_vertices[0] ) >> 8;

			for (int i = 1; i < gskCache->m_dimA; ++i)
			{
				int shapeKey2 = ( 0xff00 & gskCache->m_vertices[i] ) >> 8;
				if ( shapeKey2 != shapeKey )
				{
					m_invalidPointHit = true;
					return TOI_REJECT;
				}
			}
			hkpShapeBuffer buffer;
			const hkpShape* childShape = static_cast<const hkpConvexListShape*>(a.getShape())->getChildShape(shapeKey, buffer);
			new (&aTemp) hkpCdBody(&a);
			aTemp.setShape( childShape, shapeKey );
			aMod = &aTemp;
		}

		if (b.getShape()->getType() == HK_SHAPE_CONVEX_LIST )
		{
			int shapeKey = ( 0xff00 & gskCache->m_vertices[gskCache->m_dimA] ) >> 8;

			for (int i = gskCache->m_dimA + 1; i < gskCache->m_dimA + gskCache->m_dimB; ++i)
			{
				int shapeKey2 = ( 0xff00 & gskCache->m_vertices[i] ) >> 8;
				if ( shapeKey2 != shapeKey )
				{
					m_invalidPointHit = true;
					return TOI_REJECT;
				}
			}
			hkpShapeBuffer buffer;
			const hkpShape* childShape = static_cast<const hkpConvexListShape*>(b.getShape())->getChildShape(shapeKey, buffer);
			new (&bTemp) hkpCdBody(&b);
			bTemp.setShape( childShape, shapeKey );
			bMod = &bTemp;
		}
		return m_contactMgr->addToi( *aMod, *bMod, input, output, toi, cp, gskCache, projectedVelocity, propertiesOut );
	}

	void removeToiImpl( class hkCollisionConstraintOwner& constraintOwner, hkpContactPointProperties& properties )
	{
		m_contactMgr->removeToi( constraintOwner, properties );
	}

public:

	hkpContactMgr* m_contactMgr;
	hkBool m_invalidPointHit;

};

#	pragma warning(pop)



#endif // HK_COLLIDE2_CONVEX_LIST_SHARED_HELPERS_H

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
