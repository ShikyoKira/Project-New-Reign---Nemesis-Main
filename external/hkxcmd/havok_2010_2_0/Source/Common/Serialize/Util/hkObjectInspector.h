/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_OBJECT_INSPECTOR_H
#define HK_OBJECT_INSPECTOR_H

#include <Common/Base/Types/hkBaseTypes.h>

	/// Utility functions to inspect an object using metadata.
namespace hkObjectInspector
{
		/// Location of pointer and corresponding reflection information.
	struct Pointer
	{
			///
		void** location;
			///
		const hkClass* klass;

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, Pointer );
	};

		/// Called when an object is found.
	class ObjectListener
	{
		public:

			virtual ~ObjectListener() {}

				/// Called when an object is found.
				/// The 'klass' contains reflection information that
				/// corresponds to the specified object in 'objP'.
				/// The 'containedPointers' provides array of locations
				/// of pointers (see Pointer structure) that referenced
				/// by 'object'.
				/// The implementation may modify this array.
				/// NOTE: The 'containedPointers' may contain items where
				/// pointers referenced by Pointer::location can be
				/// equal to HK_NULL (however, the location is never equal
				/// to HK_NULL).
				/// The Pointer::klass contains pointer correct structure
				/// for non-virtual objects and pointer to the base class
				/// for virtual object. However, in some cases where the base
				/// class for virtual object is unknown it contains HK_NULL.
				/// It is callback's responsibility to correct the Pointer::klass
				/// value in order to allow farther processing of the pointers.
			virtual hkResult objectCallback( const void* objP, const hkClass& klass, hkArray<Pointer>::Temp& containedPointers ) = 0;
	};

		/// Get list of pointers and classes (array of Pointer structs) from specified object.
		/// The 'klass' must contains reflection information that corresponds to the specified 'object'.
		/// Return array of pointers in 'outContainedPointers'.
		/// NOTE: The Pointer::klass contains pointer correct structure
		/// for non-virtual objects and pointer to the base class
		/// for virtual object. However, in some cases where the base
		/// class for virtual object is unknown it contains HK_NULL.
	hkResult HK_CALL getPointers(const void* object, const hkClass& klass, hkArray<Pointer>::Temp& outContainedPointers);

		/// Walk through an object and call listener providing list of all found pointers to other objects.
		/// The listener may hold and update the list of these pointers. The function does not trace/analyse
		/// duplicated pointers, the listener is responsible to process/ignore such pointers found in objects.
		/// The function recursively walks through each object referenced by pointer from the list when the
		/// listener callback is complete.
	hkResult HK_CALL walkPointers(const void* object, const hkClass& klass, ObjectListener* listener);
}

#endif // HK_OBJECT_INSPECTOR_H

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
