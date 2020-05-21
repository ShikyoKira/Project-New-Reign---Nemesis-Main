/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKUTILITIES_BINDINGDATA_H
#define HKUTILITIES_BINDINGDATA_H

#include <Physics/Dynamics/hkpDynamics.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>
#include <Common/SceneData/Mesh/hkxMesh.h>
#include <Physics/Dynamics/World/hkpPhysicsSystem.h>

extern const class hkClass hkpDisplayBindingDataClass;
extern const class hkClass hkpDisplayBindingDataRigidBodyClass;
extern const class hkClass hkpDisplayBindingDataPhysicsSystemClass;

// A collection of display bindings
struct hkpDisplayBindingData : public hkReferencedObject
{
	//+vtable(true)
	//+version(1)
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	HK_DECLARE_REFLECTION();

	// A single 'rigid body <-> display object' binding
	struct RigidBody : public hkReferencedObject
	{
		//+vtable(true)
		//+version(2)
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
		RigidBody() {}
		RigidBody(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_rigidBody(f), m_displayObjectPtr(f) {}

		hkRefPtr<class hkpRigidBody> m_rigidBody;
		hkRefPtr<hkReferencedObject> m_displayObjectPtr; // Actually always a hkxMesh
		hkMatrix4 m_rigidBodyFromDisplayObjectTransform;
	};


	// A physics system and its collection of bindings
	struct PhysicsSystem : public hkReferencedObject
	{
		//+vtable(true)
		//+version(1)
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkArray< hkRefPtr<struct RigidBody> > m_bindings;
		hkRefPtr<class hkpPhysicsSystem> m_system;

		/// For serialization we need a ctor that does not call the ctor of the array on load.
		PhysicsSystem() { }
		PhysicsSystem(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_bindings(f), m_system(f) { }  
	};
	
	hkArray< hkRefPtr<struct RigidBody> > m_rigidBodyBindings;
	hkArray< hkRefPtr<struct PhysicsSystem> > m_physicsSystemBindings;
	
		/// For serialization we need a ctor that does not call the ctor of the arrays on load.
	hkpDisplayBindingData() { }
	hkpDisplayBindingData(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_rigidBodyBindings(f), m_physicsSystemBindings(f) { }  
};

#endif // HKUTILITIES_BINDINGDATA_H

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
