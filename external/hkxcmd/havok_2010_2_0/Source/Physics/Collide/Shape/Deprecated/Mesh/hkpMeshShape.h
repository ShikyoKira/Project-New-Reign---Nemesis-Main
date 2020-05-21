/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_MESH_SHAPE_H
#define HK_COLLIDE2_MESH_SHAPE_H

#include <Physics/Collide/Shape/Compound/Collection/hkpShapeCollection.h>
#include <Physics/Collide/Shape/Convex/Triangle/hkpTriangleShape.h>

extern const class hkClass hkpMeshShapeSubpartClass;

extern hkReal hkConvexShapeDefaultRadius;
extern const hkClass hkpMeshShapeClass;

class hkpMeshMaterial;
class hkpSimpleMeshShape;
class hkpMoppBvTreeShape;

/// DEPRECATED. This shape will be removed in future releases of Havok Physics.
///
/// A class for wrapping geometric collision detection information. It can directly reference
/// sets of triangle strips with vertex striding, and either 16 or 32 bit indices to vertices.
/// It can also directly reference triangle soups, using three indices per triangle rather than one.
/// It also handles degenerate triangles internally, so no extra checking is required by the user.
/// The mesh shape creates hkTriangleShapes in the hkpShapeBuffer passed in to the getChildShape function.
/// It gives these shapes a radius (see hkpConvexShape::getRadius()) as specified by the hkpMeshShape::getRadius() function.
class hkpMeshShape: public hkpShapeCollection
{
	//+hk.ReflectedFile("hkpMeshShape")
	public:

		HK_DECLARE_REFLECTION();

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO);


			/// Constructs a new hkpMeshShape.
			/// "numBitsForSubpart" is the number of bits used (in the 32 bit shape key) for the subpart index.
			/// The remaining bits from the 32 bit shape key are used for the triangle index. By
			/// default numBitsForSubpartIndex is 12, which means the mesh shape can have 2^12 - 1
			/// subparts (0xffffffff is the "invalid" shape key) = 4095, and each subpart can have 2^20 triangles = 1048576.
			/// The subpart is stored in the high bits, so you can extract subpart/triangle indices like this:
			///    - int subPart = key >> ( 32 - mymesh->getNumBitsForSubpartIndex() );
			///    - int triIndex = key & ( 0xffffffff >> mymesh->getNumBitsForSubpartIndex() );
		hkpMeshShape( hkReal radius = hkConvexShapeDefaultRadius, int numBitsForSubpartIndex = 12 );
			

			// hkpShapeCollection::setWeldingInfo interface implementation.
		virtual void setWeldingInfo( hkpShapeKey key, hkInt16 weldingInfo);

			// hkpShapeCollection::initWeldingInfo interface implementation.
		virtual void initWeldingInfo( hkpWeldingUtility::WeldingType weldingType );


		//
		// Subpart access
		//

		struct Subpart;

			/// Adds a subpart. To modify this subpart later on, call getSubpartAt(int ).xxxx = xxxx.
			/// If the \a part added has no material stored, an empty (all zero) material is created and stored with the part.
		virtual void addSubpart( const Subpart& part );

			/// Returns the number of subparts.
		inline int getNumSubparts() const;

			/// Gets read/write access to a subpart.
		inline Subpart& getSubpartAt( int i );

			/// Gets const access to a subpart.
		inline const Subpart& getSubpartAt( int i ) const;

			/// Gets the subpart that a shape key belongs to.
		inline const Subpart& getSubPart( hkpShapeKey shapeKey ) const;

			/// Gets the number of bits of a shape key used to encode the subpart.
		inline hkInt32 getNumBitsForSubpartIndex() const;

		//
		// Scaling and radius access
		//

			/// Set the scaling of the mesh shape
		void setScaling( const hkVector4& scaling );

			/// Get the scaling of the mesh shape
		inline const hkVector4&	getScaling() const;


			/// Gets the extra radius for every triangle.
		inline hkReal getRadius() const;

			/// Sets the extra radius for every triangle.
		inline void setRadius(hkReal r );

		//
		// hkpShape Collection interface
		//


			/// Get the first child shape key.
		virtual hkpShapeKey getFirstKey() const;

			/// This function implements hkpShapeCollection::getNextKey
			/// NOTE: This function calls hkpTriangleUtil::isDegenerate to make sure no keys for degenerate triangles are returned
			/// If you are implementing your own mesh shape, your getNextKey function must make sure that it similarly does
			/// not return keys for degenerate triangles. You can use the hkpTriangleUtil::isDegenerate utility function to check whether
			/// triangles are valid.
		virtual hkpShapeKey getNextKey( hkpShapeKey oldKey ) const;


			/// Because the hkpMeshShape references into client data,
			/// it must create a new hkpTriangleShape to return to the caller when this function is called.
			/// This triangle is stored in the char* buffer.
			/// Degenerate triangles in the client data are handled gracefully through this method.
		const hkpShape* getChildShape( hkpShapeKey key, hkpShapeBuffer& buffer ) const;

			/// Gets the mesh material by shape key or -1 if there is no material indices.
		inline int getMaterialIndex( hkpShapeKey key ) const;

			/// Gets the mesh material by shape key, or returns HK_NULL if m_materialIndexBase isn't defined.
			/// Note that addSubpart() might have created an empty (all zero) material.
		inline const hkpMeshMaterial* getMeshMaterial( hkpShapeKey key ) const;

			/// Returns getMeshMaterial(key)->m_filterInfo or zero if there is no material for the key.
		virtual hkUint32 getCollisionFilterInfo( hkpShapeKey key ) const;


		//
		// hkpShape interface
		//

			/// A precise but not very fast implementation of getting an AABB.
 		void getAabbImpl( const hkTransform& localToWorld, hkReal tolerance, hkAabb& out  ) const;


        virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls) const;

	public:

			/// The striding of mesh indices
		enum MeshShapeIndexStridingType
		{
			INDICES_INVALID, // default, will raise assert.
				/// 16 bit "short" striding
			INDICES_INT16,
				/// 32 bit "int" striding
			INDICES_INT32,
			INDICES_MAX_ID
		};

		enum MeshShapeMaterialIndexStridingType
		{
			MATERIAL_INDICES_INVALID,
			MATERIAL_INDICES_INT8,
			MATERIAL_INDICES_INT16,
			MATERIAL_INDICES_MAX_ID
		};

			/// A subpart defines a triangle, a triangle list or a triangle strip.
		struct Subpart
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_COLLIDE, hkpMeshShape::Subpart );
			HK_DECLARE_REFLECTION();

				/// A partial initializing constructor. It will only set values in Debug
				/// apart from a default material that gets set in Release too.
			inline Subpart();

				//
				//	Vertex information
				//
				/// A pointer to the first vertex, defined by three floats.
			const float*  m_vertexBase; //+nosave

				/// The byte offset between two consecutive vertices (usually 12, 16 or more).
			int		m_vertexStriding;

				/// The number of vertices.
			int		m_numVertices;

				//
				// Triangle Index Information
				//

				/// A pointer to triples of vertex indices.
				/// Used to be a union type, but to make
				/// auto serialization possible, we leave it as
				/// a void* here.
			const void*	m_indexBase; //+nosave
				
				/// A type defining whether 16 or 32 bits are used to index vertices.
			hkEnum<MeshShapeIndexStridingType,hkInt8> m_stridingType;

				/// A type defining whether 8 or 16 bits are used to index material.
			hkEnum<MeshShapeMaterialIndexStridingType,hkInt8> m_materialIndexStridingType;

				/// The byteoffset between two indices triples.
				///  - E.g. (Usually sizeof(hkUint16) if you use triangle strips
				///  - or 3 * sizeof(hkUint16) if you use independent triangles
			int m_indexStriding;

				/// A flag used to specify whether triangles should be returned wound the same way
				/// or alternate ways. It must be set to 0 or 1, and defaults to 0.
				/// If Triangle strips are used, each subsequent triangle in a list
				/// will be wound the alternate way. However for one sided welding, we require that
				/// all triangles have a consistent winding. This flag should be set to 1 in this case. This
				/// means the first triangle will be returned with vertices set to (0, 1, 2) and the second
				/// triangle will be returned with vertices set to (1, 3, 2). If this flag is set to 0 the
				/// second triangle will be returned with vertices set to (1, 2, 3). If independent triangles
				/// are used this flag should be set to 0 to maintain winding.
			int m_flipAlternateTriangles;

				/// The number of index triples, which is equal to the number of triangles.
			int	m_numTriangles;

				//
				//	Per triangle material Id info
				//

				/// Pointer to a strided array of material index (hkUint8 or hkUint16), one index for each triangle.
				///  - You are limited to a maximum of 256 or 65535 materials per subpart.
				///  - The indices may be stored in an interleaved array by setting m_materialIndexStriding appropriately.
				///  - If you do not want to use materials, simply set this element to HK_NULL
			const void* m_materialIndexBase; //+nosave

				/// The byte offset between two material indices
				/// This will be sizeof(hkUint8) or sizeof(hkUint16) for non-interleaved arrays.
			int m_materialIndexStriding;


				/// The base for the material table, the byte offset between two hkMeshMaterials is defined by
				/// m_materialStriding. If you are storing your materials externally and not using per-triangle filtering
				/// set this element to HK_NULL. The material array may be shared between meshes.
			const hkpMeshMaterial* m_materialBase; //+nosave

				/// The byte offset between two hkMeshMaterials
			int m_materialStriding;

				/// The number of materials, only used for debug checking
			int	m_numMaterials;

				/// This information is set automatically when a subpart is added to a mesh and should be left at -1.
				/// It is the offset in a global array for all triangles in the mesh of the first triangle of this sub piece
				/// This info is used for "welding" collisions between triangles.
			int m_triangleOffset; //+default(-1)

		};


	public:

		hkVector4					m_scaling;
		hkInt32						m_numBitsForSubpartIndex;

		hkArray<struct Subpart> m_subparts;

		hkArray<hkUint16> m_weldingInfo;		
		hkEnum<hkpWeldingUtility::WeldingType, hkUint8> m_weldingType; // +default(hkpWeldingUtility::WELDING_TYPE_NONE)

	public:

		hkpMeshShape( hkFinishLoadedObjectFlag flag );

	protected:

		friend class hkpExtendedMeshShape;

			/// The radius can only be set on construction.
		hkReal						m_radius;
		int m_pad[3]; // pad so same layout on 4x[10]x

		void assertSubpartValidity( const Subpart& part );
};


#include <Physics/Collide/Shape/Deprecated/Mesh/hkpMeshShape.inl>


#endif // HK_COLLIDE2_MESH_SHAPE_H

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
