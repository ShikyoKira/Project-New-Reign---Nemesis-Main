/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_SCOPEDPTR_H
#define HK_SCOPEDPTR_H

/// Scoped pointer to an object. This is a simple container to hold objects that can not exist on the stack.
/// The object is allocated using new() and the default constructor, and delete()'ed on the hkScopedPtr going
/// out of scope.
/// Pointer operators * and -> are overridden to function transparently
template <typename T>
class hkScopedPtr
{
public:
	typedef hkScopedPtr<T> ThisType;
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_BASE, ThisType );

	/// Create a new hkScopedPtr, calling the object's default constructor
	hkScopedPtr() { m_object = new T; }
	/// Pass in an already constructed object. The hkScopedPtr now owns this object
	hkScopedPtr(T* assignIn) { m_object = assignIn; }
	~hkScopedPtr() { delete m_object; }

	const T* get() const { return m_object; }
	T* get() { return m_object; }

	operator T* () { return m_object; }
	operator const T* () const  { return m_object; }

	const T& operator * () const { return *m_object; }
	T& operator * () { return *m_object; }

	const T* operator -> () const { return m_object; }
	T* operator -> () { return m_object; }

protected:
	T* m_object;

private:
	// hkScopedPtr is not copyable or assignable
	hkScopedPtr(const hkScopedPtr<T>& copyIn) { HK_ASSERT(0x3c41c6f4, 0); }
	hkScopedPtr<T>& operator = (const hkScopedPtr<T>& copyIn) { HK_ASSERT(0x3c41c6f5, 0); }
};

#endif //HK_SCOPEDPTR_H

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
