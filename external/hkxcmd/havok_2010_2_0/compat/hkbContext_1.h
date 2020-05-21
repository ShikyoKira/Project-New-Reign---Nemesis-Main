#pragma once

#include <Common/Base/hkBase.h>
#include "hkbGeneratorOutputListener_0.h"

struct hkbContext
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbContext );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbContext(void) {}
   // Properties
   hkRefVariant m_character;
   hkRefVariant m_behavior;
   hkRefVariant m_nodeToIndexMap;
   hkRefVariant m_eventQueue;
   hkRefVariant m_sharedEventQueue;
   hkRefPtr<hkbGeneratorOutputListener> m_generatorOutputListener;
   hkBool m_eventTriggeredTransition;
   hkRefVariant m_world;
   hkRefVariant m_attachmentManager;
   hkRefVariant m_animationCache;
};
extern const hkClass hkbContextClass;

