/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_STATISTICS_PROCESS_H
#define HK_STATISTICS_PROCESS_H

#include <Common/Visualize/hkProcess.h>

class hkDisplayHandler;
class hkProcessContext;
class hkpWorld;

	/// Retrieves stats from the monitors and sends them for serialization.
class hkStatisticsProcess : public hkReferencedObject, public hkProcess
{
	public:
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_VDB);

			/// Creates a hkStatisticsProcess.
		static hkProcess* HK_CALL create(const hkArray<hkProcessContext*>& contexts);

			/// Registers the hkStatisticsProcess with the hkProcessFactory.
		static void HK_CALL registerProcess();

			/// Gets the tag associated with this viewer type
		virtual int getProcessTag() { return m_tag; }

		static inline const char* HK_CALL getName() { return "Statistics"; }

		virtual void step(hkReal frameTimeInMs);

	protected:

		hkStatisticsProcess(const hkArray<hkProcessContext*>& contexts);
		virtual ~hkStatisticsProcess();
		
	protected:

		hkArray<hkProcessContext*> m_contexts;
		hkArray<char> m_infoBuffer;
		hkArray<char> m_strBuffer;
		static int m_tag;

};

#endif	// HK_STATISTICS_VIEWER_H

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
