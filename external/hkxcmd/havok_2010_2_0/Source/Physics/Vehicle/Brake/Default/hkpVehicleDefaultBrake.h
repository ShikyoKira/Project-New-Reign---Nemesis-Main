/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_BRAKE_DEFAULT_hkVehicleDefaultBrake_XML_H
#define HKVEHICLE_BRAKE_DEFAULT_hkVehicleDefaultBrake_XML_H


#include <Physics/Vehicle/Brake/hkpVehicleBrake.h>

extern const class hkClass hkpVehicleDefaultBrakeWheelBrakingPropertiesClass;

extern const class hkClass hkpVehicleDefaultBrakeClass;

	
/// The class hkpVehicleDefaultBrake implements a braking system that is independent at
/// every wheel. For each wheel there is a maximum torque the brakes can apply,
/// which is scaled by the input from the brake pedal. In addition, wheels can
/// automatically block if the driver presses the brake pedal strongly enough for
/// a certain amount of time. Finally, if the driver is using the handbrake,
/// wheels connected to it will block.
class hkpVehicleDefaultBrake : public hkpVehicleBrake
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor
		hkpVehicleDefaultBrake();
		
		//
		// Methods
		//
		
			/// Calculates information about the effects of braking on the vehicle.
		virtual void calcBreakingInfo(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInput::FilteredDriverInputOutput& FilteredDriverInputOutput, WheelBreakingOutput& breakingInfo);
		
		//
		// Members
		//
	public:
		
			/// 
		struct WheelBrakingProperties
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleDefaultBrake::WheelBrakingProperties );
			HK_DECLARE_REFLECTION();

			/// The maximum torque the wheel can apply when braking. Increasing m_wheelsMass and
			/// m_wheelsMaxBrakingTorque for each wheel allows the vehicle to brake harder.
			hkReal m_maxBreakingTorque;

			/// The minimum amount of braking from the driver that could cause the wheel to
			/// block (range [0..1])
			hkReal m_minPedalInputToBlock;

			/// True if the particular wheel is connected to handbrake (locks when handbrake is
			/// on).
			hkBool m_isConnectedToHandbrake;
		};

			/// Array of braking properties for each of the wheels.
		hkArray<struct WheelBrakingProperties> m_wheelBrakingProperties;
		
			/// The time (in secs) after which, if the user applies enough brake input, the
			/// wheel will block.
		hkReal m_wheelsMinTimeToBlock;

	public:
		hkpVehicleDefaultBrake(hkFinishLoadedObjectFlag f) : hkpVehicleBrake(f), m_wheelBrakingProperties(f) { }
};

#endif // HKVEHICLE_BRAKE_DEFAULT_hkVehicleDefaultBrake_XML_H

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
