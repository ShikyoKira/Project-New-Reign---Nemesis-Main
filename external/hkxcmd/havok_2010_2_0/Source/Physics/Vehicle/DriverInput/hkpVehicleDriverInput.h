/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_DRIVERINPUT_hkVehicleDRIVERINPUT_XML_H
#define HKVEHICLE_DRIVERINPUT_hkVehicleDRIVERINPUT_XML_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkpVehicleDriverInputStatusClass;

extern const class hkClass hkpVehicleDriverInputClass;

class hkpVehicleInstance;

///
class hkpVehicleDriverInputStatus : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

		hkpVehicleDriverInputStatus() {}
		virtual ~hkpVehicleDriverInputStatus() { }
		
		// Empty, so that we have a base type ptr (of at least some proper type)
		// to serialize in and back in again.

	public:
	
		hkpVehicleDriverInputStatus(hkFinishLoadedObjectFlag f) : hkReferencedObject(f) { }

			/// Clone this input.
		virtual hkpVehicleDriverInputStatus* clone() const = 0;
};

/// This class is an abstraction for the input of a driver to the controls of the
/// vehicle. State variables that store the input to the steering wheel, pedals,
/// and handbrake that this component calculates are stored in the hkpVehicleInstance.
/// Note that this component can be associated with a
/// device in the computer (such as a joystick or steering wheel), but can also be
/// associated with an AI controller, or a replay sequence.
class hkpVehicleDriverInput : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

			/// Container for data output by the driver input calculations.
		struct FilteredDriverInputOutput
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleDriverInput::FilteredDriverInputOutput );
			
				/// The input from the accelerator.
			hkReal m_acceleratorPedalInput;
			
				/// The input from the brake.
			hkReal m_brakePedalInput;
			
				/// The input from the steering wheel.
			hkReal m_steeringWheelInput;
			
				/// Indicates if the handbrake is on.
			hkBool m_handbrakeOn;

				/// Indicates that we think the vehicle is trying to reverse.
				///
				/// True if we are not in autoreverse mode, then we are tyring to reverse only
				/// when the driver hits the Reverse Button.
				///
				/// Otherwise, true if the break has been pressed and we are moving backwards,
				/// or
				///  - if we are reversing and moving we keep reversing
				///  - if we are reversing but stopped we keep reversing if the brake is pressed
				///  - if we are reversing, stopped and no brake is pressed, we don't reverse any more
				///
				///  Finally, if we are not reversing, we start reversing only if we are stopped and pressing the brake
			hkBool m_tryingToReverse;
		};

	
		//
		// Methods
		//

		hkpVehicleDriverInput() {}
		
			/// Calculates the effect of the driver's input on the vehicle.
		virtual void calcDriverInput(const hkReal deltaTime, const hkpVehicleInstance* vehicle, const hkpVehicleDriverInputStatus* deviceStatus, FilteredDriverInputOutput& filteredInputOut) = 0;

		hkpVehicleDriverInput(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}
};

#endif // HKVEHICLE_DRIVERINPUT_hkVehicleDRIVERINPUT_XML_H

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
