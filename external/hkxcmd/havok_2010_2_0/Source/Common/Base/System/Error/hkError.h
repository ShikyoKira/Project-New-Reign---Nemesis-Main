/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKERROR_H
#define HKBASE_HKERROR_H


/// Class with static methods to provide hooks to error reporting functions
/// You can redirect all asserts, errors and warnings by setting the handlers
/// at run time. Asserts and Warnings are only compiled into debug builds.
/// Errors are compiled in all builds. See hkDefaultError.h for a sample implementation
/// of the handlers.
class hkError : public hkReferencedObject, public hkSingleton<hkError>
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		enum Message
		{
			MESSAGE_REPORT,
			MESSAGE_WARNING,
			MESSAGE_ASSERT,
			MESSAGE_ERROR
		};

			/// Return value indicates whether or not to trigger an HK_BREAKPOINT for errors and asserts.
		virtual int message(Message m, int id, const char* description, const char* file, int line) = 0;

			/// Enables/disables diagnostic by id.
		virtual void setEnabled( int id, hkBool enabled ) = 0;

			/// Enables/disables diagnostic by id.
		virtual hkBool isEnabled( int id ) = 0;

			/// Force all diagnostics to be enabled.
		virtual void enableAll() = 0;

			/// Begin a new report section
		virtual void sectionBegin(int id, const char* sectionName) {}

			/// End the current report section
		virtual void sectionEnd() {}
};

	/// convenience class to disable one error id within a scope
class hkDisableError
{
	public:
		hkDisableError( int id )
		{
			hkError& error = hkError::getInstance();
			m_id = id;
			m_wasEnabled = error.isEnabled( id );
			error.setEnabled( id, false );
		}
		~hkDisableError()
		{
			hkError& error = hkError::getInstance();
			error.setEnabled( m_id, m_wasEnabled );
		}

	public:
		int m_id;
		hkBool m_wasEnabled;
};

class hkErrStream : public hkOstream
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_BASE);
		hkErrStream( void* buf, int bufSize );
};
typedef void (HK_CALL *hkErrorReportFunction)(const char* s, void* errorReportObject);

namespace hkCompileError
{
		// Generic compile time failure
	template <bool b> struct COMPILE_ASSERTION_FAILURE;
	template <> struct COMPILE_ASSERTION_FAILURE<true>{ };

		// The reflection parser and the compile time check disagree about whether a class has a vtable.
		// The script may not have detected that a class has a vtable. e.g. no virtual methods
		// declared in the body. In this case add //+virtual(true) at the start of the class definition.
		// Otherwise, the compile time vtable check may have failed if the object does not inherit from hkBaseObject.
		// In this case, you can mark the base class as virtual by adding the declaration
		// "hkBool::YesType hkIsVirtual(MyClass*);" after the class definition. A function body is not needed.
	template <bool b> struct REFLECTION_PARSER_VTABLE_DETECTION_FAILED;
	template <> struct REFLECTION_PARSER_VTABLE_DETECTION_FAILED<true>{ };
}

/// Cause a compile error if the assert fails.
/// Note: Use this only in c++ files, not header files as this macro introduces a symbol based on the line number.
#define HK_COMPILE_TIME_ASSERT(a) enum { HK_PREPROCESSOR_JOIN_TOKEN(compile_time_assert_, __LINE__) \
										= sizeof(hkCompileError::COMPILE_ASSERTION_FAILURE<bool(a)>) }

/// Compile time assert with an error message.
/// The MSG argument should give a hint about why the assert failed as the cause may not be apparent
/// from the compile error. MSG should be defined in the hkCompileError namespace
/// Note: Use this only in c++ files, not header files as this macro introduces a symbol based on the line number.
#define HK_COMPILE_TIME_ASSERT2(a,MSG) enum { HK_PREPROCESSOR_JOIN_TOKEN(compile_time_assert_, __LINE__) \
										= sizeof(hkCompileError::MSG<bool(a)>) }

#define HK_REPORT_SECTION_BEGIN(id, name)	hkError::getInstance().sectionBegin(id, name);

#define HK_REPORT_SECTION_END()				hkError::getInstance().sectionEnd();



// asserts and warnings may be compiled out

#	define HK_WARN_ALWAYS(id, TEXT)		HK_MULTILINE_MACRO_BEGIN																						\
	char assertBuf[512];																							\
	hkErrStream ostr(assertBuf, sizeof(assertBuf));																	\
	ostr << TEXT;																									\
	hkError::getInstance().message(hkError::MESSAGE_WARNING, id, assertBuf, __FILE__, __LINE__);					\
	HK_MULTILINE_MACRO_END


#	define HK_ERROR(id, TEXT)			HK_MULTILINE_MACRO_BEGIN																						\
										char assertBuf[512];																							\
										hkErrStream ostr(assertBuf,sizeof(assertBuf));																	\
										ostr << TEXT;																									\
										if ( hkError::getInstance().message(hkError::MESSAGE_ERROR, id, assertBuf, __FILE__, __LINE__) )				\
										{																												\
											HK_BREAKPOINT(id);																							\
										}																												\
										HK_MULTILINE_MACRO_END

#	define HK_REPORT(TEXT)				HK_MULTILINE_MACRO_BEGIN																						\
										char reportBuf[512];																							\
										hkErrStream ostr(reportBuf,sizeof(reportBuf));																	\
										ostr << TEXT;																									\
										hkError::getInstance().message(hkError::MESSAGE_REPORT, -1, reportBuf, __FILE__, __LINE__);						\
										HK_MULTILINE_MACRO_END

#	define HK_REPORT2(id, TEXT)			HK_MULTILINE_MACRO_BEGIN																						\
										char reportBuf[512];																							\
										hkErrStream ostr(reportBuf,sizeof(reportBuf));																	\
										ostr << TEXT;																									\
										hkError::getInstance().message(hkError::MESSAGE_REPORT, id, reportBuf, __FILE__, __LINE__);						\
										HK_MULTILINE_MACRO_END

#	if defined HK_DEBUG // NOSPU + DEBUG

#		define HK_ASSERT(id, a)			HK_MULTILINE_MACRO_BEGIN 																						\
										if ( !(a) )																										\
										{																												\
											if( hkError::getInstance().message(hkError::MESSAGE_ASSERT, id, #a,__FILE__,__LINE__) )						\
											{																											\
												HK_BREAKPOINT(0);																						\
											}																											\
										}																												\
										HK_MULTILINE_MACRO_END

#		define HK_ASSERT2(id, a, TEXT)	HK_MULTILINE_MACRO_BEGIN 																						\
										if ( !(a) )																										\
										{																												\
											char assertBuf[512];																						\
											hkErrStream ostr(assertBuf, sizeof(assertBuf));																\
											ostr << #a << "\n";																							\
											ostr << TEXT;																								\
											if( hkError::getInstance().message(hkError::MESSAGE_ASSERT, id, assertBuf,__FILE__, __LINE__) )				\
											{																											\
												HK_BREAKPOINT(0);																						\
											}																											\
										}																												\
										HK_MULTILINE_MACRO_END

#		define HK_WARN(id, TEXT)		HK_WARN_ALWAYS (id, TEXT)

#		define HK_WARN_ONCE(id, TEXT)	HK_MULTILINE_MACRO_BEGIN																						\
										if ( hkError::getInstance().isEnabled(id) )																									\
										{																												\
											char assertBuf[512];																						\
											hkErrStream ostr( assertBuf, sizeof(assertBuf) );															\
											ostr << TEXT;																								\
											hkError::getInstance().message(hkError::MESSAGE_WARNING, id, assertBuf, __FILE__, __LINE__);				\
											hkError::getInstance().setEnabled(id, false); 																								\
										}																												\
										HK_MULTILINE_MACRO_END


#	else	// NOSPU + RELEASE

#		define HK_WARN(id, a)				//nothing
#		define HK_WARN_ONCE(id, a)			//nothing
#		define HK_ASSERT(id, a)				//nothing 
#		define HK_ASSERT2(id, a, TEXT)		//nothing 
#	endif	


// Critical asserts - these will trigger breakpoints on the SPU. Additionally, they show up as comments in the assembly file.
//	On non-SPU platforms, they revert to normal asserts.
#	define HK_CRITICAL_ASSERT  HK_ASSERT
#	define HK_CRITICAL_ASSERT2 HK_ASSERT2

#define HK_CHECK_ALIGN16(ADDR) HK_ASSERT(0xff00ff00, (hkUlong(ADDR) & 0xf) == 0 )

#define HK_WARN_ON_DEBUG_IF(check,id,TEXT)		HK_ON_DEBUG(if (check) HK_WARN(id,TEXT))
#define HK_WARN_ONCE_ON_DEBUG_IF(check,id,TEXT)	HK_ON_DEBUG(if (check) HK_WARN_ONCE(id,TEXT))

#endif // HKBASE_HKERROR_H

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
