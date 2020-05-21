/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_VERSIONUTIL_OLD_H
#define HK_SERIALIZE_VERSIONUTIL_OLD_H

#include <Common/Compat/Deprecated/Version/hkVersionRegistry.h>
#include <Common/Serialize/Version/hkVersionUtil.h>
#include <Common/Base/Container/String/hkStringBuf.h>

class hkObjectUpdateTracker;
class hkClassNameRegistry;
class hkPackfileReader;

/// Versioning utility functions and structures.
namespace hkVersionUtil
{
		/// Get the last version of the SDK which supported packfile versioning
	const char* HK_CALL getDeprecatedCurrentVersion();

		/// Copies the old named member to the new named member.
		/// The members must have identical sizes.
	void HK_CALL renameMember( hkVariant& oldObj, const char* oldName, hkVariant& newObj, const char* newName );

		/// Copy defaults for new members.
		/// For each member in newClass which is not in oldClass and which
		/// has a specified default, copy it in into obj.
	void HK_CALL copyDefaults( void* obj, const hkClass& oldClass, const hkClass& newClass );

		/// Copy object from 'objectsIn' to preallocated 'objectsOut'.
		/// The 'tracker' stores information about all shared and new allocations
		/// for new objects.
		/// Return pointer to the block of memory with new objects, if successful. Otherwise, HK_NULL.
		/// NOTE: A corresponding hkClass must be specified for each 'objectsOut' element before calling this function.
		/// The index of the new object in 'objectsOut' will match the index of object copied from 'objectsIn'.
		/// The hkClass pointers in the copied object's members of the hkVariant and homogeneous array types are not updated.
	void* HK_CALL copyObjects( const hkArrayBase<hkVariant>& objectsIn, hkArray<hkVariant>& objectsOut, hkObjectUpdateTracker& tracker );

		/// Find variants in obj and update their class pointers.
		/// For each variant member in obj, update its class to the class
		/// from the given registry.
		/// i.e., obj.m_variant.m_class = reg.getClassByName( obj.m_variant.m_class->getName() )
	void HK_CALL updateVariantClassPointers( void* obj, const hkClass& klass, hkClassNameRegistry& reg, int numObj=1 );

		/// Low level interface to versioning.
		/// Apply the updateDescriptions to each object in objectsInOut.
		/// Note that the size and ordering of objectsInOut may change.
	hkResult HK_CALL updateSingleVersion(
		hkArray<hkVariant>& objectsInOut,
		hkObjectUpdateTracker& tracker,
		const hkVersionRegistry::UpdateDescription& updateDescription,
		const hkClassNameRegistry* newClassRegistry );

		/// Search for and apply a sequence of updates.
		/// Given fromVersion and toVersion, search for a sequence
		/// of updates which will convert between them. Calls
		/// updateSingleVersion for each step in the sequence.
	hkResult HK_CALL updateBetweenVersions(
		hkArray<hkVariant>& objectsInOut,
		hkObjectUpdateTracker& tracker,
		const hkVersionRegistry& reg,
		const char* fromVersion,
		const char* toVersion = HK_NULL );

		/// Update the packfile contents to the latest version in reg.
		/// Usually the hkVersionRegistry singleton is used for reg.
		/// Updating will fail if a binary packfile has been stripped of its
		/// table of contents. XML packfiles can always be updated.
	hkResult HK_CALL updateToCurrentVersion(
		hkPackfileReader& reader,
		const hkVersionRegistry& reg );

	hkResult HK_CALL updateToVersion(
		hkPackfileReader& reader,
		const hkVersionRegistry& reg,
		const char* targetVersion );

		/// Utility function to generate extern list of classes of current version.
		/// The classes extern declaration is output into provided hkOStream.
	hkResult HK_CALL generateCppExternClassList(hkOstream& os, const char* headerMacro, const hkClass*const* classesToGenerate, const char* registryVariableName);

		/// Utility function to generate list of classes of current version.
		/// The classes are generated as static data in C++ format and output
		/// into provided hkOStream. The pch file name may be provided for
		/// convenience, so the output can be used as source file in project
		/// without modifications.
	hkResult HK_CALL generateCppClassList(hkOstream& os, const hkClass*const* classesToGenerate, const char* pchfilename, const char* registryVariableName);
}

class CollectClassDefinitions
{
public:
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,CollectClassDefinitions);
	CollectClassDefinitions(const hkArray<const hkClass*>& originalClassList,
							hkPointerMap<const hkClassEnum*, char*>& enumNameFromPointer,
							hkStringMap<hkBool32>& enumDoneFlagFromName);

	const char* getClassExternList() const;

	const char* getClassDefinitionList() const;

	void defineClassClass(const hkClass& klass);

private:
	const hkClassEnum* m_classMemberEnumType;
	const hkArray<const hkClass*>& m_originalClassList;
	hkStringBuf m_classExternList;
	hkStringBuf m_classDefinitionList;
	hkPointerMap<const hkClassEnum*, char*>& m_enumNameFromPointer;
	hkStringMap<hkBool32>& m_enumDoneFlagFromName;
	hkStringMap<hkBool32> m_doneClasses;
};

#endif // HK_SERIALIZE_VERSIONUTIL_OLD_H

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
