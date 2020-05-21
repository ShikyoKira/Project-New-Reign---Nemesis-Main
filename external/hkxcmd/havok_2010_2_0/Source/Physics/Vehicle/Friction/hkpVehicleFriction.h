/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK2VEHICLEFRICTION
#define HK2VEHICLEFRICTION

#include <Common/Base/hkBase.h>

extern const class hkClass hkpVehicleFrictionDescriptionAxisDescriptionClass;

extern const class hkClass hkpVehicleFrictionStatusAxisStatusClass;

// This must be changed bellow in the carrays too
enum { HK_VEHICLE_FRICTION_N_AXIS = 2 };

/// Info about the time for our vehicle.
struct hkpVehicleStepInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleStepInfo );
	float m_deltaTime;
	float m_invDeltaTime;
};

extern const hkClass hkpVehicleFrictionDescriptionClass;

/// The structure hkpVehicleFrictionDescription is used by vehicle dynamics as a const description
/// of how to simulate the friction of a vehicle. This structure can be initialized using
/// hkVehicleFrictionDescriptionInitValues().
/// Please examine hkpVehicleFrictionDescription::Cinfo for details on how to setup this structure
/// and information on its various members.
struct hkpVehicleFrictionDescription
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ACTION, hkpVehicleFrictionDescription);
	
		/// A helper construction info class to fill in the values for a hkpVehicleFrictionDescription.
		/// All values should be specified in chassis local space.
	struct Cinfo
	{
			public:
				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleFrictionDescription::Cinfo );
			/// An approximation of the final wheel position of a standing car.
			/// This information is used to calculate some tables for the internal implicit integrator for
			/// the friction clipping.
			/// Note: If in the final game the wheels change position, this internal table can become slightly incorrect.
			/// However the effect is minimal (<1% in normal situations). If you are worried and you want to check the correctness
			/// of this function, you can call hkVehicleFrictionDescriptionInitValues every step with the correct wheel positions.
			/// (On PlayStation(R)2, this could be rather slow).
		hkVector4 m_wheelPosition[2];/*HK_VEHICLE_FRICTION_N_AXIS*/
		hkVector4 m_chassisCenterOfMass;
		hkVector4 m_directionRight;
		hkVector4 m_directionFront;
		hkVector4 m_directionUp;

		hkMatrix3 m_chassisFrictionInertiaInv;
		hkReal m_chassisMassInv;

			/// These are the inertias of the wheels around the axle.
		hkReal m_wheelAxleAngularInertia[2];/*HK_VEHICLE_FRICTION_N_AXIS*/
			/// These are the wheels' radius.
		hkReal m_wheelRadius[2];/*HK_VEHICLE_FRICTION_N_AXIS*/

			/// Equalizes the friction between front and back wheels: if set to 0, realistic behavior is obtained;
			/// if set to 1.0, the system redistributes the wheel down forces to be the down forces of a
			/// non moving, in its resting position car. Values between 0 and 1 interpolate between these two behaviors.
		hkReal m_frictionEqualizer;
	};


	//
	//	Internal use only
	//
	enum { hkFricionEllipseLineSegments = 16 };

	/// Used by the vehicle friction solver as a const description for a certain type of vehicle.
	struct AxisDescription
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleFrictionDescription::AxisDescription );
		HK_DECLARE_REFLECTION();
		//
		//	For friction clipping (internal optimization)
		//
		float	m_frictionCircleYtab[16];/*hkFricionEllipseLineSegments*/	
		float	m_xStep;
		float	m_xStart;

			// the rotation inertia of the wheel
		hkReal	m_wheelSurfaceInertia;
		hkReal	m_wheelSurfaceInertiaInv;

		hkReal  m_wheelChassisMassRatio; 

		hkReal  m_wheelRadius;
		hkReal  m_wheelRadiusInv;

			// the final wheel down force will be calculated using the following formula:
			// wheelDownForce = m_wheelDownForceFactor * wheelDownForce + m_wheelDownForceSumFactor * ( sumOfAllWheelDownForces)
		hkReal  m_wheelDownForceFactor;
		hkReal  m_wheelDownForceSumFactor;
	};

	hkReal m_wheelDistance;
	hkReal m_chassisMassInv;

	struct AxisDescription m_axleDescr[2];/*HK_VEHICLE_FRICTION_N_AXIS*/
};

extern const hkClass hkpVehicleFrictionStatusClass;

/// This class is used for persistent and export data of the vehicle friction solver
struct hkpVehicleFrictionStatus
{
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpVehicleFrictionStatus);

		struct AxisStatus
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, AxisStatus);

			//
			//	Export data
			//

			hkReal  m_forward_slip_velocity;	// [m/sec]
			hkReal  m_side_slip_velocity;       // [m/sec]
			hkReal  m_skid_energy_density;		// [N]
			hkReal	m_side_force;				// [N]
			
			//
			// For internal use only
			//
			hkReal m_delayed_forward_impulse;	// the to the next PSI delayed forward impulse
			hkReal m_sideRhs;
			hkReal m_forwardRhs;

			//
			// Friction circle
			//
				/// Friction force in sideways direction in the range 0..1 (>1 means the wheel is sliding)
			hkReal m_relativeSideForce;

				/// Friction force in forward direction in the range 0..1 (>1 means the wheel is sliding)
			hkReal m_relativeForwardForce;

			void init()
			{
				m_forward_slip_velocity = 0.0f;				// the velocity between a skidding tire and ground
				m_side_slip_velocity = 0.0f;
				m_skid_energy_density = 0.0f;				// the energy destroyed in one physical timestep by a skidding tire
				m_side_force = 0.0f;

				m_delayed_forward_impulse = 0.0f;
				m_sideRhs = 0.0f;
				m_forwardRhs = 0.0f;

				m_relativeSideForce = 0.0f;
				m_relativeForwardForce = 0.0f;
			}

		};
		
		struct AxisStatus m_axis[2];/*HK_VEHICLE_FRICTION_N_AXIS*/

		hkpVehicleFrictionStatus() 
		{
			m_axis[0].init(); 
			m_axis[1].init(); 
		}

		hkpVehicleFrictionStatus( hkFinishLoadedObjectFlag flag ) 
		{ 
			// skip initialization of m_axis[0/1]
		}

};

#endif //HK2VEHICLEFRICTION

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
