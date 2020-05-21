/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_VEHICLE_LINEAR_CAST_WHEEL_COLLIDE_H
#define HK_VEHICLE_LINEAR_CAST_WHEEL_COLLIDE_H

#include <Physics/Vehicle/WheelCollide/hkpVehicleWheelCollide.h>

#include <Physics/Vehicle/WheelCollide/RejectChassisListener/hkpRejectChassisListener.h>

extern const class hkClass hkpVehicleLinearCastWheelCollideWheelStateClass;

extern const class hkClass hkpVehicleLinearCastWheelCollideClass;

class hkpVehicleInstance;
class hkpRigidBody;
class hkpCylinderShape;
struct hkpPairLinearCastCommand;
struct hkpRootCdPoint;
class hkpAabbPhantom;
class hkCollidable;

/// This component manages the collision detection between the wheels and the
/// ground by casting shapes from the wheel hard-points down along the
/// suspension.
///
/// It maintains a phantom for each wheel to minimize the number of collidables
/// which need to be considered when linear casting. This is should reduce the
/// worst-case cost when the vehicle is driving over many collidables.
class hkpVehicleLinearCastWheelCollide : public hkpVehicleWheelCollide
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

			/// Default constructor.
		hkpVehicleLinearCastWheelCollide();

			/// Destructor
		virtual ~hkpVehicleLinearCastWheelCollide();

		//
		// Methods
		//

			/// Initialize the wheelCollide component.
			/// If shapes have not been provided using setWheelShapes, then this method
			/// will create cylinder shapes for each wheel.
		virtual void init( const hkpVehicleInstance* vehicle );

			/// Create a wheel shape of the appropriate size.
		hkpCylinderShape* createWheelShape( hkReal width, hkReal radius );

			/// Set the wheel collide object to use the provided shapes.
			/// Using this method before the init method allows the shapes to be shared
			/// within each vehicle and between many vehicles.
			/// The reference counts of the provided shapes will be incremented.
			/// \param vehicle the owning vehicle.
			/// \param shapes an array of pointers to shapes for the wheels.
		void setWheelShapes( const hkpVehicleInstance* vehicle, const hkArray<hkpShape*>& shapes );

		virtual void getPhantoms( hkArray<hkpPhantom*>& phantomsOut );

		virtual void updateBeforeCollisionDetection( const hkpVehicleInstance* vehicle );

		virtual hkpVehicleWheelCollide* clone( const hkpRigidBody& newChassis, const hkArray<hkpPhantom*>& newPhantoms ) const;

		virtual void addToWorld( hkpWorld* world );

		virtual void removeFromWorld();

			/// Sets the collisionFilterInfo value for both the phantoms and the raycasts.
			/// \note The change affects the linear casts immediately, but affects the phantoms only
			/// as entities subsequently enter or leave. See hkpWorld::updateCollisionFilterOnPhantom
			/// for how to update the phantoms immediately (particularly if the filter has been made
			/// more inclusive).
		virtual void setCollisionFilterInfo( hkUint32 filterInfo );

			/// Calls centerWheelContactPoint.
		virtual void wheelCollideCallback( const hkpVehicleInstance* vehicle, hkUint8 wheelIndex, CollisionDetectionWheelOutput& cdInfo );

			/// Move the contact point position to the center plane of the wheel.
			/// This is useful, as the contact point returned by the linear cast can flit from one side
			/// of the wheel to the other.
		void centerWheelContactPoint( const hkpVehicleInstance* vehicle, hkUint8 wheelIndex, CollisionDetectionWheelOutput& cdInfo ) const;

		//
		// For singlethreading
		//

		virtual void collideWheels( const hkReal deltaTime, const hkpVehicleInstance* vehicle, CollisionDetectionWheelOutput* cdInfoOut );

		//
		// For multithreading
		//

			/// Calculate the number of commands which need to be issued for this vehicle.
		virtual int getTotalNumCommands() const;

			/// Calculate the number of commands which need to be issued for the wheel.
		virtual int getNumCommands( hkUint8 numWheel ) const;

			/// Build raycast commands for the wheel.
			/// \param vehicle the owning vehicle.
			/// \param collisionFilter the collision filter.
			/// \param collidableStorage a pointer to space for a collidable for each of the vehicle's collidables.
			/// \param commandStorage a pointer to enough space to hold commands for all the collidables at the wheels.
			/// \param outputStorage a pointer to enough space to hold results for all the collidables at the wheels.
			/// \return The number of command/result slots used.
		virtual int buildLinearCastCommands( const hkpVehicleInstance* vehicle, const hkpCollisionFilter* collisionFilter, hkpCollidable* collidableStorage, hkpPairLinearCastCommand* commandStorage, hkpRootCdPoint* outputStorage ) const;

			/// Given linear cast outputs for the wheel, determine which is the nearest.
			/// \param numWheel the wheel involved.
			/// \param commandStorageOutput a pointer to the array of commands issued for this wheel.
			/// \return A pointer to the nearest result or HK_NULL if there was no hit.
		virtual const hkpRootCdPoint* determineNearestHit( hkUint8 numWheel, const hkpPairLinearCastCommand* commandStorageOutput ) const;

	public:
		//
		// Internal
		//

			/// Update the m_transform and m_to members of the WheelState.
			/// \param vehicle the owning vehicle.
			/// \param wheelNum the number of the wheel.
		void updateWheelState( const hkpVehicleInstance* vehicle, hkUint8 wheelNum );

			/// Information stored per wheel.
		struct WheelState
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpVehicleLinearCastWheelCollide::WheelState);
			HK_DECLARE_REFLECTION();

				/// A phantom which encompasses the linear cast for the wheel.
			hkpAabbPhantom* m_phantom;
				/// The wheel shape.
			const hkpShape* m_shape;
				/// The transform of the wheel in world space at the start of its linear cast.
			hkTransform m_transform;
				/// The end position of the linear cast in world space.
			hkVector4 m_to;
		};

			/// Calculates an AABB that encompasses the linear cast of the wheel, possibly excluding the top half
			/// of the wheel in its start position.
			/// \param vehicle the owning vehicle.
			/// \param wheelNum the number of the wheel.
			/// \param aabbOut the resulting AABB.
		void calcAabbOfWheel( const hkpVehicleInstance* vehicle, hkUint8 wheelNum, hkAabb& aabbOut ) const;
		
			/// Perform a linear cast for a single wheel.
			/// \param vehicle the owning vehicle.
			/// \param wheelNum the number of the wheel to be cast.
			/// \param linearCastOutput the result of the linear cast.
		virtual hkBool castSingleWheel( const hkpVehicleInstance* vehicle, hkUint8 wheelNum, hkpRootCdPoint& linearCastOutput ) const;

			/// Convert linear cast results into collision detection results for a single wheel.
			/// Unlike the case for raycasts, the result will not provide shape keys for
			/// the full shape hierarchy of the struck collidable. Only the first member
			/// of the m_contactShapeKey array will be valid and it will store the shape
			/// key of the struck subshape with respect to that subshape's parent.
			/// \param vehicle the owning vehicle.
			/// \param wheelNum the number of the wheel in the vehicle.
			/// \param linearCastOutput the results obtains from the linear casting.
			/// \param output stores the resulting CollisionDecectionWheelOutput obtained from the raycastOutput.
		virtual void getCollisionOutputFromCastResult( const hkpVehicleInstance* vehicle, hkUint8 wheelNum, const hkpRootCdPoint& linearCastOutput, CollisionDetectionWheelOutput& output ) const;

			/// Get collision results when the wheel is not touching the ground.
			/// \param vehicle the owning vehicle.
			/// \param wheelNum the number of the wheel in the vehicle.
			/// \param output stores the resulting CollisionDecectionWheelOutput.
		virtual void getCollisionOutputWithoutHit( const hkpVehicleInstance* vehicle, hkUint8 wheelNum, CollisionDetectionWheelOutput& output ) const;

		//
		// Members
		//
	public:
			/// The collision filter info of the wheels.
			/// This is initialized to 0 by the constructor. If a different value is needed,
			/// it should be assigned after construction but before calling init.
			/// After init, use setCollisionFilterInfo to modify its value.
		hkUint32 m_wheelCollisionFilterInfo;

			/// The per wheel information.
		hkArray<struct WheelState> m_wheelStates;

			/// This hkpPhantomOverlapListener is added to the phantom to ignore the chassis.
		class hkpRejectChassisListener m_rejectChassisListener;

			/// Parameters used when linear casting.
		hkReal m_maxExtraPenetration;
		hkReal m_startPointTolerance;

	public:
		hkpVehicleLinearCastWheelCollide(hkFinishLoadedObjectFlag f) :
			hkpVehicleWheelCollide(f), m_wheelStates(f), m_rejectChassisListener(f)
		{ 
			if( f.m_finishing )
			{
				m_type = LINEAR_CAST_WHEEL_COLLIDE;
			}
		}
};

#endif // HK_VEHICLE_LINEAR_CAST_WHEEL_COLLIDE_H

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
