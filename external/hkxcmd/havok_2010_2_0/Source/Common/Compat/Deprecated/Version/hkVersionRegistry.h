/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE2_VERSION_REGISTRY_H
#define HK_SERIALIZE2_VERSION_REGISTRY_H

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Base/Reflection/Registry/hkDynamicClassNameRegistry.h>

class hkBindingClassNameRegistry;
class hkStaticClassNameRegistry;
class hkObjectUpdateTracker;
template <typename T, typename A> class hkArray;
template <typename T, typename A> class hkStringMap;

/// Manages conversion between SDK versions.
/// Note that the registry has no concept of version numbers being greater
/// or less than one another. It just knows that it may call a function
/// to convert between two string identifiers.
class hkVersionRegistry : public hkReferencedObject, public hkSingleton<hkVersionRegistry>
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Signature of the generic version functions.
		typedef void (HK_CALL *VersionFunc)(hkVariant& obj, hkVariant& newObj, hkObjectUpdateTracker& tracker);

			/// Specifies how to handle class signatures.
		enum SignatureFlags
		{
			AUTO_SIGNATURE = 0xffffffff
		};

			/// Specifies how the versioning should be handled.
		enum VersionFlags
		{
				/// The object is neither copied, nor defaults applied.
			VERSION_MANUAL = 1<<1,
				/// The object can be updated in place and has defaults applied.
			VERSION_INPLACE = 1<<2,
				///	The object is copied and defaults applied.
			VERSION_COPY = 1<<3,
				/// The object is to be removed and all references to it nullified.
			VERSION_REMOVED = 1<<4,
				/// The object contains a variant.
			VERSION_VARIANT = 1<<5,
				/// The object contains a homogeneous array.
			VERSION_HOMOGENEOUSARRAY = 1<<6
		};

		struct ClassRename
		{
			const char* oldName;
			const char* newName;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, ClassRename );
		};

			/// Describes the versioning changes for a single type.
		struct ClassAction
		{
			/// Signature before updating.
			hkUint32 oldSignature;
			/// Signature after updating.
			hkUint32 newSignature;
			/// How should the type be versioned.
			int /*VersionFlags*/ versionFlags;
			/// The class name as it appears in the old version.
			const char* oldClassName;
			/// Custom function to call or HK_NULL.
			VersionFunc versionFunc;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, ClassAction );
		};

		struct UpdateDescription
		{
			UpdateDescription(const ClassRename* renames, const ClassAction* actions, const hkClassNameRegistry* staticNewClassRegistry = HK_NULL) :
				m_renames(renames), m_actions(actions), m_newClassRegistry(staticNewClassRegistry), m_next(HK_NULL)
			{
			}
				/// Find the update action for specified class.
			//const ClassAction* findActionForClass( const hkClass& classIn ) const;

				/// Null or null-terminated list of renames.
			const ClassRename* m_renames;

				/// Null or null terminated list of actions
			const ClassAction* m_actions;

				/// New versions of classes.
			const hkClassNameRegistry* m_newClassRegistry;

				/// Null or null terminated list (stack order) of update descriptions
			UpdateDescription* m_next;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, UpdateDescription );
		};

			/// Signature of an update function.
		typedef hkResult (HK_CALL* UpdateFunction)(
			hkArray<hkVariant>& loadedObjects,
			hkObjectUpdateTracker& tracker );

			/// Single entry to update between specified versions.
		struct Updater
		{
			const char* fromVersion;
			const char* toVersion;
			UpdateDescription* desc;
			UpdateFunction optionalCustomFunction;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkVersionRegistry::Updater );
		};

		hkVersionRegistry();

			///
		~hkVersionRegistry();

			/// Add an updater to the registry.
			/// Usually updaters are compiled in via StaticLinkedUpdaters, but
			/// dynamically loaded updaters may use this method.
		void registerUpdater( const Updater* updater );

			/// Find the sequence of updaters to convert between given versions.
			/// If there is a path between fromVersion and toVersion write the sequence
			/// of updaters needed into pathOut and return HK_SUCCESS.
			/// If no such path exists, return HK_FAILURE.
		hkResult getVersionPath( const char* fromVersion, const char* toVersion, hkArray<const Updater*>::Temp& pathOut ) const;

		const hkClassNameRegistry* getClassNameRegistry( const char* versionString ) const;

		hkResult registerStaticClassRegistry(const hkStaticClassNameRegistry& staticReg);
	
		hkResult registerUpdateDescription(UpdateDescription& updateDescription, const char* fromVersion, const char* toVersion);

	public:

			/// Available updaters.
		hkArray<const Updater*> m_updaters;

			/// List of updaters available at compile time - SEE DETAILS BELOW.
			/// NB Link errors (e.g. LNK2001 under .NET) for this array probably mean you
			/// have not yet registered the updaters via e.g., SDK/include/common/Common/Compat/hkCompatVersions.h using the HK_COMPAT_FILE macro -
			/// See the demo/demos/*Classes.cpp files for examples, (e.g., demo/demos/PhysicsClasses.cpp for Physics-Only customers).
		static const Updater* StaticLinkedUpdaters[];

			/// List of versions and corresponding classes available at compile time - SEE DETAILS BELOW.
			/// NB Link errors (e.g. LNK2001 under .NET) for this array probably mean you
			/// have not yet registered the classes via e.g., SDK/include/common/Common/Compat/hkCompatVersions.h using the HK_COMPAT_FILE macro -
			/// See the demo/demos/*Classes.cpp files for examples, (e.g., demo/demos/PhysicsClasses.cpp for Physics-Only customers).
		static const hkStaticClassNameRegistry* StaticLinkedClassRegistries[];

	public:

		hkDynamicClassNameRegistry* getDynamicClassNameRegistry( const char* versionString ) const;

	private:

		mutable hkStringMap<hkDynamicClassNameRegistry*> m_versionToClassNameRegistryMap;
};


class ValidatedClassNameRegistry : public hkDynamicClassNameRegistry
{
public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
	typedef hkResult (HK_CALL *ClassRegistryCallbackFunc)(hkDynamicClassNameRegistry& classRegistry, const hkClass& klass, void* userData);

	static hkResult HK_CALL processClass(hkDynamicClassNameRegistry& classRegistry, const hkClass& classToProcess, void* userData);

	ValidatedClassNameRegistry(const hkClassNameRegistry* classRegistry);

	/// Register a class possibly under a different name.
	/// If name is null, the class name is used.
	/// The name is not copied and must be valid for the lifetime
	/// of this object.
	virtual void registerClass( const hkClass* klass, const char* name);
	
private:
	hkResult validateClassRegistry(const hkClass& klass, hkStringMap<hkBool32>& doneClassesInOut, ClassRegistryCallbackFunc callbackFunc, void* userData);
};



#endif // HK_SERIALIZE2_VERSION_REGISTRY_H

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
