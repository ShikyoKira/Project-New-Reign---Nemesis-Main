/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_DRIVERINPUT_DEFAULT_hkVehicleDefaultAnalogDriverInput_XML_H
#define HKVEHICLE_DRIVERINPUT_DEFAULT_hkVehicleDefaultAnalogDriverInput_XML_H

#include <Physics/Vehicle/DriverInput/hkpVehicleDriverInput.h>

extern const class hkClass hkpVehicleDriverInputAnalogStatusClass;

extern const class hkClass hkpVehicleDefaultAnalogDriverInputClass;

/// The hkpVehicleDefaultAnalogDriverInput class implements a hkpVehicleDriverInput based on the input from an
/// analog device.
///
/// The communication between the analog device and the
/// hkpVehicleDefaultAnalogDriverInput object is done through a
/// hkpVehicleDriverInputAnalogStatus struct.
/// 
/// This is a container for the values obtained from the device. These
/// values must be updated externally: the data is "pushed" from the device.
/// This allows for updating the device data at a different frequency from
/// the simulation frequency.
/// 
/// The hkpVehicleDefaultAnalogDriverInput class transforms the status of the
/// analog device (x position, y position, handbrake/reverse button pressed)
/// into driver input variables.
class hkpVehicleDriverInputAnalogStatus : public hkpVehicleDriverInputStatus
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

		hkpVehicleDriverInputAnalogStatus() { }

		// so that the scripts know its virtual
		virtual ~hkpVehicleDriverInputAnalogStatus() { }

		/// The current position, defined from -1 to 1, in the X axis of the device
		hkReal m_positionX;

		/// The current position, defined from -1 to 1, in the Y axis of the device
		hkReal m_positionY;

		/// True if the handbrake button is pressed
		hkBool m_handbrakeButtonPressed;

		/// True if the reverse button is pressed
		hkBool m_reverseButtonPressed;

	public:
		hkpVehicleDriverInputAnalogStatus(hkFinishLoadedObjectFlag f) : hkpVehicleDriverInputStatus(f) { }

		virtual hkpVehicleDriverInputStatus* clone() const;
};

/// Default (analog) implementation of hkpVehicleDriverInput. See
/// hkpVehicleDriverInputAnalogStatus for more parameter details.
class hkpVehicleDefaultAnalogDriverInput : public hkpVehicleDriverInput
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();
	
			/// Default constructor
		hkpVehicleDefaultAnalogDriverInput();
		
		//
		// Methods
		//
		
			/// Calculates the effect of the driver input on the vehicle.
		virtual void calcDriverInput(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputStatus* deviceStatus, FilteredDriverInputOutput& filteredInputOut);
		
			/// Calculates the change in the accelerator caused by the driver input.
		virtual hkReal calcAcceleratorInput(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputAnalogStatus* deviceStatus, FilteredDriverInputOutput& input) const;
		
			/// Calculates the change in the brake caused by the driver input.
		virtual hkReal calcBrakeInput(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputAnalogStatus* deviceStatus, FilteredDriverInputOutput& FilteredDriverInputOutputOut) const;
		
			/// Calculates if the vehicle is trying to reverse, according to the driver input and the current chassis motion.
		virtual hkBool calcTryingToReverse(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputAnalogStatus* deviceStatus, FilteredDriverInputOutput& FilteredDriverInputOutputOut);
		
			/// 
		virtual hkReal calcSteeringInput(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputAnalogStatus* deviceStatus, FilteredDriverInputOutput& FilteredDriverInputOutputOut) const;
		
		//
		// Members
		//
	public:
		
			/// The input value, up to which the m_initialSlope is valid. For higher input
			/// values the factor is increased so that for an input value of 1.0f the output is
			/// also 1.0f.
		hkReal m_slopeChangePointX;
		
			/// The initial slope. Used for small steering angles.
		hkReal m_initialSlope;
		
			/// The deadZone of the joystick.
		hkReal m_deadZone;
		
			/// If true, the car will start reversing when the brake is applied and the car is
			/// stopped.
		hkBool m_autoReverse;

	public:
		hkpVehicleDefaultAnalogDriverInput(hkFinishLoadedObjectFlag f) : hkpVehicleDriverInput(f) { }
};

#endif // HKVEHICLE_DRIVERINPUT_DEFAULT_hkVehicleDefaultAnalogDriverInput_XML_H

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
