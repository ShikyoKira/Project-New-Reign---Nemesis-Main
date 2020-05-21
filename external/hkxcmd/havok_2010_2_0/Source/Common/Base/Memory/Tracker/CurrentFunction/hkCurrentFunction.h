/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_CURRENT_FUNCTION_H
#define HKBASE_CURRENT_FUNCTION_H

// !!!!!!!!!!!!!!!!!!!!!!!!!!! HK_CURRENT_FUNCTION !!!!!!!!!!!!!!!!!!!!!!

#if defined(__GNUC__) || defined(__MWERKS__) || defined(__SN_VER__)
#	define HK_CURRENT_FUNCTION __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__) // msvc
#	define HK_CURRENT_FUNCTION __FUNCSIG__
#elif defined(__INTEL_COMPILER)
#	define HK_CURRENT_FUNCTION __FUNCTION__
#else
#	error "Unknown compiler: Fixme"
#endif

// !!!!!!!!!!!!!!!!!!!!!!!!!!! HK_METHOD_CLASS_NAME !!!!!!!!!!!!!!!!!!!!!!

#if defined(_MSC_VER)
# define HK_METHOD_CLASS_NAME __FUNCTION__
#elif defined(__GNUC__) || defined(__MWERKS__) || defined(__SN_VER__)
# define HK_METHOD_CLASS_NAME __PRETTY_FUNCTION__
#elif defined(__FUNCSIG__)
#	define HK_METHOD_CLASS_NAME __FUNCSIG__
#elif defined(__INTEL_COMPILER)
#	define HK_METHOD_CLASS_NAME __FUNCTION__
#else
#	error "Unknown compiler: Fixme"
#endif


struct hkCurrentFunctionUtil
{
		/// Turns the string returned from a function that takes no parameters and returns HK_METHOD_CLASS_NAME
		/// into just the class name. The result is stored in className - the buffer must be big enough to hold the
		/// longest typename.
	static void HK_CALL getClassName(const char* methodText, char* className);
};



#endif // HKBASE_CURRENT_FUNCTION_H

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
