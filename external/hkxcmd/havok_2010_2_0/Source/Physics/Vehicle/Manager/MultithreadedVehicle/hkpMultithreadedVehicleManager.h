/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_MULTITHREAD_MANAGER_H
#define HK_VEHICLE_MULTITHREAD_MANAGER_H

#include <Physics/Vehicle/Manager/hkpVehicleManager.h>
#include <Physics/Vehicle/Manager/MultithreadedVehicle/hkpVehicleJobs.h>


/// Manages a set of vehicles and allows the vehicles to be stepped on multiple threads
/// The vehicle stepping functionality is implemented in hkpVehicleJobUtil
class hkpMultithreadedVehicleManager : public hkpVehicleManager
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE );
		HK_DECLARE_REFLECTION();

			/// Vehicle commands and results
		struct VehicleCommandBatch
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpMultithreadedVehicleManager::VehicleCommandBatch);
				/// A pointer to enough space for one command per vehicle.
				/// This space is divided into chunks, with each chunk containing
				/// all the commands for a group of vehicles.
			hkpVehicleCommand* m_commandStorage;

				/// A pointer to enough space for one vehicle output per wheel.
				/// The elements of this array correspond directly to the commands in
				/// m_commandStorage, and are arranged in the same way.
			hkpVehicleJobResults* m_outputStorage;
		};

			/// Constructor.
		hkpMultithreadedVehicleManager() { }

			/// Destructor
		~hkpMultithreadedVehicleManager() { }

			/// Build and perform vehicle jobs, and apply results to the vehicles.
			/// The calling thread also performs vehicle jobs (so numJobs should take that into account).
			/// Stack allocations are used for temporary buffer.
			/// This is a simple implementation which users may wish to re implement with optimizations.
			/// N.B. The stepInfo must be updated for the current step to allow accumulators to be built correctly.
		virtual void stepVehiclesSynchronously( hkpWorld* world, const hkStepInfo& updatedStepInfo, hkJobThreadPool* threadPool, hkJobQueue* jobQueue, int numJobs );

			/// Simulate the given array of vehicles
		static void stepVehicleArraySynchronously( hkArray<hkpVehicleInstance*>& vehicles, hkpWorld* world, const hkStepInfo& updatedStepInfo, hkJobThreadPool* threadPool, hkJobQueue* jobQueue, int numJobs );

		//
		// Interface for asynchronous multithreading.
		//

		static int getBufferSize( hkArray<hkpVehicleInstance*>& vehicles );

		static void updateBeforeCollisionDetection( hkArray<hkpVehicleInstance*>& vehicles );

		static void buildVehicleCommand( hkpVehicleWheelCollide* wheelCollide, const hkpVehicleInstance* vehicle, hkpVehicleCommand* commandStorage, hkpVehicleJobResults* outputStorage );

		static int buildAndAddJobs( hkArray<hkpVehicleInstance*>& vehicles, const hkpWorld* world, const hkStepInfo& updatedStepInfo, int numJobs, hkJobQueue* jobQueue, void* buffer );

		static void getVehicleBatchFromBuffer( hkArray<hkpVehicleInstance*>& vehicles, void* vehicleBuffer, VehicleCommandBatch& commandBatch );

		static void stepVehiclesUsingJobResults( hkArray<hkpVehicleInstance*>& vehicles, const hkStepInfo& updatedStepInfo, void* buffer );

			/// Return the threshold number of vehicles that will get a speedup from running on multiple threads
		static int getMultithreadSpeedupThreshold( hkArray<hkpVehicleInstance*>& vehicles );

	public:

		hkpMultithreadedVehicleManager( hkFinishLoadedObjectFlag f ) : hkpVehicleManager( f ) { }
};

#endif // HK_VEHICLE_MULTITHREAD_MANAGER_H

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
