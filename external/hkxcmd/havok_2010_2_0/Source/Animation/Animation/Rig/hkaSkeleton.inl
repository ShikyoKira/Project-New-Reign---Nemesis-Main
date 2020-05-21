/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

HK_FORCE_INLINE hkaSkeleton::LocalFrameOnBone::LocalFrameOnBone()
{
}

HK_FORCE_INLINE hkaSkeleton::LocalFrameOnBone::LocalFrameOnBone( hkFinishLoadedObjectFlag f )
	: m_localFrame(f)
{
}

HK_FORCE_INLINE hkaSkeleton::hkaSkeleton()
{
}

HK_FORCE_INLINE hkaSkeleton::hkaSkeleton( const hkaSkeleton& skel )
: hkReferencedObject(skel)
{
	m_name = skel.m_name;
	m_parentIndices = skel.m_parentIndices;
	m_bones = skel.m_bones;
	m_referencePose = skel.m_referencePose;
	m_referenceFloats = skel.m_referenceFloats;
	m_floatSlots = skel.m_floatSlots;
	m_localFrames = skel.m_localFrames;
}

HK_FORCE_INLINE hkaSkeleton::hkaSkeleton(hkFinishLoadedObjectFlag f)
	: hkReferencedObject(f), m_name(f), m_parentIndices(f), m_bones(f), m_referencePose(f), m_referenceFloats(f), m_floatSlots(f), m_localFrames(f)
{
}

HK_FORCE_INLINE hkLocalFrame* hkaSkeleton::getLocalFrameForBone( int boneIndex ) const
{
	
	

	for( int i = 0; i < m_localFrames.getSize(); i++ )
	{
		if ( m_localFrames[i].m_boneIndex == boneIndex )
		{
			return m_localFrames[i].m_localFrame;
		}
		else if ( m_localFrames[i].m_boneIndex > boneIndex )
		{
			break;
		}
	}

	return HK_NULL;
}

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
