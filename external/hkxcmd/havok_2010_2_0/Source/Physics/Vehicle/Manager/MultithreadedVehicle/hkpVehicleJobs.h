/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VEHICLE_JOBS_H
#define HK_VEHICLE_JOBS_H

#include <Physics/Vehicle/hkpVehicle.h>
#include <Common/Base/Thread/JobQueue/hkJobQueue.h>

class hkStepInfo;

// The base class for all vehicle jobs
// Important: the 16bit m_jobType HAS to be the first member of this class and it HAS to be 16byte aligned!
// See hkJob for more details.
struct hkpVehicleJob : public hkJob
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpVehicleJob );

	enum JobSubType
	{
		VEHICLE_JOB_INTEGRATE,
		VEHICLE_JOB_END
	};

protected:

	HK_FORCE_INLINE hkpVehicleJob( JobSubType subType, hkUint16 size );
};

/// This is a helper struct which contains velocities to apply to the vehicle and ground objects (per axle) and also
/// impulses to apply to other objects that the vehicle is in contact with
struct hkpVehicleJobResults
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpVehicleJobResults);
	static const int s_maxNumWheels = 32;

	hkVector4 m_chassisLinearVel;
	hkVector4 m_chassisAngularVel;
	hkVector4 m_groundBodyLinearVel[2];
	hkVector4 m_groundBodyAngularVel[2];
	hkpRigidBody* m_groundBodyPtr[2];
	hkVector4 m_groundBodyImpulses[s_maxNumWheels];
};


HK_CLASSALIGN16(struct) hkpVehicleCommand
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CHARACTER, hkpVehicleCommand );
	
public:

	hkpVehicleJobResults*	m_jobResults;
};

/// A vehicle job will take an arbitrary number of vehicle commands.
struct hkpVehicleIntegrateJob : public hkpVehicleJob
{
public:

	friend struct hkpVehicleJobQueueUtils;

	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpVehicleIntegrateJob );

public:

	HK_FORCE_INLINE hkpVehicleIntegrateJob(	const hkpVehicleCommand* commandArray, 
											hkpVehicleInstance** vehicleArrayPtr,
											int numCommands, 
											const hkStepInfo& stepInfo );

		/// Only necessary on PlayStation(R)3.
	void setRunsOnSpuOrPpu();


protected:

	HK_FORCE_INLINE hkJobQueue::JobPopFuncResult popJobTask( hkpVehicleIntegrateJob& out );

public:

	const hkpVehicleCommand*					m_commandArray;			
	hkpVehicleInstance**						m_vehicleArrayPtr;
	int											m_numCommands;	
	hkStepInfo									m_stepInfo;
};

#include <Physics/Vehicle/Manager/MultithreadedVehicle/hkpVehicleJobs.inl>

#endif // HK_VEHICLE_JOBS_H

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
