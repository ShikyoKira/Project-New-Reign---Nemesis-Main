/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PROXY_JOB_UTIL_H
#define HK_CHARACTER_PROXY_JOB_UTIL_H

#include <Physics/Utilities/CharacterControl/CharacterProxy/Multithreaded/hkpCharacterProxyJobs.h>

class hkpWorld;
class hkJobQueue;
class hkStepInfo;
class hkpCharacterProxy;
class hkpBroadPhase;
class hkKdTree;

/// Utility class for multithreading character proxies.
class hkpCharacterProxyJobUtil
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCharacterProxyJobUtil);
			/// Structure which describes the work to be done when multithreading character proxies.
		class JobData : public hkReferencedObject
		{
			public:
				HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ARRAY);

					/// Class info for the JobData class
				JobData( const hkStepInfo& stepInfo, hkpWorld* world );

			public:
					///	Multithreading data
				hkJobQueue* m_jobQueue;

					///	Multithreading data
				int									m_numJobs;

					/// The world
				hkpWorld* m_world;

					/// Pointer to the collision input (usually the world's)
				hkpProcessCollisionInput* m_collisionInput;

					/// The array of characters to be simulated with multithreading
				hkArray<hkpCharacterProxy*>* m_characters;

					/// This is the maximum number of character-rigid body interactions
					/// per-character that will be executed per simulation step. The number of
					/// interactions must be limited due to the fixed size of the command buffer
					/// that contains the per-character command information: see
					/// hkpCharacterProxyIntegrateCommand for more.
				hkUint8								m_maxInteractions;

					/// This is the maximum number of trigger volumes that can be picked up by a
					/// character during the step.
				hkUint8 m_maxTriggerVolumes;

					/// The gravity of the world
				const hkVector4 m_worldGravity;

					/// The step info (only m_deltaTime and m_invDeltaTime need be set)
				const hkStepInfo m_stepInfo;

					/// On CPU, if you wish to implement a custom addCdPoint function it is
					/// necessary to supply a function pointer to a (factory) function which
					/// returns a new instance of a hkpCpuCharacterProxyCollector.
				hkCreateCdPointCollectorOnCpuFunc m_createCdPointCollectorOnCpuFunc;			

				//
				// Broadphase: You need only supply these for SPU, as the world's broadphase will
				// be used by CPU jobs.
				//

					/// On SPU, world linear casts will be performed with this broadphase if
					/// supplied.
				hkpBroadPhase* m_broadphase;

					/// On SPU, world linear casts will be performed with this AABB tree if
					/// supplied.
				hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;

					/// On SPU, world linear casts will be performed with this KdTree if supplied.
					/// NOTE: hkpKdTreeWorldManager can change its m_kdTree pointer when
					/// updateFromWorld() is called, so remember to set this after updating.
				hkKdTree* m_kdTree;				

		};

		//
		// Interface for synchronous multithreading.
		//
			
			/// Build and perform integrate jobs. After jobs have been processed, update the broad
			/// phase AABBs from the characters and apply impulses to colliding bodies (if any)
			/// stored in the command. Stack allocations are used for temporary buffer.
		static void HK_CALL simulateCharactersSynchronously( class hkJobThreadPool* threadPool, const class JobData& mtData );

		//
		// Interface for asynchronous multithreading.
		//
			/// Returns the size of the buffer (in bytes) required to hold the information for
			/// asynchronous casting.
			/// \param numJobs the number of jobs intended be used.
		static int HK_CALL getBufferSize( const JobData& mtData );

			/// Build and add a collection of integrate jobs to a job queue.
			/// \param semaphore the semaphore to attach to each job.
			/// \param buffer a buffer allocated by the caller to hold information for the casts.
			/// \return The number of jobs added (does not account for jobs splitting when there
			/// are too many commands for a job).
		static void HK_CALL buildAndAddJobs( const JobData& mtData, hkSemaphoreBusyWait* semaphore, void* buffer );

			/// Update all characters using the results in the buffer.
			/// \param buffer the buffer previously passed to buildAndAddJobs.
		static void HK_CALL handleResults( const JobData& mtData , void* buffer );
	
	public:
		//
		// Internal
		//

			/// The buffer is divided as follows.
		struct CharacterJobBatch
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCharacterProxyJobUtil::CharacterJobBatch);
				/// Storage for the command
			hkpCharacterProxyIntegrateCommand* m_commandStorage;

				/// Storage for the character proxy
			hkpCharacterProxy* m_characterStorage;

				/// Storage for the character proxy's collidable
			hkpCollidable* m_collidableStorage;

				/// Storage for the impulses applied to the colliding dynamic rigid bodies
			hkpCharacterProxyInteractionResults* m_objectInteractionStorage;

				/// Storage for the trigger volumes.
			hkpTriggerVolume** m_triggerVolumeStorage;

				/// The headers of the character proxy jobs.
			hkpCharacterProxyJobHeader* m_jobHeaders;	
		};

			/// Fills out a CharacterJobBatch struct with pointers into the provided buffer.
		static void getCharacterJobBatchFromBuffer( const hkpCharacterProxyJobUtil::JobData& mtData, void* buffer, CharacterJobBatch& batchOut );
};

#endif // HK_CHARACTER_PROXY_JOB_UTIL_H

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
