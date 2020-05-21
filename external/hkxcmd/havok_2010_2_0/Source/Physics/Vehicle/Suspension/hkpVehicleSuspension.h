/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_SUSPENSION_hkVehicleSUSPENSION_XML_H
#define HKVEHICLE_SUSPENSION_hkVehicleSUSPENSION_XML_H


#include <Common/Base/hkBase.h>

#include <Physics/Vehicle/WheelCollide/hkpVehicleWheelCollide.h>

extern const class hkClass hkpVehicleSuspensionSuspensionWheelParametersClass;

extern const class hkClass hkpVehicleSuspensionClass;

class hkpVehicleInstance;

/// The hkpVehicleSuspension class is responsible for calculating and providing all
/// parameters related to the suspension, its forces and how they are applied.
/// Suspension forces have an important influence on a vehicle's driving behavior,
/// as do the position and length of the suspension. For a vehicle using the
/// hkpVehicleRaycastWheelCollide for collision detection, the
/// suspension component provides the basic information needed for the raycasting.
class hkpVehicleSuspension : public hkReferencedObject
{
	public:
	
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

		hkpVehicleSuspension() { }
		
		//
		// Methods
		//
		
			/// Calculates information about the effects of suspension on the vehicle.
			/// The caller of this method pre-allocates suspensionForceOut with a buffer the
			/// size of the number of wheels in the vehicle
		virtual void calcSuspension(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo, hkReal* suspensionForceOut) = 0;
		
		//
		// Members
		//
	public:
	
			///  Per wheel parameters
		struct SuspensionWheelParameters
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleSuspension::SuspensionWheelParameters );
			HK_DECLARE_REFLECTION();

			/// A point INSIDE the chassis to which the wheel suspension is attached.
			hkVector4 m_hardpointChassisSpace;

			/// The suspension direction (in Chassis Space).
			hkVector4 m_directionChassisSpace;

			/// The suspension length at rest i.e., the maximum distance from the hardpoint to
			/// the wheel center.
			hkReal m_length;
		};

			/// Suspension parameters for each wheel.
		hkArray<struct SuspensionWheelParameters> m_wheelParams;

	public:

		hkpVehicleSuspension(hkFinishLoadedObjectFlag f) : hkReferencedObject(f), m_wheelParams(f) {} 
};

#endif // HKVEHICLE_SUSPENSION_hkVehicleSUSPENSION_XML_H

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
