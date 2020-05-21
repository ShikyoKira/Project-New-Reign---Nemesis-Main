/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKSUBSTRING_H
#define HKBASE_HKSUBSTRING_H

#include <Common/Base/Container/String/hkString.h>

class hkOstream;

/// Holding a sub section of a string. Strings do not need null termination, but are defined by a start and end pointer.
/// the end pointer points to a character one past the end of the string. I.e., m_end - m_start = num chars.
struct hkSubString
{
	public:
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_STRING, hkSubString);
		/// Ctor from regular string
		HK_FORCE_INLINE explicit hkSubString(const char* cur);

		HK_FORCE_INLINE explicit hkSubString(const char* start, const char* end);
			
		HK_FORCE_INLINE hkSubString(const char* start, int len);

			/// Copy ctor
		HK_FORCE_INLINE hkSubString(const hkSubString& rhs);
			/// Default ctor
		HK_FORCE_INLINE hkSubString();

			/// Set from start + len
		HK_FORCE_INLINE void set(const char* cur, int len);
			/// Set from pointers
		HK_FORCE_INLINE void set(const char* cur, const char* end);

			/// Assignment
		HK_FORCE_INLINE void operator=(const hkSubString& rhs);
			/// Assignment from c string
		void operator=(const char* rhs);
			
			/// Returns as an integer. If contents isn't a valid int, result is undefined.
		int getInt() const;

			/// Returns true if its empty
		HK_FORCE_INLINE hkBool isEmpty() const { return m_end == m_start; }
			/// Empty the contents
		HK_FORCE_INLINE void empty() { m_end = m_start = HK_NULL; }

			/// Returns true if its valid
		HK_FORCE_INLINE hkBool isValid() const { return m_start <= m_end; }
			/// Make it invalid
		HK_FORCE_INLINE void setInvalid() { m_start = m_end + 1; }

			/// Returns \code -1 if <, 0 if =, 1 if > \endcode
		int compareTo(const hkSubString& rhs) const;

			/// ==
		hkBool operator==(const char* rhs) const;
			/// ==
		hkBool operator==(const hkSubString& rhs) const;
			/// !=
		HK_FORCE_INLINE hkBool operator!=(const hkSubString& rhs) const { return !(*this == rhs); }
			/// !=
		HK_FORCE_INLINE hkBool operator!=(const char* rhs) const { return !(*this == rhs); }

			/// Length
		HK_FORCE_INLINE int length() const { return int(m_end - m_start); }

			/// operator << hkOstream
		friend hkOstream& operator<<(hkOstream& stream, const hkSubString& text);

			/// Calculate a hash
		unsigned int calcHash() const;

			/// Support for being in a map
		static unsigned int hash( const hkSubString& key, unsigned int mod ) { return key.calcHash() & mod; }
		inline static void invalidate( hkSubString& key ) { key.setInvalid(); }
		inline static hkBool32 isValid( const hkSubString& key ) { return key.isValid(); }
		inline static hkBool32 equal( const hkSubString& key0, const hkSubString& key1 ) { return key0 == key1; }

		// Member variables
		const char* m_start;
		const char* m_end;
};


#include <Common/Base/Container/SubString/hkSubString.inl>

#endif // HKBASE_HKSUBSTRING_H

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
