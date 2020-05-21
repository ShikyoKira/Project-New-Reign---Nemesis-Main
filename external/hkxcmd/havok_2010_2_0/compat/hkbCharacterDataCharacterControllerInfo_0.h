#pragma once

#include <Common/Base/hkBase.h>
#include <Physics/Utilities/CharacterControl/hkpCharacterControllerCinfo.h>

struct hkbCharacterDataCharacterControllerInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbCharacterDataCharacterControllerInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbCharacterDataCharacterControllerInfo(void) {}
   // Properties
   hkReal m_capsuleHeight;
   hkReal m_capsuleRadius;
   hkUint32 m_collisionFilterInfo;
   hkRefPtr<hkpCharacterControllerCinfo> m_characterControllerCinfo;
};
extern const hkClass hkbCharacterDataCharacterControllerInfoClass;

