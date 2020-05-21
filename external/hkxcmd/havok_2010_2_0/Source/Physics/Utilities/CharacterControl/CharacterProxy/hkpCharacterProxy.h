/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CHARACTER_PROXY_H
#define HK_CHARACTER_PROXY_H

#include <Common/Base/Types/Physics/hkStepInfo.h>
#include <Physics/Dynamics/Entity/hkpEntityListener.h>
#include <Physics/Dynamics/Phantom/hkpPhantomListener.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControl.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxyCinfo.h>
#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxyListener.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTree.h>
#include <Common/Internal/Collide/Tree/AabbTree/hkAabbTreeData.h>
#include <Physics/Dynamics/Entity/hkpRigidBody.h>

class hkpShapePhantom;
struct hkpRootCdPoint;
struct hkpSurfaceConstraintInfo;
struct hkpSimplexSolverOutput;
struct hkpLinearCastInput;
struct hkpCharacterProxyIntegrateCommand;
class hkpBroadPhase;
class hkKdTree;
class hkpTriggerVolume;
template<typename T> class hkLocalArray;

	#include <Physics/Collide/Query/Collector/PointCollector/hkpAllCdPointCollector.h>
	class hkpAllCdPointCollector;
	#define HK_COLLECTOR_TYPE	hkpAllCdPointCollector
	#define HK_MANIFOLD_ARRAY_TYPE		hkArray<hkpRootCdPoint>
	#define HK_VIRTUAL_ON_PPU	virtual

/// The character proxy class is used to represent a non penetrating shape that can move dynamically around
/// the scene. It is called character proxy because it is usually used to represent game characters. It could
/// just as easily be used to represent any dynamic game object.
class hkpCharacterProxy :	public hkReferencedObject,
	public hkpEntityListener,
	public hkpPhantomListener
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CHARACTER);

			/// Initialize members from the construction info.
			/// Add references to each of the shapes.
		hkpCharacterProxy(const hkpCharacterProxyCinfo& info);

			/// Remove references to the phantom
			/// Remove the phantom if it has been added to the world
		virtual ~hkpCharacterProxy();

			/// Get construction info from this character proxy. Note that this info will reflect the current state of this object, not its original construction info.
		void getCinfo(hkpCharacterProxyCinfo& info) const;

			/// Update from a given construction info.
		void updateFromCinfo( const hkpCharacterProxyCinfo& cinfo );

			/// Update and move the character. To override the logic defining which bodies the character can collide
			/// with or capture this information for your own use (e.g., to deal with 'trigger' volumes), use ::integrateWithCollectors() instead.
			/// By default ::integrate() uses hkAllCdPointCollectors for the character's collision queries.
			/// The worldGravity parameter is only used when the character's mass is greater than 0, to apply downward impulses to dynamic objects.
		void integrate(const hkStepInfo& stepInfo, const hkVector4& worldGravity);

			/// Update and move the character. You must pass the two collectors used internally in the hkpShapePhantom::setPositionAndLinearCast()
			/// calls. Implementing your own hkpAllCdPointCollector will allow you to customize the character's movement behavior
			/// or extract information about objects it overlaps with or objects it linear casts through.
			/// The gravity parameter is only used when the character's mass is greater than 0, to apply downward impulses to dynamic objects.
		void integrateWithCollectors(const hkStepInfo& stepInfo, const hkVector4& gravity, hkpAllCdPointCollector& castCollector, hkpAllCdPointCollector& startPointCollector);

			/// Check and see if the character is supported in the given direction (and by what).
			/// This call checks the geometry immediately around the character, and does not take velocity into account.
			/// i.e., if the character is moving away from the ground, but is still touching the ground, this function
			/// will return SUPPORTED as the supported state.
		void checkSupport(const hkVector4& direction, hkpSurfaceInfo& ground);

			/// This function is the same as the checkSupportFunction, except it allows you to pass your own collectors to filter
			/// collisions. You only need to call this if you are using the integrateWithCollectors call.
		void checkSupportWithCollector(const hkVector4& direction, hkpSurfaceInfo& ground, hkpAllCdPointCollector& startPointCollector);

			/// When hkpCharacterProxy::integrate() is called, or hkpWorld::stepDeltaTime() is called, the manifold
			/// information will become out of date. If you use the manifold in your state machine, and need it to be
			/// up to date, you should call this function first. Note that this function is called by checkSupport automatically,
			/// so if you call checkSupport just before your state machine, you do not need to call this function.
		void refreshManifold( hkpAllCdPointCollector& startPointCollector );

			/// Read access to the current manifold for the character
		const hkArray<hkpRootCdPoint>& getManifold() const;

			/// Get current position
		const hkVector4& getPosition() const;

			/// Warp the character (through walls etc.)
		void setPosition(const hkVector4& position);

			/// Gets the linear velocity
		const hkVector4& getLinearVelocity() const;

			/// Sets the velocity
		void setLinearVelocity( const hkVector4& vel );

			/// Get the character phantom
		hkpShapePhantom* getShapePhantom();

			/// Get the character phantom const version
		const hkpShapePhantom* getShapePhantom() const;

			/// The 3.0.0 version of check support
		void checkSupportDeprecated(const hkVector4& direction, hkpSurfaceInfoDeprecated& ground) const;

			/// Add a hkpCharacterProxyListener
		void addCharacterProxyListener(hkpCharacterProxyListener* listener);

			/// Remove a hkpCharacterProxyListener
		void removeCharacterProxyListener(hkpCharacterProxyListener* listener);



			/// General integrate function: called by the integrate function above, or directly by an hkpCharacterProxyIntegrateJob.
			/// \param command if non-null, then this function is being called by an hkpCharacterProxyIntegrateJob.
		void integrateImplementation( const hkStepInfo& stepInfo
			, const hkVector4& gravity
			, hkpCharacterProxyIntegrateCommand* command
			HK_CPU_ARG( hkpAllCdPointCollector& castCollector )
			HK_CPU_ARG( hkpAllCdPointCollector& startPointCollector )
			HK_SPU_ARG( const Broadphase* broadphase )
			HK_SPU_ARG( hkpSpuWorldLinearCastCollector* broadphaseCollector )
			);

protected:

			/// Update the manifold of contact points without calling collision detection
		HK_VIRTUAL_ON_PPU void updateManifold( const HK_COLLECTOR_TYPE& startPointCollector
			, const HK_COLLECTOR_TYPE& castCollector
			, HK_MANIFOLD_ARRAY_TYPE& manifold
			HK_CPU_ARG( hkArray<hkpRigidBody*>& bodies )
			HK_CPU_ARG( hkArray<hkpPhantom*>& phantoms )
			HK_CPU_ARG( hkBool isMultithreaded = false )
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			);

			/// Apply impulses to the entity. The command and maxInteractions arguments are used if multithreading.
			/// In this case, interaction impulses are stored in the command and should be applied after the multithreaded phase.
			/// If the number of interactions is less than the maximum, the pointer to the colliding body after the last one encountered
			/// is set to HK_NULL to delimit the number of interactions.
		void applySurfaceInteractions( const hkStepInfo& info
			, const hkVector4& gravity
			, hkpCharacterProxyIntegrateCommand* command
			, HK_MANIFOLD_ARRAY_TYPE& manifold
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			);

			/// Build surface constraint information from the point returned by a cast.
			/// Extract the information from a contact point returned by the linear caster.
			/// This information is translated into surface constraint information.
			/// This is the bridge between the collision detector and the character controller.
		HK_VIRTUAL_ON_PPU void extractSurfaceConstraintInfo(const hkpRootCdPoint& hit
			, hkpSurfaceConstraintInfo& surfaceOut
			, hkReal timeTravelled
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			) const;

			// Search the current manifold for this surface
			// returns -1 if the surface has not been found
		int findSurface( const hkpRootCdPoint& info
			, HK_MANIFOLD_ARRAY_TYPE& manifold
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			) const;

			// Defines a distance metric for keeping and discarding planes
			// Used to update the mnaifold
		inline hkReal surfaceDistance(const hkpRootCdPoint& p1
			, const hkpRootCdPoint& p2
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			) const;

			/// Extract the surface velocity at a point returned by a cast
		inline void extractSurfaceVelocity(const hkpRootCdPoint& hit
			, hkVector4& velocityOut
			HK_SPU_ARG( hkpSpuCharacterProxyCollisionCache* collisionCache )
			) const;

			/// Converts the distance stored as a fraction in the cast collector to a proper euclidean distance.
			/// The cast collector returns distances as a fraction of the cast.
			/// We need to convert these to Euclidean distances, and project them onto the
			/// normal of the collision plane. This replaces the fraction by the
			/// correct distance and moves the fraction to the normal.w component.
		inline void convertFractionToDistance( const hkpRootCdPoint* hits, int numHits, const hkVector4& displacement ) const;
		inline void convertFractionToDistance( const HK_COLLECTOR_TYPE& collector, const hkVector4& displacement ) const;

		// utility methods
		void fireContactAdded(const hkpRootCdPoint& point) const;
		void fireContactRemoved(const hkpRootCdPoint& point) const;
		void fireConstraintsProcessed( const HK_MANIFOLD_ARRAY_TYPE& manifold, hkpSimplexSolverInput& input ) const;
		void fireObjectInteraction( const hkpCharacterObjectInteractionEvent& input, hkpCharacterObjectInteractionResult& output);
		void fireCharacterInteraction( hkpCharacterProxy* otherProxy, const hkContactPoint& contact);
		inline void addEntityOrPhantom( const hkpCollidable* collidable, hkArray<hkpRigidBody*>& bodies, hkArray<hkpPhantom*>& phantoms );

			/// Set position and return time traveled.
		hkReal moveToLinearCastHitPosition(const hkpSimplexSolverOutput& output, const hkpRootCdPoint& hit, const hkpLinearCastInput& castInput, hkVector4& position);

public:
			/// used internally to update the manifold if a body is deleted
		void entityRemovedCallback( hkpEntity* entity );

			/// used internally to update the manifold if a phantom is deleted
		void phantomRemovedCallback( hkpPhantom* entity );

		void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;


	//
	// TriggerVolume handling.
	//

	public:
			/// Processes up to maxNumTriggerVolumes infos or until the m_triggerVolume is HK_NULL.
		void processTriggerVolumes( hkpTriggerVolume** triggerVolumeAndFlags, const int maxNumTriggerVolumes );

	protected:
			/// Remove trigger volumes from the collector, putting them in touchedTriggers and
			/// the corresponding hits in triggerHits. Does not preserve the order of hits in
			/// the collector
		void removeTriggerVolumes( HK_COLLECTOR_TYPE& collector, hkLocalArray<hkpTriggerVolume*>& touchedTriggers, hkLocalArray<hkpRootCdPoint>& triggerHits );

			/// Given an array of touchedTriggers, update the triggerVolumes info.
		void updateTriggerVolumes( hkLocalArray<hkpTriggerVolume*>& touchedTriggers, hkArrayBase<hkpTriggerVolume*>& triggerVolumeAndFlags );

			/// Update the triggerVolumeInfos array, given that the triggerVolume has been touched.
		void triggerVolumeTouched( hkpTriggerVolume* triggerVolume, hkArrayBase<hkpTriggerVolume*>& triggerVolumeAndFlags );

			/// Update the triggerVolumeInfos array, with the trigger volumes closer than distance.
		void updateNearTriggerVolumes( hkLocalArray<hkpTriggerVolume*>& touchedTriggers, hkLocalArray<hkpRootCdPoint>& triggerHits, hkArrayBase<hkpTriggerVolume*>& triggerVolumeAndFlags, hkReal distance );

	public:

		hkArray<hkpRootCdPoint>	m_manifold;

			/// Kept so we know what listeners to remove on destruction.
			/// When multithreaded, this array and the corresponding listeners are
			/// cleared prior to the job and set afterwards.
		hkArray<hkpRigidBody*>	m_bodies;

			/// Kept so we know what listeners to remove on destruction.
			/// When multithreaded, this array and the corresponding listeners are
			/// cleared prior to the job and set afterwards.
		hkArray<hkpPhantom*>	m_phantoms;

			/// We do not keep trigger volume contacts in the manifold, but instead keep the
			/// list of those trigger volumes we're overlapping here.
		hkArray<hkpTriggerVolume*> m_overlappingTriggerVolumes;

		hkVector4				m_velocity;

		hkVector4				m_oldDisplacement;

		hkpShapePhantom*		m_shapePhantom;

		hkReal					m_dynamicFriction;

		hkReal					m_staticFriction;

		hkVector4				m_up;

		hkReal					m_extraUpStaticFriction;

		hkReal					m_extraDownStaticFriction;

		hkReal					m_keepDistance;

		hkReal					m_keepContactTolerance;

		hkReal					m_contactAngleSensitivity;

		int						m_userPlanes;

		hkReal					m_maxCharacterSpeedForSolver;

		hkReal					m_characterStrength;

		hkReal					m_characterMass;

		hkArray<hkpCharacterProxyListener*> m_listeners;

		hkReal					m_maxSlopeCosine;

		hkReal					m_penetrationRecoverySpeed;

		int						m_maxCastIterations;

		bool					m_refreshManifoldInCheckSupport;

		friend class hkpCharacterProxyUtil;
};

#include <Physics/Utilities/CharacterControl/CharacterProxy/hkpCharacterProxy.inl>

#endif //HK_CHARACTER_PROXY_H

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
