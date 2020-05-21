/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_PHYSICS_TO_SCENE_DATA_BRIDGE_H
#define HK_PHYSICS_TO_SCENE_DATA_BRIDGE_H

#include <Common/Serialize/Util/hkRootLevelContainer.h>

class hkpWorld;
class hkpRigidBody;
class hkxSceneDataContext;
class hkxScene;
class hkpWorldObject;

	/// Utility to link hkpWorldObjects to hkxNodes. Links are "scoped" to hkRootLevelContainers
	/// and are created if a physics object (phantom or rigid body) has the same name as an hkxNode.
class hkpPhysicsToSceneDataBridge : public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_TOOLS);
			
			/// Create a bridge between the given physics objects and scene objects.
		hkpPhysicsToSceneDataBridge( hkpWorld* physicsWorld, hkxSceneDataContext* sceneDataContext );

			/// Empty destructor
		virtual ~hkpPhysicsToSceneDataBridge() 
		{
		}

			/// Set the bridge's scene context to the given context.
		void setSceneDataContext( hkxSceneDataContext* sceneDataContext );

			/// Add the physics objects and meshes in the given container to the physics world, scene context, and bridge.
		void addRootLevelContainer( const char* name, hkRootLevelContainer* rootLevelContainer );		
		
			/// Remove the root level container with the given name from the physics world, scene context, and bridge.
		void removeRootLevelContainer( const char* name );

			/// Populate the given array with the scenes currently loaded in the bridge.
		void getLoadedScenes( hkArray<hkxScene*>& scenes );

			/// Sets the transforms of all rigid bodies in the bridge to the transforms they had when
			/// they we added to the bridge.
		void resetPhysicsTransforms() const;

			/// Sync the meshes in the bridge to match their corresponding physics object.
		void syncSceneDataToPhysics();

			/// Returns true if a container with the given name has already been added to the bridge, false otherwise.
		bool isContainerLoaded( const char* name );

			/// A structure containing a filename with its root level container
		struct RootLevelContainer
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkpPhysicsToSceneDataBridge::RootLevelContainer );

			hkStringPtr m_name;							///< The name of the file containing the container
			hkRootLevelContainer m_container;			///< The root level container in the file
		};

	protected:
			/// Collect all rigid bodies in the root level container and place them in rigidBodiesOut.
		void findAllRigidBodies( hkRootLevelContainer* rootLevelContainer, hkArray<hkpRigidBody*>& rigidBodiesOut );

			/// The scene data context modified by this bridge.
		hkxSceneDataContext* m_sceneDataContext;

			/// The physics world modified by this bridge.
		hkpWorld* m_physicsWorld;

			/// A array containing a list of containers that are currently loaded in the bridge.
		hkArray<RootLevelContainer> m_loadedContainers;

			/// Map storing the physics world objects and their corresponding mesh IDs in the bridge.
		hkPointerMap<hkpRigidBody*, hkUlong> m_rigidBodyToMeshIdMap;

			/// Map storing the physics world object's initial positions. Used to reset them.
		hkPointerMap<hkpRigidBody*, hkTransform*> m_rigidBodyToTransformMap;

			/// Map storing mesh IDs corresponding to the physics world objects and their scale and skew transform.
		hkPointerMap<hkUlong, hkMatrix4*> m_meshIdToScaleAndSkewTransformMap;
};

#endif

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
