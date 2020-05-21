/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_STEERING_DEFAULT_hkVehicleDefaultSteering_XML_H
#define HKVEHICLE_STEERING_DEFAULT_hkVehicleDefaultSteering_XML_H

#include <Physics/Vehicle/Steering/hkpVehicleSteering.h>

extern const class hkClass hkpVehicleDefaultSteeringClass;

	
/// The hkpVehicleDefaultSteering class implements a hkpVehicleSteering that maps the
/// steering input from the hkpVehicleDriverInput to a main steering angle, using a
/// velocity-dependant mapping (for high velocities the steering is reduced). The
/// steering for each wheel is either 0.0f (no steering) or equals the main steering
/// angle, depending on a hkBool value specified for each wheel.
///
/// The main steering angle is calculated using the following formula:
///
/// angle = input * maxAngle;
///
/// as input goes from -1 to 1, angle goes from -maxAngle to maxAngle .
class hkpVehicleDefaultSteering : public hkpVehicleSteering
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor
		hkpVehicleDefaultSteering();
		
		//
		// Methods
		//
		
			/// Calculates information about the effects of steering on the vehicle.
		virtual void calcSteering(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredInfoOutput, SteeringAnglesOutput& steeringOutput );
		
			/// Calculates the main steering angle.
		virtual void calcMainSteeringAngle( const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredInfoOutput, SteeringAnglesOutput& steeringOutput );

		//
		// Members
		//
	public:
		
			/// The maximum steering angle (valid for velocities less than
			/// m_maxSpeedFullSteeringAngle).
		hkReal m_maxSteeringAngle;
		
			/// The maximum speed the car still can use the m_maxSteeringAngle.
		hkReal m_maxSpeedFullSteeringAngle;
		
			/// For every wheel, should be true if the wheel is connected to steering.
		hkArray<hkBool> m_doesWheelSteer;

	public:
		hkpVehicleDefaultSteering(hkFinishLoadedObjectFlag f) : hkpVehicleSteering(f), m_doesWheelSteer(f) { }
};

#endif // HKVEHICLE_STEERING_DEFAULT_hkVehicleDefaultSteering_XML_H

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
