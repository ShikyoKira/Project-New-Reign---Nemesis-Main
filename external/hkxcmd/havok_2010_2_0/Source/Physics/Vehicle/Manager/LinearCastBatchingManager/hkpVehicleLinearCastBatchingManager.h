/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_LINEAR_CAST_BATCHING_MANAGER_H
#define HK_VEHICLE_LINEAR_CAST_BATCHING_MANAGER_H

#include <Physics/Vehicle/Manager/hkpVehicleCastBatchingManager.h>
#include <Physics/Collide/Query/Multithreaded/CollisionQuery/hkpCollisionQueryJobs.h>

extern const class hkClass hkpVehicleLinearCastBatchingManagerClass;

struct hkpShapeLinearCastCommand;

/// Manages a set of vehicles whose wheelCollide objects are instances of hkpVehicleLinearCastWheelCollide.
/// LinearCasts can be batched and performed multithreaded.
class hkpVehicleLinearCastBatchingManager : public hkpVehicleCastBatchingManager
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE );
		HK_DECLARE_REFLECTION();

			/// Constructor.
		hkpVehicleLinearCastBatchingManager() { }

		//
		// Interface for asynchronous multithreading.
		//

		virtual int getBufferSize( int numJobs, hkArray< hkpVehicleInstance* >& activeVehicles );

		virtual int buildAndAddCastJobs( const hkpWorld* world, hkInt32 filterSize, int numJobs, hkJobQueue* jobQueue, hkSemaphoreBusyWait* semaphore, void* buffer, hkArray< hkpVehicleInstance* >& activeVehicles );

		virtual void stepVehiclesUsingCastResults( const hkStepInfo& updatedStepInfo, int numJobs, void* buffer, hkArray< hkpVehicleInstance* >& activeVehicles );

	public:
		//
		// Internal
		//

			/// Returns the total number of commands which need to be issued for the vehicles.
		void getTotalNumCommands( hkArray< hkpVehicleInstance* >& activeVehicles, int& numCommands, int& numWheels ) const;

			/// The buffer is divided as follows:
		struct LinearCastBatch
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpVehicleLinearCastBatchingManager::LinearCastBatch);
				/// A pointer to enough space for one collidable per wheel.
			hkpCollidable* m_collidableStorage;
				/// A pointer to space for pair linear cast commands.
				/// Must be big enough for the number of commands returned by getTotalNumCommands().
			hkpPairLinearCastCommand* m_commandStorage;
				/// A pointer to enough space for one hkpRootCdPoint per command.
				/// The elements of this array correspond directly to the commands in
				/// m_commandStorage, and are arranged in the same way.
			hkpRootCdPoint* m_outputStorage;
				/// The headers of the LinearCast jobs.
			hkpCollisionQueryJobHeader* m_jobHeaders;
		};

			/// Fills out a LinearCastBatch structure with pointers into the provided buffer.
			/// \param numJobs the number of jobs which are intended to be built.
		void getLinearCastBatchFromBuffer( void* buffer, int numJobs, LinearCastBatch& batchOut, hkArray< hkpVehicleInstance* >& activeVehicles );

	public:
		hkpVehicleLinearCastBatchingManager( hkFinishLoadedObjectFlag f ) : hkpVehicleCastBatchingManager(f) { }
};

#endif // HK_VEHICLE_LINEAR_CAST_BATCHING_MANAGER_H

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
