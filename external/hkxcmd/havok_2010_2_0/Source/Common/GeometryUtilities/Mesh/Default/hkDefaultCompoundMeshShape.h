/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DEFAULT_COMPOUND_MESH_SHAPE_H
#define HK_DEFAULT_COMPOUND_MESH_SHAPE_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
#include <Common/GeometryUtilities/Mesh/hkMeshBody.h>
#include <Common/GeometryUtilities/Mesh/IndexedTransformSet/hkIndexedTransformSet.h>

class hkMeshSystem;

extern const hkClass hkDefaultCompoundMeshShapeClass;

/// A simple default implementation of hkMeshShape, which could be used as a default implementation for compound shapes
///
/// When using an hkMeshSystem, there is the method 'createCompoundShape' which creates a new shape out of a combination
/// of pre-existing shapes. Each of the contained shapes is controlled via a unique transform in the transform set.
///
/// The default compound mesh shape, provides a simple way of implementing this functionality, in that it will use the
/// functionality that has already been implemented in for normal shapes on hkMeshSystem to emulate the compound shape
/// result.
///
/// In order to make this work in an implementation of hkMeshSystem, when a body is constructed from a hkDefaultCompoundMeshShape,
/// a hkDefaultCompoundMeshBody should be created. This will then manufacture hkMeshBodys for all of the contained hkMeshShapes.
/// When a hkDefaultCompoundMeshBody is added to a hkMeshSystem, the addToSystem method should be called. When it is removed the
/// addFromSystem should be called. These methods will add and remove all of the contained hkMeshBodys respectively.
///
/// In an implementation of hkMeshSystem - identification of the shape or bodies type so an appropriate code route can be
/// taken can be achieved by using the getClassType method.
///
/// \sa hkMeshShape
class hkDefaultCompoundMeshShape: public hkMeshShape
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

            // Ctor
		hkDefaultCompoundMeshShape( const hkMeshShape*const * shapes, const hkMatrix4* transforms, int numShapes);

            // Dtor
        virtual ~hkDefaultCompoundMeshShape();

            // hkMeshShape implementation
        virtual int getNumSections() const;

            // hkMeshShape implementation
		virtual void lockSection(int sectionIndex, hkUint8 accessFlags, hkMeshSection& sectionOut) const;

            // hkMeshShape implementation
		virtual void unlockSection(const hkMeshSection& section) const;

            // hkReferencedObjectImplementation
        virtual const hkClass* getClassType() const { return &hkDefaultCompoundMeshShapeClass; }

            // hkReferencedObjectImplementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

            /// Create a list of shapes (ref'd) for each transform index
        static void HK_CALL createTransformIndexedShapeList(hkMeshSystem* meshSystem,  const hkMeshSectionCinfo* sectionsIn, int numSections, hkArray<const hkMeshShape*>& shapesOut);

            /// Returns true if all of the sections have a transform index.
        static hkBool HK_CALL hasIndexTransforms(const hkMeshSectionCinfo* sections, int numSections);

            /// Will create a compound shape, made up of shapes which have the same transform index
        static hkDefaultCompoundMeshShape* HK_CALL createTransformIndexedShape(hkMeshSystem* meshSystem,  const hkMeshSectionCinfo* sectionsIn, int numSections);

	public:
		//
		// Members
		//

        struct MeshSection
        {
            int m_shapeIndex;
            int m_sectionIndex;
        };

        hkArray<const hkMeshShape*> m_shapes;	///< The child shapes
		hkArray<hkMatrix4> m_defaultChildTransforms;	 ///< An optional array to transform from child shape to this shape
        hkArray<MeshSection> m_sections;		///< The mesh sections
};


extern const hkClass hkDefaultCompoundMeshBodyClass;

/// A simple implementation of hkMeshBody - which simulates a compound body using multiple contained bodies.
///
/// This class should be used with the hkDefaultCompoundMeshShape class. It holds the hkMeshBody objects that
/// are constructed from the contained shapes from the hkDefaultCompoundMeshShape.
///
/// To make function correctly in an implementation of hkMeshSystem, the method addToSystem and removeFromSystem
/// should be called when the hkDefaultCompoundMeshBody is added or removed from the world. Use the getClassType
/// method to identity the type of object in the implementation of addBody/removeBody on the hkMeshSystem.
///
/// For an implementation of this - look at the hkgMeshSystem implementation.
///
/// \sa hkMeshBody
class hkDefaultCompoundMeshBody: public hkMeshBody
{
    public:
        HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SCENE_DATA);

			/// Ctor
		hkDefaultCompoundMeshBody(hkMeshSystem* meshSystem, const hkDefaultCompoundMeshShape* shape, const hkMatrix4& transform, hkIndexedTransformSetCinfo* transformSetCinfo);

			// Dtor
		virtual ~hkDefaultCompoundMeshBody();

            // hkReferencedObjectImplementation
        virtual const hkClass* getClassType() const { return &hkDefaultCompoundMeshBodyClass; }

            // hkReferencedObjectImplementation
        virtual void calcContentStatistics( hkStatisticsCollector* collector,const hkClass* cls ) const;

            // hkMeshBody implementation
        virtual const hkMeshShape* getMeshShape() const;

            // hkMeshBody implementation
        virtual void getTransform( hkMatrix4& transform ) const;

            // hkMeshBody implementation
        virtual void setTransform(const hkMatrix4& matrix);

            // hkMeshBody implementation
        virtual hkResult setPickingData(int id, void* data);

            // hkMeshBody implementation
        virtual hkMeshVertexBuffer* getVertexBuffer(int sectionIndex);

            // hkMeshBody implementation
        virtual int getNumIndexedTransforms() { return m_transformSet ? m_transformSet->getNumMatrices() : 0; }

            // hkMeshBody implementation
        virtual void setIndexedTransforms(int startIndex, const hkMatrix4* matrices, int numMatrices);

            // hkMeshBody implementation
        virtual void getIndexedTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x423432, m_transformSet); m_transformSet->getMatrices(startIndex, matrices, numMatrices); }

            // hkMeshBody implementation
        virtual void getIndexedInverseTransforms(int startIndex, hkMatrix4* matrices, int numMatrices) { HK_ASSERT(0x34243206, m_transformSet); m_transformSet->getInverseMatrices(startIndex, matrices, numMatrices); }

            // hkMeshBody implementation
        virtual void completeUpdate();
		virtual void completeUpdate(const hkMatrix4& transform);

        void addToSystem(hkMeshSystem* meshSystem);

        void removeFromSystem(hkMeshSystem* meshSystem);

		const hkArray<hkMeshBody*>& getChildBodies() const { return m_bodies; }

    protected:

		void _update();


		//
		// Members
		//
	protected:

        // With a compound shape, there are multiple contained shapes, otherwise there is just one
        hkArray<hkMeshBody*> m_bodies;

        hkMatrix4 m_transform;

        // The shape this has been created from
        const hkDefaultCompoundMeshShape* m_shape;

        hkRefPtr<hkIndexedTransformSet> m_transformSet;
        //
		hkBool m_transformIsDirty;
		hkBool m_transformSetUpdated;
};


#endif // HK_DEFAULT_COMPOUND_MESH_SHAPE_H

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
