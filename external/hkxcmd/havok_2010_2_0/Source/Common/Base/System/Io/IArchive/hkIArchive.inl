/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#define READ_SINGLE(TYPE) TYPE t; readArrayGeneric(&t, sizeof(TYPE), 1); return t

HK_FORCE_INLINE hkChar hkIArchive::read8()
{
	READ_SINGLE(hkChar);
}

HK_FORCE_INLINE hkUchar hkIArchive::read8u()
{
	READ_SINGLE(hkUchar);
}

HK_FORCE_INLINE hkInt16 hkIArchive::read16()
{
	READ_SINGLE(hkInt16);
}

HK_FORCE_INLINE hkUint16 hkIArchive::read16u()
{
	READ_SINGLE(hkUint16);
}

HK_FORCE_INLINE hkInt32 hkIArchive::read32()
{
	READ_SINGLE(hkInt32);
}

HK_FORCE_INLINE hkUint32 hkIArchive::read32u()
{
	READ_SINGLE(hkUint32);
}

HK_FORCE_INLINE hkInt64 hkIArchive::read64()
{
	READ_SINGLE(hkInt64);
}

HK_FORCE_INLINE hkUint64 hkIArchive::read64u()
{
	READ_SINGLE(hkUint64);
}

HK_FORCE_INLINE hkFloat32 hkIArchive::readFloat32()
{
	READ_SINGLE(hkFloat32);
}

HK_FORCE_INLINE hkDouble64 hkIArchive::readDouble64()
{
	READ_SINGLE(hkDouble64);
}

#undef READ_SINGLE

HK_FORCE_INLINE void hkIArchive::readArray8(hkInt8* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkInt8), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray8u(hkUint8* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkUint8), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray16(hkInt16* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkInt16), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray16u(hkUint16* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkUint16), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray32(hkInt32* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkInt32), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray32u(hkUint32* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkUint32), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray64(hkInt64* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkInt64), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArray64u(hkUint64* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkUint64), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArrayFloat32(hkFloat32* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkFloat32), nelem);
}

HK_FORCE_INLINE void hkIArchive::readArrayDouble32u(hkDouble64* buf, int nelem)
{
	readArrayGeneric(buf, sizeof(hkDouble64), nelem);
}

HK_FORCE_INLINE void hkIArchive::setByteSwap(hkBool onoff)
{
	m_byteSwap = onoff;
}

HK_FORCE_INLINE hkBool hkIArchive::getByteSwap() const
{
	return m_byteSwap;
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
