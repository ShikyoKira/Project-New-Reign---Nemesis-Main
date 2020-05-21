/* 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HAVOK_CREATE_SHAPE_UTILITY__H
#define HAVOK_CREATE_SHAPE_UTILITY__H


class hkxMesh;
class hkMeshShape;

class hkpCreateShapeUtility : public hkReferencedObject
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_COLLIDE);

	// Input structure
	class CreateShapeInput
	{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_COLLIDE, CreateShapeInput);

		// Default constructor
		CreateShapeInput()
			:	m_vertexTM(hkMatrix4::getIdentity())
			,	m_szMeshName(HK_NULL)
			,	m_bestFittingShapes(false)
			,	m_decomposedWorldT(hkTransform::getIdentity())
			,	m_extraShapeTransform(hkTransform::getIdentity())
			,	m_enableAutomaticShapeShrinking(true)
			,	m_defaultConvexRadius(0.0f)
			,	m_relShrinkRadius(0.05f)
			,	m_maxVertexDisplacement(0.03f)
			,	m_extraRadiusOverride(-1.0f)
			,	m_minBoxExtent(-1.0f)
		{}

	public:
		hkArray<hkVector4>	m_vertices;				// Array of mesh vertices in object space
		hkMatrix4			m_vertexTM;				// The node / mesh transform matrix 
		const char *		m_szMeshName;			// The node / mesh name (for debug messages)
		hkBool				m_bestFittingShapes;	// If true, shapes will be orientated for a best fitting

		hkTransform			m_decomposedWorldT;		// space that the shape was created in once the scale etc was baked out
		hkTransform			m_extraShapeTransform;	// information about the extra transformation required to create the shape

		hkBool				m_enableAutomaticShapeShrinking;	// If true, automatic shape shrinking is applied to a shape. If false, the old style default convex radius will be used.
		hkReal				m_defaultConvexRadius;				// This is an override to the global hard-coded default convex radius value (hkConvexShapeDefaultRadius).
		hkReal				m_relShrinkRadius;
		hkReal				m_maxVertexDisplacement;

		hkReal				m_extraRadiusOverride;	// Convex / extra radius override. -1.0 if not set

		hkReal				m_minBoxExtent; // If > 0, all shapes created will have at least this bounding box extent. This can be used to avoid creating degenerated volumes.
	};

	// Output structure
	struct ShapeInfoOutput
	{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkpCreateShapeUtility::ShapeInfoOutput);
		class hkpShape	*	m_shape;				// The shape itself
		bool				m_isConvex;				// Whether the object is convex
		hkTransform			m_decomposedWorldT;		// space that the shape was created in once the scale etc was baked out
		hkTransform			m_extraShapeTransform;	// information about the extra transformation required to create the shape
	};

public:
	static hkResult HK_CALL createBoxShape		(CreateShapeInput& input, ShapeInfoOutput& output);
	static hkResult HK_CALL createSphereShape	(CreateShapeInput& input, ShapeInfoOutput& output);
	static hkResult HK_CALL createCapsuleShape	(CreateShapeInput& input, ShapeInfoOutput& output);
	static hkResult HK_CALL createCylinderShape	(CreateShapeInput& input, ShapeInfoOutput& output);

public:
	static hkResult HK_CALL computeOBB	(const hkArray<hkVector4>& vertices, const hkMatrix4& vertexTransform, hkReal minBoxExtent, hkVector4& halfExtentsOut, hkTransform& obbTransformOut);
	static hkResult HK_CALL computeAABB	(const hkArray<hkVector4>& vertices, const hkMatrix4& vertexTransform, hkReal minBoxExtent, hkVector4& halfExtentsOut, hkTransform& obbTransformOut);

};


#endif	//HAVOK_CREATE_SHAPE_UTILITY__H

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
