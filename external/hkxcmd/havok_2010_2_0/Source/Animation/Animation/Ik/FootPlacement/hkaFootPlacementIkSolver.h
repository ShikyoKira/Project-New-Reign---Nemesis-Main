/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_HKA_FOOTPLACEMENT_H
#define INC_HKA_FOOTPLACEMENT_H

class hkaSkeleton;
class hkaPose;

/// This interface class defines a single method, castRay(), and it's used by the hkaFootPlacementIkSolver object to cast rays
/// into the application's world. If you are using Havok Physics, you can use the optimized ray cast functionality provided there,
/// including the ability to filter objects, etc.. or otherwise you can wrap your custom ray casting functionality with this interface.
class hkaRaycastInterface
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkaRaycastInterface);
		virtual ~hkaRaycastInterface() { }

			/// Abstract method, should be implemented by the user in a derived class. Given two points, "from" and "to", specified in
			/// world space, it should return whether a ray between those two points intersects the scene or not. If it does (if it returns true),
			/// the hitFractionOut parameter should return a value between (0..1) to specify the point of intersection with the ray (where 0 is the
			/// start ("from") of the ray and 1 the end ("to") of the ray). Also in that case the parameter normalWSOut should return the normal of
			/// the surface intersected, in world space.
		virtual hkBool castRay ( const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut );

			/// This interface is used instead of the above if hkaFootPlacementIkSolver::Input::m_useCollisionFilterInfo is true.
			/// This interface provides a collision filter word that your implementation can use to filter collisions.
		virtual hkBool castRay ( const hkVector4& fromWS, const hkVector4& toWS, hkUint32 collisionFilterInfo, hkReal& hitFractionOut, hkVector4& normalWSOut );

};

/// The Foot Placement solver tracks changes on the height of the ground. It then modifies the extension of a leg according to the
/// original (desired) distance to the ground. It also modifies the orientation of the foot over time
/// according to the original (desired) orientation of the foot and the slope of the ground. This original (desired) height and orientation are taken
/// from m_originalAnkleTransformMS, part of the Input structure. It also locks and unlocks the feet based on m_footPlantedAnkleHeightInMS.
/// This solver, in contrast with the other IK solvers, operates in a stateful manner, i.e., it keeps state between calls, and therefore must be instantiated for
/// each leg/lower limb to be solved over time. Because of that, instead of a static solve() method, there is a non-static doFootPlacement() method.
/// The solver operates with different sets of data. Setup data is passed on construction, and it contains information about the skeleton, joint/bone
/// indices, axis and limits. Alongside a pose, an hkaFootPlacementIkSolver::Input struct is passed on every call to doFootPlacement(), and contains information about
/// gains and current location of the model in world, as well as an interface to raycast functionality. An hkaFootPlacementIkSolver::Output struct is
/// returned, containing information regarding the amount of fix-up done by the solver in the vertical direction, which can be used by the application
/// to update the location of the model (i.e., update the pelvis).
class hkaFootPlacementIkSolver : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME );

		/// This struct is passed on construction of an hkaFootPlacementIkSolver, and contains information about
		/// the structure of the character as well as axis and limits for the joints.
		struct Setup
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaFootPlacementIkSolver::Setup );

			/*
			** Information about the skeleton
			*/
			
				/// Skeleton of the character to which we are applying foot placement
			const hkaSkeleton* m_skeleton;

				/// Index of the joint representing the hip (== bone representing the thigh)
			hkInt16 m_hipIndex;

				/// Index of the joint representing the knee (== bone representing the calf)
			hkInt16 m_kneeIndex;

				/// Index of the joint representing the ankle (== bone representing the foot)
			hkInt16 m_ankleIndex;

				/// Axis of rotation of the knee, in local space of the knee
			hkVector4 m_kneeAxisLS;

				/// The end of the foot, in the local space of the ankle/foot. If this is 0,0,0 (default), the
				/// length of the foot won't be considered (only one raycast will be used)
			hkVector4 m_footEndLS;

			/*
			** Information about the world and model spaces
			*/

				/// The UP direction in the world (from ground to sky), in world space
			hkVector4 m_worldUpDirectionWS;

				/// The UP direction in the model (from feet to head), in model space
			hkVector4 m_modelUpDirectionMS;

				/// The height of the ground where the model was created/animated, in model space. For example, if the root bone of the skeleton
				/// is the pelvis, then this is the distance pelvis-ground (a negative value). If the root bone of the skeleton is located at the ground, this is 0.
			hkReal m_originalGroundHeightMS;

			/*
			** Foot planted / raised heights
			*/

				/// The height of the ankle below which the foot is considered planted. Used to calculate gains.
			hkReal m_footPlantedAnkleHeightMS;

				/// The height of the ankle above which the foot is considered fully raised. Used to calculate gains.
			hkReal m_footRaisedAnkleHeightMS;

			/*
			** Foot Placement limits (used to clamp the IK results)
			*/
			
				/// Maximum height the ankle can reach (in model space)
			hkReal m_maxAnkleHeightMS;

				/// Minimum height the ankle can reach (in model space)
			hkReal m_minAnkleHeightMS;

				/// Limit the knee angle (to avoid knee popping artifacts). Default is -1 (180 deg).
			hkReal m_cosineMaxKneeAngle;

				/// Limit the hinge angle (to avoid knee artifacts). Default is 1 (0 deg).
			hkReal m_cosineMinKneeAngle;
			

			/*
			** Some other internal tweaking values
			*/

				/// When ray casting from the foot towards the ground, the (positive) distance, from the foot and in the up direction, where the ray starts. Default is 0.5(m), you may
				/// want to change this according to the scale of your character
			hkReal m_raycastDistanceUp;

				/// When ray casting from the foot towards the ground, the (positive) distance, from the foot and in the down direction, where the ray ends. Default is 0.8(m), you may
				/// want to change this according to the scale of your character
			hkReal m_raycastDistanceDown;

				/// If true, the foot/ankle will be locked and unlocked.
			bool m_useFootLocking;

				/// Constructor, sets defaults (mostly to invalid values to enforce proper setup)
			Setup();
		};

		/// This structure, passed to each call to doFootPlacement(), alongside the pose, contains information about the
		/// location of the model in world, the original/desired position(height) and orientation of the foot, and
		/// an interface to raycast functionality.
		struct Input
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaFootPlacementIkSolver::Input );

				/// The original position and orientation of the ankle (the one we base the foot placement on)
			hkQsTransform m_originalAnkleTransformMS;

				/// The transform that converts from model to world
			hkQsTransform m_worldFromModel;

				/// If false, foot placement will be eased out/kept off, otherwise it will be eased in/kept on
			bool m_footPlacementOn;

				/// An interface to ray cast functionality. The implementation should only hit objects that the user wants to
				/// do foot placement on. For example, it may ignore debris objects if the foot is not supposed to land on top of them.
			hkaRaycastInterface* m_raycastInterface;

			/*
			** GAINS
			*/
				/// Gain used when transitioning from foot placement on and off. Default value is 0.2f.
			hkReal m_onOffGain;

				/// Gain used when the ground height is increasing (going up). Default value is 1.0f.
			hkReal m_groundAscendingGain;

				/// Gain used when the ground height is decreasing (going down). Default value is 1.0f.
			hkReal m_groundDescendingGain;

				/// Gain used when the foot is fully planted (as defined in Setup::m_footPlantedAnkleHeightMS).
				/// Depending on the height of the ankle, a value is interpolated between m_footPlantedGain
				/// and m_footRaisedGain and then multiplied by the ascending/descending gain to give
				/// the final gain used. Default (and most common) value is 1.0f.
			hkReal m_footPlantedGain;

				/// Gain used when the foot is fully raise (as defined in Setup::m_footRaisedAnkleHeightMS).
				/// Depending on the height of the ankle, a value is interpolated between m_footPlantedGain
				/// and m_footRaisedGain and then multiplied by the ascending/descending gain to give
				/// the final gain used. Default value is 1.0f.
			hkReal m_footRaisedGain;

				/// Gain used when the foot becomes unlocked. When the foot goes from being locked to unlocked,
				/// there can be a gap between the previously locked foot position and the desired position.
				/// This gain is used to smoothly transition to the new foot position. This gain only affects
				/// the horizontal component of the position, since the other gains take care of vertical
				/// smoothing. Default value is 1.0f.
			hkReal m_footUnlockGain;

				/// If m_useCollisionFilterInfo is true, this member is passed into hkaRaycastInterface::castRay() when performing
				/// raycasts. You can use this if you want to use the same raycast interface for handling cases with
				/// different collision properties.
			hkUint32 m_collisionFilterInfo;

				/// Whether or not to pass m_collisionFilterInfo into hkaRaycastInterface::castRay().
			bool m_useCollisionFilterInfo;

				/// Constructor. It sets some defaults.
			Input();

		};

		/// This structure is filled by the foot placement solver, and contains information that can be used by the
		/// application logic
		struct Output
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_ANIM_RUNTIME, hkaFootPlacementIkSolver::Output );

				/// The normal of the ground where the foot hit the ground.
			hkVector4 m_groundNormal;

				/// A measure of how much displacement was required for the leg to reach its final location. It can
				/// be used to update the pelvis of the character
			hkReal m_verticalError;

				/// True if the foot placement detected ground under the foot
			bool m_hitSomething;
		};

			/// Constructor. You need an instance of hkaFootPlacementIkSolver for each leg to be solved. The Setup structure contains
			/// information about the character setup.
		hkaFootPlacementIkSolver(const Setup& setup);

			/// Call this method to perform the foot placement (usually every frame). It modifies the pose so the leg is placed at
			/// a reasonable location given the input and the environment.
		void doFootPlacement (const Input& input, Output& output, hkaPose& poseInOut);
		
			/// This is the same setup data passed on construction. It has been made public should you need to change any
			/// of the setup values on the fly, although this is uncommon.
		Setup m_setup;

	private:


		/*
		** Internal State Data
		*/

			// Current weight for the foot placement
		hkReal m_currentWeight;

			// Previous ground height, in world space
		hkReal m_previousGroundHeightWS;

			// Previous ground normal, in world space
		hkVector4 m_previousGroundNormalWS;

			// Previous vertical displacement applied to the foot
		hkReal m_previousVerticalDisplacement;

			// Whether the foot is currently locked to the ground or not
		hkBool m_isFootLocked;

			// The locked foot/ankle position.
		hkVector4 m_lockedFootPositionInWS;

			// The locked position of the end/toes of the foot.
		hkVector4 m_lockedFootEndPositionInWS;

			// When unlocking the foot the locked foot position and the new position could be different. The horizontal
			// component of this difference is stored in m_footUnlokcingOffset when the foot becomes unlocked. This is used 
			// to do blend smoothly from the locked position back to the position dictated by the animation.
		hkVector4 m_footUnlockingOffset;

			// Uses two rays to cast the foot into the ground
		bool castFoot( const Input& input, const hkVector4& footWS, const hkVector4& footEndWS, hkVector4& projectedFootWSOut, hkVector4& groundNormalWSOut ) const;

			// Cast a ray using the information in the Input.
		bool castRay( const Input& input, const hkVector4& fromWS, const hkVector4& toWS, hkReal& hitFractionOut, hkVector4& normalWSOut ) const;

};

#endif //INC_HKA_FOOTPLACEMENT_H

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
