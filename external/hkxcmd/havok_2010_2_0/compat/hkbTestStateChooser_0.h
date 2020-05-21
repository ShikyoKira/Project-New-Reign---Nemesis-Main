#pragma once

#include <Common/Base/hkBase.h>
#include "hkbStateChooser_0.h"

class hkbTestStateChooser : public hkbStateChooser
{
public:
   HK_DECLARE_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BEHAVIOR_RUNTIME );
   HK_DECLARE_REFLECTION();

public:
   HK_FORCE_INLINE hkbTestStateChooser(void) {}
   HK_FORCE_INLINE hkbTestStateChooser( hkFinishLoadedObjectFlag flag ) : hkbStateChooser(flag) 
   , m_string(flag)
      {}
   // Properties
   hkInt32 m_int;
   hkReal m_real;
   hkStringPtr m_string;
};
extern const hkClass hkbTestStateChooserClass;

