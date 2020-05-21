/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESH_SIMPLIFIER_CONVERTER_H
#define HK_MESH_SIMPLIFIER_CONVERTER_H

#include <Common/Internal/GeometryProcessing/Topology/hkgpVertexTriangleTopology.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Common/Base/Container/MinHeap/hkMinHeap.h>

#include <Common/GeometryUtilities/Mesh/Simplifiers/QemSimplifier/hkQemSimplifier.h>
#include <Common/GeometryUtilities/Mesh/hkMeshVertexBuffer.h>
#include <Common/GeometryUtilities/Mesh/hkMeshSystem.h>
#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>


class hkQuadricMetric;

class hkVertexSharingUtil;

class hkQemSimplifier;
class hkFindUniquePositionsUtil;

class hkMeshSectionLockSet;
class hkVertexRealConverter;
struct hkMeshSection;
class hkMeshMaterial;


class hkMeshSimplifierConverter
{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkMeshSimplifierConverter);
		typedef hkQemSimplifier::TopologyType TopologyType;

		typedef TopologyType::EdgeId EdgeId;
		typedef TopologyType::Edge Edge;
		typedef TopologyType::VertexIndex VertexIndex;
		typedef hkQemSimplifier::Triangle Triangle;
		
		struct Group
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkMeshSimplifierConverter::Group);
			int m_modelIndex;									///< The model this group is associated with
			hkVertexFormat m_vertexFormat;                      ///< The vertex format in this group
			hkRefPtr<hkMeshMaterial> m_material;                ///< The material associated with this group
			int m_qemGroupIndex;								///< The group in the qem simplifier
		};
		
		
			/// Scale the weights by scale
		void scaleWeights(hkReal scale);

			/// Get the weight being used for this usage/subUsage
		hkReal getWeight(hkVertexFormat::ComponentUsage usage, int subUsage) const;
			/// Set the weight being used for this usage/subUsage
		void setWeight(hkVertexFormat::ComponentUsage usage, int subUsage, hkReal weight);
			/// Clear the weights
		void clearWeights();

			/// Set the weight (for position) of a material
		void setMaterialWeight(hkMeshMaterial* material, hkReal weight);
			/// Get the weight (for position) of a material
		hkReal getMaterialWeight(hkMeshMaterial* material) const;

			/// Set the default
		void setDefaultMaterialWeight(hkReal weight) { m_defaultMaterialWeight = weight; }
			/// Get the default
		hkReal getDefaultMaterialWeight() const { return m_defaultMaterialWeight; }

			/// Set up a mesh to simplify
		hkResult initMesh(const hkMeshShape* shape, hkQemSimplifier& simplifier);
			/// Add lots of meshes to simplify simultaneously
		hkResult initMeshes(const hkMeshShape*const* srcMeshes, int numMeshes, hkQemSimplifier& simplifier);
			/// Create a mesh from the contents - must have been finalized before this call
		hkMeshShape* createMesh(hkMeshSystem* system, int modelIndex, hkQemSimplifier& simplifier);
		
			/// Works out how many triangles in a model
		int calcNumModelTriangles(int modelIndex, const hkQemSimplifier& simplifier) const;

			/// Ctor
		hkMeshSimplifierConverter();

		hkBool m_positionOnly;													///< Do simplification through position only

	protected:
		
		void _addMeshSection(hkFindUniquePositionsUtil& positionUtil, hkVertexSharingUtil& sharingUtil, const hkMeshSection& section, int groupIndex, hkQemSimplifier& simplifier);
		static void HK_CALL _calcAabb(const hkVector4* pos, int numPos, hkAabb& aabb);
		static hkResult HK_CALL _addPositions(const hkMeshSectionLockSet& sectionLockSet, hkFindUniquePositionsUtil& positionUtil );
		void _addModelGroups(hkMeshSectionLockSet& sectionLockSet, int modelIndex, hkArray<int>& sectionMapOut);


		int _findGroup(int modelIndex, hkMeshMaterial* material, const hkVertexFormat& vertexFormat) const;
			/// Add a group, returns the index of the new group
		int _addGroup(int modelIndex, hkMeshMaterial* material, const hkVertexFormat& vertexFormat);
		void _getVertexFormat(hkMeshVertexBuffer* buffer, hkVertexFormat& vertexFormat);

		void _setWeights(hkMeshMaterial* material, const hkVertexFormat& format, hkVertexRealConverter& converter);
		
		hkReal m_defaultMaterialWeight;
		hkArray<hkRefPtr<hkMeshMaterial> > m_materials;
		hkArray<hkReal> m_materialWeight;

		hkArray<Group> m_groups;												///< Storage for all of the groups
		hkAabb m_srcAabb;
		hkArray<hkReal> m_weights;												///< This works with m_weightElement to define a weight
		hkArray<hkUint32> m_weightElements;
		hkReal m_defaultWeights[hkVertexFormat::USAGE_LAST];					///< The default weights by type
};

#endif // HK_MESH_SIMPLIFIER_CONVERTER_H

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
