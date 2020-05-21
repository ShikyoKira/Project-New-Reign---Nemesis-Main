/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_DATA_DICT_OBJECT
#define HK_DATA_DICT_OBJECT

#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Serialize/Util/hkSerializeMultiMap.h>
#include <Common/Serialize/Data/hkDataObject.h>

class hkDataClassDict;
class hkDataObjectDict;

	/// This class implements the dictionary type of hkDataWorld.
	/// You may instantiate, modify and destroy hkDataClass,
	/// hkDataObject and hkDataArray objects using this dictionary world.
	///
	/// This class is used to load a tagfile, update loaded content
	/// (with patch versioning) if required, and copy it to native
	/// objects. And you may also use it to save world content to a
	/// tagfile.
class hkDataWorldDict : public hkDataWorld
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);

			/// Constructor.
		hkDataWorldDict();
			/// Constructor.
		hkDataWorldDict(hkMemoryAllocator* mall);
			/// Destructor.
		~hkDataWorldDict();
			/// Implements hkDataWorld::getContents().
		virtual hkDataObject getContents() const;

			/// Implements hkDataWorld::newClass().
		virtual hkDataClassImpl* newClass(const hkDataClass::Cinfo& cinfo);
			/// Implements hkDataWorld::newArray().
		virtual hkDataArrayImpl* newArray(hkDataObject& obj, const hkDataClass::MemberInfo& minfo) const;
			/// Implements hkDataWorld::newObject().
		virtual hkDataObjectImpl* newObject(const hkDataClass &) const;

			/// Implements hkDataWorld::getType(). Return hkDataWorld::TYPE_DICTIONARY.
		virtual hkEnum<hkDataWorld::DataWorldType, hkInt32> getType() const;

		// manage classes

			/// Implements hkDataWorld::findAllClasses().
		virtual void findAllClasses(hkArray<hkDataClassImpl*>::Temp& classesOut) const;
			/// Implements hkDataWorld::findClass().
		virtual hkDataClassImpl* findClass(const char* name) const;
			/// Create and register a new dictionary class based on the given hkClass.
		hkDataClassImpl* wrapClass(const hkClass& klass);
			/// Create and register a new dictionary class based on the class named 'name' from another world.
		hkDataClassImpl* copyClassFromWorld(const char* name, const hkDataWorld& worldFrom);
			/// Rename registered class.
		void renameClass(hkDataClass& klass, const char* newName);
			/// Remove registered class.
		void removeClass(hkDataClass& klass);

			/// Set new version for the given registered class.
		void setClassVersion(hkDataClass& klass, int newVersion);
			/// Set new parent for the given registered class.
		void setClassParent(hkDataClass& klass, hkDataClass& parent);

			/// Add new member to the given registered class.
		void addClassMember(hkDataClass& klass, const char* name, hkDataObject::Type type, const char* typeName, int tupleCount, const void* valuePtr);
			/// Rename specified member of the given registered class.
		void renameClassMember(hkDataClass& klass, const char* oldName, const char* newName);
			/// Remove specified member from the given registered class.
		void removeClassMember(hkDataClass& klass, const char* name);
			/// Set the default value for a named member of the given registered class
		void setClassMemberDefault(hkDataClass& klass, const char* name, const void* valuePtr);

		// manage objects

			/// Implements hkDataWorld::findObject().
		virtual hkDataObject findObject(const hkDataObject::Handle& handle) const;
			/// Cast object type to the given registered class.
		void castObject(hkDataObject& obj, const hkDataClass& newClass);
			/// Find all objects in the class hierarchy of the given base class name.
		void findObjectsByBaseClass(const char* baseClassName, hkArray<hkDataObjectImpl*>::Temp& objectsOut) const;
			/// Find all objects of the given specific class name only.
		void findObjectsByExactClass(const char* className, hkArray<hkDataObjectImpl*>::Temp& objectsOut) const;

			// Used internally.
		class ObjectTracker;
		ObjectTracker* m_tracker;
		hkMemoryAllocator* m_allocator;
};

#endif // HK_DATA_DICT_OBJECT

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
