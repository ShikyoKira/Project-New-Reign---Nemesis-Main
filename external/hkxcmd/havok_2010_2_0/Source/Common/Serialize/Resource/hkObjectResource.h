/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_OBJECTRESOURCE_H
#define HK_SERIALIZE_OBJECTRESOURCE_H

#include <Common/Base/Reflection/hkClass.h>
#include <Common/Serialize/Resource/hkResource.h>
#include <Common/Base/Reflection/Registry/hkTypeInfoRegistry.h>
#include <Common/Base/Reflection/Registry/hkClassNameRegistry.h>

	/// Abstract base class for resources which require cleanup.
class hkObjectResource : public hkResource
{
	public:
	HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
			/// Construct hkObjectResource from hkVariant.
		hkObjectResource(const hkVariant& v);

			/// Get the name of this resource.
		virtual const char* getName() const;

			/// Destroy the resource data.
			/// Decrease reference count for the top level object if it is reference counted,
			/// otherwise call the non-virtual object's destructor and deallocate its memory.
			/// Note: You may free hkObjectResource even if your other objects still reference
			/// the loaded reference counted objects, assuming you maintain reference
			/// counting accordingly.
		virtual ~hkObjectResource();

		virtual void getImportsExports( hkArray<Import>& impOut, hkArray<Export>& expOut ) const;

			/// Implements hkResource::getContentsPointer().
		virtual void* getContentsPointer(const char* typeName, const hkTypeInfoRegistry* typeRegistry) const;

			/// Analogous to getContents except that cleanup is now the responsibility of the caller, not the resource.
		template<typename T>
		T* stealContents()
		{
			return static_cast<T*>(stealContentsPointer(T::staticClass().getName(), hkBuiltinTypeRegistry::getInstance().getLoadedObjectRegistry()));
		}

			/// Similar to getContentsPointer except that cleanup is now the responsibility of the caller, not the resource.
		virtual void* stealContentsPointer(const char* typeName, const hkTypeInfoRegistry* typeRegistry);

			/// Implements hkResource::getContentsTypeName().
		virtual const char* getContentsTypeName() const;

			// Set object resource class name registry.
		void setClassNameRegistry(const hkClassNameRegistry* classReg);
			// Set object resource type info registry.
		void setTypeInfoRegistry(const hkTypeInfoRegistry* typeReg);

	private:

		hkVariant m_topLevelObject;
		hkRefPtr<const hkClassNameRegistry> m_classRegistry;
		hkRefPtr<const hkTypeInfoRegistry> m_typeRegistry;
};

#endif // HK_SERIALIZE_OBJECTRESOURCE_H

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
