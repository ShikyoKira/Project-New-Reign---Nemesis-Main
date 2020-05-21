/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_BASE_TYPE_INFO_H
#define HK_BASE_TYPE_INFO_H

class hkClass;

class hkTypeInfo
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE, hkTypeInfo);

		typedef void (HK_CALL *FinishLoadedObjectFunction)(void*, int);
		typedef void (HK_CALL *CleanupLoadedObjectFunction)(void*);

		hkTypeInfo( const char* name,
			        const char* scopedName,
				FinishLoadedObjectFunction finish,
				CleanupLoadedObjectFunction cleanup,
				const void* vtable,
				hk_size_t size)
			:   m_typeName(name),
				m_scopedName(scopedName),
				m_finishLoadedObjectFunction(finish),
				m_cleanupLoadedObjectFunction(cleanup),
				m_vtable(vtable),
				m_size(size)
		{
		}
		const char* getTypeName() const
		{
			return m_typeName;
		}
		const char* getScopedName() const
		{
			return m_scopedName;
		}
		const void* getVtable() const
		{
			return m_vtable;
		}
			/// Cleanup a loaded object (run destructor etc)
		void cleanupLoadedObject(void* ptr) const;
			/// Finish loading an object
		void finishLoadedObject(void* ptr, int finishFlag) const;
			/// Finish the loaded object without informing the tracker
			/// The object must be cleaned up independently of cleanupLoadedObject - as that function will
			/// inform the tracker.
		void finishLoadedObjectWithoutTracker(void* ptr, int finishFlag) const;

			/// Returns true if has a finish function
		hkBool hasFinishFunction() const { return m_finishLoadedObjectFunction != HK_NULL; }
			/// Returns true if has a cleanup function
		hkBool hasCleanupFunction() const { return m_cleanupLoadedObjectFunction != HK_NULL; }

			/// Get the size of an instantiation in bytes
		hk_size_t getSize() const { return m_size; }

			/// Returns true if the type is virtual
		hkBool isVirtual() const { return m_vtable != HK_NULL; }

	private:

		const char* m_typeName;
		const char* m_scopedName;
		FinishLoadedObjectFunction m_finishLoadedObjectFunction;
		CleanupLoadedObjectFunction m_cleanupLoadedObjectFunction;
		const void* m_vtable;
		hk_size_t m_size;
};

//
// Internal definitions, used by public macros later in the file.
//

#define HK_REFLECTION_TEMPLATE_SPECIFIER template<>
#define HK_REFLECTION_NOTEMPLATE_SPECIFIER


#define HK_REFLECTION_DEFINE_COMMON( TEXT_CLASS, SCOPED_CPP_CLASS, FINISH, CLEANUP, VTABLE, TEMPLATE_SPECIFIER ) \
	TEMPLATE_SPECIFIER const hkClass& HK_CALL SCOPED_CPP_CLASS::staticClass() { return TEXT_CLASS##Class; } \
	extern const hkTypeInfo TEXT_CLASS##TypeInfo; \
	const hkTypeInfo TEXT_CLASS##TypeInfo( #TEXT_CLASS, "!" #SCOPED_CPP_CLASS, FINISH, CLEANUP, VTABLE, sizeof(SCOPED_CPP_CLASS))


#define HK_REFLECTION_DEFINE_INTERNAL_FINISH_VIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	extern const hkTypeInfo TEXT_CLASS##TypeInfo; \
	static void HK_CALL finishLoadedObject##TEXT_CLASS(void* p, int finishing = 1) \
{ \
	hkFinishLoadedObjectFlag f; f.m_finishing = finishing; \
	new (p) SCOPED_CPP_CLASS(f); \
}

#define HK_REFLECTION_DEFINE_INTERNAL_FINISH_NONVIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	extern const hkTypeInfo TEXT_CLASS##TypeInfo; \
	static void HK_CALL finishLoadedObject##TEXT_CLASS(void* p, int finishing = 1) \
{ \
	hkFinishLoadedObjectFlag f; f.m_finishing = finishing; \
	new (p) SCOPED_CPP_CLASS(f); \
}

#define HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_VIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	static void HK_CALL cleanupLoadedObject##TEXT_CLASS(void* p) \
{ \
	static_cast<SCOPED_CPP_CLASS*>(p)->~CPP_CLASS(); \
}

#define HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_NONVIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	static void HK_CALL cleanupLoadedObject##TEXT_CLASS(void* p) \
{ \
	static_cast<SCOPED_CPP_CLASS*>(p)->~CPP_CLASS(); \
}

#define HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_NONVIRTUAL_NONTRACKED( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	static void HK_CALL cleanupLoadedObject##TEXT_CLASS(void* p) \
{ \
	static_cast<SCOPED_CPP_CLASS*>(p)->~CPP_CLASS(); \
}

#define HK_REFLECTION_DEFINE_INTERNAL_VTABLE( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	static const void* HK_CALL getVtable##TEXT_CLASS() \
	{ \
		union { HK_ALIGN16(void* ptr); char buf[sizeof(SCOPED_CPP_CLASS)]; } u; \
		hkFinishLoadedObjectFlag f; \
		new (u.buf) SCOPED_CPP_CLASS(f); \
		return u.ptr; \
	}

#define HK_REFLECTION_DEFINE_INTERNAL_VIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS, TEMPLATE_SPECIFIER ) \
	HK_COMPILE_TIME_ASSERT2( sizeof(hkIsVirtual(static_cast<SCOPED_CPP_CLASS*>(0))) == sizeof(hkBool::CompileTimeTrueType), REFLECTION_PARSER_VTABLE_DETECTION_FAILED ); \
	HK_REFLECTION_DEFINE_INTERNAL_FINISH_VIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_VIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_INTERNAL_VTABLE( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_COMMON( TEXT_CLASS, SCOPED_CPP_CLASS, finishLoadedObject##TEXT_CLASS, cleanupLoadedObject##TEXT_CLASS, getVtable##TEXT_CLASS(), TEMPLATE_SPECIFIER )

#define HK_REFLECTION_DEFINE_INTERNAL_NONVIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS, TEMPLATE_SPECIFIER ) \
	HK_COMPILE_TIME_ASSERT2( sizeof(hkIsVirtual(static_cast<SCOPED_CPP_CLASS*>(0))) == sizeof(hkBool::CompileTimeFalseType), REFLECTION_PARSER_VTABLE_DETECTION_FAILED  ); \
	HK_REFLECTION_DEFINE_INTERNAL_FINISH_NONVIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_NONVIRTUAL( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_COMMON( TEXT_CLASS, SCOPED_CPP_CLASS, finishLoadedObject##TEXT_CLASS, cleanupLoadedObject##TEXT_CLASS, HK_NULL, TEMPLATE_SPECIFIER )

#define HK_REFLECTION_DEFINE_INTERNAL_SIMPLE( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS, TEMPLATE_SPECIFIER ) \
	HK_COMPILE_TIME_ASSERT2( sizeof(hkIsVirtual(static_cast<SCOPED_CPP_CLASS*>(0))) == sizeof(hkBool::CompileTimeFalseType), REFLECTION_PARSER_VTABLE_DETECTION_FAILED  ); \
	HK_REFLECTION_DEFINE_INTERNAL_CLEANUP_NONVIRTUAL_NONTRACKED( TEXT_CLASS, SCOPED_CPP_CLASS, CPP_CLASS ) \
	HK_REFLECTION_DEFINE_COMMON( TEXT_CLASS, SCOPED_CPP_CLASS, HK_NULL, cleanupLoadedObject##TEXT_CLASS, HK_NULL, TEMPLATE_SPECIFIER )

//
// Public macro definitions
//

/// Define an hkTypeInfo for a class which is virtual.
#define HK_REFLECTION_DEFINE_VIRTUAL(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_VIRTUAL( KLASSNAME, KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a class which is virtual.
#define HK_REFLECTION_DEFINE_SCOPED_VIRTUAL(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_VIRTUAL( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )

/// Define an hkTypeInfo for a non-virtual class. The vtable member will be set to null.
#define HK_REFLECTION_DEFINE_NONVIRTUAL(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_NONVIRTUAL( KLASSNAME, KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a non-virtual class. The vtable member will be set to null.
#define HK_REFLECTION_DEFINE_SCOPED_NONVIRTUAL(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_NONVIRTUAL( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )

/// Define an hkTypeInfo for a non-virtual class which has no constructor (for simple structures).
#define HK_REFLECTION_DEFINE_SIMPLE(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_SIMPLE( KLASSNAME, KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a non-virtual class which has no constructor (for simple structures).
#define HK_REFLECTION_DEFINE_SCOPED_SIMPLE(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_SIMPLE( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_NOTEMPLATE_SPECIFIER )

// TEMPLATED VERSIONS

/// Define an hkTypeInfo for a class which is virtual.
#define HK_REFLECTION_DEFINE_VIRTUAL_TEMPLATED(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_VIRTUAL( KLASSNAME, KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a class which is virtual.
#define HK_REFLECTION_DEFINE_SCOPED_VIRTUAL_TEMPLATED(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_VIRTUAL( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )

/// Define an hkTypeInfo for a non-virtual class. The vtable member will be set to null.
#define HK_REFLECTION_DEFINE_NONVIRTUAL_TEMPLATED(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_NONVIRTUAL( KLASSNAME, KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a non-virtual class. The vtable member will be set to null.
#define HK_REFLECTION_DEFINE_SCOPED_NONVIRTUAL_TEMPLATED(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_NONVIRTUAL( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )

/// Define an hkTypeInfo for a non-virtual class which has no constructor (for simple structures).
#define HK_REFLECTION_DEFINE_SIMPLE_TEMPLATED(KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_SIMPLE( KLASSNAME, KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )
/// Define an hkTypeInfo for a non-virtual class which has no constructor (for simple structures).
#define HK_REFLECTION_DEFINE_SCOPED_SIMPLE_TEMPLATED(SCOPE, SCOPENAME, KLASS, KLASSNAME) \
	HK_REFLECTION_DEFINE_INTERNAL_SIMPLE( SCOPENAME##KLASSNAME, SCOPE::KLASS, KLASS, HK_REFLECTION_TEMPLATE_SPECIFIER )

/// Utility to define an empty stub hkClass instance.
#define HK_REFLECTION_DEFINE_STUB_VIRTUAL_BASE(KLASS) \
    const hkClass KLASS##Class(#KLASS, HK_NULL, sizeof(KLASS), HK_NULL, 0, HK_NULL, 0, HK_NULL, 0, HK_NULL, HK_NULL, 0);
/// Utility to define an empty stub hkClass instance.
#define HK_REFLECTION_DEFINE_STUB_VIRTUAL(KLASS, PARENT) \
    const hkClass KLASS##Class(#KLASS, &PARENT##Class, sizeof(KLASS), HK_NULL, 0, HK_NULL, 0, HK_NULL, 0, HK_NULL, HK_NULL, 0);

#endif // HK_BASE_TYPE_INFO_H

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
