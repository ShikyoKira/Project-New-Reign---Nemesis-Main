#pragma once

#include <Common/Base/hkBase.h>

struct hkbMessageLog
{
   HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME, hkbMessageLog );
   HK_DECLARE_REFLECTION();

   HK_FORCE_INLINE hkbMessageLog(void) {}
   // Properties
   hkRefVariant m_messages;
   hkInt32 m_maxMessages;
};
extern const hkClass hkbMessageLogClass;

