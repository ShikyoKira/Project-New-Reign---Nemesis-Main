#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGeneratorSyncInfoSyncPoint_0.h"

struct hkbGeneratorSyncInfo
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbGeneratorSyncInfo );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbGeneratorSyncInfo(void) {}
   // Properties
   hkbGeneratorSyncInfoSyncPoint m_syncPoints[8];
   hkReal m_baseFrequency;
   hkReal m_localTime;
   hkReal m_playbackSpeed;
   hkInt8 m_numSyncPoints;
   hkBool m_isCyclic;
   hkBool m_isMirrored;
   hkBool m_isAdditive;
};
extern const hkClass hkbGeneratorSyncInfoClass;

