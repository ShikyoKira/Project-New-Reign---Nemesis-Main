/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_ACTION_H
#define HK_DYNAMICS2_ACTION_H


#include <Common/Base/hkBase.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>

class hkpEntity;
class hkpPhantom;
class hkpSimulationIsland;
class hkStepInfo;
class hkpWorld;

extern const hkClass hkpActionClass;

/// This is the base class from which user actions (or controllers) are derived. Actions
/// are the interface between user controllable behavior of the physical simulation and the Havok core.
class hkpAction : public hkReferencedObject
{
	public:
		// +hk.ModelerNodeType("LOCATOR")
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ACTION);


			/// Default constructor
		inline hkpAction(hkUlong userData);
		
			/// Virtual Destructor.
		virtual ~hkpAction() { }

			/// The applyAction() method does the actual work of the action, and is called at every simulation step.
		virtual void applyAction( const hkStepInfo& stepInfo ) = 0;


			/// This method is used to maintain simulation islands.
			/// An action that requires that the entities it operates on are simulated together
			/// must return these entities in this function call. See the hkpBinaryAction
			/// implementation for an example.
		virtual void getEntities( hkArray<hkpEntity*>& entitiesOut ) = 0;

			/// Get referenced phantoms. This is used in order to generically clone hkpAction objects
			/// that reference phantoms. This has a default implementation that returns no phantoms.
		virtual void getPhantoms( hkArray<hkpPhantom*>& phantomsOut ) {}

			/// Returns the hkpWorld that owns this action.
		inline hkpWorld* getWorld();

			// hkpEntityListener-like interface implementation
		virtual void entityRemovedCallback(hkpEntity* entity) = 0;

			/// Get the user data for the action
		inline hkUlong getUserData() const;

			/// Set the user data of the action.
		inline void setUserData( hkUlong data );

			/// Get the name of this action.
		inline const char* getName() const;

			/// Set the name of this action.
			/// IMPORTANT: This data will not be cleaned up by the hkpAction destructor. You are required to track it yourself.
		inline void setName( const char* name );

			/// In order to clone an action, with a varying number of dependant rigid bodies
			/// that you would like to repoint the clone at (and null the world ptr etc)
			/// we provide a list of entities, in the same order as given by getEntities()
			/// that are the new cloned bodies to use in the process.
			/// We also provide a list of phantoms. Actions do not generally reference phantoms,
			/// but there are exceptions, as in the case of hkpVehicleInstance.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const = 0;

	private:

		hkpWorld* m_world; //+nosave
		hkpSimulationIsland* m_island; //+nosave

	protected:
		hkUlong m_userData; // +default(0)

		hkStringPtr m_name;

		friend class hkpWorld;
		friend class hkpWorldOperationUtil;
		friend class hkpSimulationIsland;

		//
		// Internal methods
		//
	public:

			// Gets the simulation island to which the action belongs.
		inline hkpSimulationIsland* getSimulationIsland();

	public:
	
			// Sets the simulation island.
		inline void setSimulationIsland(hkpSimulationIsland* island);

			// Sets the hkpWorld that owns this action.
		inline void setWorld( hkpWorld* owner );

	public:
		hkpAction( class hkFinishLoadedObjectFlag flag ) : hkReferencedObject(flag), m_name(flag) {}
};

#include <Physics/Dynamics/Action/hkpAction.inl>

#endif // HK_DYNAMICS2_ACTION_H

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
