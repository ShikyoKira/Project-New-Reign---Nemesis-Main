/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_HKVEHICLEDATA_XML_H
#define HKVEHICLE_HKVEHICLEDATA_XML_H

#include <Common/Base/hkBase.h>
#include <Physics/Vehicle/Suspension/hkpVehicleSuspension.h>
#include <Physics/Vehicle/Friction/hkpVehicleFriction.h>

extern const class hkClass hkpVehicleDataWheelComponentParamsClass;

extern const class hkClass hkpVehicleDataClass;


/// Contains data about a vehicle that does not change after initialization.
/// This means that a hkpVehicleData class can be shared between multiple
/// vehicles.
class hkpVehicleData : public hkReferencedObject
{
	public:
		//+version(1)

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();
	
			/// Data about wheels that is shared between several components of the vehicle.
		struct WheelComponentParams
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleData::WheelComponentParams );
			HK_DECLARE_REFLECTION();

				/// The wheel radius (in meters).
			hkReal m_radius;
			
				/// The mass of the wheel.
			hkReal m_mass;

				/// The wheel width (in meters).
			hkReal m_width;
			
				/// The wheel friction coefficient. This will be multiplied by the friction value of
				/// the landscape to get the final value. Realistic values range from 0.5f to 1.0f.
				/// However, to get good gameplay, try values up to 4.0f.
			hkReal m_friction;
			
				/// An extra velocity dependent friction factor. This factor allows us to increase
				/// the friction if the car slides. NOTE: This is unrealistic for driving on roads
				/// but makes sense in some off road environments. It also allows the car to be
				/// tweaked for better handling. Values between 0.0f and 0.1f are reasonable.
			hkReal m_viscosityFriction;
			
				/// Clips the final friction. Formula: resulting friction = min( wheelsFriction +
				/// slipVelocity * wheelsViscosityFriction, wheelsMaxFriction). Values between
				/// wheelsFriction and wheelsFriction* 2.0f are reasonable.
			hkReal m_maxFriction;
			
				/// The slip angle of the tyre for a car where the cornering forces are one g
			hkReal m_slipAngle;
		
				/// The calculated suspension force is scaled by this constant before it is applied
				/// to any dynamic body the vehicle is driving on. Usually this should be set to
				/// 1.0. The acceleration the vehicle can inflict on a contact body due to this
				/// forceFeedback can be limited by setting the m_contactBodyAccelerationLimit
				/// parameter.
			hkReal m_forceFeedbackMultiplier;

				/// Limits the acceleration of any dynamic body the vehicle is driving over (due to the
				/// scaled calculated suspension force) to this value.
			hkReal m_maxContactBodyAcceleration;

				/// The axle the wheel is on.
			hkInt8 m_axle;
		};
		
			/// Default constructor
		hkpVehicleData();
		
			/// The init function must be called after all the components of the vehicle have been initialized.
		virtual void init( const hkArray<struct hkpVehicleSuspension::SuspensionWheelParameters>& suspensionWheelParams, hkpRigidBody* chassis);

		//
		// Members
		//
	public:
		
			/// The gravity direction used by the vehicle simulation.
		hkVector4 m_gravity;

			/// The number of wheels in the vehicle.
		hkInt8 m_numWheels;
		
			/// The local coordinate system of the chassis. The first column is the 'up' direction of
			/// the vehicle, the second column is the 'forward' direction and the third column is perpendicular
			/// to both. Positive rotations about the third column correspond to the wheel motion when going
			/// forward. This direction affects gear shifting, braking and accelerating.
		hkRotation m_chassisOrientation;
		
			/// Scales the torques applied by the simulation at the roll axis by the given
			/// factor.
		hkReal m_torqueRollFactor;
		
			/// Scales the torques applied by the simulation at the pitch axis by the given
			/// factor.
		hkReal m_torquePitchFactor;
		
			/// Scales the torques applied by the simulation at the yaw axis by the given
			/// factor.
		hkReal m_torqueYawFactor;
		
			/// An extra torque which is applied to the car when steering is pointing left or
			/// right.
		hkReal m_extraTorqueFactor;
		
			/// To avoid sliding at slow speeds or when the handbrake is turned on, we
			/// introduced a positional friction model (since Havok 2.2.1) into the vehicle
			/// friction solver. This extra friction is particular useful for slow driving
			/// cars. To avoid letting this friction influence fast cars, you can set
			/// maxVelocityForPositionalFriction to the maximum velocity, up to which you want
			/// this extra algorithm to be activated. Defines up to which speed the positional
			/// friction model will be used.
		hkReal m_maxVelocityForPositionalFriction;
		
			/// The rotation inertia in yaw direction for a car of the mass 1.0f kilo.
		hkReal m_chassisUnitInertiaYaw;
		
			/// The rotation inertia in roll direction for a car of the mass 1.0f kilo.
		hkReal m_chassisUnitInertiaRoll;
		
			/// The rotation inertia in pitch direction for a car of the mass 1.0f kilo.
		hkReal m_chassisUnitInertiaPitch;
		
			/// Specifies how the effect of dynamic load distribution is averaged with static
			/// load distribution. A value of 0.0f doesn't do any averaging (dynamic load
			/// distribution is fully taken into account). A value of 1.0f doesn't take
			/// dynamic load distribution, and the car behaves always as if it was in static
			/// load distribution. Values between 0.0f and 1.0f give a certain amount of both.
		hkReal m_frictionEqualizer;
		
			/// Change this value to clip the normal in suspension force calculations. In
			/// particular, when mounting a curb, the raycast vehicle calculations can produce
			/// large impulses as the rays hit the face of the step, causing the vehicle to
			/// spin around its up-axis. Setting this value to 0.2f produces considerably more
			/// stability.
		hkReal m_normalClippingAngleCos;

			/// Limits the mass ratio between the chassis and any dynamic body the vehicle is driving over.
			/// This prevents small objects from shooting away from an accelerating vehicle.
		hkReal m_maxFrictionSolverMassRatio;	//+default(30.0)
		
			/// Contains information that affects the behavior of each wheel.
		hkArray<struct WheelComponentParams> m_wheelParams;
		
			/// The number of wheels on each axle.
		hkArray<hkInt8> m_numWheelsPerAxle;

			/// For the friction: const data
		struct hkpVehicleFrictionDescription m_frictionDescription;

			/// The inertia used for friction calculation.
		hkVector4 m_chassisFrictionInertiaInvDiag;

			/// The vehicleData can be shared between multiple vehicles, so it should be
			/// initialized only once.
		hkBool m_alreadyInitialised;
	public:

		hkpVehicleData( hkFinishLoadedObjectFlag f ) : hkReferencedObject(f), m_wheelParams(f), m_numWheelsPerAxle(f) { }
};

#endif // HKVEHICLE_HKVEHICLEDATA_XML_H

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
