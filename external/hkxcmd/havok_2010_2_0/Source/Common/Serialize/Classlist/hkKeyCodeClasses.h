/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

// Build up a classlist based on the Havok products in use.

//HK_REFLECTION_PARSER_EXCLUDE_FILE

#include <Common/Base/KeyCode.h>

#ifdef HK_FEATURE_PRODUCT_PHYSICS
#	define HK_FEATURE_REFLECTION_PHYSICS
#endif

#ifdef HK_FEATURE_PRODUCT_ANIMATION
#	define HK_FEATURE_REFLECTION_ANIMATION
#endif

#ifdef HK_FEATURE_PRODUCT_BEHAVIOR
#	define HK_FEATURE_REFLECTION_BEHAVIOR
#endif

#ifdef HK_FEATURE_PRODUCT_CLOTH
#	define HK_FEATURE_REFLECTION_CLOTH
#endif

#ifdef HK_FEATURE_PRODUCT_DESTRUCTION
#	define HK_FEATURE_REFLECTION_DESTRUCTION
#endif

#ifdef HK_FEATURE_PRODUCT_AI
#	define HK_FEATURE_REFLECTION_AI
#endif

#include <Common/Serialize/Classlist/hkClasses.h>

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
