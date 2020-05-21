// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_CLASSES_FILE
#	error Please define HK_CLASSES_FILE before including this file
#endif

// The default is to register all typeinfos and classes

#ifndef HK_REGISTER_TYPEINFOS
#	define HK_REGISTER_TYPEINFOS 1
#endif

#ifndef HK_REGISTER_CLASSES
#	define HK_REGISTER_CLASSES 1
#endif
#include <Common/Serialize/hkSerialize.h>

#include <Common/Serialize/Util/hkBuiltinTypeRegistry.h>
#include <Common/Base/Reflection/hkTypeInfo.h>

class hkClass;
class hkTypeInfo;

// Forward declarations of all used classes and typeinfos
#if HK_REGISTER_TYPEINFOS || HK_REGISTER_CLASSES
#	define HK_CLASS(A) extern const hkClass A##Class; extern const hkTypeInfo A##TypeInfo;
#	define HK_STRUCT(A) extern const hkClass A##Class; extern const hkTypeInfo A##TypeInfo;
#	define HK_ABSTRACT_CLASS(A) extern const hkClass A##Class;
#		include HK_CLASSES_FILE
#	undef HK_STRUCT
#	undef HK_CLASS
#	undef HK_ABSTRACT_CLASS
#endif

#if HK_REGISTER_TYPEINFOS
	// nonvirtual type infos
#	define HK_CLASS(A) // nothing
#	define HK_STRUCT(A) // nothing
#	define HK_ABSTRACT_CLASS(A) // nothing
#	include HK_CLASSES_FILE
#	undef HK_STRUCT
#	undef HK_CLASS
#	undef HK_ABSTRACT_CLASS

	// List of all typeinfos
	const hkTypeInfo* const hkBuiltinTypeRegistry::StaticLinkedTypeInfos[] =
	{
#		define HK_CLASS(A) &A##TypeInfo,
#		define HK_STRUCT(A) // nothing
#		define HK_ABSTRACT_CLASS(A) // nothing
#		include HK_CLASSES_FILE

#		undef HK_CLASS
#		define HK_CLASS(A) // nothing
#		undef HK_STRUCT
#		define HK_STRUCT(A) &A##TypeInfo,
#		include HK_CLASSES_FILE

#		undef HK_STRUCT
#		undef HK_CLASS
#		undef HK_ABSTRACT_CLASS
		HK_NULL
	};
#else

	// empty typeinfo list
	const hkTypeInfo* const hkBuiltinTypeRegistry::StaticLinkedTypeInfos[] =
	{
		HK_NULL
	};

#endif

#if HK_REGISTER_CLASSES

	// List of all classes
	// The strange construction is to ensure that the classes
	// are in the same order of the typeinfo list.


	const hkClass* const hkBuiltinTypeRegistry::StaticLinkedClasses[] =
	{
#		define HK_CLASS(A) &A##Class,
#		define HK_STRUCT(A)			// nothing
#		define HK_ABSTRACT_CLASS(A) // nothing
#		include HK_CLASSES_FILE
#		undef HK_STRUCT
#		undef HK_CLASS
#		undef HK_ABSTRACT_CLASS

#		define HK_CLASS(A)			// nothing
#		define HK_ABSTRACT_CLASS(A) // nothing
#		define HK_STRUCT(A) &A##Class,
#		include HK_CLASSES_FILE
#		undef HK_STRUCT
#		undef HK_CLASS
#		undef HK_ABSTRACT_CLASS

#		define HK_CLASS(A)			// nothing
#		define HK_STRUCT(A)			// nothing
#		define HK_ABSTRACT_CLASS(A) &A##Class,
#		include HK_CLASSES_FILE
#		undef HK_STRUCT
#		undef HK_CLASS
#		undef HK_ABSTRACT_CLASS

		HK_NULL
	};

#else

	// empty classes list
	const hkClass* const hkBuiltinTypeRegistry::StaticLinkedClasses[] =
	{
		HK_NULL
	};

#endif

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
