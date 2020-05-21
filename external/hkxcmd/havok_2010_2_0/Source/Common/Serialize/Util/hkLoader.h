/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef LOADER_H
#define LOADER_H

class hkTypeInfoRegistry;
class hkClassNameRegistry;
class hkStreamReader;
class hkPackfileData;
class hkRootLevelContainer;
class hkStatisticsCollector;
class hkVersionRegistry;
class hkResource;

/// Utility class to simplify loading packfiles and tagfiles.
/// This class is mainly useful for demo code where it keeps a reference
/// to all the resources it loads and cleans them up on destruction.
/// If you need more control, use hkSerializeUtil instead.
class hkLoader : public hkReferencedObject
{
	public:

		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE);

			/// Releases the references to all loaded memory
		~hkLoader();

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Implicitly uses the built-in registry
		virtual hkRootLevelContainer* load( const char* filename );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Implicitly uses the built-in registry.
		virtual hkRootLevelContainer* load( hkStreamReader* reader );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Uses the hkTypeInfoRegistry passed explicitly.
		virtual hkRootLevelContainer* load( const char* filename, hkTypeInfoRegistry* finish );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Uses the hkTypeInfoRegistry passed explicitly.
		virtual hkRootLevelContainer* load( hkStreamReader* reader, hkTypeInfoRegistry* finish );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Does not assume that the hkRoootLevelContainer is the top level class.
			/// Implicitly uses the built-in registry.
		virtual void* load( const char* filename, const hkClass& expectedTopLevelClass );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Does not assume that the hkRoootLevelContainer is the top level class.
			/// Implicitly uses the built-in registry.
		virtual void* load( hkStreamReader* reader, const hkClass& expectedTopLevelClass );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Does not assume that the hkRoootLevelContainer is the top level class.
			/// Uses the hkTypeInfoRegistry passed explicitly.
		virtual void* load( const char* filename, const hkClass& expectedTopLevelClass, hkTypeInfoRegistry* finish );

			/// Loads from a packfile or tagfile, applies versioning steps if needed,
			/// explicitly allocates and keeps a handle to the memory allocated.
			/// This method is usually called multiple times to load several different assets.
			/// Does not assume that the hkRoootLevelContainer is the top level class.
			/// Uses the hkTypeInfoRegistry passed explicitly.
		virtual void* load( hkStreamReader* reader, const hkClass& expectedTopLevelClass, hkTypeInfoRegistry* finish );


		virtual void calcContentStatistics( hkStatisticsCollector* collector, const hkClass* cls ) const;

	public:

		hkArray<hkResource*> m_loadedData;
};

#endif // LOADER_H

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
