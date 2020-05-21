/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_VISUALIZE_SHAPE_GEOMETRY_H
#define HK_VISUALIZE_SHAPE_GEOMETRY_H

#include <Common/Base/hkBase.h>
#include <Common/Visualize/Shape/hkDisplayGeometryTypes.h>
struct hkGeometry;

/// A representation of a shape intended for visualization.
/// There are several subclasses corresponding to different shape types, but all can return
/// an hkpGeometry from their getGeometry() method. The geometry should be combined with
/// the transform returned by getTransform().
class hkDisplayGeometry : public hkReferencedObject
{
	public:
		
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DISPLAY);

			/// Construct a hkDisplayGeometry with a null hkGeometry.
		inline hkDisplayGeometry(hkDisplayGeometryType type);

			/// Destructor.
		virtual ~hkDisplayGeometry();

			/// Returns a representation of this object (which should be combined with the
			/// transform obtained from getTransform). You may need to call buildGeometry
			/// to ensure that the geometry has been built.
		inline const hkGeometry* getGeometry() const;
		inline hkGeometry* getGeometry();

			/// Return the transform.
		inline const hkTransform& getTransform() const;

			/// Set the transform.
			/// Not all hkpDisplayGeometry types can have a transform.
		inline void setTransform( const hkTransform& transform );

			/// Set the translation.
			/// Not all hkpDisplayGeometry types can have a transform.
		inline void setTranslation( const hkVector4& translation );

			/// Ensure that the object holds a valid hkGeometry.
		virtual void buildGeometry() = 0;

			/// Returns the type of this geometry (see hkpDisplayGeometryType).
		inline hkDisplayGeometryType getType() const;

			/// Set the hkGeometry member to HK_NULL so the destructor doesn't delete it.
		inline void releaseGeometry();

			/// method returns array of points s,e,s,e,s,e - start, end ...
		inline void getWireframeGeometry( hkArray<hkVector4>& lines )
		{ 
			getWireframeGeometry(lines, hkContainerHeapAllocator().get(&lines) );
		}

		virtual void getWireframeGeometry( hkArrayBase<hkVector4>& lines, hkMemoryAllocator& a ) = 0;

	protected:
			/// You typically call buildGeometry to ensure this is valid.
		hkGeometry* m_geometry;
		hkTransform m_transform;
		hkDisplayGeometryType m_type;
};

#include <Common/Visualize/Shape/hkDisplayGeometry.inl>

#endif // HK_GEOMETRY2_DISPLAY_GEOMETRY_H

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
