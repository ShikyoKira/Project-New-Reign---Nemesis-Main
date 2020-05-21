/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_RESOURCE_H
#define HK_SERIALIZE_RESOURCE_H

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>

class hkTypeInfoRegistry;

	/// Abstract base class for resources which require cleanup.
class hkResource : public hkReferencedObject
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Object which this resource provides.
		struct Export
		{
				/// Symbol of exported object.
			const char* name;
				/// Exported object pointer.
			void* data;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkResource::Export );
		};

			/// Object which this resource requires.
		struct Import
		{
				/// Symbol of imported object.
			const char* name;
				/// Location inside this resource which points to the Export with the same name.
			void** location;

			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkResource::Import );
		};

	public:

			/// Get the name of this resource.
		virtual const char* getName() const = 0;

			/// Destroy the resource data.
			/// All contained objects are first destroyed then any allocated
			/// memory is freed. See also callDestructors().
		virtual ~hkResource() { }

			/// Destruct objects.
			/// This method is separate from ~hkResource() because if multiple
			/// resources reference each other, it will usually be necessary to perform
			/// two passes. e.g because otherwise the destructors could call removeReference
			/// on freed memory. The first pass calls all destructors. The second pass frees memory.
		virtual void callDestructors() { }

			/// Get a list of the imports/exports from this resource.
		virtual void getImportsExports( hkArray<Import>& impOut, hkArray<Export>& expOut ) const = 0;

			/// This function assumes that the packfile version matches the in-memory
			/// version. You may use hkVersionUtil if this is not the case. See the
			/// serialization section of the manual for more details.
			/// Usually you will use hkResource::getContents() instead of this function.
			/// If the packfile contents type do not match the expected class type
			/// this method will return HK_NULL. You may pass HK_NULL as the expected
			/// class to disable this check.
			/// Some objects will require a finishing step. i.e., to initialize
			/// members which have not been serialized or to initialize vtables.
			/// If "typeRegistry" is not HK_NULL, apply these finishing steps.
			/// If required hkTypeInfo is not found in "typeRegistry" while applying
			/// the finishing steps then this method will return HK_NULL.
		virtual void* getContentsPointer(const char* typeName, const hkTypeInfoRegistry* typeRegistry) const = 0;

			/// Return pointer to a top level object of specified type T.
			/// See hkResource::getContentsPointer(const char*, const hkTypeInfoRegistry*) method for details.
		template<typename T>
		T* getContentsWithRegistry(const hkTypeInfoRegistry* typeRegistry) const;

			/// Return pointer to a top level object of specified type T.
			/// The function finishes objects using type registry from hkBuiltinTypeRegistry.
			/// See hkResource::getContentsPointer(const char*, const hkTypeInfoRegistry*) method for details.
		template<typename T>
		T* getContents() const;

			/// Return the top level object type name.
		virtual const char* getContentsTypeName() const = 0;
};

#include<Common/Serialize/Resource/hkResource.inl>

#endif // HK_SERIALIZE_RESOURCE_H

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
