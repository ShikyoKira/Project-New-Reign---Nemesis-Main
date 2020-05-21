/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_EASE_PENETRATION_ACTION_H
#define HK_EASE_PENETRATION_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>
#include <Common/Base/hkBase.h>

	/// You can use this action to reduce CPU hits that occur when a new body is added to a hkpWorld in a penetrating state.
	///
	/// When you add a simple shape, like a box, and place it in a way that it deeply intersects with around 8 triangles of a landscape
	/// mesh -- then you can expect high numbers of unjustified TOI events being handled. In a simple test case we performed, there were
	/// around 10 TOIs for a moving-quality body, and around 80!!! for a critical-quality body.
	///
	/// This utility controls the m_allowedPenetrationDepth property of a rigid body over a short time of, e.g., a few seconds, to
	/// avoid those unneeded TOIs and to allow the bodies to recover from penetration with normal collision response.
	/// After m_timePassed reaches m_duration, the m_alloedPenetrationDepth of the hkpRigidBody is set back to its original value.
	///
	/// Additionally this utility can also reduce the strength at which the solver corrects inter-body penetrations. This is done by
	/// iterating over all contact points, and reducing the penetration distance, that is later fed into the constraint solver.
	/// As the result, less jitter may occur when bodies recover from penetration.
class hkpEasePenetrationAction : public hkpUnaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			// This allows you to specify duration of the action. Other values can be set directly after the action is constructed.
		hkpEasePenetrationAction(hkpEntity* entity, hkReal duration);

		virtual	~hkpEasePenetrationAction();

			// hkpAction implementation.
		virtual void applyAction( const hkStepInfo& stepInfo );

		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const ;

	public:

			/// Duration of this action. After the time passes the action removes itself from the hkpWorld.
		hkReal m_duration;

			/// Initial multiplier for the m_entity's m_allowedPenetrationDepth. It's reduced linearly with time to reach 1.0f at the end of m_duration.
		hkReal m_initialAllowedPenetrationDepthMultiplier;
			/// Initial added value for the m_entity's m_allowedPenetrationDepth. It's reduced linearly with time to reach 0.0f at the end of m_duration.
		hkReal m_initialAdditionalAllowedPenetrationDepth;

			/// Shall the action soften the solver response too, by reducing penetration distances for contact points.
		bool m_reducePenetrationDistance;

			/// Initial distance multiplier applied for all penetrating contact points. It's reduced linearly with time to reach 1.0f at the end of m_duration.
			/// Don't use small values, as this may cause bodies to fall through the ground. It's set to 0.2 by default.
		hkReal m_initialContactDepthMultiplier;

	private:
			/// Time passed since the action was added to the world.
		hkReal m_timePassed;
			/// Original allowed penetration depth of the m_entity.
		hkReal m_originalAllowedPenetrationDepth;
};

#endif // HK_EASE_PENETRATION_ACTION_H

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
