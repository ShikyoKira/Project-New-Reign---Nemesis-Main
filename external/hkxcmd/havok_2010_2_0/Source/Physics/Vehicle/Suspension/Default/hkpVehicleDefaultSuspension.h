/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_SUSPENSION_DEFAULT_hkVehicleDefaultSuspension_XML_H
#define HKVEHICLE_SUSPENSION_DEFAULT_hkVehicleDefaultSuspension_XML_H

#include <Physics/Vehicle/Suspension/hkpVehicleSuspension.h>

extern const class hkClass hkpVehicleDefaultSuspensionWheelSpringSuspensionParametersClass;

extern const class hkClass hkpVehicleDefaultSuspensionClass;

/// The hkpVehicleDefaultSuspension class implements a default, spring-based, suspension
/// module for vehicles.
class hkpVehicleDefaultSuspension : public hkpVehicleSuspension
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpVehicleDefaultSuspension() { }

		//
		// Methods
		//

			/// Calculates information about the effects of suspension on the vehicle.
		virtual void calcSuspension(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo, hkReal* suspensionForceOut);

		//
		// Members
		//
	public:


			/// A struct containing all the wheel spring suspension parameters.
		struct WheelSpringSuspensionParameters
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleDefaultSuspension::WheelSpringSuspensionParameters );
			HK_DECLARE_REFLECTION();

				/// The strength [N/m] of the suspension at each wheel.
			hkReal m_strength;

				/// The damping force [N/(m/sec)] of the suspension at each wheel.
			hkReal m_dampingCompression;

				/// The damping force [N/(m/sec)] of the suspension at each wheel.
			hkReal m_dampingRelaxation;
		};

			/// Suspension wheel parameters for each wheel.
		hkArray<struct WheelSpringSuspensionParameters> m_wheelSpringParams;

	public:
		hkpVehicleDefaultSuspension(hkFinishLoadedObjectFlag f) : hkpVehicleSuspension(f), m_wheelSpringParams(f) { }
};

#endif // HKVEHICLE_SUSPENSION_DEFAULT_hkVehicleDefaultSuspension_XML_H

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
