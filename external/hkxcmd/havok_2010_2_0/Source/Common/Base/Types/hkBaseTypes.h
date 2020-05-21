/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HKBASE_HKBASETYPES_H
#define HKBASE_HKBASETYPES_H

//
// compiler
//
#if defined(__INTEL_COMPILER)
#	define HK_COMPILER_INTEL
#elif defined(_MSC_VER) // Intel compiler defines this too (most of the time, not on LRB etc though)
#	define HK_COMPILER_MSVC
#elif defined(__SNC__)
#	define HK_COMPILER_SNC
#elif defined(__GNUC__)
#	define HK_COMPILER_GCC
#elif defined(__MWERKS__)
#	define HK_COMPILER_MWERKS
#else
#	error Could not detect compiler
#endif

//
// architecture
//
#if defined(__i386__) || defined(_M_IX86)
#	define HK_ARCH_IA32
#	define HK_ENDIAN_LITTLE 1
#	define HK_ENDIAN_BIG	0
#	define HK_POINTER_SIZE 4
#elif defined(_M_AMD64) || defined(_M_X64) || defined(__amd64) || defined(__x86_64)
#	define HK_ARCH_X64
#	define HK_ENDIAN_LITTLE 1
#	define HK_ENDIAN_BIG	0
#	define HK_POINTER_SIZE 8
#elif defined(_PPC_) || defined(__POWERPC__) || defined(_M_PPC) || defined(_M_PPCBE) || defined(GEKKO)
#	define HK_ARCH_PPC
#	define HK_ENDIAN_LITTLE 0
#	define HK_ENDIAN_BIG	1
#	define HK_POINTER_SIZE 4
#elif defined(__PPU__) && defined(__CELLOS_LV2__)
#	define HK_ARCH_PS3
#	define HK_ENDIAN_LITTLE 0
#	define HK_ENDIAN_BIG	1
#elif defined(__SPU__) && defined(__CELLOS_LV2__)
#	define HK_ARCH_PS3SPU
#	define HK_ENDIAN_LITTLE 0
#	define HK_ENDIAN_BIG	1
#elif defined(arm) || defined(__arm__)
#   define HK_ARCH_ARM
#   if defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7__)
#       define HK_ARCH_ARM_V7
#   elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
#       define HK_ARCH_ARM_V6
#   endif
#   if defined(__ARMEB__)
#	   define HK_ENDIAN_LITTLE 0
#	   define HK_ENDIAN_BIG	1
#   else 
#	   define HK_ENDIAN_LITTLE 1
#	   define HK_ENDIAN_BIG	0
#   endif
#	define HK_POINTER_SIZE 4
#else
#	error Could not autodetect target architecture
#endif

//
// platform
//
#	define HK_PLATFORM_WIN32
#	if defined(_WIN64)
#		define HK_PLATFORM_X64
#	endif
#	define HK_PLATFORM_IS_CONSOLE 0

#	define HK_ALIGN_OF(T) __alignof(T)

//
// types
//

/// hkReal is the default floating point type.
typedef float  hkReal;
/// hkFloat is provided if floats are explicitly required.
typedef float  hkFloat32;
/// hkDouble is provided if doubles are explicit required.
typedef double hkDouble64;


/// Signed 8 bit integer
typedef signed char		hkChar;
/// Signed 8 bit integer
typedef signed char		hkInt8;
/// Signed 16 bit integer
typedef signed short	hkInt16;
/// Signed 32 bit integer
typedef signed int		hkInt32;

/// Unsigned 8 bit integer
typedef unsigned char	hkUchar;
/// Unsigned 8 bit integer
typedef unsigned char	hkUint8;
/// Unsigned 16 bit integer
typedef unsigned short	hkUint16;
/// Unsigned 32 bit integer
typedef unsigned int	hkUint32;

#if defined(HK_ARCH_X64)
#	if defined(HK_PLATFORM_X64)
		typedef unsigned __int64 hkUlong;  // MS- specific
		typedef signed __int64 hkLong;  // MS- specific
#	else
		typedef unsigned long hkUlong; // UNIX64
		typedef signed long hkLong; // UNIX64
#	endif
#elif defined(HK_COMPILER_MSVC) && (_MSC_VER >= 1300)
	typedef unsigned long __w64 hkUlong; // VC7.0 or higher, 64bit warnings
	typedef signed long __w64 hkLong; 
#else
	typedef unsigned long hkUlong;
	typedef signed long hkLong;
#endif


#define HK_CPU_PTR( A ) A

typedef void* hk_va_list;

/// a simple success/failure enum.
enum hkResult
{
	HK_SUCCESS = 0,
	HK_FAILURE = 1
};


//
// useful macros
//

#if  defined(DEBUG) || defined(_DEBUG) || defined(HK_DEBUG)
#	undef HK_DEBUG
#	define HK_DEBUG
#	define HK_ON_DEBUG(CODE)		CODE
#else
#	define HK_ON_DEBUG(CODE)
#endif

// use the compiler friendly but programmer ugly version for release only
#ifdef HK_DEBUG
#	define HK_MULTILINE_MACRO_BEGIN	do {
#	define HK_MULTILINE_MACRO_END		} while(0)
#else
#		define HK_MULTILINE_MACRO_BEGIN	if(1) {
#		define HK_MULTILINE_MACRO_END		} else
#endif

#if defined(HK_ARCH_IA32)
#		define HK_BREAKPOINT(ID) __asm { int 3 }
#elif defined(HK_PLATFORM_LRB)
#	include <unistd.h>
#	include <signal.h>
#	define HK_BREAKPOINT(ID) signal(SIGSTOP, HK_NULL);
#elif defined(HK_PLATFORM_PS3_PPU) 
#		define HK_BREAKPOINT(ID) __asm__ volatile ( "tw 31,1,1" )
#elif defined(HK_PLATFORM_PS3_SPU)
#	define HK_BREAKPOINT(ID) __asm__ volatile ("ilhu $0, %0\n\tiohl $0, %1\n\tstopd $0,$0,$0" : : "i"(ID>>16), "i"(ID&0xFFFF) ); 
#elif defined(HK_PLATFORM_XBOX360)
#	define HK_BREAKPOINT(ID) __debugbreak();
#elif defined(HK_PLATFORM_GC)
#	define HK_BREAKPOINT(ID)    HK_MULTILINE_MACRO_BEGIN						\
								{                                               \
    								register unsigned int HK_ORIGINAL_MSR;		\
    								register unsigned int HK_NEW_MSR;			\
    								asm volatile								\
									{											\
    									mfmsr HK_ORIGINAL_MSR; 					/* Grab the original value of the MSR */ \
    									ori HK_NEW_MSR, HK_ORIGINAL_MSR, 0x400; /* or the original MSR with 0x400 and store it */ \
    									mtmsr HK_NEW_MSR; 						/* Set the MSR to the new value - this will throw exception 0xD00 */ \
    									mtmsr HK_ORIGINAL_MSR; 					/* This puts the MSR back to its original state; required for*/\
										    								    /* getting out of single-step mode (when the user resumes) */\
    								}											\
                                }   											\
								HK_MULTILINE_MACRO_END
#elif defined(HK_ARCH_ARM)
#	define HK_BREAKPOINT(ID) asm("bkpt 0"); 
#else
#	define HK_BREAKPOINT(ID) ((*((int*)0)) = ID);
#endif

#define HK_NULL 0


/// Note that ALIGNMENT must be a power of two for this to work.
/// Note: to use this macro you must cast your pointer to a byte pointer or to an integer value.
#define HK_NEXT_MULTIPLE_OF(ALIGNMENT, VALUE)  ( ((VALUE) + ((ALIGNMENT)-1)) & (~((ALIGNMENT)-1)) )

/// The offset of a member within a structure
#define HK_OFFSET_OF(CLASS,MEMBER) int(reinterpret_cast<hkLong>(&(reinterpret_cast<CLASS*>(16)->MEMBER))-16)

/// A check for whether the offset of a member within a structure is as expected
#define HK_OFFSET_EQUALS(CLASS,MEMBER,OFFSET) (HK_OFFSET_OF(CLASS,MEMBER)==OFFSET)

/// Join two preprocessor tokens, even when a token is itself a macro.
#define HK_PREPROCESSOR_JOIN_TOKEN(A,B) HK_PREPROCESSOR_JOIN_TOKEN2(A,B)
#define HK_PREPROCESSOR_JOIN_TOKEN2(A,B) HK_PREPROCESSOR_JOIN_TOKEN3(A,B)
#define HK_PREPROCESSOR_JOIN_TOKEN3(A,B) A##B

/// Creates an uninitialized buffer large enough for object of type TYPE to fit in while aligned to ALIGN boundary. Creates a pointer VAR to this aligned address.
#define HK_DECLARE_ALIGNED_LOCAL_PTR( TYPE, VAR, ALIGN  )											\
	const int VAR ## BufferSize = ALIGN + sizeof(TYPE);												\
	char VAR ## Buffer[VAR ## BufferSize];															\
	TYPE* VAR = reinterpret_cast<TYPE*>( HK_NEXT_MULTIPLE_OF(ALIGN, hkUlong( VAR ## Buffer )) );	\


//
// compiler specific settings
//


	// *************************************
	//			GCC and SN
	// *************************************
#if defined(HK_COMPILER_GCC) || defined(HK_COMPILER_SNC) || defined(__GCCXML__)
#   if defined(__GNUC_PATCHLEVEL__)
#   	define HK_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 +__GNUC_PATCHLEVEL__)
#	else
#		define HK_COMPILER_GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100)
#   endif
#	define HK_RESTRICT __restrict
#	if (HK_COMPILER_GCC_VERSION >= 40000)
#		undef HK_OFFSET_OF
#		define HK_OFFSET_OF(CLASS,MEMBER) __builtin_offsetof(CLASS,MEMBER)
#	elif ( HK_COMPILER_GCC_VERSION >= 30400 ) && !defined( HK_COMPILER_SNC )
#		undef HK_OFFSET_OF
#		define HK_OFFSET_OF(CLASS,MEMBER) (__offsetof__(reinterpret_cast<long>(&reinterpret_cast<CLASS*>(16)->MEMBER)-16))
#	endif
#	if ( HK_COMPILER_GCC_VERSION >= 40000 ) && !defined( HK_COMPILER_SNC )
#		undef HK_OFFSET_EQUALS
#		define HK_OFFSET_EQUALS(C,M,O) true
#	endif
#	define HK_ALIGN(DECL, ALIGNMENT) DECL __attribute__((aligned(ALIGNMENT)))
#	define HK_ALIGN16(DECL) DECL __attribute__((aligned(16)))
#	define HK_ALIGN128(DECL) DECL __attribute__((aligned(128)))
#	define HK_CLASSALIGN(DECL, ALIGNMENT) HK_ALIGN(DECL, ALIGNMENT)
#	define HK_CLASSALIGN16(DECL) HK_ALIGN16(DECL)

#		define HK_VISIBILITY_HIDDEN __attribute__((visibility("hidden")))

	    typedef unsigned long long hkUint64;
	    typedef long long hkInt64;
		typedef long hkSystemTime;
#	if defined(HK_PLATFORM_MAC386) || defined(HK_PLATFORM_MACPPC) || defined(HK_PLATFORM_MACARM) || ( defined(HK_PLATFORM_PS3_PPU) && ( HK_CELL_SDK_VERSION < 0x080000 ) ) || defined (HK_PLATFORM_PS3_SPU)
		typedef unsigned long hk_size_t;
#	elif (defined(HK_PLATFORM_UNIX) && defined(HK_ARCH_X64)) || (defined(__GCCXML__) && defined(__x86_64)) // UNIX64
		typedef long unsigned int hk_size_t;
#	else
		typedef unsigned hk_size_t;
#	endif
#	if defined(HK_ARCH_IA32) && HK_COMPILER_GCC_VERSION >= 30100
#		define HK_COMPILER_HAS_INTRINSICS_IA32
#	endif
#ifdef __HAVOK_PARSER__
#define HK_FORCE_INLINE
#else
#	define HK_FORCE_INLINE inline
#endif
// calling convention
#	define HK_CALL
#	define HK_FAST_CALL
// deprecation
#	define HK_DEPRECATED /* nothing */
#	define HK_DEPRECATED2(MSG) /* nothing */
// section attribute
#if !defined(HK_PLATFORM_MACARM) && !defined(HK_PLATFORM_MAC386) && !defined(HK_PLATFORM_PSP)
#	define HK_INIT_FUNCTION( FN ) __attribute__ ((section (".text_init." #FN ))) FN
#	define HK_AABB_TREE_FUNCTION( FN ) __attribute__ ((section (".text_aabb." #FN ))) FN
#endif

	// *************************************
	//			MICROSOFT and INTEL
	// *************************************
#elif defined(HK_COMPILER_MSVC) || defined(HK_COMPILER_INTEL)
#	define HK_COMPILER_SUPPORTS_PCH
#	define HK_COMPILER_MSVC_VERSION _MSC_VER
#	ifdef __INTEL_COMPILER
#		define HK_COMPILER_INTEL_VERSION __INTEL_COMPILER
#	else
#		define HK_COMPILER_INTEL_VERSION _MSC_VER
#	endif
#	if (_MSC_VER >= 1400) // 2005 only
#		define HK_RESTRICT __restrict
#	else
#		define HK_RESTRICT 
#	endif
#	pragma warning( disable : 4786 ) // Debug tuncated to 255:
#	pragma warning( disable : 4530 ) // C++ Exception handler used but not enabled:(used in <xstring>)
#		define HK_ALIGN(DECL, ALIGNMENT) __declspec(align(ALIGNMENT)) DECL
#		define HK_ALIGN16(DECL) __declspec(align(16)) DECL
#		define HK_ALIGN64(DECL) __declspec(align(64)) DECL
#		define HK_ALIGN128(DECL) __declspec(align(128)) DECL
#		if !defined(HK_COMPILER_INTEL)
#			define HK_FORCE_INLINE __forceinline
#		else // ICC has no force inline intrinsic
#			define HK_FORCE_INLINE inline
#		endif
#		define HK_CLASSALIGN(DECL, ALIGNMENT) HK_ALIGN(DECL, ALIGNMENT)
#		define HK_CLASSALIGN16(DECL) HK_ALIGN16(DECL)
#		define HK_VISIBILITY_HIDDEN /*empty*/
#	if defined(HK_ARCH_IA32)
		typedef unsigned __int64 hkUint64;
		typedef __int64 hkInt64;
		typedef long hkSystemTime;
#		if defined(HK_COMPILER_MSVC) && (_MSC_VER >= 1300)
			typedef unsigned __w64 hk_size_t; // VC7.0 or higher, 64bit warnings
#		else
			typedef unsigned hk_size_t;
#		endif
#		define HK_COMPILER_HAS_INTRINSICS_IA32
#	elif defined( HK_ARCH_PPC )
		typedef unsigned __int64 hkUint64;
		typedef __int64 hkInt64;
		typedef unsigned hk_size_t;
		typedef __int64 hkSystemTime; // 64bit time
#		define HK_COMPILER_HAS_INTRINSICS_PPC
#	elif defined(HK_ARCH_X64) || defined(HK_ARCH_LRB)
		typedef unsigned __int64 hkUint64;
		typedef __int64 hkInt64;
		typedef __int64 hkSystemTime;
		typedef unsigned __int64 hk_size_t;
#		ifdef HK_ARCH_LRB
#			define HK_COMPILER_HAS_INTRINSICS_LRB
#		else
#			define HK_COMPILER_HAS_INTRINSICS_IA32
#		endif
#	else
#		error No defs for this architecture
#	endif
// calling convention
#	ifndef HK_COMPILER_INTEL
#		define HK_CALL __cdecl
#		define HK_FAST_CALL __fastcall
#	else
#		define HK_CALL 
#		define HK_FAST_CALL 
#	endif
// deprecation
#	if defined(HK_PLATFORM_WIN32) && (_MSC_VER >= 1300) && !defined(MIDL_PASS)
#		define HK_DEPRECATED __declspec(deprecated)
#		define HK_DEPRECATED2(MSG) __declspec(deprecated(MSG))
#	else
#		define HK_DEPRECATED /* nothing */
#		define HK_DEPRECATED2(MSG) /* nothing */
#	endif

	// *************************************
	//			METROWERKS
	// *************************************
#elif defined(HK_COMPILER_MWERKS)
#	define HK_ALIGN(DECL, ALIGNMENT) DECL __attribute__((aligned(ALIGNMENT)))
#	define HK_ALIGN16(DECL) DECL __attribute__((aligned(16)))
#	define HK_ALIGN128(DECL) DECL __attribute__((aligned(128)))
#	define HK_CLASSALIGN(DECL, ALIGNMENT) DECL
#	define HK_CLASSALIGN16(DECL) DECL
#	define HK_VISIBILITY_HIDDEN /*empty*/
		typedef unsigned long long hkUint64;
		typedef long long hkInt64;
			typedef unsigned long hkSystemTime;
		typedef unsigned long hk_size_t;
#	define HK_FORCE_INLINE inline
// calling convention
#	define HK_CALL
#	define HK_FAST_CALL
// deprecation
#	define HK_DEPRECATED /* nothing */
#	define HK_DEPRECATED2(MSG) /* nothing */

#else
#	error Unknown compiler
#endif // compilers


#	define HK_ON_PLATFORM_HAS_SPU(code)

#	define HK_PLATFORM_MULTI_THREAD

#	define HK_ALWAYS_INLINE HK_FORCE_INLINE
#	define HK_LOCAL_INLINE HK_FORCE_INLINE
#	define HK_ASM_SEP(a) 

#ifndef HK_NEVER_INLINE
#		define HK_NEVER_INLINE
#endif

#	define HK_NOSPU_VIRTUAL virtual

#ifndef HK_RESTRICT
#	define HK_RESTRICT
#endif
#ifndef HK_VERY_UNLIKELY
#	define HK_VERY_UNLIKELY(EXPR) EXPR
#	define HK_VERY_LIKELY(EXPR) EXPR
#endif
#ifndef HK_INIT_FUNCTION
#	define HK_INIT_FUNCTION( FN ) FN
#	define HK_AABB_TREE_FUNCTION( FN ) FN
#endif

typedef hkUint16 hkObjectIndex;
typedef hkReal hkTime;

#define HK_INVALID_OBJECT_INDEX 0xffff


HK_FORCE_INLINE hkInt32 HK_CALL hkPointerToInt32( const void* ptr )
{
	return static_cast<int>( hkUlong(ptr) );
}

/// get the byte offset of B - A, as a full long.
HK_FORCE_INLINE hkLong HK_CALL hkGetByteOffset( const void* base, const void* pntr)
{
	return hkLong(pntr) - hkLong(base);
}

/// get the byte offset of B - A, as an int (64bit issues, so here for easy code checks)
HK_FORCE_INLINE int HK_CALL hkGetByteOffsetInt( const void* base, const void* pntr)
{
	return static_cast<int>( hkGetByteOffset( base, pntr ) );
}

/// get the byte offset of B - A, as a full 64bit hkUint64.
HK_FORCE_INLINE hkInt32 HK_CALL hkGetByteOffsetCpuPtr( const HK_CPU_PTR(void*) base, const HK_CPU_PTR(void*) pntr)
{
	return hkInt32(hkLong((HK_CPU_PTR(const char*))(pntr) - (HK_CPU_PTR(const char*))(base)));
}

template <typename TYPE>
HK_ALWAYS_INLINE TYPE* HK_CALL hkAddByteOffset( TYPE* base, hkLong offset )
{
	return reinterpret_cast<TYPE*>( reinterpret_cast<char*>(base) + offset );
}

template <typename TYPE>
HK_ALWAYS_INLINE TYPE HK_CALL hkAddByteOffsetCpuPtr( TYPE base, hkLong offset )
{
	return reinterpret_cast<TYPE>( reinterpret_cast<char*>(base) + offset );
}

template <typename TYPE>
HK_ALWAYS_INLINE const TYPE* HK_CALL hkAddByteOffsetConst( const TYPE* base, hkLong offset )
{
	return reinterpret_cast<const TYPE*>( reinterpret_cast<const char*>(base) + offset );
}

template <typename TYPE>
HK_ALWAYS_INLINE TYPE HK_CALL hkAddByteOffsetCpuPtrConst( TYPE base, hkLong offset )
{
	return reinterpret_cast<const TYPE>( reinterpret_cast<const char*>(base) + offset );
}

	/// If you have a pair of pointers and you have one pointer, than this function allows you to quickly get the other pointer of the pair.
template <typename TYPE>
HK_ALWAYS_INLINE TYPE HK_CALL hkSelectOther( TYPE a, TYPE pairA, TYPE pairB )
{
	return (TYPE)( hkUlong(a) ^ hkUlong(pairA) ^ hkUlong(pairB) );
}

/// If you have a pair of pointers and you have one pointer, than this function allows you to quickly get the other pointer of the pair.
template <typename TYPE>
HK_ALWAYS_INLINE TYPE* HK_CALL hkSelect( int select, TYPE* pairA, TYPE* pairB )
{
	//HK_ASSERT( 0xf0345456, select == 0 || select == 1);
	hkUlong ua = hkUlong(pairA);
	hkUlong ub = hkUlong(pairB);
	return reinterpret_cast<TYPE*>( ua ^ ((ua^ub)&(-select)) );
}




HK_FORCE_INLINE hkUint32 hkNextPowerOf2(hkUint32 in) 
{
	in -= 1;

	in |= in >> 16;
	in |= in >> 8;
	in |= in >> 4;
	in |= in >> 2;
	in |= in >> 1;

	return in + 1; 
}

class hkFinishLoadedObjectFlag
{
	//+hk.MemoryTracker(ignore=True)
public:
	hkFinishLoadedObjectFlag() : m_finishing(0) {}
	int m_finishing;
};

#define hkSizeOf(A) int(sizeof(A))

#define HK_REFLECTION_CLASSFILE_DESTINATION(PATH)
#define HK_REFLECTION_CLASSFILE_HEADER(PATH)
#define HK_DECLARE_REFLECTION() \
	static const struct hkInternalClassMember Members[]; \
	static const hkClass& HK_CALL staticClass(); \
	struct DefaultStruct

class hkClass;

/// A generic object with metadata.

struct hkVariant
{
	//+hk.MemoryTracker(ignore=True)
	void* m_object;
	const hkClass* m_class;
};


	/// False is zero, true is _any_ non-zero value.
	/// Thus comparisons like bool32 will not work as expected.
typedef int hkBool32;

	/// A wrapper to store a hkBool in one byte, regardless of compiler options.
class hkBool
{
	//+hk.MemoryTracker(ignore=True)
	public:

			// used in compile time asserts
		typedef char CompileTimeTrueType;
		typedef int CompileTimeFalseType;

		HK_FORCE_INLINE hkBool()
		{
		}

		HK_FORCE_INLINE hkBool(bool b)
		{
			m_bool = static_cast<char>(b);
		}

		HK_FORCE_INLINE operator bool() const
		{
			return m_bool != 0;
		}

		HK_FORCE_INLINE hkBool& operator=(bool e)
		{
			m_bool = static_cast<char>(e);
			return *this;
		}

		HK_FORCE_INLINE hkBool operator==(bool e) const
		{
			return (m_bool != 0) == e;
		}

		HK_FORCE_INLINE hkBool operator!=(bool e) const
		{
			return (m_bool != 0) != e;
		}

	private:
		char m_bool;
};

    /// A wrapper to store a float in 16 bit. This is a non IEEE representation.
    /// Basically we simply chop off the last 16 bits. That means the whole floating point range
    /// will be supported, but only with 7 bit precision
class hkHalf
{
	//+hk.MemoryTracker(ignore=True)
   public:
	    inline hkHalf() { }
    
	    inline explicit hkHalf(const float& f)
	    {
		    int t = ((const int*)&f)[0];
		    m_value = hkInt16(t>>16);
	    }
    
	    inline hkHalf& operator=(const float& f)
	    {
		    int t = ((const int*)&f)[0];
		    m_value = hkInt16(t>>16);
		    return *this;
	    }

		inline void store( float* f ) const
		{
			hkUint32* asInt = (hkUint32*)f;
			asInt[0] = m_value << 16;
		}
    
	    inline operator float() const
	    {
		    union
		    {
			    int i;
			    float f;
		    } u;
		    u.i = (m_value <<16);
		    return u.f;
	    }

		inline hkInt16 getInt16() const
		{
			return m_value;
		}
    
    private:
	    hkInt16 m_value;
};

#define HK_UFLOAT8_MAX_VALUE 256
extern "C"
{
	extern const hkReal hkUFloat8_intToReal[HK_UFLOAT8_MAX_VALUE];
}

	/// A wrapper to store an unsigned float into 8 bit.
	/// This has a reduced range. Basically the encoding
	/// uses a table holding an exponential function.
	/// The range is [0.010f to 1000002.f] with an average error of 7%
class hkUFloat8
{
	//+hk.MemoryTracker(ignore=True)
public:
	enum { MAX_VALUE = HK_UFLOAT8_MAX_VALUE };

		// the minimum value to encode which is non zero
#define hkUFloat8_eps 0.01f

		// the maximum value to encode
#define hkUFloat8_maxValue 1000000.0f

		inline hkUFloat8(){		}

		hkUFloat8& operator=(const float& fv);
		
		inline hkUFloat8(const float f)
		{
			*this = f;
		}

			/// this = sqrt(a*b)
		inline void setArithmeticMean( const hkUFloat8& a, const hkUFloat8& b)
		{
			m_value = hkUint8( (int(a.m_value) + int(b.m_value) )>>1 );
		}


		inline operator float() const
		{
			return hkUFloat8_intToReal[m_value];
		}

	public:
		hkUint8 m_value;
};

// A lookup table for converting unsigned char to float
// useful for avoiding LHS

extern "C"
{
	extern const hkReal hkUInt8ToReal[256];
}

	/// A wrapper to store an enum with explicit size.
template<typename ENUM, typename STORAGE>
class hkEnum
{
	public:

		hkEnum()
		{
		}

		hkEnum(ENUM e)
		{
			m_storage = static_cast<STORAGE>(e);
		}

		operator ENUM() const
		{
			return static_cast<ENUM>(m_storage);
		}
		void operator=(ENUM e)
		{
			m_storage = static_cast<STORAGE>(e);
		}
		hkBool operator==(ENUM e) const
		{
			return m_storage == static_cast<STORAGE>(e);
		}
		hkBool operator!=(ENUM e) const
		{
			return m_storage != static_cast<STORAGE>(e);
		}

	private:

		STORAGE m_storage;
};

	/// A wrapper to store bitfield with an with explicit size.
template<typename BITS, typename STORAGE>
class hkFlags
{
	public:

		hkFlags()
		{
		}

		hkFlags(STORAGE s)
		{
			m_storage = s;
		}

		void clear()
		{
			m_storage = 0;
		}
		void setAll( STORAGE s )
		{
			m_storage = s;
		}

		void orWith( STORAGE s )
		{
			m_storage |= s;
		}

		void xorWith( STORAGE s )
		{
			m_storage ^= s;
		}

		void andWith( STORAGE s )
		{
			m_storage &= s;
		}

		void setWithMask( STORAGE s, STORAGE mask )
		{
			m_storage = (m_storage & ~mask) | (s & mask);
		}

		STORAGE get() const
		{
			return m_storage;
		}

		STORAGE get( STORAGE mask ) const
		{
			return m_storage & mask;
		}

		bool anyIsSet( STORAGE mask ) const
		{
			return (m_storage & mask) != 0;
		}
		
		bool allAreSet( STORAGE mask ) const
		{
			return (m_storage & mask) == mask;
		}

		bool operator==( const hkFlags& f ) const
		{
			return f.m_storage == m_storage;
		}

		bool operator!=( const hkFlags& f ) const
		{
			return f.m_storage != m_storage;
		}

	private:

		STORAGE m_storage;
};

#	define HK_PADSPU_PROMOTE(e) e
#	define HK_PADSPU_EXTRACT(e) e

	/// wrapper class for variables in structures.
	/// Basically on the PlayStation(R)3 SPU, the SPU can only poorly
	/// access non aligned members. This class give each variable
	/// 16 bytes, thereby dramatically decreasing code size and CPU overhead
template <typename TYPE>
class hkPadSpu
{
	public:

		HK_FORCE_INLINE hkPadSpu() {}

		HK_FORCE_INLINE hkPadSpu(TYPE e)
			: m_storage( HK_PADSPU_PROMOTE(e) )
		{
		}

		HK_FORCE_INLINE void operator=(TYPE e)
		{
			m_storage = HK_PADSPU_PROMOTE(e);
		}

		HK_FORCE_INLINE TYPE val() const
		{
			return HK_PADSPU_EXTRACT(m_storage);
		}

		HK_FORCE_INLINE TYPE operator->() const
		{
			return HK_PADSPU_EXTRACT(m_storage);
		}

		HK_FORCE_INLINE operator TYPE() const
		{
			return val();
		}

		HK_FORCE_INLINE void operator+=(TYPE e)
		{
			*this = val() + e;
		}

		HK_FORCE_INLINE void operator-=(TYPE e)
		{
			*this = val() - e;
		}

		HK_FORCE_INLINE void operator++()
		{
			operator+=(1);
		}

		HK_FORCE_INLINE void operator++(int)
		{
			operator+=(1);
		}

		HK_FORCE_INLINE void operator--()
		{
			operator-=(1);
		}

		HK_FORCE_INLINE void operator--(int)
		{
			operator-=(1);
		}

	private:

		TYPE m_storage;
};

#	define HK_PAD_ON_SPU(TYPE) TYPE
#	define HK_ON_CPU(code) code
#	define HK_ON_SPU(code)
#	define HK_CPU_ARG(code) , code
#	define HK_SPU_ARG(code)

#define HK_HINT_SIZE16(A) hkInt16(A)


struct hkCountOfBadArgCheck
{
	//+hk.MemoryTracker(ignore=True)
	class ArgIsNotAnArray;
	template<typename T> static ArgIsNotAnArray isArrayType(const T*, const T* const*);
	static int isArrayType(const void*, const void*);
};

	/// Returns the number of elements in the C array.
#define HK_COUNT_OF(x) ( \
	0 * sizeof( reinterpret_cast<const ::hkCountOfBadArgCheck*>(x) ) + \
	0 * sizeof( ::hkCountOfBadArgCheck::isArrayType((x), &(x)) ) + \
	sizeof(x) / sizeof((x)[0]) ) 

#	define HK_SPU_INIT_STACK_SIZE_TRACE()
#	define HK_SPU_UPDATE_STACK_SIZE_TRACE()
#	define HK_SPU_OUTPUT_STACK_SIZE_TRACE()
#	define HK_SPU_BSS_GUARD_INIT()
#	define HK_SPU_BSS_GUARD_CHECK()
#	define HK_SPU_STACK_POINTER_CHECK()

#endif // HKBASE_HKBASETYPES_H

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
