/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_SERIALIZE_STRUCTURELAYOUT_H
#define HK_SERIALIZE_STRUCTURELAYOUT_H

class hkClassMember;
template <typename K, typename V, typename Allocator> class hkPointerMap;

/// Class which can duplicate the compiler structure layout procedure.
class hkStructureLayout
{
	public:

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_SERIALIZE, hkStructureLayout);

			/// Structure to define how the compiler lays out struct members.
		struct LayoutRules
		{
				/// Number of bytes in a pointer. i.e., sizeof(void*)
				/// Valid values are 4 or 8.
			hkUint8 m_bytesInPointer;

				/// Target is little endian.
				/// e.g. True for PlayStation(R)2. False for powerpc.
			hkUint8 m_littleEndian;

				/// Derived members appear after alignment padding.
				/// e.g., struct A { hkVector4 x; int i; };
				/// struct B : public A { int j; };
				/// All compilers have sizeof(A)==2*sizeof(hkVector4)
				/// because of the alignment padding. Some compilers simply
				/// append members to the base class so that offsetof(B,j)==sizeof(A)
				/// Others can reuse the padding for derived classes so that
				/// sizeof(B)==sizeof(A) and offsetof(B,j)==offsetof(B,i)+sizeof(int)
				/// e.g. True for all gcc based compilers except on OsX, false for msvc and cw.
			hkUint8 m_reusePaddingOptimization;

				/// The target implements the empty base class optimization.
				/// e.g., struct A {}; struct B : public A { int x; };
				/// All compilers have sizeof(A) > 0. Some compilers reuse the padding
				/// for derived class members such that offsetof(B,x) == 0.
				/// e.g., gcc2 does not implement this optimization.
			hkUint8 m_emptyBaseClassOptimization;

			hkBool operator==(const LayoutRules& other) const
			{
				return (m_bytesInPointer == other.m_bytesInPointer) && 
					(m_littleEndian == other.m_littleEndian) && 
					(m_reusePaddingOptimization == other.m_reusePaddingOptimization) && 
					(m_emptyBaseClassOptimization == other.m_emptyBaseClassOptimization);
			}
		private:
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_SERIALIZE, hkStructureLayout::LayoutRules );
		};

			/// Create a structure layout with the host layout rules.
		hkStructureLayout();
		
			/// Create a structure layout with the given layout rules.
		hkStructureLayout( const LayoutRules& rules );

			/// Compute the offsets of the members of klass according to the target rules.
			/// The offsets are written into the supplied class. The read-write parameter
			/// classesDone keeps track of which classes have been processed so far. This
			/// may persist across several calls.
		void computeMemberOffsetsInplace( hkClass& klass, hkPointerMap<const hkClass*, int>& classesDone, hkBool32 recurse=true ) const;

			/// Access the rules of this layout.
		const LayoutRules& getRules() const { return m_rules; }

			/// The layout rules for the platform being compiled.
		static const LayoutRules HostLayoutRules;

			// Layout for various compilers/platforms.
			// If no rules match your particular compiler/host combination, you
			// can create your own by examining HostLayoutRules on the target.

			/// MSVC Win32
		static const LayoutRules MsvcWin32LayoutRules;
			/// The original Xbox. See also Xbox360LayoutRules.
		static const LayoutRules MsvcXboxLayoutRules;
			/// MSVC AMD64
		static const LayoutRules MsvcAmd64LayoutRules;

			/// CodeWarrior PlayStation(R)2
		static const LayoutRules CwPs2LayoutRules;
			/// CodeWarrior Nintendo GameCube
		static const LayoutRules CwNgcLayoutRules;
			/// CodeWarrior PSP(R) (PlayStation(R)Portable)
		static const LayoutRules CwPspLayoutRules;
			/// CodeWarrior Wii
		static const LayoutRules CwWiiLayoutRules;

			/// GCC 3.2 PlayStation(R)2
		static const LayoutRules Gcc32Ps2LayoutRules;
			/// GCC 1.51 PSP(R)
		static const LayoutRules Gcc151PspLayoutRules;
			/// GCC 2.95 Linux
		static const LayoutRules Gcc295LinuxLayoutRules;
			/// GCC 3.3 Linux x86
		static const LayoutRules Gcc33LinuxLayoutRules;
			/// GCC 3.3 Linux x64
		static const LayoutRules Gcc33Amd64LinuxLayoutRules;

			/// SN 3.1 PlayStation(R)2
		static const LayoutRules Sn31Ps2LayoutRules;
			/// SN 1.0 PSP(R)
		static const LayoutRules Sn10PspLayoutRules;
			/// SN 3.93 Nintendo GameCube
		static const LayoutRules Sn393NgcLayoutRules;

			/// Xbox 360
		static const LayoutRules Xbox360LayoutRules;

			/// GCC PlayStation(R)3
		static const LayoutRules GccPs3LayoutRules;

			/// GCC 4.0 Mac-PPC
		static const LayoutRules Gcc40MacPpcLayoutRules;
			/// GCC 4.0 Mac-Intel
		static const LayoutRules Gcc40MacIntelLayoutRules;
			/// GCC 4.0 Mac-Arm (Iphone/ipod touch etc)
		static const LayoutRules Gcc40MacArmLayoutRules;

	protected:

		LayoutRules m_rules;
};

#endif // HK_SERIALIZE_STRUCTURELAYOUT_H

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
