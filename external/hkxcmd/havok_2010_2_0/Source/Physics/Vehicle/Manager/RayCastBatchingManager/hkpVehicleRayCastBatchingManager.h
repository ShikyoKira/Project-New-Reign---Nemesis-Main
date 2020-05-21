/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_RAY_CAST_BATCHING_MANAGER_H
#define HK_VEHICLE_RAY_CAST_BATCHING_MANAGER_H

#include <Physics/Vehicle/Manager/hkpVehicleCastBatchingManager.h>
#include <Common/Base/Thread/Semaphore/hkSemaphoreBusyWait.h>
#include <Physics/Collide/Query/Multithreaded/RayCastQuery/hkpRayCastQueryJobs.h>

extern const class hkClass hkpVehicleRayCastBatchingManagerClass;

struct hkpShapeRayCastCommand;

/// Manages a set of vehicles whose wheelCollide objects are instances of hkpVehicleRayCastWheelCollide.
/// Ray casts can be batched and performed multithreaded.
class hkpVehicleRayCastBatchingManager : public hkpVehicleCastBatchingManager
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE );
		HK_DECLARE_REFLECTION();

			/// Constructor.
		hkpVehicleRayCastBatchingManager() { }

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

			/// The buffer is divided as follows:
		struct RaycastBatch
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpVehicleRayCastBatchingManager::RaycastBatch);
				/// A pointer to enough space for one command per wheel.
				/// This space is divided into chunks, with each chunk containing
				/// all the commands for the wheels of a single vehicle.
				/// The chunks are consecutive, so if some vehicles do not issue
				/// commands, there will be unused commands at the end.
			hkpShapeRayCastCommand* m_commandStorage;
				/// A pointer to enough space for one raycast output per wheel.
				/// The elements of this array correspond directly to the commands in
				/// m_commandStorage, and are arranged in the same way.
			hkpWorldRayCastOutput* m_outputStorage;
				/// A pointer to an array of the number of commands issued for each vehicle.
			hkUint8* m_index;
				/// The headers of the raycast jobs.
			hkpCollisionQueryJobHeader* m_jobHeaders;
		};

			/// Fills out a RaycastBatch structure with pointers into the provided buffer.
		void getRaycastBatchFromBuffer( void* raycastBuffer, int numJobs, RaycastBatch& raycastBatch, hkArray< hkpVehicleInstance* >& activeVehicles );

	public:
		hkpVehicleRayCastBatchingManager( hkFinishLoadedObjectFlag f ) : hkpVehicleCastBatchingManager(f) { }
};

#endif // HK_VEHICLE_RAY_CAST_BATCHING_MANAGER_H

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
