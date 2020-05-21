/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKTYPETRAITS_H
#define HKBASE_HKTYPETRAITS_H

#ifdef HK_COMPILER_INTEL
#pragma warning disable 327 // #Null reference not allowed#  from the type traits cast below
#endif

template <bool N> struct hkTraitBool { typedef hkTraitBool<N> type; enum { result = N }; };
typedef hkTraitBool<true> hkTypeIsPod;
typedef hkTraitBool<false> hkTypeIsClass;

template <typename T>
hkBool::CompileTimeFalseType operator==(const T&, const hkTypeIsPod&);

// Default hkIsPodType<T>::type() == hkBoolean<false>.
#if defined(__GCCXML__)
  template <typename T>
  struct hkIsPodType : public hkTraitBool< ( sizeof(*static_cast<T*>(0) == *static_cast<const hkTypeIsPod*>(0)) == sizeof(hkBool::CompileTimeTrueType) ) > {};
#else
  template <typename T>
  struct hkIsPodType : public hkTraitBool< ( sizeof(*reinterpret_cast<T*>(128) == *reinterpret_cast<const hkTypeIsPod*>(128)) == sizeof(hkBool::CompileTimeTrueType) ) > {};
#endif

// This declaration is required to treat a class or struct as a pod type. Must have public visibility.
#define HK_DECLARE_POD_TYPE() \
	hkBool::CompileTimeTrueType operator== (const hkTypeIsPod&) const;

#define HK_DECLARE_POD_TYPE_IF_POD(TYPE) \
	hkBool::CompileTimeTrueType operator== (const hkTraitBool< hkIsPodType< TYPE >::result >&) const;

// This can be used to define a type as a pod type.
#define HK_DEFINE_AS_POD_TYPE(T) \
	template<> struct hkIsPodType< T > : public hkTypeIsPod {};

//
// Define Havok pod types.
//
HK_DEFINE_AS_POD_TYPE(bool);
HK_DEFINE_AS_POD_TYPE(hkBool);
HK_DEFINE_AS_POD_TYPE(char);
HK_DEFINE_AS_POD_TYPE(hkInt8);
HK_DEFINE_AS_POD_TYPE(hkUint8);
HK_DEFINE_AS_POD_TYPE(hkInt16);
HK_DEFINE_AS_POD_TYPE(hkUint16);
HK_DEFINE_AS_POD_TYPE(hkInt32);
HK_DEFINE_AS_POD_TYPE(hkUint32);
HK_DEFINE_AS_POD_TYPE(hkInt64);
HK_DEFINE_AS_POD_TYPE(hkUint64);
#if HK_POINTER_SIZE != 8
	HK_DEFINE_AS_POD_TYPE(hkLong);
	HK_DEFINE_AS_POD_TYPE(hkUlong);
#endif
HK_DEFINE_AS_POD_TYPE(hkUFloat8);
HK_DEFINE_AS_POD_TYPE(hkHalf);
HK_DEFINE_AS_POD_TYPE(hkReal);
HK_DEFINE_AS_POD_TYPE(hkDouble64);

// built-in hkVariant
HK_DEFINE_AS_POD_TYPE(hkVariant);

// Pointers are pod types
template <typename T> struct hkIsPodType<T*> : public hkTypeIsPod {};
// Arrays
template <typename T, unsigned int N> struct hkIsPodType<T[N]> : public hkIsPodType< T > {};

#endif // HKBASE_HKTYPETRAITS_H

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
