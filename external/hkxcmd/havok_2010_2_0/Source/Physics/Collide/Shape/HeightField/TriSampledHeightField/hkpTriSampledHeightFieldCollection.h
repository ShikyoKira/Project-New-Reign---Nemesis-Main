/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_TRI_SAMPLED_HEIGHTFIELD_COLLECTION
#define HK_TRI_SAMPLED_HEIGHTFIELD_COLLECTION

#include <Common/Base/hkBase.h>
#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/HeightField/SampledHeightField/hkpSampledHeightFieldShape.h>

extern const hkClass hkpTriSampledHeightFieldCollectionClass;

class hkpSampledHeightFieldShape;
class hkpTriSampledHeightFieldBvTreeShape;
extern hkReal hkConvexShapeDefaultRadius;


/// You can use this shape to wrap a hkSampledHeightFieldBase shape. To use this, you must also combine this shape
/// with the hkpTriSampledHeightFieldBvTreeShape, i.e., create a hkpTriSampledHeightFieldBvTreeShape, passing
/// this shape into its constructor. This shape provides an alternative implementation to the sampled heightfield,
/// where a different collision algorithm is used, while maintaining the memory usage of the heightfield.
/// Instead of the approximate collision detection algorithm used by the heightfield shape (where colliding objects
/// are represented by sets of spheres), this shape converts the heightfield into triangles on the fly.
/// Collision detection may be slower, but will be fully exact.
/// Because the shapes pulled out during collision detection are actual hkTriangleShapes, they may need a radius
/// (which would not be necessary if using the approximate collision detection algorithm).
/// Ray casting is performed using the standard heightfield raycast implementation.
/// N.B. Per-triangle filtering can be done by inheriting and implementing the method hkpShapeCollection::getCollisionFilterInfo( hkpShapeKey key )
/// as required. The current implementation does not implement this method because the user can best decide where to store the filter info.
/// NOTE: Linear casting is performed using a simple implementation where the AABB of the linear cast is queried against
/// the heightfield, and all overlapping triangles are checked with a linear cast. WARNING: for long linear casts,
/// (particularly those parallel to the heightfield surface) this can be very slow.
class hkpTriSampledHeightFieldCollection : public hkpShapeCollection
{
	//+hk.ReflectedFile("hkpHeightField")

	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		HK_DECLARE_REFLECTION();

			/// The constructor takes a hkSampledHeightFieldBase, and a radius. Note that the heightfield
			/// does not have a radius by default, but when converting the heightfield into triangles for collision detection
			/// it is advisable to give the triangles a non-zero radius for best performance.
			/// This adds a reference to the hkSampledHeightFieldBase passed in.
		hkpTriSampledHeightFieldCollection( const hkpSampledHeightFieldShape* shape, hkReal radius = hkConvexShapeDefaultRadius );

		hkpTriSampledHeightFieldCollection( hkFinishLoadedObjectFlag flag ) : hkpShapeCollection( flag ), m_weldingInfo(flag)
		{
			if( flag.m_finishing )
			{
				m_type = HK_SHAPE_COLLECTION; 
				m_collectionType = COLLECTION_TRISAMPLED_HEIGHTFIELD;
			}
		}

			/// Removes a reference from the referenced heightfield.
		~hkpTriSampledHeightFieldCollection();

		//
		// hkpShapeCollection interface
		//

			/// Gets the first key for the collection.
			/// The key returned by this function can be used to create a triangle shape corresponding to the key.
			/// The mapping is as follows:
			/// The lowest bit is used for the triangle "flag" determining which triangle of the quad is being referenced.
			/// The next lowest 15 bits of the key are used for the x coordinate of the quad.
			/// The upper 16 bits of the key are used for the z coordinate of the quad.
			/// So you can extract subpart/triangle indices like this:
			/// \verbatim
			/// int xCoord = (key & 0x0000ffff) >> 1;
			/// int zCoord = key >> 16;
			/// int whichTriangle = (key & 1); // will be 0 or 1
			/// \endverbatim
			/// For this reason only heightfields of resolution less than 65535 are allowed to be used with this shape.
			/// For more detail please refer to the heightfield section of the user guide.
		virtual hkpShapeKey getFirstKey() const;

			/// Get the next key. This iterates over the heightfield's x-z grid. See getFirstKey() for a description of the
			/// key returned.
		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;

			// hkpShapeCollection interface implementation.
		HKP_SHAPE_VIRTUAL const hkpShape* getChildShapeImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key, hkpShapeBuffer& buffer ) HKP_SHAPE_VIRTUAL_CONST;
		
		HKP_SHAPE_VIRTUAL hkUint32 getCollisionFilterInfoImpl( HKP_SHAPE_VIRTUAL_THIS hkpShapeKey key ) HKP_SHAPE_VIRTUAL_CONST;

			// hkpShapeCollection interface implementation.
		virtual void initWeldingInfo( hkpWeldingUtility::WeldingType weldingType );

			// hkpShapeCollection interface implementation.
		virtual void setWeldingInfo(hkpShapeKey key, hkInt16 weldingInfo);


		//
		// hkpShape interface
		//

			// hkpShape interface implementation.
		HKP_SHAPE_VIRTUAL void getAabbImpl( HKP_SHAPE_VIRTUAL_THIS const hkTransform& localToWorld, hkReal tolerance, hkAabb& out ) HKP_SHAPE_VIRTUAL_CONST;
		
			// hkpShape interface implementation.
		virtual int calcSizeForSpu(const CalcSizeForSpuInput& input, int spuBufferSizeLeft) const;


			/// Get the heightfield referenced by the shape, DMAing from PPU if necessary
		const hkpSampledHeightFieldShape* getHeightFieldShape() const;

			/// Get the radius of this shape. This will be the radius of all triangles created by the shape.
		inline hkReal getRadius() const;

			/// Sets the radius of this shape.
		inline void setRadius(hkReal newRadius);

			// Gets the extrusion used by triangles returned from getChildShape.
		inline const hkVector4& getTriangleExtrusion() const;

			/// Sets the extrusion used by triangles returned from getChildShape.
		inline void setTriangleExtrusion(hkVector4Parameter e);
			
	protected:

			/// Utility method
		inline int getXFromShapeKey(hkpShapeKey key) const;
			/// Utility method
		inline int getZFromShapeKey(hkpShapeKey key) const;

			/// Gets the index into m_weldingInfo for a given shape key
		inline int getIndexFromShapeKey(hkpShapeKey key) const;


		const hkpSampledHeightFieldShape* m_heightfield;
		mutable int	m_childSize;	//+nosave

			/// Convex radius applied to all child triangles.
		hkReal m_radius;

			/// Stored information for welding triangles. These are computed by computeWeldingInfo();
			/// The welding type is always hkpWeldingUtility::WELDING_TYPE_ANTICLOCKWISE
		hkArray<hkUint16> m_weldingInfo;

		HK_FORCE_INLINE hkUint16 getWeldingInfo(hkpShapeKey key) const;

			/// Extrusion applied to all child triangles. See hkpTriangleShape::m_extrusion for more information.
		hkVector4 m_triangleExtrusion;

};

#include <Physics/Collide/Shape/HeightField/TriSampledHeightField/hkpTriSampledHeightFieldCollection.inl>

#endif // #ifndef HK_TRI_SAMPLED_HEIGHTFIELD_COLLECTION

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
