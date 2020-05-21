/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MOUSE_SPRING_ACTION_H
#define HK_MOUSE_SPRING_ACTION_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

class hkpRigidBody;
extern const hkClass hkpMouseSpringActionClass;

	/// An action that allows users to drag a rigid body using the mouse. The mouse spring applies
	/// an impulse to the rigid body to move it around. You can see this action being
	/// used in the visual debugger and the Havok 2 demo framework.
class hkpMouseSpringAction: public hkpUnaryAction
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		typedef void ( HK_CALL *mouseSpringAppliedCallback)(class hkpMouseSpringAction* mouseSpring, const hkStepInfo& stepInfo, const hkVector4& impulseApplied);

		hkpMouseSpringAction( hkpRigidBody* body = HK_NULL );

			/// Creates a new hkpMouseSpringAction for the specified rigid body.
			/// The mouse spring is created between the specified point in the rigid body's space and the specified
			/// mouse position in world space.
		hkpMouseSpringAction(
			const hkVector4& positionInRbLocal, const hkVector4& mousePositionInWorld,
			const hkReal springDamping, const hkReal springElasticity,
			const hkReal objectDamping, hkpRigidBody* rb,
			const hkArray<hkpMouseSpringAction::mouseSpringAppliedCallback>* appliedCallbacks = HK_NULL);

		void entityRemovedCallback(hkpEntity* entity);

			///	Sets the mouse position in world space
		void setMousePosition( const hkVector4& mousePositionInWorld );

			///	Sets the maximum relative force applied to the body
		void setMaxRelativeForce(hkReal newMax);

	public:

			/// Mouse spring end-point in the rigid body's local space
		hkVector4 m_positionInRbLocal;

			/// Mouse position in world space
		hkVector4 m_mousePositionInWorld;

			/// Damping for the mouse spring
		hkReal	m_springDamping;

			/// Elasticity for the mouse spring
		hkReal	m_springElasticity;

			/// Allows you to clip the impulse applied to the body (this gets multiplied by the mass of the object)
		hkReal  m_maxRelativeForce;

			/// Damping applied to the linear and angular velocities of the picked body
		hkReal	m_objectDamping;

			// This is the shape key that the mouse spring action is attached to.
			// This is used in demos with the integrity solver.
		hkpShapeKey m_shapeKey;

			// Callbacks fired at the end of hkpMouseSpringAction::applyAction();
		hkArray<mouseSpringAppliedCallback> m_applyCallbacks; //+overridetype(hkArray<void*>) +serialized(false)

		void applyAction( const hkStepInfo& stepInfo );

			/// hkpAction clone interface.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;

	public:

		hkpMouseSpringAction( class hkFinishLoadedObjectFlag flag ) : hkpUnaryAction(flag) {}

		static char s_name[];
};

#endif // HK_MOUSE_SPRING_H

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
