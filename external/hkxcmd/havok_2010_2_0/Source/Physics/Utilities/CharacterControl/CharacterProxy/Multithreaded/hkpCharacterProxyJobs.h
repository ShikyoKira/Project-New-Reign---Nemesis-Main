/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CHARACTER_PROXY_JOBS_H
#define HK_CHARACTER_PROXY_JOBS_H

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxyListener.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>

class hkKdTree;
class hkpBroadPhase;
struct hkpProcessCollisionInput;
class hkpCpuCharacterProxyCollector;
template <typename, typename> class hkAabbTree;
class hkpTriggerVolume;

typedef hkpCpuCharacterProxyCollector* (HK_CALL *hkCreateCdPointCollectorOnCpuFunc)	();

	/// Each character proxy job needs an hkpCharacterProxyJobHeader.
	/// Usually this is passed in through the job's constructor.
struct hkpCharacterProxyJobHeader
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpCharacterProxyJobHeader );

		// This member can only be accessed if the job queue is locked once its governing job has been added to the job queue.	
		HK_ALIGN16( mutable int m_openJobs );
};


///
/// The character proxy job class.
/// Important: the 16bit m_jobType HAS to be the first member of this class and it HAS to be 16byte aligned!
/// See hkJob for more details.
///
struct hkpCharacterProxyJob : public hkJob
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpCharacterProxyJob );

		enum JobSubType
		{
			CHARACTER_PROXY_JOB_INTEGRATE,
						/// The number of job subtypes.
			CHARACTER_PROXY_JOB_END
		};

		void atomicIncrementAndReleaseSemaphore() const;

	protected:

		HK_FORCE_INLINE hkpCharacterProxyJob( JobSubType subType, hkUint16 size );

	public:

				/// This semaphore is released once the original job (and all its spawned children)
				/// has finished.
		hkSemaphoreBusyWait*		m_semaphore;

				/// This header must be set for all jobs that potentially spawn additional jobs or
				/// that have been spawned by others
		hkpCharacterProxyJobHeader*	m_sharedJobHeaderOnPpu;

				/// The variable at this location will be incremented (atomically) when the job is
				/// done.
		hkUint32*					m_jobDoneFlag;

};

// =====================================================================================================================
// CHARACTER PROXY INTEGRATE
// =====================================================================================================================

	/// This is a helper struct based on hkpCharacterObjectInteractionResult which contains
	/// the impulse values that will later be applied to the colliding rigid body (m_collidingBody)
struct hkpCharacterProxyInteractionResults : hkpCharacterObjectInteractionResult
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCharacterProxyInteractionResults);
		/// The body to which we will later apply the impulse values from the base class
	hkpRigidBody* m_collidingBody;
};


	/// hkpCharacterProxyCommand contains the details for integrating a single character proxy.
HK_CLASSALIGN16(struct) hkpCharacterProxyIntegrateCommand
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpCharacterProxyIntegrateCommand );
		
	public:

		// ===================================================================
		// Input
		// ===================================================================

			/// One character per command
		hkpCharacterProxy*						m_character;

			/// The character's collidable.
		const hkpCollidable*					m_collidable;

			/// The maximum number of impulses to be applied to dynamic rigid bodies in a single
			/// integration step
		int										m_maxInteractions;

			/// The maximum number of trigger volumes.
		int										m_maxTriggerVolumes;

		// ===================================================================
		// Output
		// ===================================================================

			/// This is a pointer to the interaction results associated with the
			/// character-rigid_body collision. Each interaction result has a pointer to the
			/// colliding dynamic rigid body as well as the respective impulse to be applied.
		hkpCharacterProxyInteractionResults*	m_objectInteraction;

			/// A pointer to storage which the job will fill with trigger volume pointers,
			/// except the least significant bit records whether the character is still
			/// touching the trigger volume at the end of the frame.
		hkpTriggerVolume**						m_triggerVolumeAndFlags;

			/// The position of the character proxy shape phantom after integration
		hkVector4								m_position;

};

	/// An hkpCharacterProxyJob will take an arbitrary number of hkpCharacterProxyCommand.
	/// This job is able to split itself into two jobs if it holds more commands than the maximum
	/// allowed number that can be executed in one go.
struct hkpCharacterProxyIntegrateJob : public hkpCharacterProxyJob
{
	public:

		friend struct hkpCharacterProxyJobQueueUtils;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpCharacterProxyIntegrateJob );

		enum { MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK = 128 };			

	public:

			/// When creating an hkpCharacterProxyJob you must pass in an unique jobHeader as well as an array of commands.
			/// The supplied semaphore is released once all commands of this job have been completed and the job has been
			/// removed from the job queue. The number of commands that are grouped into one task is customizable.
		HK_FORCE_INLINE hkpCharacterProxyIntegrateJob( const hkpProcessCollisionInput* input
			, hkpCharacterProxyJobHeader*				jobHeader
			, hkSemaphoreBusyWait*						semaphore
			, const hkpCharacterProxyIntegrateCommand*	commandArray
			, int										numCommands
			, hkReal									deltaTime
			, hkReal									invDeltaTime
			, hkVector4Parameter						worldGravity
			, hkCreateCdPointCollectorOnCpuFunc		createAllCdPointCollectorOnCpuFunc		= HK_NULL
			, const hkpBroadPhase*											broadphase			= HK_NULL
			, const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	aabbTreeInt16		= HK_NULL
			, const hkKdTree*												kdTree				= HK_NULL
			, int	numCommandsPerTask = MAXIMUM_NUMBER_OF_COMMANDS_PER_TASK
			);
	

			/// Only necessary on PlayStation(R)3.
			/// Use this method to assign this job to be processed on the SPU or PPU. This is automatically set depending on what the job references.
			/// If it references objects which are not supported on the SPU
			/// this function will produce a warning (in debug) and the job will be processed on PPU.
		void setRunsOnSpuOrPpu();

protected:

		HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpCharacterProxyIntegrateJob& out );

public:

		const hkpCharacterProxyIntegrateCommand*	m_commandArray;			
				
		int											m_numCommands;	

			/// maximum # of commands per task; once this limit is breached a subjob is spawned
		const int									m_numCommandsPerTask;	

		const hkpProcessCollisionInput*				m_collisionInput;

			/// Simulation time step
		const hkReal								m_deltaTime;

			/// Simulation inverse time step
		const hkReal								m_invDeltaTime;

		hkVector4									m_worldGravity;

			/// On CPU, if you wish to implement a custom addCdPoint function it is necessary to
			/// supply a function pointer to a (factory) function which returns a new instance
			/// of a hkpCpuCharacterProxyCollector.
		hkCreateCdPointCollectorOnCpuFunc		m_createCdPointCollectorOnCpuFunc;

			/// On SPU, Linear casts are world linear casts and as such need one of the following.
			/// Implementation note: when the job is being processed on SPU, these pointers are
			/// overwritten with addresses in local storage.
		const hkpBroadPhase*											m_broadphase;
		const hkAabbTree<hkAabbTreeData::hkAabbTreeAabb16, hkUint16>*	m_aabbTreeInt16;
		const hkKdTree*													m_kdTree;
		
};

#include <Physics/Utilities/CharacterControl/CharacterProxy/Multithreaded/hkpCharacterProxyJobs.inl>

#endif // HK_CHARACTER_PROXY_JOBS_H

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
