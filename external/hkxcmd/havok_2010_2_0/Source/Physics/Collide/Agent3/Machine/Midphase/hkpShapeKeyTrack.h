/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SHAPE_KEY_TRACK_H
#define HK_SHAPE_KEY_TRACK_H

#include <Common/Base/Container/BlockList/hkBlockList.h>
#include <Physics/Collide/Shape/hkpShape.h>

#define HK_SIZE_OF_SHAPE_KEY_BLOCKS 512

class hkpShapeKeyTrack : public hkBlockList<hkpShapeKey, HK_SIZE_OF_SHAPE_KEY_BLOCKS>
{
	public:
		HK_FORCE_INLINE hkpShapeKeyTrack(): hkBlockList<hkpShapeKey, HK_SIZE_OF_SHAPE_KEY_BLOCKS>(){}
};

class hkpShapeKeyTrackWriter : public hkpShapeKeyTrack::BatchWriter
{
	public:

		HK_FORCE_INLINE void writeBatch( const hkpShapeKey* data, int numElements )
		{
			HK_TIMER_BEGIN("WriteShapeKeys", HK_NULL);
			hkpShapeKeyTrack::BatchWriter::writeBatch( data, numElements );
			HK_TIMER_END();
		}

};

class hkpShapeKeyTrackConsumer : public hkpShapeKeyTrack::BatchConsumer
{
	public:

		hkpShapeKey* getShapeKeysInBuffer( int numShapeKeys );

		HK_FORCE_INLINE void freeShapeKeyBuffer( int numShapeKeys, hkpShapeKey* buffer )
		{
			hkDeallocateStack( buffer, numShapeKeys + 2 );
		}
};

#endif // HK_SHAPE_KEY_TRACK_H

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
