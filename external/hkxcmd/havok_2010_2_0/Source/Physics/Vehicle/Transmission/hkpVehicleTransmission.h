/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_TRANSMISSION_hkVehicleTRANSMISSION_XML_H
#define HKVEHICLE_TRANSMISSION_hkVehicleTRANSMISSION_XML_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkpVehicleTransmissionClass;

class hkpVehicleInstance;

/// The hkpVehicleTransmission of a vehicle is responsible for calculating all the
/// information associated with the gear system of a vehicle, including the
/// vehicle's current gear, and whether it is reversing. It also deals with the
/// torque and rpm transmission between the vehicle engine and the different wheels.
/// This component usually collaborates with the hkpVehicleEngine, the
/// hkpVehicleDriverInput and the hkpVehicleInstance in order to calculate its new
/// state.
class hkpVehicleTransmission : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();
	
			/// Container for data output by the transmission calculations.
		struct TransmissionOutput
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleTransmission::TransmissionOutput );
			
				/// The RPM transferred through the transmission.
			hkReal m_transmissionRPM;
			
				/// The torque transferred through the transmission.
			hkReal m_mainTransmittedTorque;
			
				/// The torqued transmitted to each wheel.
			hkReal* m_wheelsTransmittedTorque;
			
				/// The number of wheels.
			hkInt8 m_numWheelsTramsmittedTorque;

				/// Indicates if the vehicle is actually reversing at the moment.
			hkBool m_isReversing;

				/// The gear currently in. This variable is also stored in the vehicle.
			hkInt8 m_currentGear;

				/// Set true to use the clutch delay, i.e., to wait a short time before changing gear.
			hkBool m_delayed;

				/// The clutch delay specifies how long to wait before changing gear.
			hkReal m_clutchDelayCountdown;
		};

		//
		// Methods
		//

			/// Calculates information about the effects of transmission on the vehicle.
		virtual void calcTransmission(const hkReal deltaTime, const hkpVehicleInstance* vehicle, TransmissionOutput& transmissionOut) = 0;

	public:

		hkpVehicleTransmission(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}

	protected:

		hkpVehicleTransmission() {}
};

#endif // HKVEHICLE_TRANSMISSION_hkVehicleTRANSMISSION_XML_H

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
