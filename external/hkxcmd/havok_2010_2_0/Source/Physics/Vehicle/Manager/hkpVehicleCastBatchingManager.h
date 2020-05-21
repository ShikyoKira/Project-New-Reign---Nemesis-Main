/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_CAST_BATCHING_MANAGER_H
#define HK_VEHICLE_CAST_BATCHING_MANAGER_H

#include <Physics/Vehicle/Manager/hkpVehicleManager.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>

extern const class hkClass hkpVehicleCastBatchingManagerClass;

class hkJobThreadPool;
class hkJobQueue;

/// An abstract class which provides a common interface to vehicle managers which support multithreaded
/// ray cast and linear cast batching.
class hkpVehicleCastBatchingManager : public hkpVehicleManager
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE );
		HK_DECLARE_REFLECTION();

			/// Constructor.
		hkpVehicleCastBatchingManager() : m_totalNumWheels( 0 ) { }

			/// Register a new vehicle with the manager.
		virtual void addVehicle( hkpVehicleInstance* vehicle );

			/// Remove a vehicle which has been registered with the manager.
		virtual void removeVehicle( hkpVehicleInstance* vehicle );

		//
		// Synchronous multithreading.
		//

			/// Build and perform cast jobs, and update and simulate the vehicles.
			/// The calling thread also performs casts (so numJobs should take that into account).
			/// Stack allocations are used for temporary buffer.
			/// This method assumes that a custom collision filter is not being used.
			/// This is a simple implementation which users may wish to reimplement with optimizations
			/// such as switching to singlethreading when m_totalNumWheels is below a certain threshold.
			/// \param numJobs the number of jobs to build.
			/// \param threadPool the threadPool.
			/// \param jobQueue the jobQueue.
		virtual void stepVehiclesSynchronously( hkpWorld* world, const hkStepInfo& updatedStepInfo, hkJobThreadPool* threadPool, hkJobQueue* jobQueue, int numJobs, hkArray< hkpVehicleInstance* >& activeVehicles );

		//
		// Interface for asynchronous multithreading.
		//

			/// Update all vehicles prior to collision detection.
		virtual void updateBeforeCollisionDetection( hkArray< hkpVehicleInstance* >& activeVehicles );

			/// Returns the size of the buffer (in bytes) required to holds the information for
			/// asynchronous casting.
			/// This should be called after updateBeforeCollisionDetection.
			/// \param numJobs the number of jobs intended be used.
		virtual int getBufferSize( int numJobs, hkArray< hkpVehicleInstance* >& activeVehicles ) = 0;

			/// Build and add a collection of cast jobs to a job queue.
			/// \param filterSize the size of the world's hkpCollisionFilter object.
			/// \param numJobs the number of jobs to add.
			/// \param jobQueue the queue to which the jobs should be added.
			/// \param semaphore the semaphore to attach to each job.
			/// \param buffer a buffer allocated by the caller to hold information for the casts.
			/// \return The number of jobs added (does not account for jobs splitting when there are too many commands
			/// for a job).
		virtual int buildAndAddCastJobs( const hkpWorld* world, hkInt32 filterSize, int numJobs, hkJobQueue* jobQueue, hkSemaphoreBusyWait* semaphore, void* buffer, hkArray< hkpVehicleInstance* >& activeVehicles ) = 0;

			/// Update and simulate all registered vehicles using the results in the buffer.
			/// \param buffer the buffer previously passed to buildAndAddCastJobs.
			/// \param numJobs the number of jobs which was used (this should be the same as the
			///   value passed to buildAndAddRaycastJobs).
		virtual void stepVehiclesUsingCastResults( const hkStepInfo& updatedStepInfo, int numJobs, void* buffer, hkArray< hkpVehicleInstance* >& activeVehicles ) = 0;

			/// A convenience method which returns an appropriate filterSize when a custom
			/// filter is not being used.
			/// \return sizeof( hkpGroupFilter ) if the world's collision filter is a group filter, otherwise zero.
		static hkInt32 HK_CALL getStandardFilterSize( const hkpWorld* world );

	public:
			/// The total number of wheels of all registered vehicles.
		hkUint16 m_totalNumWheels;

		hkpVehicleCastBatchingManager( hkFinishLoadedObjectFlag f ) : hkpVehicleManager(f) { }
};

#endif // HK_VEHICLE_CAST_BATCHING_MANAGER_H

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
