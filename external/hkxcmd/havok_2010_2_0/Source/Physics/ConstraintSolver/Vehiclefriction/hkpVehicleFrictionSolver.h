/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK2VEHICLEFRICTIONSOLVER
#define HK2VEHICLEFRICTIONSOLVER

#include <Common/Base/hkBase.h>
#include <Physics/Vehicle/Friction/hkpVehicleFriction.h>
#include <Physics/ConstraintSolver/Accumulator/hkpVelocityAccumulator.h>

/// The class hkVehicleFrictionSolver is used is an input and output to the vehicle dynamics.
/// It holds all values which change every frame.
struct hkpVehicleFrictionSolverAxleParams
{

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ACTION, hkpVehicleFrictionSolverAxleParams);

		//
		//	Geometry of the car
		//
		hkVector4 m_contactPointWs;
		hkVector4 m_constraintNormalWs;
		hkVector4 m_forwardDirWs;

		//
		//	Ground Object
		//
		class hkpVelocityAccumulator* m_groundObject;
		class hkpVelocityAccumulator* m_groundObjectAtLastIntegration;

		//
		// Friction Info: 
		//  formula:   finalFriction = min( m_maxFrictionCoefficient, m_frictionCoefficient + slipVelocity * m_viscosityFrictionCoefficient );
		//             maxForce = finalFriction * m_wheelDownForce
		//
		hkReal	m_frictionCoefficient;
		hkReal	m_viscosityFrictionCoefficient;
		hkReal  m_maxFrictionCoefficient;
		hkReal	m_wheelDownForce;

			/// The net combined braking and transmission force applied at the surface of a wheel.
			/// If the total force applied at the wheel (forward force and side force) exceed a threshold then the wheel will slide.
		hkReal	m_forwardForce;

			/// This parameter defines how fast the vehicle tyres will slip (not slide)
			/// if the sideforce = downforce (that means the vehicle is standing on a 45 degree slope).
			/// The purpose of this parameter is to control the slip angle of the wheel:
			/// Assume you want to have a slip angle of 0.1 radians for a car cornering with 0.6g while driving
			/// at 20 meters/second. In this case you set m_slipVelocityFactor to (sin(0.1f) * 20.0f)/0.6g
			/// This parameter only works if the velocity of the car is greater than params.m_maxVelocityForPositionalFriction
		hkReal  m_slipVelocityFactor;

		//
		//	Handbrake Info
		//
		hkUint8	m_wheelFixed;

			/// Initializes all data members to default values.
		inline void initialize();
};



	/// The class hkpVehicleFrictionSolverParams is used by vehicle dynamics.
	/// See hkRaycastVehicle for more details.
struct hkpVehicleFrictionSolverParams
{

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_ACTION, hkpVehicleFrictionSolverParams);
		//
		// dynamic section
		//

		hkpVehicleFrictionSolverAxleParams m_axleParams[2];/*HK_VEHICLE_FRICTION_N_AXIS*/

		hkReal m_maxVelocityForPositionalFriction;

		class hkpVelocityAccumulator m_chassis;
		class hkpVelocityAccumulator m_chassisAtLastIntegration;
};

inline void hkpVehicleFrictionSolverAxleParams::initialize()
{
	m_contactPointWs.setZero4();
	m_constraintNormalWs.setZero4();
	m_forwardDirWs.setZero4();
	m_frictionCoefficient = 0.0f;
	m_viscosityFrictionCoefficient = 0.0f;
	m_maxFrictionCoefficient = 0.0f;
	m_wheelDownForce = 0.0f;
	m_forwardForce = 0.0f;
	m_wheelFixed = false;
	m_groundObject = HK_NULL;
	m_groundObjectAtLastIntegration = HK_NULL;
	m_slipVelocityFactor = 0.0f;
}


extern "C"
{
		/// Setup internal cache information hkpVehicleFrictionDescription for fast solving of the vehicle.
	void hkVehicleFrictionDescriptionInitValues( const hkpVehicleFrictionDescription::Cinfo& ci, hkpVehicleFrictionDescription& out);

		/// Solve the vehicle friction.
	void hkVehicleFrictionApplyVehicleFriction(	const hkpVehicleStepInfo& stepInfo,
												const hkpVehicleFrictionDescription& descr,
												hkpVehicleFrictionSolverParams &params,
												hkpVehicleFrictionStatus &status);
}




#endif //HK2VEHICLEFRICTIONSOLVER

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
