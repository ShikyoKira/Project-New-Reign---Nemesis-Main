/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_REORIENT_ACTION_H
#define HK_REORIENT_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

class hkpRigidBody;
extern const hkClass hkpReorientActionClass;

	/// This action will try to orientate a rigid body so that it's local up
	/// matches a specified world up vector.
	/// It is similar in functionality to an hkpAngularDashpotAction however,
	/// it will only apply rotational impulses around the specified axis.
class hkpReorientAction : public hkpUnaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpReorientAction();

			/// Creates an angular dashpot with the specified construction info.
			/// Assumes that the rotation axis is perpendicular to the up axis.
		hkpReorientAction( hkpRigidBody* body,
			const hkVector4& rotationAxis,
			const hkVector4& upAxis,
			hkReal strength = 1.0f,
			hkReal damping = 0.1f );

			/// Applies angular impulses to a rigid body to try and lock
			/// orientation about a specified axis.
		void applyAction( const hkStepInfo& stepInfo );

			/// hkpAction clone interface.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

	public:

		hkVector4 m_rotationAxis;			
		hkVector4 m_upAxis;			
		hkReal m_strength; 
		hkReal m_damping;

	public:

		hkpReorientAction( class hkFinishLoadedObjectFlag flag ) : hkpUnaryAction(flag) {}
};

#endif // HK_REORIENT_ACTION_H

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
