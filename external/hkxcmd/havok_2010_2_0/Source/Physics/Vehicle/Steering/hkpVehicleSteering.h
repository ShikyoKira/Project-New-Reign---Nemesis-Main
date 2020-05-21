/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_STEERING_hkVehicleSTEERING_XML_H
#define HKVEHICLE_STEERING_hkVehicleSTEERING_XML_H

#include <Common/Base/hkBase.h>
#include <Physics/Vehicle/DriverInput/hkpVehicleDriverInput.h>

extern const class hkClass hkpVehicleSteeringClass;

class hkpVehicleInstance;
	

/// An abstract component for vehicle steering for both the main (chassis) and
/// individual wheel steering angles. The hkpVehicleSteering class is responsible
/// for calculating the main steering angle of the vehicle and the steering angle of
/// each wheel. A typical implementation will be likely to use the steering wheel
/// input value from the vehicle's hkpVehicleDriverInput and transform it into a
/// steering angle.
class hkpVehicleSteering : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

		/// Container for data output by the steering calculations.
		struct SteeringAnglesOutput
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleSteering::SteeringAnglesOutput );
				/// The angle the steering wheel has been turned.
			hkReal m_mainSteeringAngle;

				/// The angle a wheel turns (if it can be steered).
			hkInplaceArray< hkReal, 32 > m_wheelsSteeringAngle;
		};

		//
		// Methods
		//
		
			/// Calculates information about the effects of steering on the vehicle.
		virtual void calcSteering(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredInfoOutput, SteeringAnglesOutput& steeringOutput ) = 0;

	public:

		hkpVehicleSteering(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}

	protected:

		hkpVehicleSteering() {}
};

#endif // HKVEHICLE_STEERING_hkVehicleSTEERING_XML_H

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
