/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKP_VEHICLE_AERODYNAMICS_H
#define HKP_VEHICLE_AERODYNAMICS_H

#include <Common/Base/hkBase.h>

extern const class hkClass hkpVehicleAerodynamicsClass;

class hkpVehicleInstance;

/// The hkpVehicleAerodynamics is responsible for providing the total force and
/// torque applied to the chassis by environmental forces. Examples of
/// environmental forces include aerodynamic drag and aerodynamic lift.
class hkpVehicleAerodynamics : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

			/// Container for data output by the aerodynamics calculations.
		struct AerodynamicsDragOutput
		{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleAerodynamics::AerodynamicsDragOutput );
				/// The total force applied by all aerodynamics forces.
			hkVector4 m_aerodynamicsForce;
			
				/// The total torque applied by all aerodynamics forces
			hkVector4 m_aerodynamicsTorque;
		};

		//
		// Methods
		//
		
			/// Calculates the effect of aerodynamic forces on the vehicle.
		virtual void calcAerodynamics(const hkReal deltaTime, const hkpVehicleInstance* vehicle, AerodynamicsDragOutput& dragInfoOut) = 0;

	public:

		hkpVehicleAerodynamics(hkFinishLoadedObjectFlag flag) : hkReferencedObject(flag) {}

	protected:

		hkpVehicleAerodynamics() {}
};

#endif // HKP_VEHICLE_AERODYNAMICS_H

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
