/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKBASE_H
#define HKBASE_HKBASE_H


#include <Common/Base/Types/hkBaseTypes.h>

	// C4100 'identifier' : unreferenced formal parameter
	//		a function need not use all its arguments
	// C4127 conditional expression is constant
	//		constant conditionals are often used inside asserts
	// C4505 'function' : unreferenced local function has been removed
	//		lots of inline functions are not used in a compilation unit
	// C4510 'class' : default constructor could not be generated
	// C4511 'class' : copy constructor could not be generated
	// C4512 'class' : assignment operator could not be generated
	//		many classes are not designed with value semantics
	// C4514 unreferenced inline/local function has been removed
	//		lots of inline functions are not used in a compilation unit
	// C4714 force inlined function not inlined. This warning is only disabled in debug modes.
#	pragma warning(push)
#	pragma warning(disable: 4100 4127 4324 4505 4510 4511 4512 4514)
//#	pragma warning(disable: 1684 981 1419 1418 271 1572 128 ) // Intel compiler warnings
#	if defined(HK_DEBUG)
#		pragma warning(disable: 4714) 
#	endif
#	ifndef _CRT_SECURE_NO_DEPRECATE
#		define _CRT_SECURE_NO_DEPRECATE 1
#	endif
#	ifndef _CRT_NONSTDC_NO_DEPRECATE
#		define _CRT_NONSTDC_NO_DEPRECATE 1
#	endif



#include <Common/Base/Memory/MemoryClasses/hkMemoryClassDefinitions.h>

#include <Common/Base/Memory/Router/hkMemoryRouter.h>

#include <Common/Base/Object/hkBaseObject.h>
#include <Common/Base/Object/hkReferencedObject.h>

#include <Common/Base/Object/hkSingleton.h>

#include <Common/Base/System/Io/OStream/hkOStream.h>	/* <todo> remove */

#include <Common/Base/System/Error/hkError.h>
#include <Common/Base/Types/hkRefPtr.h>
#include <Common/Base/Types/hkScopedPtr.h>

struct hkPlacementNewArg;
inline void* operator new(hk_size_t, hkPlacementNewArg* p) { return p; }
inline void* operator new[](hk_size_t, hkPlacementNewArg* p) { return p; }
inline void operator delete(void* /*unused*/, hkPlacementNewArg* /*unused*/) { HK_ASSERT(0x5c4071a3, false); }
inline void operator delete[](void* /*unused*/, hkPlacementNewArg* /*unused*/) { HK_ASSERT(0x7ca45d11, false); }

#include <Common/Base/Types/hkTypeTraits.h>
#include <Common/Base/Memory/Util/hkMemUtil.h>
#include <Common/Base/Container/Array/hkArray.h>
#include <Common/Base/Container/Array/hkSmallArray.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>
#include <Common/Base/Types/hkRefVariant.h>

#include <Common/Base/Math/hkMath.h>

#include <Common/Base/Container/String/hkString.h>
#include <Common/Base/Container/String/hkStringPtr.h>

#endif // HKBASE_HKBASE_H

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
