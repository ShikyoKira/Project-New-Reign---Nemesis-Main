/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_MOPP_COMPILER_INPUT_H
#define HK_COLLIDE2_MOPP_COMPILER_INPUT_H

#include <Common/Base/hkBase.h>

/// This class holds the parameters that affect the building of an hkpMoppCode. See the following long description for details.
/// The hkpMoppCompilerInput will be
/// initialised with default values that may be sub-optimal
/// for a particular geometry. Because of this, the values should be set
/// with the appropriate methods before the hkpMoppCode is built. The following
/// description explains these values and how to set them:
///
/// What does a bounding volume tree do?
///
/// In order to quickly find collisions between a moving object and a large static geometry,
/// the triangles of the static geometry are hierarchically grouped in
/// a binary bounding volume tree.
///
/// To do collision detection between a
/// moving body and a static geometry (for instance, a landscape), the bounding box of the body
/// is checked against the bounding volume tree. A list of possibly colliding triangles
/// is then passed to the narrowphase collision detection. You can view
/// the bounding volume tree as a filter to the narrowphase collision
/// detection system.
///
/// This bounding volume tree is also used when performing raycasts and linear casts.
/// At every node in the tree there exists a bounding polytope, which encapsulates all
/// of its children.
///
/// The nodes on the leaf levels encapsulate one hkpShape, normally a
/// triangle. (The rest of this explanation uses triangles as an example of a leaf node).
/// The fit of this bounding volume can be perfect (as in some AABB trees), or can
/// have an extra margin/tolerance built in (e.g. MOPP). For more information please see the
/// section on Creating Shapes in the Collision Detection chapter of the Physics manual.
///
/// Why is the MOPP so memory efficient?
///
/// The MOPP-tree is an optimised polytope tree, based on the same principles as KDOP
/// trees. In order to keep the memory down, the MOPP data structures:
///    - use a reduced floating point resolution to store the extents of each MOPP node.
///    - store the minimum number of extents for each node.
///    - implement various compression techniques to reduce the overall size.
///
/// How do the parameters affect the building process?
///
/// There is a tolerance (called the fit tolerance) for the MOPP that describes
/// how closely the bounding volumes at the leaf nodes approximate the ideal bounding volume.
/// The MOPP building process uses the hkpMoppCompilerInput to allow you to specify which is
/// more highly optimised - the size of the tree, or how closely the leaf nodes bound the triangles.
/// This means that:
///    - The smaller the fit tolerance of the bounding volumes, the more memory is needed
///      to store the bounding volume information.
///    - The smaller the fit tolerance, the less triangles are passed to the narrow
///      phase collision detector and the less CPU is needed.
///
/// How can I get the best trade off between speed and memory usage?
///
/// Fit tolerance has the largest influence on speed and memory size.
/// There are two fit tolerance settings:
///    - one scalar for the general case \b m_absoluteFitToleranceOfTriangles
///    - and one hkVector4 specifically for axis aligned triangles \b _absoluteFitToleranceOfAxisAlignedTriangles
///
/// The fit tolerance defines how accurately the MOPP bounding
/// volumes approximates perfect bounding volumes. You can define a fit
/// tolerance for triangle level nodes and internal nodes. Normally only the triangle
/// level fit tolerance is important for the overall performance - the internal
/// fit tolerance has only a limited effect on the tree and you should use the
/// default values.
///
/// If your game can afford a loose fit of the bounding volumes, your memory consumption decreases.
/// The following are some simple guidelines to calculate the necessary fit of the MOPP tree:
///    - If your application is a flight simulator, the player's avatar will spend most of the time
///      away from the landscape. Assuming that you have an average triangle edge length of 10 meters,
///      a pretty loose m_absoluteFitToleranceOfTriangles of 2-5 meters (which is 20%-50% of the triangle edge length)
///      should suffice.
///    - In a quake level environment with lots of triangles of all sizes, the player's avatar will
///      almost always be in contact with the landscape. This means that the narrowphase will always
///      have some possible collisions to resolve. As any narrowphase check is far more expensive than
///      the MOPP check, decreasing m_absoluteFitToleranceOfTriangles to ~10% of the triangle edge length
///      will keep the CPU time to a minimum. In a level with an average triangle edge length of 1 meter, this is
///      approximately 0.1 meters.
///    - For a racing game on a mostly flat terrain (triangle edge length ~5 meters),
///      you should ensure that the flat triangles on the course
///      get a very tight fit in direction of the up axis (e.g. 'y-axis').
///      A general m_absoluteFitToleranceOfTriangles of ~0.5 meters (~10% of the triangle edge length) and a
///      m_absoluteFitToleranceOfAxisAlignedTriangles.y = 0.02 meters (~0.4% of the triangle edge length)
///      should give optimal results. You should try to ensure that the distance of the car to the
///      ground does not get too small, unless the game does not allow for this (Formula 1 and
///      Nascar racing cars have very low suspension).
///    - For a game in a city with tall straight buildings, it is useful to set the axis aligned
///      fit tolerance to a small value for collision detection with the walls. This means decreasing
///      the x and z components of m_absoluteFitToleranceOfAxisAlignedTriangles to roughly the same
///      as the y component, at ~4% (or slightly more as collision detection between the street and the users
///      avatar will be more common than with the buildings, depending on game logic).
///    - In a hilly terrain motorbike/offroad vehicle game with an average triangle edge length of 5 meters,
///      most of the triangles normals are not parallel to the main major axes. Therefore, the fit
///      of an AABB or a KDOP node is already suboptimal and setting a very tight fit for the MOPP
///      will not improve the overall performance.
///      In this case m_absoluteFitToleranceOfTriangles = 0.5f and m_absoluteFitToleranceOfAxisAlignedTriangles = hkVector4(0.1f, 0.1f, 0.1f)
///      should give a good result.
///
class hkpMoppCompilerInput
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CDINFO, hkpMoppCompilerInput);

			/// The default constructor. This sets default values
			/// that should produce good results. However, to get the best
			/// performance and size metrics from your MOPP tree, you may need to change these values.
		hkpMoppCompilerInput();

			/// Sets the fit tightness of the bounding volume leaf nodes for each triangle.
			/// This scalar value represents the maximum distance between the extents of a
			/// MOPP bounding volume node for a triangle and the extents of a
			/// perfectly fitting bounding volume node for the same triangle.
			/// Good values range from 1% * average triangle size (near-perfect fit)
			/// to 50% * average triangle size (loose fit).
		void setAbsoluteFitToleranceOfTriangles(float inTight);

			/// Returns the current value for this MOPP parameter.
			/// Returns the fit tightness of the bounding volume leaf nodes for each triangle.
		float getAbsoluteFitToleranceOfTriangles() const;

			/// Sets this MOPP parameter.
			/// Sets the fit tightness of the bounding volume leaf nodes for axis aligned triangles.
			/// If a triangle is coplanar to one of the major axes, this fit
			/// tolerance is used instead of the scalar parameter. There is one value for each axis.
			/// Normally only the axis which is parallel to the gravity should get a very low value.
			/// This parameter is very important for racing games over a flat racing course.
			/// NOTE: if this parameter is uninteresting, each component should be set equal
			/// to the scalar absoluteFitToleranceOfTriangles
		void setAbsoluteFitToleranceOfAxisAlignedTriangles(const hkVector4& inTight);

			/// Returns the current value for this MOPP parameter.
			/// Returns the fit tightness of the bounding volume leaf nodes for axis aligned triangles.
		hkVector4 getAbsoluteFitToleranceOfAxisAlignedTriangles() const;

			/// Sets this MOPP parameter.
			/// Sets the minimum fit tolerance for internal nodes as
			/// a value relative to the size of the node.
			/// This parameter has a subtle effect on the MOPP performance
			/// a default range of [0.3f,0.7f] works well.
		void setRelativeFitToleranceOfInternalNodes(float inUnused);

			/// Returns the current value for this MOPP parameter.
			/// Returns the minimum fit tolerance for internal nodes as
			/// a value relative to the size of the node.
		float getRelativeFitToleranceOfInternalNodes() const;

			/// Sets this MOPP parameter.
			/// Sets the minimum fit tolerance of internal nodes.
			/// This parameter has a subtle effect on the MOPP performance.
			/// A good value is about 1/5 of your small object diameter.
		void setAbsoluteFitToleranceOfInternalNodes(float inMin);

			/// Returns the current value for this MOPP parameter.
			/// Returns the minimum fit tolerance of internal nodes.
		float getAbsoluteFitToleranceOfInternalNodes() const;

	public:

		hkVector4 m_absoluteFitToleranceOfAxisAlignedTriangles;
		
		float m_relativeFitToleranceOfInternalNodes;

		float m_absoluteFitToleranceOfInternalNodes;

		float m_absoluteFitToleranceOfTriangles;

			/// By default the MOPP compiler tries to group similar shapekeys into
			/// one node. This helps reducing the code size. However sometimes you want
			/// to reorder your input without changing the tree. In this case you can set
			/// this parameter to false;
		hkBool  m_useShapeKeys;

			/// In typical landscapes, a single very big triangle can cause the tree to become pretty bad.
			/// Therefore by default, the compiler can decide to split a triangle and put in triangle
			/// twice into the tree. If you don't want this behavior, set m_enablePrimitiveSplitting to false.
		hkBool  m_enablePrimitiveSplitting;

			///	This tells the MOPP compiler to organize the MOPP code into smaller chunks so it can be processed on the PlayStation(R)3 SPU.
		hkBool m_enableChunkSubdivision;

			/// By default (m_enableInterleavedBuilding=true)
			/// the MOPP compiler uses a fixed-sized 2 megabyte buffer for triangle storage. This works well for
			/// large sets of triangles as memory consumption is kept limited. Yet for small sets of triangles
			/// (<4000) it is better to only allocate the actually needed memory size (which is smaller than
			/// the fixed-sized buffer). Set this member to false to disable interleaved building and to make
			/// the compiler allocate the correct memory block needed for all triangles.
		hkBool  m_enableInterleavedBuilding;

			/// Enable faster but more memory-consuming version of the MOPP shape mediator.
			///
			/// By default the MOPP shape mediator recalculates a primitive's maximum/minimum extent on each axis
			/// during each access. By enabling this flag a 20% faster version of the shape mediator will be used
			/// that only calculates the extents once for all axes upfront and caches the results. Note that this
			/// mediator version has an increased memory consumption: for each of the shape's primitive an additional
			/// 108 bytes block is allocated.
		hkBool m_cachePrimitiveExtents;
};

#endif // HK_COLLIDE2_MOPP_COMPILER_INPUT_H

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
