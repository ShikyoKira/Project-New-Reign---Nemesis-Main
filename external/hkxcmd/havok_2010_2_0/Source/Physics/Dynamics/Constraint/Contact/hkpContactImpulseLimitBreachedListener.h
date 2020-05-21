/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */


#ifndef HK_DYNAMICS2_CONTACT_IMPULSE_LIMIT_BREACHED_LISTENER_H
#define HK_DYNAMICS2_CONTACT_IMPULSE_LIMIT_BREACHED_LISTENER_H

#include <Physics/Dynamics/Constraint/hkpConstraintInstance.h>
#include <Physics/Dynamics/Collide/hkpSimpleConstraintContactMgr.h>

class hkpContactPointProperties;
class hkContactPointMaterial;

	/// Data used to identify breached contact points.
	/// This does not store any hkpShapeKey information,
	/// You have to retrieve the hkpShapeKey by using the userDataField in the hkContactPointMaterial structure
class hkpContactImpulseLimitBreachedListenerInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactImpulseLimitBreachedListenerInfo );

			/// Returns the contact mgr
		hkpSimpleConstraintContactMgr* getContactMgr() const;

			/// returns the array of all contact point properties
		HK_FORCE_INLINE hkpContactPointProperties* getContactPointProperties() const;

		HK_FORCE_INLINE hkContactPoint* getContactPoint() const;

		hkpRigidBody* getBodyA() const { return m_data.m_single.m_constraintInstance->getRigidBodyA(); }
		hkpRigidBody* getBodyB() const { return m_data.m_single.m_constraintInstance->getRigidBodyB(); }

		const hkpConstraintInstance* getConstraintInstance() const { return m_data.m_solver.m_constraintInstance; }
		
		HK_FORCE_INLINE void set( hkpConstraintInstance* constraintInstance, hkpContactPointProperties* properties, hkContactPoint* cp, bool isToi );

		HK_FORCE_INLINE hkBool isToi() const { return m_data.m_single.m_type == 2; }

		HK_FORCE_INLINE hkBool isContact() const { return m_data.m_solver.m_isContact == 1; }

	protected:
		struct SingleImpulseElem
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactImpulseLimitBreachedListenerInfo::SingleImpulseElem );

			HK_ALIGN16(hkUint16				m_type);				// 1
			hkUint16 m_isContact;
			hkpConstraintInstance*			m_constraintInstance;	
			hkpContactPointProperties*		m_properties;
			hkContactPoint*			m_contactPoint;
		};

		union ListenerData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_DYNAMICS, hkpContactImpulseLimitBreachedListenerInfo::SingleImpulseElem );

			hkpImpulseLimitBreachedElem m_solver;
			SingleImpulseElem          m_single;
		};
		ListenerData m_data;
};

	/// This
class hkpContactImpulseLimitBreachedListener
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpContactImpulseLimitBreachedListener);
		virtual ~hkpContactImpulseLimitBreachedListener(){}
		virtual void contactImpulseLimitBreachedCallback( const hkpContactImpulseLimitBreachedListenerInfo* breachedContacts, int numBreachedContacts ) = 0;
};

HK_FORCE_INLINE hkpSimpleConstraintContactMgr* hkpContactImpulseLimitBreachedListenerInfo::getContactMgr() const
{
	const hkUlong offset = hkUlong(-HK_OFFSET_OF( hkpSimpleConstraintContactMgr, m_constraint ));
	return (hkpSimpleConstraintContactMgr*)hkAddByteOffset( m_data.m_single.m_constraintInstance, offset );
}

hkpContactPointProperties *hkpContactImpulseLimitBreachedListenerInfo::getContactPointProperties() const
{
	if ( m_data.m_single.m_type != 0)
	{
		return m_data.m_single.m_properties;
	}
	return static_cast<hkpContactPointProperties*>( m_data.m_solver.m_solverResult );
}

hkContactPoint *hkpContactImpulseLimitBreachedListenerInfo::getContactPoint() const
{
	if ( m_data.m_single.m_type != 0)
	{
		return m_data.m_single.m_contactPoint;
	}
	hkpSimpleConstraintContactMgr* mgr = getContactMgr();
	hkpSimpleContactConstraintAtom* atom = mgr->m_contactConstraintData.m_atom;
	const hkpContactPointPropertiesStream* cpp = reinterpret_cast<hkpContactPointPropertiesStream*>( m_data.m_solver.m_solverResult );
	hkUlong contactIndex = hkGetByteOffset(atom->getContactPointPropertiesStream(), cpp) / atom->getContactPointPropertiesStriding() ;
	hkContactPoint* cp = &atom->getContactPoints()[contactIndex];
	return cp;
}


void hkpContactImpulseLimitBreachedListenerInfo::set(hkpConstraintInstance* constraintInstance, hkpContactPointProperties* properties, hkContactPoint* cp, bool toi)
{
	m_data.m_single.m_type = 1 + toi;
	m_data.m_single.m_isContact = true;
	m_data.m_single.m_constraintInstance = constraintInstance;
	m_data.m_single.m_contactPoint = cp;
	m_data.m_single.m_properties = properties;
}

#endif // HK_DYNAMICS2_CONTACT_IMPULSE_LIMIT_BREACHED_LISTENER_H

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
