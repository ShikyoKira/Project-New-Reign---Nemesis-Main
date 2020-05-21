/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

//
// String Internal Representation
//

//
// private methods
//

// inline methods used by other inlines come first


HK_FORCE_INLINE int hkStringOld::getLength() const
{
	// the length of the string is the size -1 (the null termination)
	return m_string.getSize() - 1;
}

HK_FORCE_INLINE const char* hkStringOld::cString() const
{
	return m_string.begin();
}

HK_FORCE_INLINE void hkStringOld::setLength( int length )
{
	m_string.setSize( length + 1 );
}

//
// constructors
//

HK_FORCE_INLINE hkStringOld::hkStringOld()
:m_string(1, 0) // "" string, size==1 => length==0
{
}

HK_FORCE_INLINE hkStringOld::hkStringOld(const hkStringOld& other)
{
	setLength( other.getLength() );
	memCpy( m_string.begin(), other.cString(), other.m_string.getSize() );
}

HK_FORCE_INLINE hkStringOld::hkStringOld(const char* s)
{
	if( s != HK_NULL )
	{
		setLength( strLen(s) );
		memCpy(m_string.begin(), s, m_string.getSize() ); // copy null too
	}
	else
	{
		// empty string
		setLength(0);
		m_string[0] = 0;
	}	
}

HK_FORCE_INLINE hkStringOld::hkStringOld(char* ptr, int size, int capacity)
{
	m_string.setDataAutoFree( ptr, size, capacity );
}


HK_FORCE_INLINE hkStringOld::hkStringOld(const char* buf, int len)
{
	HK_ASSERT(0x1b0d49f3, len >= 0);
	setLength( len );
	hkString::memCpy( m_string.begin(), buf, len );
	m_string[len] = 0; // null terminate
}

HK_FORCE_INLINE hkStringOld& hkStringOld::operator= (const hkStringOld& other)
{
	setLength( other.getLength() );
	memCpy( m_string.begin(), other.cString(), m_string.getSize() );
	return *this;
}
		
HK_FORCE_INLINE hkStringOld& hkStringOld::operator= (const char* s)
{
	if(s != HK_NULL && *s != 0)
	{
		int slen = strLen(s);
		setLength( slen );
		hkString::memCpy(m_string.begin(), s, slen+1); // copy null too
	}
	else
	{
		setLength(0);
		m_string[0] = 0;
	}
	return *this;
}

HK_FORCE_INLINE hkStringOld::~hkStringOld()
{
}

HK_FORCE_INLINE char hkStringOld::operator[] (int index) const
{
	return m_string[index];
}

HK_FORCE_INLINE int hkStringOld::compareTo(const hkStringOld& other) const
{
	return strCmp( cString(), other.cString() );
}
HK_FORCE_INLINE int hkStringOld::compareTo(const char* other) const
{
	return strCmp( cString(), other );
}

HK_FORCE_INLINE int hkStringOld::compareToIgnoreCase(const hkStringOld& other) const
{
	return strCasecmp( cString(), other.cString() );
}

HK_FORCE_INLINE int hkStringOld::compareToIgnoreCase(const char* other) const
{
	return strCasecmp( cString(), other );
}

HK_FORCE_INLINE hkBool hkStringOld::operator< (const hkStringOld& other) const
{
	return compareTo(other) < 0;
}

HK_FORCE_INLINE hkBool hkStringOld::operator== (const hkStringOld& other) const
{
	return compareTo(other) == 0;
}
HK_FORCE_INLINE hkBool hkStringOld::operator!= (const hkStringOld& other) const
{
	return compareTo(other) != 0;
}
HK_FORCE_INLINE hkBool hkStringOld::operator== (const char* other) const
{
	return compareTo(other) == 0;
}
HK_FORCE_INLINE hkBool hkStringOld::operator!= (const char* other) const
{
	return compareTo(other) != 0;
}

HK_FORCE_INLINE void hkStringOld::setCapacity(int capacity) 
{
	m_string.reserve( capacity+1 );
}



HK_FORCE_INLINE int hkStringOld::getCapacity() const
{
	return m_string.getCapacity();
}

HK_FORCE_INLINE const hkArray<char>& hkStringOld::getArray() const
{
	return m_string;
}

HK_FORCE_INLINE hkStringOld hkStringOld::substr(int index, int maxChars) const
{
	if(maxChars > getLength() - index)
	{
		maxChars = getLength() - index;
	}
	return hkStringOld(m_string.begin()+index, maxChars);
}

HK_FORCE_INLINE void hkStringOld::setAsSubstr(int index, int maxChars)
{
	if(maxChars > getLength() - index)
	{
		maxChars = getLength() - index;
	}
	hkString::memMove( m_string.begin(), m_string.begin()+index, maxChars+1 );
	m_string[ maxChars ] = 0;
	setLength( maxChars );
}

HK_FORCE_INLINE hkBool hkStringOld::beginsWith (const hkStringOld& other) const
{
	return this->beginsWith( other.cString() );
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
