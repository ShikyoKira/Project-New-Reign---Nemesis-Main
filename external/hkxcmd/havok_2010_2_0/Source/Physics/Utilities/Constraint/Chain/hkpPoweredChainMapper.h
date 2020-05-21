/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_UTILITIES_POWERED_CHAIN_MAPPER_H
#define HK_UTILITIES_POWERED_CHAIN_MAPPER_H

#include <Common/Base/hkBase.h>
#include <Common/Base/Container/PointerMap/hkPointerMap.h>

extern const class hkClass hkpPoweredChainMapperLinkInfoClass;

extern const class hkClass hkpPoweredChainMapperTargetClass;

class hkpConstraintInstance;
class hkpEntity;
class hkpConstraintMotor;
class hkQuaternion;
class hkpConstraintChainData;
class hkpPoweredChainData;
class hkpConstraintChainInstance;

extern const hkClass hkpPoweredChainMapperClass;

	/// This class allows you to manage several overlapping powered chains.
	/// It allows you to set max forces and target orientations for motors.
class hkpPoweredChainMapper : public hkReferencedObject
{
	public:
		HK_DECLARE_REFLECTION();
		HK_DECLARE_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT);

	private:
			// You should only create the mapper with the buildChainMapper() function.
		hkpPoweredChainMapper() {}

	public:

		virtual ~hkpPoweredChainMapper();


			/// Holds parameters for the mapper-building function.
		struct Config
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpPoweredChainMapper::Config );

				/// Tells whether limit constraints should be created in parallel with the chains.
				///
				/// Limit constraints use information from the existing powered/non-powered ragdoll/hinge constraints only.
				/// No limit constraints are created for any other types of constraints.
			hkBool m_createLimitConstraints;

				/// Should the chains clone motors or reuse the ones from the original constraints.
			hkBool m_cloneMotors;

			Config()
			{
				m_createLimitConstraints = false;
				m_cloneMotors = false;
			}
		};

		
			/// Used to specify the first and the last entity of a new chain.
		struct ChainEndpoints
		{
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_UTILITIES, hkpPoweredChainMapper::ChainEndpoints );

				/// First entity in the chain.
			hkpEntity* m_start;
				/// Last entity in the chain.
			hkpEntity* m_end;
		};

			/// Builds a mapper with chains specified by entity pairs in the ChainEndpoints array.
			/// Returns HK_NULL upon failure.
		static hkpPoweredChainMapper* HK_CALL buildChainMapper(const Config config, const hkArray<hkpConstraintInstance*>& allConstraints, const hkArray<ChainEndpoints>& pairs, hkArray<hkpConstraintInstance*>* unusedConstraints = HK_NULL);



			//
			// Setting link properties
			//


			/// Sets min/max force for one motor (specified by the coordinateIndex) in all chains overlapping at the specified link (linkIndex)
		void setForceLimits(int linkIndex, int coordinageIndex /*motorIndex*/, hkReal minForce, hkReal maxForce);

			/// This appends the motors related to a given link to the motorsOut array.
			/// Note: there may be only one motor for a given link/coordinate index pair in a chain; but there may be several chains overlapping at the specified link.
		void getMotors(int linkIndex, int coordinateIndex, hkArray<hkpConstraintMotor*>& motorsOut);

			/// Return the total number of links.
			/// Note: as links order is a one-to-one match with the constraintArray passed to the buildChainMapper function,
			/// some of the links might not lead to any target chains.
		inline int getNumLinks() { return m_links.getSize(); }

			/// Set motor for the given link/coordinate index
		void setMotors(int linkIndex, int coordinateIndex, hkpConstraintMotor* newMotor);

			/// Sets target orientations for all chains overlapping at the specified link.
		void setTargetOrientation(int linkIndex, const hkQuaternion& newTarget);

			//
			// Internal
			//


			/// Specifies a chain/index referenced by a link.
		struct Target
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, Target );
				/// Targeted powered chain
			hkpPoweredChainData* m_chain;
				/// Index of the targeted ConstraintInfo in the chain
			int m_infoIndex;
		};

			/// Holds information about a single link of the mapper.
			/// One link may point to none, one, or a few chains.
			/// Also a link may have a limit constraint associated with it.
		struct LinkInfo
		{
			HK_DECLARE_REFLECTION();
			HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, LinkInfo );
			
				/// This specifies the index of the first target for this link. All targets for a given link are grouped together.
				/// Those targets are set, when you call setMaxForce() or setTargetOrientation() for that link.
			int m_firstTargetIdx;
				/// Specifies the number of targets for this link.
			int m_numTargets;

				/// A limit constraint that functions in parallel with the chains.
				/// It is directly related to using chains, and should be added/removed to/from the world when chains are removed.
			hkpConstraintInstance* m_limitConstraint;

			LinkInfo() : m_firstTargetIdx(-1), m_numTargets(0), m_limitConstraint(HK_NULL) {}
			LinkInfo(const LinkInfo& info) { m_firstTargetIdx = info.m_firstTargetIdx; m_numTargets = info.m_numTargets; m_limitConstraint = info.m_limitConstraint; }
		};

			/// Matches 1-1 the hkpConstraintInstance array passed to buildChainMapper()
			/// Note that some m_links may have no targets -- when a certain constraint is not used by the mapper.
		hkArray<struct LinkInfo> m_links;
			
			/// Combined array of targets for all the links.
		hkArray<struct Target> m_targets;

			/// Just an extra array listing directly all the chains owned by the mapper.
		hkArray<class hkpConstraintChainInstance*> m_chains;

			/// Finish up ctor.
		hkpPoweredChainMapper( hkFinishLoadedObjectFlag f ) :
			hkReferencedObject(f), m_links(f), m_targets(f), m_chains(f) {}

};


#endif // HK_UTILITIES_POWERED_CHAIN_MAPPER_H

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
