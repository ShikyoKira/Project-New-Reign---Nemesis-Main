/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_MATH_STEP_INFO_H
#define HK_MATH_STEP_INFO_H

/// Stores step delta time and inverse delta time for simulation step.
class hkStepInfo
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE_CLASS, hkStepInfo );

		hkStepInfo(){}

		void set( hkTime startTime, hkTime endTime)
		{
			m_startTime = startTime;
			m_endTime   = endTime;
			m_deltaTime = endTime - startTime;
			m_invDeltaTime = m_deltaTime == 0.0f ? 0.0f : 1.0f / m_deltaTime;
		}

		hkStepInfo( hkTime startTime, hkTime endTime)
		{
			set( startTime, endTime );
		}

	public:

			/// The time at the start of the integration step
			/// We need the align to be able to upload this structure to the SPU
		HK_ALIGN16( hkPadSpu<hkTime> m_startTime );

			/// The time at the end of the integration step
		hkPadSpu<hkTime> m_endTime;

			/// the current time step being simulated
		hkPadSpu<hkReal> m_deltaTime;

			/// the inverse of the current time step being simulated
		hkPadSpu<hkReal> m_invDeltaTime;
};

#endif // HK_MATH_STEP_INFO_H

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
