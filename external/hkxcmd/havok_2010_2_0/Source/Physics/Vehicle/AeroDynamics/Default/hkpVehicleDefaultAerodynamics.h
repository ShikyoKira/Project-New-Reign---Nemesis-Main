/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_AERODYNAMICs_DEFAULT_hkVehicleDefaultAerodynamics_XML_H
#define HKVEHICLE_AERODYNAMICs_DEFAULT_hkVehicleDefaultAerodynamics_XML_H

#include <Physics/Vehicle/AeroDynamics/hkpVehicleAerodynamics.h>

extern const class hkClass hkpVehicleDefaultAerodynamicsClass;

/// The hkpVehicleDefaultAerodynamics class implements an
/// aerodynamics component that accounts for the following: aerodynamic lift
/// (up/down force cause by pressure differences at high speeds),
/// aerodynamic drag (caused by air resistance) and extra gravity (non
/// physically-based force, but good for gameplay).
class hkpVehicleDefaultAerodynamics : public hkpVehicleAerodynamics
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// Default constructor
		hkpVehicleDefaultAerodynamics();
		
		//
		// Methods
		//
		
			/// Calculates the effect of aerodynamic forces on the vehicle.
		virtual void calcAerodynamics(const hkReal deltaTime, const hkpVehicleInstance* vehicle, AerodynamicsDragOutput& dragInfoOut);
		
			/// Calculates the drag force on the vehicle.
		virtual hkReal calcAerodynamicDrag(hkReal forwardVelocity) const;
		
			/// Calculates the lift force on the vehicle.
		virtual hkReal calcAerodynamicLift(hkReal forwardVelocity) const;
		
		//
		// Members
		//
	public:
		
			/// The density of the air that surrounds the vehicle, usually, 1.3 kg/m3.
		hkReal m_airDensity;
		
			/// The frontal area, in m2, of the car.
		hkReal m_frontalArea;
		
			/// The drag coefficient of the car.
		hkReal m_dragCoefficient;
		
			/// The lift coefficient of the car (it can be either positive or negative).
		hkReal m_liftCoefficient;
		
			/// An extra, fictional (non-physical) gravity force, to be applied to the body.
		hkVector4 m_extraGravityws;

	public: 
		hkpVehicleDefaultAerodynamics( hkFinishLoadedObjectFlag f ) : hkpVehicleAerodynamics(f) { }
	
};

#endif // HKVEHICLE_AERODYNAMICs_DEFAULT_hkVehicleDefaultAerodynamics_XML_H

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
