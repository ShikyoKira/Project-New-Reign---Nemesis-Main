/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_MANAGER_H
#define HK_VEHICLE_MANAGER_H

#include <Physics/Dynamics/World/Listener/hkpWorldPostSimulationListener.h>

extern const class hkClass hkpVehicleManagerClass;

class hkpVehicleInstance;

/// A VehicleManager offers a way to simulate a set of vehicles together, and provides an alternative
/// to using actions.
/// It is intended that subclasses will multithread parts of the vehicle simulation, particularly wheel
/// collision detection.
///
/// The following property is assumed to be true of the vehicles in the manager:
/// EITHER ( all the vehicles' chassis and phantoms are added to the world )
/// OR ( none of the vehicles' chassis and phantoms are added to the world ).
class hkpVehicleManager : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE );
		HK_DECLARE_REFLECTION();

		hkpVehicleManager() { }
		
		virtual ~hkpVehicleManager();

			/// Register a vehicle with the manager.
			/// \param vehicle the vehicle to be added.
		virtual void addVehicle( hkpVehicleInstance* vehicle );

			/// Remove a vehicle which has been registered with the manager.
			/// \param vehicle the vehicle to be removed.
		virtual void removeVehicle( hkpVehicleInstance* vehicle );

			/// Build array of active vehicles from vehicles registered with the manager.
			/// Inactive vehicles should not be stepped.
		void getActiveVehicles( hkArray< hkpVehicleInstance* >& activeVehicles ) const;

		//
		// Singlethreaded simulation.
		//

			/// Update and simulate all registered vehicles.
			/// The stepInfo must be updated for the current step in order to correctly
			/// calculate vehicles' centers of mass
		virtual void stepVehicles( const hkStepInfo& updatedStepInfo );

		static void stepVehicleArray( hkArray<hkpVehicleInstance*>& activeVehicles, const hkStepInfo& updatedStepInfo );

		//
		// Simple accessors to the array.
		//

			/// Returns the number of registered vehicles.
		inline int getNumVehicles() const;

			/// Read/write access to the vehicle at the specified index.
		inline hkpVehicleInstance& getVehicle( int index );
		
			/// Read-only access to the vehicle at the specified index.
		inline const hkpVehicleInstance& getVehicle( int index ) const;

		//
		// Adding and removing vehicles to and from the world.
		//

			/// Add the chassis and phantoms of all registered vehicles to the world.
		void addToWorld( hkpWorld* world );

			/// Remove the chassis and phantoms of all registered vehicles from the world.
		void removeFromWorld();

	public:
			/// The array of currently registered vehicles.
		hkArray<hkpVehicleInstance*> m_registeredVehicles;

		hkpVehicleManager( hkFinishLoadedObjectFlag f ) : hkReferencedObject(f), m_registeredVehicles(f) { }
};

#include<Physics/Vehicle/Manager/hkpVehicleManager.inl>

#endif // HK_VEHICLE_MANAGER_H

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
