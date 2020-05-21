/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_INSTANCE_H
#define HK_VEHICLE_INSTANCE_H

#include <Physics/Dynamics/Action/hkpUnaryAction.h>

#include <Common/Base/hkBase.h>

#include <Physics/Vehicle/Brake/hkpVehicleBrake.h>
#include <Physics/Vehicle/Engine/hkpVehicleEngine.h>
#include <Physics/Vehicle/TyreMarks/hkpTyremarksInfo.h>
#include <Physics/Vehicle/Steering/hkpVehicleSteering.h>
#include <Physics/Vehicle/Suspension/hkpVehicleSuspension.h>
#include <Physics/Vehicle/DriverInput/hkpVehicleDriverInput.h>
#include <Physics/Vehicle/AeroDynamics/hkpVehicleAerodynamics.h>
#include <Physics/Vehicle/Transmission/hkpVehicleTransmission.h>
#include <Physics/Vehicle/WheelCollide/hkpVehicleWheelCollide.h>
#include <Physics/Vehicle/VelocityDamper/hkpVehicleVelocityDamper.h>

#include <Physics/Vehicle/hkpVehicleData.h>

extern const class hkClass hkpVehicleInstanceWheelInfoClass;

extern const class hkClass hkpVehicleInstanceClass;

struct hkpVehicleFrictionSolverParams;
struct hkpVehicleFrictionSolverAxleParams;
class hkpVelocityAccumulator;
class hkStepInfo;
struct hkpVehicleJobResults;

/// This is the main class for a vehicle - it is a container for all the
/// runtime data the vehicle needs, and also contains pointers to all the
/// components that can be shared between different vehicles.
/// This class cannot be shared between different vehicles.
/// 
/// Because this class is an hkpUnaryAction, it can be added to the world
/// as an action. See the VehicleApi demos for examples of usage.
class hkpVehicleInstance : public hkpUnaryAction
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();
		
			/// This structure stores all data that is useful to the user and is shared
			/// among vehicle components.
		struct WheelInfo
		{
			//+version(1)
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_VEHICLE, hkpVehicleInstance::WheelInfo );
			HK_DECLARE_REFLECTION();

				/// The maximum number of shape keys which is reported by vehicle wheel collider (see also hkpShapeRayCastOutput::MAX_HIERARCHY_DEPTH)
			enum { MAX_NUM_SHAPE_KEYS = 8  };	

				/// The point of contact of the wheel with the ground (if the wheel is in contact with the ground).
			class hkContactPoint m_contactPoint;

				/// The friction coefficient at the point of contact.
			hkReal m_contactFriction;

				/// The ground body the vehicle is in contact. This value is HK_NULL
				/// if none of the wheels are in contact with the ground.
			hkpRigidBody* m_contactBody; //+nosave

				/// The shapeKey hierarchy of the object at the point of contact.
			hkpShapeKey m_contactShapeKey[hkpVehicleInstance::WheelInfo::MAX_NUM_SHAPE_KEYS];

				/// Location of the wheel hardPoint in world space
			hkVector4 m_hardPointWs;

				/// Farthest point on the wheel from the vehicle in the direction of the suspension.
			hkVector4 m_rayEndPointWs;

				/// Current length of the suspension.
			hkReal m_currentSuspensionLength;

				/// Current direction of the suspension (in world space).
			hkVector4 m_suspensionDirectionWs;

				/// Axis relative to the chassis that the wheel is spinning around.
			hkVector4 m_spinAxisChassisSpace;
			
				/// Axis in world space that the wheel is spinning around.
			hkVector4 m_spinAxisWs;

				/// The current rotation of the steering wheel.
			hkQuaternion m_steeringOrientationChassisSpace;
			
				/// The current spin velocity of the wheel in rad/s.
			hkReal m_spinVelocity;
			
				/// The current spin angle of the wheel in rads.
			hkReal m_spinAngle;
						
				/// The energy density lost when skidding (useful to implement tyre marks or skid sounds).
			hkReal m_skidEnergyDensity;
			
				/// The side force at the particular wheel.
			hkReal m_sideForce;
			
				/// The forward velocity lost by this particular wheel when slipping.
				/// This is the difference between the linear velocity of the wheels
				/// (angular velocity projected) and the actual velocity of the vehicle
			hkReal m_forwardSlipVelocity;
			
				/// The slip velocity in the side direction for the particular wheel.
			hkReal m_sideSlipVelocity;

			void init();
		};

			/// Default constructor, the chassis rigid body must be constructed already.
		hkpVehicleInstance( hkpRigidBody* chassis );

			/// Destructor
		virtual ~hkpVehicleInstance();
		
		//
		// Methods
		//
		
			/// Call to initialize any data that is derived from the initially setup data, such
			/// as the number of wheels on each axle.
		virtual void init();
	
			/// Obtain the phantom.
		virtual void getPhantoms( hkArray<hkpPhantom*>& phantomsOut );

			/// Applies the vehicle controller.
			/// Calls stepVehicle.
		virtual void applyAction( const class hkStepInfo& stepInfo );

			/// Update and simulate the vehicle.
		void stepVehicle( const class hkStepInfo& stepInfo );

			/// Update the wheels' hardpoints and the wheelCollide component's phantom with the
			/// vehicle current transform.
			/// This should be called before wheel collision detection.
		void updateBeforeCollisionDetection();

			/// Apply the calculated forces to the chassis and bodies in contact with the wheels
		void applyForcesFromStep( hkpVehicleJobResults& vehicleResults );

			/// Update and simulate the vehicle given the collision information.
		void stepVehicleUsingWheelCollideOutput( const hkStepInfo& stepInfo, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo, hkpVehicleJobResults& vehicleResults );		

			/// Clone functionality from hkpAction. Will make a new vehicle instance
			/// sharing as much data as it can.
		virtual hkpAction* clone( const hkArray<hkpEntity*>& newEntities, const hkArray<hkpPhantom*>& newPhantoms ) const;


		//
		// Adding and removing the vehicle to and from the world.
		//

			/// Add the vehicle's chassis and phantoms to the world.
			/// Does not add the vehicle instance as an action to the world.
		virtual void addToWorld( hkpWorld* world );

			/// Removes the vehicle's chassis and phantoms from the world.
			/// This hkpVehicleInstance is also removed since it's an hkpAction attached to the hkpRigidBody chassis.
		virtual void removeFromWorld();


		//
		// Functions to calculate useful information.
		//

			/// Calculate the current position and rotation of a wheel for the graphics engine.
		virtual void calcCurrentPositionAndRotation( const hkpRigidBody* chassis, const hkpVehicleSuspension* suspension, int wheelNo, hkVector4& posOut, hkQuaternion& rotOut );

			/// Retrieves the current RPM of the vehicle
		virtual hkReal calcRPM() const;
		
			/// Retrieves the current speed of the vehicle in KM/H
		virtual hkReal calcKMPH() const;
		
			/// Retrieves the current speed of the vehicle in MP/H
		virtual hkReal calcMPH() const;
		
			/// Retrieve the velocity for a fixed ground object.
			/// You can override this function if you want to assign velocity to ground objects
			/// like speed pads
		virtual void handleFixedGroundAccum( hkpRigidBody* ground, hkpVelocityAccumulator& accum );
		
			/// Returns the number of wheels.
		inline hkUint8 getNumWheels() const;

	public:
			/// An upper bound useful for creating local arrays for wheels.
		static const hkUint8 s_maxNumLocalWheels = 16;

	public:

		//
		// Internal functions
		//

			/// Update the components of the vehicle, given the collision detection information.
			/// This puts data in the provided aerodynamicsDragInfo structure and the suspensionForceAtWheel and totalLinearForceAtWheel arrays which is needed
			/// by the simulation step.
		void updateComponents( const hkStepInfo& stepInfo, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo, hkpVehicleAerodynamics::AerodynamicsDragOutput& aerodynamicsDragInfo, hkArray<hkReal>& suspensionForceAtWheel, hkArray<hkReal>& totalLinearForceAtWheel );

			/// Calculate and apply forces to the chassis and the rigid bodies the vehicle is riding on.
		void simulateVehicle( const hkStepInfo& stepInfo, const hkpVehicleAerodynamics::AerodynamicsDragOutput& aerodynamicsDragInfo, const hkArray<hkReal>& suspensionForceAtWheel, const hkArray<hkReal>& totalLinearForceAtWheel, hkpVehicleJobResults& vehicleResults );		

	protected:

		// These methods update the state of the components prior to simulation.
		
		void updateWheels( hkReal deltaTime, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo );
		void updateDriverInput( hkReal deltaTime, hkpVehicleDriverInput::FilteredDriverInputOutput& filteredDriverInputInfo );
		void updateSteering( hkReal deltaTime, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredDriverInputInfo );
		void updateTransmission( hkReal deltaTime, hkpVehicleTransmission::TransmissionOutput& transmissionInfo );
		void updateEngine( hkReal deltaTime, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredDriverInputInfo, const hkpVehicleTransmission::TransmissionOutput& transmissionInfo );
		void updateBrake( hkReal deltaTime, const hkpVehicleDriverInput::FilteredDriverInputOutput& filteredDriverInputInfo, hkpVehicleBrake::WheelBreakingOutput& wheelBreakingInfo );
		void updateSuspension( hkReal deltaTime, const hkpVehicleWheelCollide::CollisionDetectionWheelOutput* cdInfo, hkArray<hkReal>& suspensionForces );
		void updateAerodynamics( hkReal deltaTime, hkpVehicleAerodynamics::AerodynamicsDragOutput& aerodynamicsDragInfo );
	
		// Setup accumulators for the simulation step

		void prepareChassisParams( const hkStepInfo& stepInfo, hkpVehicleFrictionSolverParams& frictionParams );
		void prepareAxleParams( hkReal deltaTime, const hkArray<hkReal>& suspensionForceAtWheel, const hkArray<hkReal>& totalLinearForceAtWheel, hkpRigidBody* groundBody[], hkpVehicleFrictionSolverParams& frictionParams, const hkStepInfo& stepInfo, hkpVelocityAccumulator groundAccum[], hkpVelocityAccumulator groundAccumAtIntegration[] );
		void getAxleParamsFromWheel( int w_it, hkReal linearForceAtWheel, hkReal suspensionForceAtWheel, hkReal estimatedCarSpeed, hkpVehicleFrictionSolverAxleParams& axle_params );

		// Apply forces to accumulators

		void applyAerodynamicDrag( const hkpVehicleAerodynamics::AerodynamicsDragOutput& aerodynamicsDragInfo, hkpVehicleFrictionSolverParams& frictionParams, const hkReal deltaTime );
		void applyVelocityDamping( const hkReal deltaTime, hkpVehicleFrictionSolverParams& frictionParams, const hkpVehicleVelocityDamper* damper );
		void applySuspensionForces( hkReal deltaTime, const hkArray<hkReal>& suspensionForceAtWheel, hkpRigidBody* groundBody[], hkpVehicleFrictionSolverParams& frictionParams, hkVector4* suspensionForcesOnGround );
		void applyDampingToAxleAccumulators( const hkStepInfo& stepInfo, hkpRigidBody** groundBodies, hkpVelocityAccumulator* accumulators, hkVector4* originalLinearVels, hkVector4* originalAngularVels );
		void applyDampingToChassisAccumulator( const hkStepInfo& stepInfo, hkpVelocityAccumulator& accumulator, const hkpMotion* motion );
		void getExtraTorqueFactor( hkReal deltaTime, hkpVelocityAccumulator& accumulatorForChassis ) const;
		void applyFrictionSolver( const hkStepInfo& stepInfo, hkpVehicleFrictionSolverParams& frictionParams );

		// Apply results of the friction solver.
		
		void calcChassisVelocities( hkpVelocityAccumulator& accumulatorForChassis, hkpVehicleJobResults& vehicleResults );
		void calcGroundBodyVelocities( hkReal deltaTime, const hkpVehicleFrictionSolverAxleParams axleParams[], hkpRigidBody* groundBody[], hkpVehicleJobResults& vehicleResults, hkVector4* originalLinearVels, hkVector4* originalAngularVels );
		void applyResultsToWheelInfo( hkReal deltaTime, const hkpRigidBody *const groundBody[], const hkpVehicleFrictionSolverParams& frictionParams );
		
		//
		// Members
		//
	public:

			/// Contains data about the vehicle that can be shared across several vehicles.
		class hkpVehicleData* m_data;
		
			/// The DriverInput for the vehicle.
		class hkpVehicleDriverInput* m_driverInput;
		
			/// The Steering for the vehicle.
		class hkpVehicleSteering* m_steering;
		
			/// The Engine for the vehicle.
		class hkpVehicleEngine* m_engine;
		
			/// The Transmission for the vehicle.
		class hkpVehicleTransmission* m_transmission;
		
			/// The Brake for the vehicle.
		class hkpVehicleBrake* m_brake;
		
			/// The Suspension for the vehicle.
		class hkpVehicleSuspension* m_suspension;
		
			/// The Aerodynamics for the vehicle.
		class hkpVehicleAerodynamics* m_aerodynamics;
		
			/// The Collision for the vehicle.
		class hkpVehicleWheelCollide* m_wheelCollide;
		
			/// The Tyremarks controller for the vehicle.
		class hkpTyremarksInfo* m_tyreMarks;

			/// The list of external vehicle controllers.
		class hkpVehicleVelocityDamper* m_velocityDamper;
	
			/// The WheelInfo class holds all wheel information generated externally (from the
			/// physics engine) such as each wheel's ground contact, sliding state, forces,
			/// contact friction etc.
		hkArray<struct WheelInfo> m_wheelsInfo;

		struct hkpVehicleFrictionStatus m_frictionStatus;

		
		//
		// Variables used by the components to cache data.
		// This is slightly ugly, but necessary until it is possible until a 
		// decent runtime cache manager can be constructed.
		//
		

		///	Current controller input state.
		class hkpVehicleDriverInputStatus* m_deviceStatus; 

		//		from brake
		hkArray<hkBool> m_isFixed;
		hkReal m_wheelsTimeSinceMaxPedalInput;

		//		from driver input
		hkBool m_tryingToReverse;

		//		from engine
		hkReal m_torque;
		hkReal m_rpm;

		//		from steering
		hkReal m_mainSteeringAngle;
		hkArray<hkReal> m_wheelsSteeringAngle;

		//		from transmission
		hkBool m_isReversing;
		hkInt8 m_currentGear;
		hkBool m_delayed;
		hkReal m_clutchDelayCountdown;

	public:

			/// Get a pointer to the vehicle's chassis.
		inline class hkpRigidBody* getChassis ( void ) const;

			/// Set the chassis of the vehicle.
			/// This should only be called before the vehicle has been added to the
			/// world.
		void setChassis ( class hkpRigidBody* chassis );
	
		hkpVehicleInstance( hkFinishLoadedObjectFlag f ) : hkpUnaryAction(f), m_wheelsInfo(f), m_frictionStatus(f), m_isFixed(f), m_wheelsSteeringAngle(f) { }
};

#include <Physics/Vehicle/hkpVehicleInstance.inl>

#endif // HK_VEHICLE_INSTANCE_H

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
