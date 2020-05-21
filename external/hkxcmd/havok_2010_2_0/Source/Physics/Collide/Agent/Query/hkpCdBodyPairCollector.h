/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_COLLIDE2_CD_BODY_PAIR_COLLECTOR
#define HK_COLLIDE2_CD_BODY_PAIR_COLLECTOR


class hkpCdBody;

	/// This class is used as an interface to the collision detector ( e.g., getPenetrations() )
	/// to collect pairs of hkpCdBody
class hkpCdBodyPairCollector
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_AGENT, hkpCdBodyPairCollector);

			/// Default constructor calls reset(), initializing the early out variable to false.
		inline hkpCdBodyPairCollector();

		inline virtual ~hkpCdBodyPairCollector();

			/// This is the function called for every hit of the collision detector.
			/// Note: for optimization purposes this should set the m_earlyOut:
			/// - true if you want to get no more hits
			/// - false if you want to get more hits (which is the default)
		virtual void addCdBodyPair( const hkpCdBody& bodyA, const hkpCdBody& bodyB ) = 0;

			/// resets m_earlyOut to false. You must call this function if you want to reuse an object of this class.
		inline virtual void reset();	

			/// Gets the early out, if true, no more CdBodyPairs are reported
		inline hkBool getEarlyOut( ) const;

	protected:

		hkBool m_earlyOut;

};

#include <Physics/Collide/Agent/Query/hkpCdBodyPairCollector.inl>


#endif // HK_COLLIDE2_CD_BODY_PAIR_COLLECTOR

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
