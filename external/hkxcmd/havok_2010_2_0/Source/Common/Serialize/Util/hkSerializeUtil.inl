/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

template<typename T>
inline T* hkSerializeUtil::loadObject(hkStreamReader* reader, ErrorDetails* errorOut)
{
	hkRefPtr<hkObjectResource> res; res.setAndDontIncrementRefCount( loadOnHeap(reader, errorOut) );
	if( res )
	{
		return res->stealContents<T>();
	}
	return HK_NULL;
}

template<typename T>
inline T* hkSerializeUtil::loadObject(const char* filename, ErrorDetails* errorOut)
{
	hkRefPtr<hkObjectResource> res; res.setAndDontIncrementRefCount( loadOnHeap(filename, errorOut) );
	if( res )
	{
		return res->stealContents<T>();
	}
	return HK_NULL;
}

template<typename T>
inline T* hkSerializeUtil::loadObject(const void* buf, int bufLen, ErrorDetails* errorOut)
{
	hkRefPtr<hkObjectResource> res; res.setAndDontIncrementRefCount( loadOnHeap(buf, bufLen, errorOut) );
	if( res )
	{
		return res->stealContents<T>();
	}
	return HK_NULL;
}

hkSerializeUtil::SaveOptions& hkSerializeUtil::SaveOptions::useBinary(hkBool32 b)
{
	if(b)
	{
		andWith(~SAVE_TEXT_FORMAT);
	}
	else
	{
		orWith(SAVE_TEXT_FORMAT);
	}
	return *this;
}

hkSerializeUtil::SaveOptions& hkSerializeUtil::SaveOptions::useConcise(hkBool32 b)
{
	if(b)
	{
		orWith(SAVE_CONCISE);
	}
	else
	{
		andWith(~SAVE_CONCISE);
	}
	return *this;
}

hkSerializeUtil::SaveOptions& hkSerializeUtil::SaveOptions::useTextNumbers(hkBool32 b)
{
	if(b)
	{
		orWith(SAVE_TEXT_NUMBERS);
	}
	else
	{
		andWith(~SAVE_TEXT_NUMBERS);
	}
	return *this;
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
