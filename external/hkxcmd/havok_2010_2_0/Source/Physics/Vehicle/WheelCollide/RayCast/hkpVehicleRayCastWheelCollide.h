/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKVEHICLE_COLLISIONDETECTION_DEFAULT_hkVehicleRaycastWheelCollide_XML_H
#define HKVEHICLE_COLLISIONDETECTION_DEFAULT_hkVehicleRaycastWheelCollide_XML_H

#include <Physics/Vehicle/hkpVehicleInstance.h>
#include <Physics/Vehicle/WheelCollide/hkpVehicleWheelCollide.h>
#include <Physics/Dynamics/Phantom/hkpPhantomOverlapListener.h>
#include <Physics/Vehicle/WheelCollide/RejectChassisListener/hkpRejectChassisListener.h>

extern const class hkClass hkpVehicleRayCastWheelCollideClass;

class hkpAabbPhantom;
class hkAabb;
struct hkpShapeRayCastCommand;
struct hkpWorldRayCastOutput;

/// Default implementation of hkpVehicleWheelCollide which uses raycasts to determine
/// wheel collision detection.
/// This component cannot be shared between vehicles.
/// It supports singlethreaded collision detection through the inherited collideWheels method,
/// and supports multithreaded raycasting with buildRaycastCommands.
class hkpVehicleRayCastWheelCollide : public hkpVehicleWheelCollide
{
	public:
		// +version(1)

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VEHICLE);
		HK_DECLARE_REFLECTION();

			/// Default constructor
		hkpVehicleRayCastWheelCollide();

		virtual ~hkpVehicleRayCastWheelCollide();
		
		//
		// Methods
		//
		
		virtual void init( const hkpVehicleInstance* vehicle );

		virtual void getPhantoms( hkArray<hkpPhantom*>& phantomsOut );

		virtual void updateBeforeCollisionDetection( const hkpVehicleInstance* vehicle );
	
		virtual hkpVehicleWheelCollide* clone( const hkpRigidBody& newChassis, const hkArray<hkpPhantom*>& newPhantoms ) const;

		virtual void addToWorld( hkpWorld* world );

		virtual void removeFromWorld();

			///	Sets the collisionFilterInfo value for both the phantom and the raycasts.
			/// Note: The change affects the raycasts immediately, but affects the phantom only as
			/// entities subsequently enter or leave. See hkpWorld::updateCollisionFilterOnPhantom
			/// for how to update the phantom immediately (particularly if the filter has been made
			/// more inclusive).
		virtual void setCollisionFilterInfo( hkUint32 filterInfo );

		//
		// For singlethreading
		//
		
			/// Implements the singlethreaded approach to wheel collisions by calling
			/// castSingleWheel and collideSingleWheelFromRaycast for each wheel.
		virtual void collideWheels( const hkReal deltaTime, const hkpVehicleInstance* vehicle, CollisionDetectionWheelOutput* cdInfoOut );
	
		//
		// For multithreading
		//

			/// Build raycast commands which can be batched and run multithreaded.
			/// \param vehicle the owning vehicle.
			/// \param collisionFilter the collision filter.
			/// \param filterSize the sizeof the provided collision filter object.
			/// \param commandStorage a pointer to enough space to hold commands for all the wheels.
			/// \param outputStorage a pointer to enough space to hold results for all the wheels.
			/// \return The number of command/result slots used (which will either be the number of wheels or zero).
		virtual int buildRayCastCommands( const hkpVehicleInstance* vehicle, const hkpCollisionFilter* collisionFilter, hkInt32 filterSize, hkpShapeRayCastCommand* commandStorage, hkpWorldRayCastOutput* outputStorage ) const;

	public:
			/// Calculates an AABB that encompasses the raycasts.
			/// \param vehicle the owning vehicle.
			/// \param aabbOut the resulting AABB.
		virtual void calcWheelsAABB( const hkpVehicleInstance* vehicle, hkAabb& aabbOut ) const;

			/// Perform a raycast for a single wheel. This implementation performs a castRay call
			/// on the hkpAabbPhantom.
		virtual void castSingleWheel( const hkpVehicleInstance::WheelInfo& wheelInfo, hkpWorldRayCastOutput& output ) const;

			/// Convert raycast results into collision detection results for a single wheel.
			/// \param vehicle the owning vehicle.
			/// \param wheelInfoNum the number of the wheel in the vehicle.
			/// \param raycastOutput the results obtains from raycasting.
			/// \param output stores the resulting CollisionDecectionWheelOutput obtained from the raycastOutput.
		virtual void getCollisionOutputFromCastResult( const hkpVehicleInstance* vehicle, hkUint8 wheelInfoNum, const hkpWorldRayCastOutput& raycastOutput, CollisionDetectionWheelOutput& output ) const;

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
		
			/// The phantom which encompasses all the wheel rays.
		hkpAabbPhantom* m_phantom;
		
			/// This hkpPhantomOverlapListener is added to the phantom to ignore the chassis.
		class hkpRejectChassisListener m_rejectRayChassisListener;

	public: 

		hkpVehicleRayCastWheelCollide(hkFinishLoadedObjectFlag f) :
			hkpVehicleWheelCollide(f), m_rejectRayChassisListener(f)
		{
			if( f.m_finishing )
			{
				m_type = RAY_CAST_WHEEL_COLLIDE;
			}
		}
};

#endif // HKVEHICLE_COLLISIONDETECTION_DEFAULT_hkVehicleRaycastWheelCollide_XML_H

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
