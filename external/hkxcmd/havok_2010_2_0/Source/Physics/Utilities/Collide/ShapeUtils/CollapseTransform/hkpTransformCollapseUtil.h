/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef INC_UTILITIES_TRANSFORM_COLLAPSE_UTIL_H
#define INC_UTILITIES_TRANSFORM_COLLAPSE_UTIL_H

#include <Common/Base/hkBase.h>

class hkpRigidBody;
class hkpShape;

	/// This utility class provides functionality to navigate the shape hierarchy of one or multiple rigid bodies and manipulate transform,
	/// convex transform and convex translate shapes - possibly collapsing them into the child transform (by updating vertices, for example).
class hkpTransformCollapseUtil
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpTransformCollapseUtil );

			/// The SharedShapeBehaviour enum determines what happens when a transforms can be collapsed into a shape shared
			/// by more than one shape or rigid body (since collapsing will cause duplication in that case - the shape will no longer
			/// be shared).
		enum SharedShapeBehaviour
		{
				/// The transform will be collapsed into the shape, creating a unique (non-shared) shape.
			ALWAYS_COLLAPSE,

				/// The transform will not be collapsed into the shape.
			NEVER_COLLAPSE,

				/// The transform will be collapsed into the shape if the number of shapes/rigid bodies sharing it is less than
				/// a threshold (specified by the m_sharedShapeThreshold member of CollapseOptions)
			COLLAPSE_IF_LESS_THAN_THRESHOLD,
		};

			/// Options used by the "collapseTransforms" method.
		struct Options
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpTransformCollapseUtil::Options );

				/// Specified how to behave when a transform can be collapsed into a shape shared by other shapes/rigid bodies.
				/// Default is ALWAYS_COLLAPSE.
			SharedShapeBehaviour m_sharedShapeBehaviour;

				/// If using the COLLAPSE_IF_LESS_THAN_THRESHOLD behavior, shared shapes will only be collapsed if they are
				/// shared by less than this amount of shapes/rigid bodies.
			int m_sharedShapeThreshold;

				/// If true, transform shapes applied to a list shape will be moved to the children in the list (and possibly
				/// collapsed there).
				/// Default is true.
			hkBool m_propageTransformInList;

				/// Constructor, sets defaults.
			Options ();

		};

			/// This struct will be filled by the "collapseTransforms" methods with information about how much work was done.
		struct Results
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpTransformCollapseUtil::Results );

				/// Number of transform shapes collapsed into their childs (T->x becoming x')
			int m_numCollapsedShapes;

				/// Number of unnecessary transform shapes removed (T->x becoming x)
			int m_numIdentityTransformsRemoved;

				/// Number of transform shapes converted to specialized versions (convex transform, convex translate)
			int m_numSpecializedTransformShapes;

				/// Number of transforms shapes propagated to list shapes (T->L->x,y,z.. becoming L'->(T->x),(T->y),(T->z))
			int m_numPropagatedTransformsToLists;

				/// Constructors, sets defaults.
			Results();

				/// Uses the HK_REPORT macro to report the values stored in the struct.
			void report() const;

				/// Resets all results to 0.
			void reset();
		};

			/// Collapses, if possible, transform shapes in the given rigid body.
		static hkResult HK_CALL collapseTransforms (hkpRigidBody* rigidBody, const Options& options, Results& resultsOut);

			/// Collapses, if possible, transform shapes in the given set of rigid bodies.
		static hkResult HK_CALL collapseTransforms (hkArray<hkpRigidBody*>& rigidBodies, const Options& options, Results& resultsOut);

			/// Collapses, if possible, transform shapes in the given hkpShape.
		static hkResult HK_CALL collapseTransforms (const hkpShape* shape, const Options& options, Results& resultsOut, hkpShape** shapeOut);

			/// Try to modify the transform shape (of shape is a transform shape), otherwise create a transform shape with shape as its child
			/// If the final transform has an identity transform, remove the extra transform shape
		static const hkpShape* HK_CALL transformTransformShape (const hkpShape* shape, const hkTransform& transform );

	private:

			// Always returns a "new" shape
		static const hkpShape* collapseShapesRecursively (const hkpShape* shape, const Options& options, class _SharedShapeData& sharedShapeData, Results& resultsOut);

			// Always returns a "new" shape
		static const hkpShape* collapseTransformShape (const hkpShape* transformShape, const Options& options, class _SharedShapeData& sharedShapeData, Results& resultsOut);
};


#endif //INC_UTILITIES_TRANSFORM_COLLAPSE_UTIL_H

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
