/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//HK_REFLECTION_PARSER_EXCLUDE_FILE

// Build up a Havok classlist from the specified products.

#if defined(USING_HAVOK_PHYSICS) 	|| defined(USING_HAVOK_ANIMATION) 	|| defined(USING_HAVOK_BEHAVIOR) 	|| defined(USING_HAVOK_CLOTH) 	|| defined(USING_HAVOK_DESTRUCTION) 	|| defined(USING_HAVOK_AI)
	#error Found an old USING_HAVOK_<X> macro. These have been replaced by HK_FEATURE_PRODUCT_<X>
#endif

#include <Common/Serialize/Classlist/hkCommonClasses.h>

#if defined(HK_FEATURE_REFLECTION_PHYSICS)
#  include <Common/Serialize/Classlist/hkPhysicsClasses.h>
#endif

#if defined(HK_FEATURE_REFLECTION_ANIMATION)
#  include <Common/Serialize/Classlist/hkAnimationClasses.h>
#endif

#if defined(HK_FEATURE_REFLECTION_BEHAVIOR)
#  include <Common/Serialize/Classlist/hkBehaviorClasses.h>
#endif

#if defined(HK_FEATURE_REFLECTION_CLOTH)
#  include <Common/Serialize/Classlist/hkClothClasses.h>
#endif

#if defined(HK_FEATURE_REFLECTION_DESTRUCTION)
#  include <Common/Serialize/Classlist/hkDestructionClasses.h>
#endif

#if defined(HK_FEATURE_REFLECTION_AI)
#  include <Common/Serialize/Classlist/hkPathfindingClasses.h>
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
