/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKSTRING_OLD_H
#define HKBASE_HKSTRING_OLD_H

#include <Common/Base/Math/hkMath.h>

/// Deprecated string object.
/// Replaced by hkStringBuf and hkStringPtr for temporary and persistent strings.
class hkStringOld : public hkString
{
	public:	

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_STRING, hkStringOld);

	public: // object methods

			/// Creates an empty string.
		HK_FORCE_INLINE hkStringOld();

			/// Creates a new string as a copy from a null terminated character array.
		HK_FORCE_INLINE hkStringOld(const char* s);

			/// Creates a new string as a copy from a buffer of length len.
			/// The copied string will automatically be null terminated.
		HK_FORCE_INLINE hkStringOld(const char* b, int len);

			/// Creates a new string as a copy of an existing one.
		HK_FORCE_INLINE hkStringOld(const hkStringOld& s);
	private:
			/// For internal use only
			/// Noncopying constructor, the buffer pointed by ptr is used. Memory will be deallocated on destruction.
		hkStringOld(char* ptr, int size, int capacity);

			/// sets the size as length+1
		HK_FORCE_INLINE	void setLength( int length );
	public:
			/// Copy of an existing string.
		HK_FORCE_INLINE hkStringOld& operator=(const hkStringOld& s);

			/// Copy of an existing c string.
		HK_FORCE_INLINE hkStringOld& operator=(const char* s);

			/// Destroys this string.
		HK_FORCE_INLINE ~hkStringOld();

			/// Read only access the i'th character.
		HK_FORCE_INLINE char operator[] (int i) const;

			/// Sets the capacity of the string, can speed up printf operations
		HK_FORCE_INLINE void setCapacity(int capacity);

			/// Returns the capacity
		HK_FORCE_INLINE int getCapacity() const;

			/// Returns the raw data
		HK_FORCE_INLINE const hkArray<char>& getArray() const;

			/// Overwrite the current value. Arguments are the same as for ::sprintf.
		void printf(const char* format, ...); // member function, not HK_CALL
	
			/// Returns the first index of c given range, or -1 if not found.
		int indexOf (char c, int startIndex=0, int endIndex=HK_INT32_MAX) const;

			/// Returns the last index of c, or -1 if not found.
		int lastIndexOf (char c, int startIndex=0, int endIndex=HK_INT32_MAX) const;

			/// Read only access the internal buffer.
		HK_FORCE_INLINE const char* cString() const;

			// overload each comparison for char* to avoid creating temporary string objects.

			/// Returns <0,0,>0 if *this is lexicographically less than, equal to or greater than other.
		int compareTo(const hkStringOld& other) const;

			/// Returns <0,0,>0 if *this is lexicographically less than, equal to or greater than other.
		int compareTo(const char* other) const;

			/// Returns <0,0,>0 if *this is lexicographically less than, equal to or greater than other, ignoring case.
		int compareToIgnoreCase(const hkStringOld& other) const;

			/// Returns <0,0,>0 if *this is lexicographically less than, equal to or greater than other, ignoring case.
		int compareToIgnoreCase(const char* other) const;
	
			/// Convenience operator for use in map<>
		HK_FORCE_INLINE hkBool operator< (const hkStringOld& s) const;

			/// Returns compareTo(s)==0
		HK_FORCE_INLINE hkBool operator== (const hkStringOld& s) const;

			/// Returns compareTo(s)!=0
		HK_FORCE_INLINE hkBool operator!= (const hkStringOld& s) const;

			/// Returns compareTo(s)==0
		HK_FORCE_INLINE hkBool operator== (const char* s) const;

			/// Returns compareTo(s)!=0
		HK_FORCE_INLINE hkBool operator!= (const char* s) const;

			/// Does this string begin with s?
		HK_FORCE_INLINE hkBool beginsWith(const hkStringOld& s) const;

			/// Does this string begin with s?
		hkBool beginsWith(const char* s) const;

			/// Does this string end with s?
		hkBool endsWith(const hkStringOld& s) const;

			/// Does this string end with s?
		hkBool endsWith(const char* s) const;

			/// Returns the concatenation of *this and other.
		hkStringOld operator+ (const hkStringOld& other) const;

			/// Returns the concatenation of *this and other.
		hkStringOld operator+ (const char* other) const;

			/// Sets *this as the concatenation of *this and other.
		hkStringOld& operator+= (const hkStringOld& other);

			/// Sets *this as the concatenation of *this and other.
		hkStringOld& operator+= (const char* other);


			/// Returns the number of characters in this string excluding the trailing NULL
		int getLength() const;

			
			/// Returns a new string where occurrences of 'from' have been replaced with 'to'.
			/// If ReplaceType==REPLACE_ONE only the first occurrence is replaced.
		hkStringOld replace(char from, char to, ReplaceType=REPLACE_ALL) const;
			/// Replaces occurrences of 'from' with 'to'.
			/// If ReplaceType==REPLACE_ONE only the first occurrence is replaced. Returns true if at least an occurrence is found.
		hkBool replaceInplace(char from, char to, ReplaceType=REPLACE_ALL);

			/// Returns a new string where occurrences of 'from' have been replaced with 'to'.
			/// If ReplaceType==REPLACE_ONE only the first occurrence is replaced.
		hkStringOld replace(const hkStringOld& from, const hkStringOld& to, ReplaceType=REPLACE_ALL) const;
			/// Replaces occurrences of 'from' with 'to'.
			/// If ReplaceType==REPLACE_ONE only the first occurrence is replaced. Returns true if at least an occurrence is found.
		hkBool replaceInplace(const hkStringOld& from, const hkStringOld& to, hkStringOld::ReplaceType rtype=REPLACE_ALL);

			/// Returns a copy of the string as upper case.
		hkStringOld asUpperCase() const;
			/// Makes the string uppercase.
		void makeUpperCase();

			/// Returns a copy of the string as lower case.
		hkStringOld asLowerCase() const;
			/// Makes the string lowercase.
		void makeLowerCase();

			/// Returns a copy of the substring from the index onward.
			/// The index begins at 0.
		HK_FORCE_INLINE hkStringOld substr(int index, int maxChars=HK_INT32_MAX) const;
			/// Sets the string as the substring from the index onward.
			/// The index begins at 0.
		HK_FORCE_INLINE void setAsSubstr(int index, int maxChars=HK_INT32_MAX);

		void split( int c, hkArray<hkStringOld>& bits ) const;

	private:

			/// the string is kept in an array of chars
		hkArray<char> m_string;

	private:

		static void HK_CALL copyAndReplace( char* dest, const char* orig, int origLength, const hkStringOld& from, const hkStringOld& to, const hkArray<int>& indices);
};

#include <Common/Base/Container/String/Deprecated/hkStringOld.inl>

#endif // HKBASE_HKSTRING_OLD_H

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
