/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_ROOT_CONTAINER_H
#define HK_ROOT_CONTAINER_H

#include <Common/Base/Reflection/hkClass.h>
extern const class hkClass hkRootLevelContainerClass;
extern const class hkClass hkRootLevelContainerNamedVariantClass;

/// This is the root level class exported by our tools.
/// It contains an array of Variants and associated names.
class hkRootLevelContainer
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, hkRootLevelContainer);

		HK_DECLARE_REFLECTION();

	public:

			/// hkRootLevelContainer constructor.
		HK_FORCE_INLINE hkRootLevelContainer();
			/// hkRootLevelContainer copy constructor.
		HK_FORCE_INLINE hkRootLevelContainer(const hkRootLevelContainer& other);

			/// hkRootLevelContainer destructor.
		HK_FORCE_INLINE ~hkRootLevelContainer();

			/// hkRootLevelContainer finish constructor.
		hkRootLevelContainer(hkFinishLoadedObjectFlag f);

			/// A variant with a name string.
			/// This class also redundantly stores the class name so that you can identify
			/// the type even if this object comes from a packfile without metadata (i.e., with
			/// null m_variant.m_class)
		class NamedVariant
		{
			public:

				HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkRootLevelContainer::NamedVariant );
				HK_DECLARE_REFLECTION();

					/// hkRootLevelContainer::NamedVariant constructor.
				HK_FORCE_INLINE NamedVariant();

				NamedVariant(const char* name, void* object, const hkClass* klass);
				NamedVariant(const char* name, const hkVariant& v);

					/// hkRootLevelContainer::NamedVariant finish constructor.
				NamedVariant(hkFinishLoadedObjectFlag flag);

					/// Set named variant name, object and class.
				HK_FORCE_INLINE void set(const char* name, void* object, const hkClass* klass);

					/// Set named variant name, object and class using variant v.
				HK_FORCE_INLINE void set(const char* name, const hkVariant& v);

					/// Get named variant type name.
				HK_FORCE_INLINE const char* getTypeName() const;

					/// Get named variant name.
				HK_FORCE_INLINE const char* getName() const;

					/// Get named variant object pointer.
				HK_FORCE_INLINE void* getObject() const;

					/// Get named variant class.
				HK_FORCE_INLINE const hkClass* getClass() const;

					/// Get named variant.
				HK_FORCE_INLINE const hkRefVariant& getRefVariant() const;

			private:
				hkStringPtr m_name;
				hkStringPtr m_className;
				hkRefVariant m_variant;
		};

			/// Iterates over the objects held in the container returning a pointer
			/// to the first object after prevObject, with a type corresponding to 'typeName'.
			/// If prevObject is null then the search begins from the start of the container.
		template<typename T>
		inline T* findObject( const void* prevObject = HK_NULL ) const;

			/// Iterates over the objects held in the container returning a pointer
			/// to the first object after prevObject, with a type corresponding to 'typeName'.
			/// If prevObject is null then the search begins from the start of the container.
		void* findObjectByType( const char* typeName, const void* prevObject = HK_NULL ) const;

			/// Iterates over the objects held in the container returning a pointer
			/// to the first object after prevObject, with a name corresponding to 'objectName'.
			/// If prevObject is null then the search begins from the start of the container.
		void* findObjectByName( const char* objectName, const void* prevObject = HK_NULL ) const;

	public:

			/// An array of NamedVariants.
		hkArray<class NamedVariant> m_namedVariants;
};

#include <Common/Serialize/Util/hkRootLevelContainer.inl>

#endif //HK_ROOT_CONTAINER_H

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
