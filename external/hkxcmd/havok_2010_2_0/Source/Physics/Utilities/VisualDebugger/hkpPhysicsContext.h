/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES2_PHYSICS_CONTEXT_H
#define HK_UTILITIES2_PHYSICS_CONTEXT_H

#include <Common/Visualize/hkProcessContext.h>
#include <Physics/Dynamics/World/hkpWorldCinfo.h>
#include <Physics/Dynamics/World/Listener/hkpWorldDeletionListener.h>
#include <Physics/Dynamics/Action/hkpActionListener.h>
#include <Physics/Dynamics/Constraint/hkpConstraintListener.h>
#include <Physics/Dynamics/Phantom/hkpPhantomListener.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpWorld;
class hkpEntity;
class hkpPhantom;

/// A simple interface that viewers that want to know
/// when hkWorlds are added and removed from the physics
/// context can implement.
class hkpPhysicsContextWorldListener
{
    public:
    HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpPhysicsContextWorldListener);
        virtual ~hkpPhysicsContextWorldListener() { }
        virtual void worldAddedCallback( hkpWorld* newWorld ) = 0;
        virtual void worldRemovedCallback( hkpWorld* newWorld ) = 0;
};

#define HK_PHYSICS_CONTEXT_TYPE_STRING "Physics"

/// This is the context that processes (here called the older name
/// of 'viewers') can use if they understand physics Worlds. It listens on all
/// added worlds and can trigger deletion and addition callbacks
/// to processes (viewers) if requested. A context itself is just a data store
/// for any information that a viewer will need. In this case it is pointers to
/// hkWorlds, as from that any of the physics viewers can find the parts they are
/// interested in and then register themselves with the world to get the appropriate
/// callbacks to keep their state up to date.
class hkpPhysicsContext : 
	public hkReferencedObject, public hkProcessContext, 
	public hkpWorldDeletionListener, 
	public hkpEntityListener, 
	public hkpPhantomListener,
	public hkpConstraintListener,
	public hkpActionListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

		hkpPhysicsContext();

			/// Register all Processes (Physics Viewers)
			/// in the Utilities. If you don't call this
			/// or a subset of what it calls, all you will get
			/// is the common viewers in hkVisualize (debug display and stats).
			/// If you don't see the hkpShapeDisplayViewer for instance in the
			/// list of available viewers in the VDB client, it is because
			/// this has not been called.
		static void HK_CALL registerAllPhysicsProcesses();

			/// As there can be any number of different user types
			/// of data contexts, the type is simply identified by string.
		virtual const char* getType() { return HK_PHYSICS_CONTEXT_TYPE_STRING; }

			/// Register a world. This context is a deletion listener, so will
			/// remove the world when it sees it deleted. It does not reference
			/// the world, just keeps track of it by way of the listeners.
		void addWorld( hkpWorld* newWorld );

			/// Explicitly remove a world from the context. If
			/// you delete a world it will remove itself from
			/// the context anyway as this context is a deletion
			/// listener.
		void removeWorld( hkpWorld* newWorld ); 

			/// Find the index of the given world, returns -1
			/// if not found in this context.
		int findWorld(hkpWorld* world); 

			/// Get number of worlds registered in this context.
		inline int getNumWorlds() { return m_worlds.getSize(); }

			/// Get the i-th world registered in this context.
		inline hkpWorld* getWorld(int i) { return m_worlds[i]; }

			/// So that processes can see all the worlds dynamically,
			/// they can be hkpPhysicsContextWorldListener which simply
			/// get world added to this context events. As such they would
			/// be able to then register themselves as rigidbody added
			/// listeners and so on and for instance be able to create
			/// bodies to display upon those further callbacks.
		void addWorldAddedListener( hkpPhysicsContextWorldListener* cb );

			/// So that processes can see all the worlds dynamically,
			/// they can be hkpPhysicsContextWorldListener which simply
			/// can get world removed from this context, and can update
			/// their state accordingly (remove some display geoms etc).
		void removeWorldAddedListener( hkpPhysicsContextWorldListener* cb );

	public:
		
			/// Set the VDB that owns this context. This is called by the VDB itself.
		virtual void setOwner(hkVisualDebugger* vdb);

			// Physics callbacks we want to track

			/// Raised when the given world has been deleted, default action is
			/// to remove it from the list of registered worlds and notify
			/// any interested viewers that the world has been removed.
		virtual void worldDeletedCallback( hkpWorld* world );

			/// Inspection (tweaking on the client side affecting this server side)
			/// requires that at least high level objects get removed from the
			/// list of available tweakable objects. Tweakables are registered here
			/// from the physics contexts worlds through this context owner, the VDB itself.
		virtual void entityAddedCallback( hkpEntity* entity );
		virtual void entityRemovedCallback( hkpEntity* entity );
		virtual void phantomAddedCallback( hkpPhantom* phantom );
		virtual void phantomRemovedCallback( hkpPhantom* phantom );
		virtual void constraintAddedCallback( hkpConstraintInstance* constraint );
		virtual void constraintRemovedCallback( hkpConstraintInstance* constraint );
		virtual void actionAddedCallback( hkpAction* action );
		virtual void actionRemovedCallback( hkpAction* action );

	protected:

			/// As a context must exist at least as long as the VDB, we explicitly
			/// do not allow local variables of it to force the use of 'new' and removeRefernce().
			/// The VDB itself can't add a reference a context is just a abstract interface that
			/// any user data item can implement for their own viewers.
		virtual ~hkpPhysicsContext();

			/// Iterates through existing objects (entities, phantoms etc)
			/// and adds them for Inspection (tweaking) and then adds
			/// listeners to pick up when that state changes and objects are
			/// added or removed etc. Called upon addWorld.
			/// Non const as has to add listeners
		void addForInspection( hkpWorld* w );

			/// Iterates through existing objects (entities, phantoms etc)
			/// and removes them for Inspection (tweaking). Called upon removeWorld.
			/// Non const as has to remove listeners
		void removeFromInspection( hkpWorld* w);

		hkArray<hkpWorld*> m_worlds;
		hkArray<hkpWorldCinfo> m_worldCinfos;
		hkArray<hkpPhysicsContextWorldListener*> m_addListeners;
};

#endif // HK_VISUALIZE_PROCESS_CONTEXT_H

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
