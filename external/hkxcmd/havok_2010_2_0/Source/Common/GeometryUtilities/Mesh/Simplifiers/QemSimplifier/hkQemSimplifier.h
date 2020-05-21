/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_QEM_SIMPLIFIER_H
#define HK_QEM_SIMPLIFIER_H

#include <Common/Internal/GeometryProcessing/Topology/hkgpVertexTriangleTopology.h>

#include <Common/Base/Algorithm/Sort/hkSort.h>

#include <Common/Base/Container/MinHeap/hkMinHeap.h>
#include <Common/Base/Types/Geometry/Aabb/hkAabb.h>

class hkQuadricMetric;

class hkVertexSharingUtil;

class hkQemSimplifier: public hkReferencedObject
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

		struct Triangle;
		typedef hkgpVertexTriangleTopology<Triangle> TopologyType;

		typedef TopologyType::EdgeId EdgeId;
		typedef TopologyType::Edge Edge;
		typedef TopologyType::VertexIndex VertexIndex;
		
		// Attributes are stored, with first the attribute data, followed by the quadric error data.
		// Ie an attribute size is m_attributeVecSize + m_quadricVecSize
		struct Group
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkQemSimplifier::Group);
			Group() {}
			Group(const Group& rhs)
				: m_positionOffset(rhs.m_positionOffset)
				, m_attributeSize(rhs.m_attributeSize)
				, m_attributeVecSize(rhs.m_attributeVecSize)
				, m_quadricSize(rhs.m_quadricSize)
				, m_quadricVecSize(rhs.m_quadricVecSize)
				, m_positionToAttribScale(rhs.m_positionToAttribScale)
			{
				m_availableAttributeIndices = rhs.m_availableAttributeIndices;
				m_attributes = rhs.m_attributes;
				m_attributeFreeList = rhs.m_attributeFreeList;
			}
				/// Returns the attribute data at the index
			hkVector4* getAttribute(int index) { return m_attributes[index]; }
				/// Get an attribute
			const hkVector4* getAttribute(int index) const { return m_attributes[index]; }

				/// Get the quadric data at the index
			hkVector4* getQuadric(int index) { return m_attributes[index] + m_attributeVecSize; }
				/// Get the attribute position
			hkReal* getAttributePosition(int index) { return ((hkReal*)m_attributes[index]) + m_positionOffset; }
				/// Get the attribute position
			const hkReal* getAttributePosition(int index) const { return ((hkReal*)m_attributes[index]) + m_positionOffset; }
				/// Set the attribute position
			void setAttributePosition(int index, hkVector4Parameter pos) { pos.store3(((hkReal*)m_attributes[index]) + m_positionOffset); }
				/// Create an attribute, zero it and add to the end of the m_attributes list
			int createAttribute();
				/// Delete an attribute
			void deleteAttribute(int index);
				/// Get the position
			void getPosition(int index, hkVector4& pos) { pos.load3(((hkReal*)getAttribute(index)) + m_positionOffset); }
				/// Get the num attributes
			int getNumAttributes() const { return m_attributes.getSize(); }

			int m_positionOffset;                               ///< The offset of the position held int the attributes (offset is in hkReals)

			// There is a quadric for each vertex/attribute
			int m_attributeSize;
			int m_attributeVecSize;								///< Size in vec4s

			int m_quadricSize;									///< Size of the quadric store size
			int m_quadricVecSize;								///< Size in vec4s

			hkReal m_positionToAttribScale;						///< Multiply by a position in 0-1 space to get in weighted attribute space

			hkArray<int> m_availableAttributeIndices;			///< List of indices available
			hkArray<hkVector4*> m_attributes;					///< Attribute index to attribute data
			hkFreeList m_attributeFreeList;						///< Storage for attributes
		};
		struct EdgeContraction
		{
			enum Type
			{
				TYPE_SELECT_START,			///< Shift to the start
				TYPE_SELECT_END,			///< Shift to the end
				TYPE_NEW,					///< Its a new vertex
				TYPE_USED,					///< Its been used, so if there is an attribute index, don't delete it
			};
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_DEMO, EdgeContraction);

			HK_FORCE_INLINE static hkBool lessThan(const EdgeContraction* a, const EdgeContraction* b)
			{
				return a->m_error < b->m_error;
			}
			HK_FORCE_INLINE static void setIndex(EdgeContraction* a, int index) { a->m_contractionIndex = index; }
			HK_FORCE_INLINE static void swap(EdgeContraction*& a, EdgeContraction*&b)
			{
				hkAlgorithm::swap(a->m_contractionIndex, b->m_contractionIndex);
				hkAlgorithm::swap(a, b);
			}
			HK_FORCE_INLINE static hkBool32 hasIndex(const EdgeContraction* a, int index) { return a->m_contractionIndex == index; }

			Type m_type;

			int m_start;										///< The start vertex index
			int m_end;											///< The end vertex index. The start is always

			int m_contractionIndex;								///< The current index in the hkMinHeap

			hkReal m_error;										///< The amount of error in applying this contraction
			int m_groupIndex;									///< The group the new attribute belongs to (will be != -1 is TYPE_NEW)
			int m_attributeIndex;								///< The new attribute index (Will be != -1 if TYPE_NEW)
		};
		struct Triangle: public hkgpVertexTriangleTopologyBase::Triangle
		{
			// The extra data we need
			int m_attributeIndices[3];			///< The attribute indices
			int m_groupIndex;					///< The group this triangle belongs to
		};

		struct Attribute
		{
			HK_FORCE_INLINE hkBool32 operator==(const Attribute& rhs) const { return m_attributeIndex == rhs.m_attributeIndex && m_groupIndex == rhs.m_groupIndex; }

			int m_attributeIndex;
			int m_groupIndex;
		};

			/// Start the mesh
		void startMesh(const hkAabb& aabb, const hkVector4* uniquePositions, int numPositions);
			/// Start a group
			/// The AABBs
		void startGroup(int attribSize, int positionOffset, hkReal positionToAttribScale);
			/// Add a triangle
		void addTriangle(const int positionIndices[], const int attributeIndices[]);
			/// Add an attribute, returns the index for the attribute
		int addAttribute(hkReal* attribute);
			/// Adds an attrib + returns its index
		hkReal* addAttribute(int& attribIndex);
			/// Adds an attribute to the current group
		hkReal* addAttribute();
			/// End a group
		void endGroup();
			/// End the mesh construction
		hkResult endMesh();
		
			/// Get all of the groups
		const hkArray<Group>& getGroups() const { return m_groups; }
			/// Get the num of triangles
		int getNumTriangles() const { return m_topology.getNumTriangles(); }
			/// Get a triangle
		const Triangle* getTriangle(int index) const { return m_topology.getTriangle(index); }

			/// Get the top contraction, returns HK_NULL if there aren't any remaining contractions
		EdgeContraction* getTopContraction() const;
			/// Returns true if applying the contraction will cause a triangle to flip
		hkBool doesTopContractionFlipTriangle();
			/// Apply the contraction
		void applyTopContraction();
			/// Discard the top contraction
		void discardTopContraction();
			/// Creates a shape with the currently stored configuration
		//hkMeshShape* createShape();

			/// Returns the cost of the top contraction
		hkReal getTopContractionError() const;

			/// Get the number of remaining contractions
		int getNumContractions() const { return m_contractions.getSize(); }

			/// Apply contractions
		void applyContractions(int numContractions, hkBool allowFlip);
			/// Simplify by applying contractions until the percentage of triangles requested is removed.
		void simplify(hkReal percentageRemoveTris, hkBool allowFlip);

			/// Removes all content
		void clear();

			/// Returns true if everything appears with internal structures
		hkBool32 isOk() const; 

            /// Remove all the unused attributes and vertices, and clear up memory.
        void finalize();

			/// Display
		void debugDisplay();
		
			/// Get the positions
		const hkArray<hkVector4>& getPositions() const { return m_positions; }

			/// Calculates the number of triangles per group
		void calcNumTrianglesPerGroup(hkArray<int>& trisPerGroup) const;

			/// Ctor
		hkQemSimplifier();

	protected:
		
		void _accumulateTriangleQem(const Triangle& tri, const hkVector4* triQem);
		bool _calcQuadrics();

		static void HK_CALL _calcNormal(hkVector4Parameter a, hkVector4Parameter b, hkVector4Parameter c, hkVector4& normal);
		hkBool _doesFlipTriangle(int vertexIndex, const EdgeContraction& contraction, hkVector4Parameter newPosition);
		hkBool _doesContractionFlipTriangle(const EdgeContraction& contraction);

		void _applyContraction(EdgeContraction* contraction);
		void _applyVertexChange(int positionIndex, const EdgeContraction& contraction);

		hkBool32 _findMove(const Attribute& attrib, const hkArray<EdgeId>& contractionEdgeIds, Attribute& move);
		void _calcVertexUniqueAttributes(int vertexIndex, hkArray<Attribute>& attribs);
		hkReal _calcChooseError(int startIndex, int endIndex, const hkArray<EdgeId>& contractionEdgeIds);

		int _addGroup(int attribSize, int positionOffset, hkReal positionToAttribScale);

		hkBool _areGroupIndicesValid(int groupIndex) const;

			/// Tries to calculate best contraction position - if fails falls back on just choosing the start or end
			/// Only works on edges where all edges have the same attributes (as determined by _allEdgesHaveSameAttributes)
		EdgeContraction* _createBestContraction(int start, int end, const hkArray<EdgeId>& edgeIds);
			/// Work out a contraction which introduces the least error - works on any type of edge (even different attributes)
		EdgeContraction* _createChooseContraction(int start, int end, const hkArray<EdgeId>& edgeIds);
			/// Create a contraction
		EdgeContraction* _createContraction(int start, int end);
			/// Goes through all triangles, adding contractions for edges, that don't currently have any
		bool _calcContractions();
			/// Calc the sum of the qems for attrib indices start + end
		void _calcSumQems(int groupIndex, int start, int end, hkQuadricMetric& qm);

			/// Delete a contraction - removes from maps and min heap too
		void _deleteContraction(EdgeContraction* contraction);

		void _applyChooseContraction(int fromIndex, int targetIndex);

			/// Find the contraction for an edge (start, end)
		EdgeContraction* _findContraction(int start, int end) const;

			/// Delete all of the contractions indexing the passed in vertex index
		void _deleteVertexContractions(VertexIndex index);

			/// Checks to see if all of the edges have the same attributes (and are in the same group id)
		hkBool32 _allEdgesHaveSameAttribs(const hkArray<EdgeId>& edgeIds);

			/// Check if attribute indexing seems reasonable
		hkBool32 _areAttributesOk() const;

			/// Get a position in the 0-1 space.
		void _getPosition01(int index, hkVector4& pos) const;
			/// Get a position int the attrib space.
		void _getPositionAttrib(int groupIndex, int index, hkVector4& pos) const;
			/// Get the position from an attribute, in world space.
		void _getAttribPosition(int groupIndex, int attribIndex, hkVector4& pos) const;

			/// Returns true if there is a triangle, which is facing in the opposite direction to tri.
		hkBool _oppositeTriangleExists(Triangle* tri) const;

		hkFreeList m_contractionFreeList;										///< Storage for all the contractions.
		hkMinHeap<EdgeContraction*, EdgeContraction> m_contractions;			///< Provides a fast way to get the min error.
		hkPointerMap<hkUint64, EdgeContraction*> m_edgeContractionMap;			///< Map edge keys, to contractions.

		hkArray<Group> m_groups;												///< Storage for all of the groups.

		hkgpVertexTriangleTopology<Triangle> m_topology;						///< The topology.

		hkAabb m_aabb;															///< AABB surrounding all positions in world space.
		hkVector4 m_aabbScale;														///< subtract AABB min, and mult by scale to get in 0-1 space.

		hkArray<hkVector4> m_positions;											///< All of the vertex positions in world space.

		hkBool m_isFinalized;
		hkBool m_inMesh;
		hkBool m_inGroup;
};

#endif // HK_QEM_SIMPLIFIER_H

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
