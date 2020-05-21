/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_GROUP_FILTER_H
#define HK_DYNAMICS2_GROUP_FILTER_H

#include <Physics/Collide/Filter/hkpCollisionFilter.h>

extern const hkClass hkpGroupFilterClass;


/// This is an example collision filter. It is open source and can be modified to produce a
/// filter which exactly matches your requirements. However, the implementation of the collision
/// filter can be a significant factor in performance of your physics. This filter is designed to
/// allow custom filtering between bones in different ragdolls or other constrained systems in your world,
/// by doing simple operations on two 32 bit numbers. The collision detection pipeline is setup to produce
/// these 32 bit numbers, called collision filter infos. Using this filter (or one very like it) for all
/// your filtering will give optimal cache performance, and we strongly recommend using this if possible.
///
/// It inherits from hkpCollisionFilter, and so implements each of the 4 hkpCollisionFilter::isCollisionEnabled
/// methods. For each method, it extracts two 32 bit values from the inputs (one representing each object to be
/// tested), and passes these values to a common filtering mechanism.
///
/// We split the 32 bit value into a layer (5 bits), a system group (16 bits), a subsystem id (5 bits),
/// and the id of another subsystem that we do not want to collide with (5 bits). The filter allows the user
/// to specify whether collisions are enabled or disabled between every pair of layers.
///
/// The exact logic of the filter is as follows:
///
///     - If the system group differs or is zero, lookup the pair of layers in a table.
///     - Else we have the same nonzero system group, check that both bodies are not set to ignore one another.
///       i.e. A.subSystemId != B.subSystemDontCollideWith and B.subSystemId != A.subSystemDontCollideWith.
///
/// Some things to keep in mind:
///
///     - Collisions between layer 0 and any other layer should not be disabled (by default all layers
///       collide with all other layers, including themselves). This is a policy decision for shape collection filtering.
///       All shape collections are required to implement hkpShapeCollection::getCollisionFilterInfo for each child shape.
///       By default this returns 0 (i.e., if no special child shape filter information is set).
///       So if you disable layer 0 in the group filter and use this filter as a shape collection filter,
///       it will disable collisions with children of all default shape collections.
///     - System group 0 is special in that if two objects of system group 0 are under consideration, their subsystem id
///       will not be considered. Instead, only the layer information will be used.
///
/// The class provides helper methods for getting and setting these values.
///
/// Examples:
///
///     - Keyframed objects should not collide with static geometry. Therefore
///       we can put all static geometry into one layer and all the keyframed object into
///       another layer. Then we disable the collision between these layers.
///     - All elements of a ragdoll should collide with each other, except those that are connected by
///       a constraint. In this case we assign all bodies of the ragdoll to the same system group. Also we assign
///       a unique subSystemId for each body in the ragdoll. Then we set subSystemDontCollideWith
///       to the id of the parent rigidbody (assuming you see the ragdoll as a hierarchy of bodies
///       and constraints, see the hkaRagdollUtils for details).
///
/// Note:
///
/// If two shape collections A and B collide, the following will happen
///     - The collidable of A and B will be checked.
///     - Then, one of the collections will be expanded, say in our case A will be replaced by
///       its children A# = { A0, A1, ... An }.
///     - Each collisionInfo of child A# (see hkpShapeCollection::getCollisionFilterInfo()) will
///       be tested against hkpCollidable::getCollisionFilterInfo() of B.
///     - Then for each A# B will be expanded into its children B# = {B0, B1, ...Bn}.
///     - Each collisionInfo of child A# (see hkpShapeCollection::getCollisionFilterInfo()) will be tested
///       against collisionInfo of child B#.
///
/// Interestingly, B was never tested against A#. That means if you have a complex shape collection hierarchy,
/// you think about your filters very carefully.
class hkpGroupFilter : public hkpCollisionFilter
{
	// we make the size of this class 256 bytes, so that the spu simulator always finds a valid allocated memory piece
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

		    /// A collision filter info value which tells the group filter to take the collision filter info from the root collidable.
		    /// Usually this will be used for the hkpExtendedMeshShape::m_defaultCollisionFilterInfo.
		enum { USE_COLLIDABLE_FILTER_INFO = 0xffffffff };

	public:

		HK_DECLARE_REFLECTION();

			/// Constructor, by default enables all collisions between all layers.
		hkpGroupFilter();

		~hkpGroupFilter();

			/// Enable the collision between layer layerA and layerB.
		void enableCollisionsBetween(int layerA, int layerB);

			/// Disables collisions between the layers A and B.
		void disableCollisionsBetween(int layerA, int layerB);

			/// Enables collisions between the specified layers.
			/// layerBitsA and layerBitsB are bitfields, one bit for every layer.
			/// e.g., to enable collisions between one layer and all other layers,
			/// call enableCollisionsUsingBitfield( 1<< myLayer, 0xfffffffe)
		void enableCollisionsUsingBitfield(hkUint32 layerBitsA, hkUint32 layerBitsB);

			/// Disables collisions between the specified collision layers.
			/// See enableCollisionsUsingBitfield for how to use bitfields.
		void disableCollisionsUsingBitfield(hkUint32 layerBitsA, hkUint32 layerBitsB);

			/// Combine the layer and systemGroup information into one 32 bit integer.
			/// This resulting collision filter info can be used in entities and hkEntityCinfo.
		static inline hkUint32 HK_CALL calcFilterInfo( int layer, int systemGroup = 0, int subSystemId = 0, int subSystemDontCollideWith = 0);

			/// Extract the layer from a given filterInfo.
		static inline int HK_CALL getLayerFromFilterInfo( hkUint32 filterInfo );

			/// Returns the filter info provided with the layer replaced by newLayer.
		static inline int HK_CALL setLayer( hkUint32 filterInfo, int newLayer );

			/// Extract the system group from a given filterInfo.
		static inline int HK_CALL getSystemGroupFromFilterInfo( hkUint32 filterInfo );

			/// Extract the subsystem id from a given filterInfo.
		static inline int HK_CALL getSubSystemIdFromFilterInfo( hkUint32 filterInfo );

			/// Extract the subSystemDontCollideWith from a given filterInfo.
		static inline int HK_CALL getSubSystemDontCollideWithFromFilterInfo( hkUint32 filterInfo );


			/// Creates a new unique identifier for system groups (maximum 65k).
		inline int getNewSystemGroup();

			/// Checks two collidables. This is the implementation of the hkpCollidableCollidableFilter::isCollisionEnabled method.
			/// This extracts a 32 bit value from each hkpCollidable, using the getCollisionFilterInfo() method.
		virtual hkBool isCollisionEnabled( const hkpCollidable& a, const hkpCollidable& b ) const;

			/// This is an implementation of hkpShapeCollectionFilter::isCollisionEnabled()
			/// This gets the 32 bit collision info for hkpCdBody "b" by calling hkpShapeContainer::getCollisionFilterInfo( bKey ).
			/// For body "a" we do one of two things, depending on whether a has a parent or not.
			///     -# If "a" does not have a parent, we know that it is not a child shape of a shape collection, and so we simply call
			///        hkpCollidable::getCollisionFilterInfo() on its root collidable (hkpCdBody::getRootCollidable).
			///     -# If "a" does have a parent, we know that it _may_ have a hkpShapeCollection in an ancestor hkpCdBody. We then travel up the hkpCdBody
			///        hierarchy until we reach the first hkpCdBody being a child of a hkpShapeCollection or hkpBvTreeShape (or we reach the root collidable and act as in point 1.). We then call
			///        hkpShapeCollection::getCollisionFilterInfo() using the found shape collection and the found child hkpCdBody.
		virtual	hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& a, const hkpCdBody& b, const HK_SHAPE_CONTAINER& bContainer, hkpShapeKey bKey  ) const;

			// hkpShapeCollectionFilter interface forwarding
		virtual hkBool isCollisionEnabled( const hkpCollisionInput& input, const hkpCdBody& collectionBodyA, const hkpCdBody& collectionBodyB, const HK_SHAPE_CONTAINER& containerShapeA, const HK_SHAPE_CONTAINER& containerShapeB, hkpShapeKey keyA, hkpShapeKey keyB ) const;

			/// This is an implementation of hkpRayShapeCollectionFilter::isCollisionEnabled()
			/// This gets the 32 bit collision info for the ray by getting hkpShapeRayCastInput::m_filterInfo (for aInput)
			/// To get the 32 bit collision info for the shape, we call hkpShapeCollection::getCollisionFilterInfo() for bCollection, using bKey.
		virtual hkBool isCollisionEnabled( const hkpShapeRayCastInput& aInput, const hkpShape& bShape, const HK_SHAPE_CONTAINER& bContainer, hkpShapeKey bKey ) const;

			/// This is an implementation of hkpRayCollidableFilter::isCollisionEnabled()
			/// This gets the 32 bit collision info for the ray by getting hkpWorldRayCastInput::m_filterInfo (for inputA)
			/// To get the 32 bit collision info for the collidableB, we simply call hkpCollidable::getCollisionFilterInfo() collidableB
		virtual hkBool isCollisionEnabled( const hkpWorldRayCastInput& inputA, const hkpCollidable& collidableB ) const;

			/// The actual filter implementation between two hkUint32 values. All the other isCollisionEnabled functions call this method.
			/// Returns true if the objects are enabled to collide, based on their collision groups.
		hkBool isCollisionEnabled(hkUint32 infoA, hkUint32 infoB) const;

	public:

		int m_nextFreeSystemGroup;
		hkUint32 m_collisionLookupTable[32];
		hkVector4 m_pad256[4];

	public:

		hkpGroupFilter( class hkFinishLoadedObjectFlag flag ) : hkpCollisionFilter(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_FILTER_GROUP;
			}
		}

		virtual int dummyUnused() { return 0; }
};

#include <Physics/Collide/Filter/Group/hkpGroupFilter.inl>



#endif // HK_DYNAMICS2_GROUP_FILTER_H

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
