/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HKBASE_HKSTRINGPTR_H
#define HKBASE_HKSTRINGPTR_H

#include <Common/Base/Types/hkBaseTypes.h>
#include <Common/Base/Container/String/hkString.h>

class hkTrackerStringPtrLayoutHandler;

	/// This class is used to store a char* c-string.
	/// It automatically handles allocations and deallocation of
	/// stored c-string. The string memory may be externally or internally
	/// allocated, the lowest bit of the pointer being used as a
	/// should-deallocate flag. This simplifies destruction of objects
	/// which may be allocated in a packfile for instance.
class hkStringPtr
{
	public:
		friend class hkTrackerStringPtrLayoutHandler;
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, hkStringPtr );

			///
		enum StringFlags
		{
				/// If we own it, we free it.
			OWNED_FLAG = 0x1,
		};

			/// Default constructor.
		hkStringPtr();

			/// Copy from a const char*.
		hkStringPtr(const char* string);

			/// Copy from a const char*, null terminated after len characters.
		hkStringPtr(const char* string, int len);

			/// Copy constructor.
		hkStringPtr(const hkStringPtr& strRef);

			/// Finish constructor.
		hkStringPtr(hkFinishLoadedObjectFlag f);

			/// Destructor.
		~hkStringPtr();

			/// Copy from c string.
		hkStringPtr& operator=(const char* s);

			/// Copy from hkStringPtr.
		hkStringPtr& operator=(const hkStringPtr& s);


			/// Return const pointer to stored string.
		inline const char* cString() const;

			/// Implicit conversion to c string.
		inline operator const char*() const;

			/// Number of characters in this string.
		int getLength() const;

			/// Const index operator.
		inline char operator[](int idx) const;


			/// Replace this strings value with the formatted string.
		void printf(const char* fmt, ...);

			/// Replace this strings value with the given string.
		void set(const char* s, int len=-1);


			/// Comparison.
		inline hkBool32 operator==(const char* s) const;
			/// Comparison.
		inline hkBool32 operator!=(const char* s) const;

			/// Return true if s is a prefix of this string
		inline hkBool32 startsWith(const char* s) const;
			/// Return true if s is a suffix of this string
		inline hkBool32 endsWith(const char* s) const;
			/// Forward to hkString::strCmp
		inline int compareTo(const char* s) const;

	private:

			/// m_stringAndFlag&~1 is the real pointer, m_stringAndFlag&1 is the deallocate flag.
		const char* m_stringAndFlag;
};

#include <Common/Base/Container/String/hkStringPtr.inl>

#endif // HKBASE_HKSTRINGPTR_H

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
