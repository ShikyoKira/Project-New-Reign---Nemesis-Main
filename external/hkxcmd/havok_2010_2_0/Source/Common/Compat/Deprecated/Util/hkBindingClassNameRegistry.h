/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BINDING_CLASS_NAME_REGISTRY_H
#define HK_BINDING_CLASS_NAME_REGISTRY_H

#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Serialize/Util/hkChainedClassNameRegistry.h>
#include <Common/Base/Container/StringMap/hkStringMap.h>
#include <Common/Compat/Deprecated/Version/hkVersionRegistry.h>
#include <Common/Serialize/Util/hkSerializeMultiMap.h>

struct hkInternalClassEnum;
class hkBindingClassNameRegistry;
class hkClass;

/// Associates string type names with hkClass objects.
class hkBindingClassNameRegistry : public hkChainedClassNameRegistry
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

	hkBindingClassNameRegistry(const hkVersionRegistry::ClassRename* renames = HK_NULL, const hkClassNameRegistry* next = HK_NULL);
	~hkBindingClassNameRegistry();

		/// Get a class by name or HK_NULL if it was not registered.
	virtual const hkClass* getClassByNameNoRecurse( const char* className ) const;

		/// Get a class by name or HK_NULL if it was not registered.
	virtual const hkClass* getClassByName( const char* className ) const;

		/// Register renames.
	virtual void registerRenames( const hkVersionRegistry::ClassRename* renames );

		/// Register renames.
	virtual void registerRenames( const hkStringMap<const char*>& newNameFromOldNameMap );

protected:

	const char* getNewName(const char* oldName) const;

private:

	class ClassAllocationsTracker
	{
	public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkBindingClassNameRegistry::ClassAllocationsTracker);
		ClassAllocationsTracker();
		~ClassAllocationsTracker();

		hkClass* restoreClassHierarchy(const hkClass& classToCopy, const hkBindingClassNameRegistry* classRegistry, const hkStringMap<const char*>& oldClassNameFromNewNameMap, hkStringMap<hkClass*>& classesInOut );

	private:

		struct ClassData
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkBindingClassNameRegistry::ClassAllocationsTracker::ClassData);
			ClassData() {}
			ClassData(const ClassData& rhs)
				: allocatedClass(rhs.allocatedClass)
			{
				hkPointerMap<hkInternalClassEnum*, hkBool32>::Iterator iter = rhs.enumsAllocatedForMembers.getIterator();
				while (rhs.enumsAllocatedForMembers.isValid(iter))
				{
					enumsAllocatedForMembers.insert(rhs.enumsAllocatedForMembers.getKey(iter), rhs.enumsAllocatedForMembers.getValue(iter));
					iter = rhs.enumsAllocatedForMembers.getNext(iter);
				}
			}
			hkClass* allocatedClass;
			hkPointerMap<hkInternalClassEnum*, hkBool32> enumsAllocatedForMembers;
		};

		struct UnresolvedClassPointerTracker
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkBindingClassNameRegistry::ClassAllocationsTracker::UnresolvedClassPointerTracker);
			UnresolvedClassPointerTracker();
			~UnresolvedClassPointerTracker();

			void classPointedBy( const hkClass* newClass, const hkClass** fromWhere );
			// replace class entry and remove it from tracking
			void replaceClassAndRemove( const hkClass* oldClass, const hkClass* newClass );
			void resolveClasses(hkPointerMap<const hkClass*, const hkClass*>& classFromUnresolved);
			int getSize() const;

            hkSerializeMultiMap<void*, const hkClass**> m_pointers;
		};

		// allocate class functions
		hkInternalClassEnum* allocateEnums(const hkInternalClassEnum* enumsToCopy, int numEnums);
		hkInternalClassMember* allocateMembers(const hkInternalClassMember* membersToCopy, int numMembers,
			const hkInternalClassEnum* originalEnums, const hkInternalClassEnum* copiedEnums, int numAvailableEnums,
			hkPointerMap<hkInternalClassEnum*, hkBool32>& enumsAllocatedForMembersOut, UnresolvedClassPointerTracker& unresolvedClassPointersInOut);
		void* allocateDefaults(const hkClass& klass, void* defaultsToCopy);
		hkClass* allocateClass(const hkClass& classToDuplicate, const char* classNameToSet, UnresolvedClassPointerTracker& unresolvedClassPointersOut);

		// deallocate class functions
		void deallocateEnums(hkInternalClassEnum* enums, int numEnums);
		void deallocateMembers(hkInternalClassMember* members, int numMembers, hkPointerMap<hkInternalClassEnum*, hkBool32>& enumsAllocatedForMembersInOut);
		void deallocateClass(hkClass* classToDeallocate, hkPointerMap<hkInternalClassEnum*, hkBool32>& enumsAllocatedForMembersInOut);

	private:

		hkArray<ClassData> m_trackedClassData; // array of key entries for class allocations
	};

	mutable ClassAllocationsTracker m_tracker;

	hkStringMap<const char*> m_newNameFromOldName;
	hkStringMap<const char*> m_oldNameFromNewName;
};

#endif // HK_BINDING_CLASS_NAME_REGISTRY_H

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
