/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_RIGIDBODY_INERTIA_VIEWER_H
#define HK_UTILITIES2_RIGIDBODY_INERTIA_VIEWER_H

#include <Physics/Utilities/VisualDebugger/Viewer/Dynamics/hkpWorldViewerBase.h>

#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Common/Visualize/Shape/hkDisplayBox.h>

class hkDebugDisplayHandler;
class hkpWorld;
class hkpRigidBody;

	/// Shows the inertia tensor of all rigid bodies added to the world.
	/// The Inertia Tensor is displayed as a box whose dimensions are such that it would have the same inertia
	/// tensor as the body in question, if it was also of equal mass. In other words, it shows an object which
	/// would behave identically from a dynamic point of view (though obviously, not from a collision detection
	/// point of view).
	/// In the case of a rigid body being simulated as an Oriented Particle this box will always be a cube aligned in local space.
	/// In the case of a rigid body being simulated with a DIAGONAL inertia tensor this box may have different edge
	/// lengths, but will still be aligned in local space.
	/// Currently, a FULL inertia tensor is not supported.
	/// N.B. Actually, we display a box which is *slightly bigger* than the one defined above by a factor of 1.01, just for
	/// ease of display.
class hkpRigidBodyInertiaViewer :public hkpWorldViewerBase,
								protected hkpEntityListener, protected hkpWorldPostSimulationListener
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Creates a hkpRigidBodyInertiaViewer.
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);

			/// Registers the hkpRigidBodyInertiaViewer with the hkViewerFactory.
		static void HK_CALL registerViewer();

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

		virtual void init();

		static inline const char* HK_CALL getName() { return "Inertia Tensors"; }

	protected:

		hkpRigidBodyInertiaViewer(const hkArray<hkProcessContext*>& contexts);
		virtual ~hkpRigidBodyInertiaViewer();

		void addWorld(hkpWorld* world);
		void removeWorld(hkpWorld* world);

		virtual void entityAddedCallback( hkpEntity* entity );
		virtual void entityRemovedCallback( hkpEntity* entity );

		virtual void postSimulationCallback( hkpWorld* world );

		virtual void worldAddedCallback( hkpWorld* world );
		virtual void worldRemovedCallback( hkpWorld* world );

	protected:
		static int m_tag;

		hkArray<hkpRigidBody*> m_entitiesCreated;
		hkArray<hkDisplayBox> m_displayBoxes;
};

#endif	// HK_UTILITIES2_RIGIDBODY_INERTIA_VIEWER_H

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
