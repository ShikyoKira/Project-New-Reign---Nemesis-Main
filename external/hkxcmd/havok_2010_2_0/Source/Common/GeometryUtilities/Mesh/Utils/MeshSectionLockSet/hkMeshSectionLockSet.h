/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MESH_SECTION_LOCK_SET_H
#define HK_MESH_SECTION_LOCK_SET_H

#include <Common/GeometryUtilities/Mesh/hkMeshShape.h>
class hkMeshVertexBuffer;

/// A utility to handle automatic unlocking of hkMeshSections from a hkMeshShape
///
/// When an instantiation of hkMeshSectionLockSet goes out of scope and hkMeshSections locked using it
/// will be automatically unlocked. Using the class will help avoid problems causing by incorrectly balancing
/// locks and unlocks of hkMeshSection on a hkMeshShape - which can cause memory leaks.
///
/// \sa hkMeshShape
class hkMeshSectionLockSet
{
    public:
        HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SCENE_DATA, hkMeshSectionLockSet );

            /// Dtor
        ~hkMeshSectionLockSet();

            /// Will add and lock all of the sections in shape in order
        void addMeshSections(const hkMeshShape* shape, hkUint8 accessFlags);

            /// Add a mesh section (note that the reference will be invalidated if any other sections are added, or clear is called)
        const hkMeshSection& addMeshSection(const hkMeshShape* shape, int index, hkUint8 accessFlags);

			/// Remove a mesh section. Will do a normal array removal (i.e., the section at the end will replace the entry at the current position)
		void removeSectionAtIndex(int index);

            /// Remove all of the current locked sections
        void clear();

            /// Finds a section - return HK_NULL if not found
        const hkMeshSection* findSection(const hkMeshShape* shape, int sectionIndex) const;

            /// Get a section
        HK_FORCE_INLINE const hkMeshSection& getSection(int index) const { return m_sections[index]; }

            /// Get the shape a section was locked on
        const hkMeshShape* getShape(int index) const { return m_shapes[index]; }

			/// Get the number of locked sections currently held
		int getNumSections() const { return m_sections.getSize(); }

			/// Get an array of all of the sections
		const hkMeshSection* getSections() const { return m_sections.begin(); }

            /// Finds all of the unique vertex buffers out of all of the locked sections
        void findUniqueVertexBuffers(hkArray<hkMeshVertexBuffer*>& buffersOut);

    protected:

		hkArray<hkMeshSection> m_sections;		///< the sections that are locked
        hkArray<const hkMeshShape*> m_shapes;	///< the mesh shapes the locks are on
};

#endif // HK_MESH_SECTION_LOCK_SET_H

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
