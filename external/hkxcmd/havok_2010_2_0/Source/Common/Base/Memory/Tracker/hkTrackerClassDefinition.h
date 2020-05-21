/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_TRACKER_CLASS_DEFINITION_H
#define HKBASE_TRACKER_CLASS_DEFINITION_H

#include <Common/Base/Memory/Tracker/hkMemoryTracker.h>

// Template for working out the alignment of an arbitrary type

class hkTrackerTypeInit //+reflected(false)
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE,hkTrackerTypeInit);
		typedef hkMemoryTracker::TypeDefinition TypeDefinition;
			/// Ctor
		hkTrackerTypeInit(const TypeDefinition& def);

		static void HK_CALL init();

	protected:
		const TypeDefinition& m_definition;
		hkTrackerTypeInit* m_next;

		static hkTrackerTypeInit* s_initLinkedList;
};

/* 

Example

class Fred
{
	protected:
		class Bloggs
		{
		private:
			int m_a;
			int m_b;

			enum State
			{
				STATE_B
			};
		};

	private:
		class ScanType
		{
		private:
			void* m_ptr;
		};

		enum Type
		{
			TYPE_A
		};
		int m_c;
		int m_d;
};

enum GlobalEnum { GLOBAL_ENUM_1 };

class ScanType
{
	private:
	void* m_ptr;
};


// GlobalEnum

HK_TRACKER_IMPLEMENT_SIMPLE(GlobalEnum, libraryName)

// Fred

HK_TRACKER_DECLARE_CLASS_BEGIN(Fred)
	HK_TRACKER_DECLARE_CHILD_SIMPLE(Type)
HK_TRACKER_DECLARE_CLASS_END

HK_TRACKER_IMPLEMENT_CHILD_SIMPLE(Fred, Type, libraryName)

HK_TRACKER_CLASS_MEMBERS_BEGIN(Fred)
	HK_TRACKER_MEMBER(Fred, m_c, 0)
	HK_TRACKER_MEMBER(Fred, m_d, 0)
HK_TRACKER_CLASS_MEMBERS_END()
HK_TRACKER_IMPLEMENT_CLASS_BASE(Fred, libraryName)

// Bloggs

HK_TRACKER_DECLARE_CLASS_BEGIN(Fred::Bloggs)
	HK_TRACKER_DECLARE_CHILD_SIMPLE(State)
HK_TRACKER_DECLARE_CLASS_END

HK_TRACKER_IMPLEMENT_CHILD_SIMPLE(Fred::Bloggs, State, libraryName)

HK_TRACKER_CLASS_MEMBERS_BEGIN(Fred::Bloggs)
	HK_TRACKER_MEMBER(Fred::Bloggs, m_a, 0)
	HK_TRACKER_MEMBER(Fred::Bloggs, m_b, 0)
HK_TRACKER_CLASS_MEMBERS_END()
HK_TRACKER_IMPLEMENT_CLASS_BASE(Fred::Bloggs, libraryName)

// ScanType

HK_TRACKER_DECLARE_CLASS_BEGIN(ScanType)
HK_TRACKER_DECLARE_CLASS_END
HK_TRACKER_IMPLEMENT_SCAN_CLASS(ScanType, libraryName)

// Fred::ScanType
HK_TRACKER_DECLARE_CLASS_BEGIN(Fred::ScanType)
HK_TRACKER_DECLARE_CLASS_END
HK_TRACKER_IMPLEMENT_SCAN_CLASS(Fred::ScanType, libraryName)

*/

#if defined(HK_MEMORY_TRACKER_ENABLE)


// Members

#define HK_TRACKER_MEMBER(TYPE, FIELD, FLAGS, TYPENAME) \
	{hkUint16(HK_OFFSET_OF(TYPE, FIELD)), hkUint16(sizeof( ((TYPE*)0)->FIELD) ), TYPENAME, #FIELD, FLAGS },


#define HK_TRACKER_CLASS_MEMBERS_BEGIN(TYPE) \
/* static */ const hkMemoryTracker::Member TYPE::TrackerStruct::s_members[] = \
{
#define HK_TRACKER_CLASS_MEMBERS_END() \
	{0, 0, HK_NULL, HK_NULL, 0} \
}; 

// SIMPLE CHILDREN

// Simple children (can be used for types with no pointers, or enums/dumb types, that are members of a class etc)

#define HK_TRACKER_DECLARE_CHILD_SIMPLE(TYPE) \
	static hkMemoryTracker::TypeDefinition s_##TYPE##Definition; \
	static hkTrackerTypeInit s_##TYPE##Init;

#define HK_TRACKER_IMPLEMENT_CHILD_SIMPLE(SCOPE, TYPE, LIBRARY_NAME) \
/* static */ hkMemoryTracker::TypeDefinition SCOPE::TrackerStruct::s_##TYPE##Definition = \
{ \
	#SCOPE "::" #TYPE, \
	LIBRARY_NAME, \
	hkUint16(sizeof(SCOPE::TYPE)), \
	hkUint8(HK_ALIGN_OF(SCOPE::TYPE)), \
	hkMemoryTracker::TypeDefinition::TYPE_BASIC \
}; \
/* static */hkTrackerTypeInit SCOPE::TrackerStruct::s_##TYPE##Init(SCOPE::TrackerStruct::s_##TYPE##Definition);


// SIMPLE GLOBAL

// If its global scope - and simple , we can just used the following

#define HK_TRACKER_IMPLEMENT_SIMPLE(TYPE, LIBRARY_NAME) \
/* static */const hkMemoryTracker::TypeDefinition s_##TYPE##Definition = \
{ \
	#TYPE, \
	LIBRARY_NAME, \
	hkUint16(sizeof(TYPE)), \
	hkUint8(HK_ALIGN_OF( TYPE)), \
	hkMemoryTracker::TypeDefinition::TYPE_BASIC \
}; \
/* static */hkTrackerTypeInit s_##TYPE##Init(s_##TYPE##Definition);

// NAMESPACE SIMPLE GLOBAL
#define HK_TRACKER_IMPLEMENT_NAMESPACE_SIMPLE(TYPE, LIBRARY_NAME, TYPENAME) \
	/* static */hkMemoryTracker::TypeDefinition s_##TYPENAME##Definition = \
{ \
	#TYPE, \
	LIBRARY_NAME, \
	hkUint16(sizeof(TYPE)), \
	hkUint8(HK_ALIGN_OF( TYPE)), \
	hkMemoryTracker::TypeDefinition::TYPE_BASIC \
}; \
/* static */hkTrackerTypeInit s_##TYPENAME##Init(s_##TYPENAME##Definition);

// CLASS 

#define HK_TRACKER_DECLARE_CLASS_BEGIN(TYPE) \
struct TYPE::TrackerStruct \
{ \
	static const hkMemoryTracker::Member s_members[]; \
	static const hkMemoryTracker::InitClassDefinition s_definition; \
	static hkTrackerTypeInit s_init; 

#define HK_TRACKER_DECLARE_CLASS_END \
};


#define HK_TRACKER_CLASS_MEMBERS_BEGIN(TYPE) \
/* static */ const hkMemoryTracker::Member TYPE::TrackerStruct::s_members[] = \
{
#define HK_TRACKER_CLASS_MEMBERS_END() \
	{0, 0, HK_NULL, HK_NULL, 0} \
}; 

#define HK_TRACKER_IMPLEMENT_CLASS_SCAN_(TYPE, LIBRARY_NAME) \
\
/* static */const hkMemoryTracker::InitClassDefinition TYPE::TrackerStruct::s_definition = \
{ \
	#TYPE,					\
	LIBRARY_NAME,						\
	hkUint16(sizeof(TYPE)),	 \
	hkUint8(HK_ALIGN_OF(TYPE)), \
	hkMemoryTracker::TypeDefinition::TYPE_SCAN, \
	\
	HK_NULL,			\
	HK_NULL,						\
	0,		\
	sizeof(hkIsVirtual(static_cast<TYPE*>(0))) == sizeof(hkBool::CompileTimeTrueType),	\
	0 \
}; \
\
/* static */ hkTrackerTypeInit TYPE::TrackerStruct::s_init(*(hkMemoryTracker::ClassDefinition*)&s_definition);

// Basic definition
#define HK_TRACKER_IMPLEMENT_CLASS_(TYPE, ALIGN_TYPE, LIBRARY_NAME, PARENT) \
\
/* static */const hkMemoryTracker::InitClassDefinition TYPE::TrackerStruct::s_definition = \
{ \
	#TYPE,					\
	LIBRARY_NAME,						\
	hkUint16(sizeof(TYPE)),	 \
	hkUint8(HK_ALIGN_OF(ALIGN_TYPE)), \
	hkMemoryTracker::TypeDefinition::TYPE_CLASS, \
	\
	s_members,			\
	PARENT,						\
	hkUint16(sizeof(s_members) / sizeof(s_members[0])) - 1,		\
	sizeof(hkIsVirtual(static_cast<TYPE*>(0))) == sizeof(hkBool::CompileTimeTrueType),	\
	0 \
}; \
\
/* static */ hkTrackerTypeInit TYPE::TrackerStruct::s_init(*(hkMemoryTracker::ClassDefinition*)&s_definition);

// All the different types
#define HK_TRACKER_IMPLEMENT_CLASS(TYPE, LIBRARY_NAME, PARENT) \
	HK_TRACKER_IMPLEMENT_CLASS_(TYPE, TYPE, LIBRARY_NAME, #PARENT)

#define HK_TRACKER_IMPLEMENT_CLASS_BASE(TYPE, LIBRARY_NAME) \
	HK_TRACKER_IMPLEMENT_CLASS_( TYPE, TYPE, LIBRARY_NAME, HK_NULL)

#define HK_TRACKER_IMPLEMENT_CLASS_ABSTRACT_BASE(TYPE, LIBRARY_NAME) \
	HK_TRACKER_IMPLEMENT_CLASS_(TYPE, void*, LIBRARY_NAME, HK_NULL)

#define HK_TRACKER_IMPLEMENT_CLASS_ABSTRACT(TYPE, LIBRARY_NAME, PARENT) \
	HK_TRACKER_IMPLEMENT_CLASS_(TYPE, void*, LIBRARY_NAME, #PARENT)

// Scan
#define HK_TRACKER_IMPLEMENT_SCAN_CLASS(TYPE, LIBRARY_NAME) \
	HK_TRACKER_IMPLEMENT_CLASS_SCAN_(TYPE, LIBRARY_NAME)

#else

// SIMPLE GLOBAL
#define HK_TRACKER_IMPLEMENT_SIMPLE(TYPE, LIBRARY_NAME) 

#define HK_TRACKER_IMPLEMENT_NAMESPACE_SIMPLE(TYPE, LIBRARY_NAME, TYPENAME)
// SIMPLE CHILDREN (children of classes)

// Simple children (can be used for types with no pointers, or enums/dumb types, that are members of a class etc)
#define HK_TRACKER_DECLARE_CHILD_SIMPLE(TYPE) 	
#define HK_TRACKER_IMPLEMENT_CHILD_SIMPLE(SCOPE, TYPE, LIBRARY_NAME) 

// CLASS 
#define HK_TRACKER_DECLARE_CLASS_BEGIN(TYPE) 
#define HK_TRACKER_DECLARE_CLASS_END 

#define HK_TRACKER_CLASS_MEMBERS_BEGIN(TYPE) 
#define HK_TRACKER_MEMBER(TYPE, FIELD, FLAGS, TYPENAME)
#define HK_TRACKER_CLASS_MEMBERS_END() 

// All the different types
#define HK_TRACKER_IMPLEMENT_CLASS(TYPE, LIBRARY_NAME, PARENT) 
#define HK_TRACKER_IMPLEMENT_CLASS_BASE(TYPE, LIBRARY_NAME) 
 
#define HK_TRACKER_IMPLEMENT_CLASS_ABSTRACT(TYPE, LIBRARY_NAME, PARENT) 
#define HK_TRACKER_IMPLEMENT_CLASS_ABSTRACT_BASE(TYPE, LIBRARY_NAME)

#define HK_TRACKER_IMPLEMENT_SCAN_CLASS(TYPE, LIBRARY_NAME) 

#endif

#endif // HKBASE_TRACKER_CLASS_DEFINITION_H

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
