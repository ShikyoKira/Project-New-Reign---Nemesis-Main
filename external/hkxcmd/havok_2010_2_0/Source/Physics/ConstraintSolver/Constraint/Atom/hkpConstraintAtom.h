/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */

#ifndef HK_DYNAMICS2_CONSTRAINT_ATOM_H
#define HK_DYNAMICS2_CONSTRAINT_ATOM_H


#include <Common/Base/hkBase.h>
#include <Physics/ConstraintSolver/Constraint/Contact/hkpSimpleContactConstraintInfo.h>
#include <Common/Base/Types/Physics/ContactPoint/hkContactPoint.h>
#include <Physics/ConstraintSolver/Solve/hkpSolve.h>
#include <Physics/Dynamics/Constraint/hkpConstraintInfo.h>

extern const class hkClass hkpMovingSurfaceModifierConstraintAtomClass;

extern const class hkClass hkpViscousSurfaceModifierConstraintAtomClass;

extern const class hkClass hkpCenterOfMassChangerModifierConstraintAtomClass;

extern const class hkClass hkpMassChangerModifierConstraintAtomClass;

extern const class hkClass hkpSoftContactModifierConstraintAtomClass;

extern const class hkClass hkpModifierConstraintAtomClass;

extern const class hkClass hkpPulleyConstraintAtomClass;

extern const class hkClass hkpLinMotorConstraintAtomClass;

extern const class hkClass hkpLinFrictionConstraintAtomClass;

extern const class hkClass hkpRagdollMotorConstraintAtomClass;

extern const class hkClass hkpAngMotorConstraintAtomClass;

extern const class hkClass hkpAngFrictionConstraintAtomClass;

extern const class hkClass hkpConeLimitConstraintAtomClass;

extern const class hkClass hkpTwistLimitConstraintAtomClass;

extern const class hkClass hkpAngLimitConstraintAtomClass;

extern const class hkClass hkpAngConstraintAtomClass;

extern const class hkClass hkp2dAngConstraintAtomClass;

extern const class hkClass hkpLinLimitConstraintAtomClass;

extern const class hkClass hkpLinSoftConstraintAtomClass;

extern const class hkClass hkpLinConstraintAtomClass;

extern const class hkClass hkpOverwritePivotConstraintAtomClass;

extern const class hkClass hkpSetLocalRotationsConstraintAtomClass;

extern const class hkClass hkpSetLocalTranslationsConstraintAtomClass;

extern const class hkClass hkpSetLocalTransformsConstraintAtomClass;

extern const class hkClass hkpStiffSpringConstraintAtomClass;

extern const class hkClass hkpBallSocketConstraintAtomClass;

extern const class hkClass hkpSimpleContactConstraintAtomClass;

extern const class hkClass hkpBridgeAtomsClass;

extern const class hkClass hkpBridgeConstraintAtomClass;

extern const class hkClass hkpConstraintAtomClass;

extern const class hkClass hkpIgnoreModifierConstraintAtomClass;

extern const class hkClass hkpSetupStabilizationAtomClass;

HK_REFLECTION_CLASSFILE_DESTINATION("../../../Dynamics/Constraint/Atom");

class hkContactPoint;
class hkpConstraintMotor;
class hkpContactPointPropertiesStream;

#define HK_CONSTRAINT_FILL_PADDING_WITH_ZERO(fromAfter, to) { for (hkUint8* ptr = reinterpret_cast<hkUint8*>(fromAfter.next()); ptr < reinterpret_cast<hkUint8*>(&to); *(ptr++) = 0) { } }


/// Constraint atoms are building blocks that specify hkConstraintDatas.
///
/// hkConstraintDatas either use hkpBridgeAtoms, which allow them to implement their custom logic, or use a set of generic hkConstraintAtoms
/// to describe the geometry of the constraint.
///
/// Each hkpConstraintAtom is used to specify a constraint's orientation in space or to create one or more solver-constraints of a given type.
/// During simulation, hkConstraintAtoms are processed in the order in which they're organized in a hkpConstraintData.
///
/// Generally the first constraint in a list is one that specifies the local bases of the constraint in each of the constrained bodies' spaces.
/// Those bases are persistent throughout processing of a list of atoms. The following atoms apply a kind of a constraint (linear, angular,
/// limit, motor, etc.) in relation to one or more of the axes of the specified local bases. See individual descriptions of atoms for more info.
///
///
struct hkpConstraintAtom
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:

		// Atom enum values are saved in asset files. New atoms must be added from TYPE_OVERWRITE_PIVOT in order to
		// maintain backward compatibility
		enum AtomType
		{
			TYPE_INVALID = 0,

			TYPE_BRIDGE,					// 1

			TYPE_SET_LOCAL_TRANSFORMS,		// 2
			TYPE_SET_LOCAL_TRANSLATIONS,	// 3	
			TYPE_SET_LOCAL_ROTATIONS,		// 4

			TYPE_BALL_SOCKET,				// 5
			TYPE_STIFF_SPRING,				// 6

			TYPE_LIN,						// 7
			TYPE_LIN_SOFT,					// 8
			TYPE_LIN_LIMIT,					// 9
			TYPE_LIN_FRICTION,				// 10
			TYPE_LIN_MOTOR,					// 11

			TYPE_2D_ANG,					// 12

			TYPE_ANG,						// 13
			TYPE_ANG_LIMIT,					// 14
			TYPE_TWIST_LIMIT,				// 15
			TYPE_CONE_LIMIT,				// 16
			TYPE_ANG_FRICTION,				// 17
			TYPE_ANG_MOTOR,					// 18

			TYPE_RAGDOLL_MOTOR,				// 19

			TYPE_PULLEY,					// 20
			TYPE_RACK_AND_PINION,			// 21
			TYPE_COG_WHEEL,					// 22

			TYPE_SETUP_STABILIZATION,		// 23

			TYPE_OVERWRITE_PIVOT, //xx patch values from here [inclusive] on.
			

			TYPE_CONTACT,

				//
				// modifiers, must be the end of the list
				//
			TYPE_MODIFIER_SOFT_CONTACT,		// not supported by the spu
			TYPE_MODIFIER_MASS_CHANGER,		// spu supported
			TYPE_MODIFIER_VISCOUS_SURFACE,	// not supported by the spu
			TYPE_MODIFIER_MOVING_SURFACE,	// spu supported
			TYPE_MODIFIER_IGNORE_CONSTRAINT,  // spu supported
			TYPE_MODIFIER_CENTER_OF_MASS_CHANGER, // not supported by the spu

			TYPE_MAX
		};

		// flag indicating whether this constraint needs some special callback treatment
		// those flags can be combined
		enum CallbackRequest
		{
			CALLBACK_REQUEST_NONE = 0,
			CALLBACK_REQUEST_NEW_CONTACT_POINT = 1,
			CALLBACK_REQUEST_SETUP_PPU_ONLY = 2,
			CALLBACK_REQUEST_SETUP_CALLBACK = 4,
			CALLBACK_REQUEST_CONTACT_POINT_CALLBACK = 8
		};


			/// Possible types of solving methods used by the constraint atoms.
		enum SolvingMethod
		{
			/// Stabilized - constraint equations are typically solved in block form, which adds extra stiffness.
			/// The virtual mass matrix is usually re-computed per solver step, which increases the CPU cost.
			METHOD_STABILIZED = 0,

			/// Old - constraint equations are solved sequentially and the virtual mass matrix is assumed to be constant
			/// during the solver iterations.
			METHOD_OLD = 1,
		};

	public:
		HK_FORCE_INLINE enum AtomType getType() const { return m_type; }

		HK_FORCE_INLINE int isModifierType() const { return m_type >= TYPE_MODIFIER_SOFT_CONTACT; }

	protected:
		hkpConstraintAtom(enum AtomType type) : m_type(type) {}
	public:
		hkpConstraintAtom(hkFinishLoadedObjectFlag f) {}
	

	private:
			// Illegal constructor
		hkpConstraintAtom();

	public:
		hkEnum<AtomType,hkUint16> m_type;
};



/// Atom that computes common quantities for the stabilized versions of the atoms. Specifically, it computes the arms where the joint forces will be applied,
/// and the maximum impulse that can be applied by the solver in a step to maintain the constraints.
struct hkpSetupStabilizationAtom : public hkpConstraintAtom
{
	// +version(1)
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpSetupStabilizationAtom);
	HK_DECLARE_REFLECTION();

	hkpSetupStabilizationAtom()
	:	hkpConstraintAtom(TYPE_SETUP_STABILIZATION)
	,	m_enabled(false)
	,	m_maxAngle(HK_REAL_HIGH)
	{}

	hkpSetupStabilizationAtom(hkFinishLoadedObjectFlag f)
	:	hkpConstraintAtom(f)
	{}

	HK_FORCE_INLINE hkpConstraintAtom* next() const
	{
		return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this + 1) );
	}

	HK_FORCE_INLINE int numSolverResults() const
	{
		return 0;
	}

		/// Sets the maximum angle that can be swept by an object in a solver time-step. Used when estimating the maximum impulse
		/// the solver can apply during a step.
	HK_FORCE_INLINE void setMaxAngle(const hkReal maxAngle)
	{
		m_maxAngle = maxAngle;
	}

		/// Gets the maximum angle
	HK_FORCE_INLINE hkReal getMaxAngle() const
	{
		return m_maxAngle;
	}

		/// True if the atom is enabled
	hkBool m_enabled;	//+default(false)

		/// The maximum angle that can be swept by an object in a solver time-step. Used when estimating the maximum impulse
		/// the solver can apply during a step.
	hkReal m_maxAngle;	//+default(HK_REAL_HIGH)

		/// Padding to 16 bytes
	hkUint8	m_padding[8];
};

typedef void (HK_CALL *hkConstraintAtomBuildJacobianFunc) ( class hkpConstraintData* m_constraintData, const hkpConstraintQueryIn &in, hkpConstraintQueryOut &out );

	/// This atom is used to allow to call the old hkpConstraintData classes
struct hkpBridgeConstraintAtom: public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpBridgeConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpBridgeConstraintAtom(  ): hkpConstraintAtom( TYPE_BRIDGE ){ }

	hkpBridgeConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f)
	{
		if( f.m_finishing )
		{
			init(m_constraintData);
		}
	}

		// call this to do stuff not done in the constructor yet
	void init (class hkpConstraintData* m_constraintData);

	HK_FORCE_INLINE hkpConstraintAtom* next()		{ return (this+1); }

		// bridge atoms are always the last atom, so no need to increment solver result, just make sure the
		// program crashes if the result of this function is used
	HK_FORCE_INLINE int numSolverResults() const    { return 100000; }

	// addToConstraintInfo not needed

	hkConstraintAtomBuildJacobianFunc       m_buildJacobianFunc;	//+nosave +overridetype(void*)

	class hkpConstraintData* m_constraintData; //+owned(false)
};

struct hkpBridgeAtoms
{
	HK_DECLARE_REFLECTION();
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpBridgeAtoms );

	struct hkpBridgeConstraintAtom m_bridgeAtom;

	hkpBridgeAtoms(){}

	hkpBridgeAtoms(hkFinishLoadedObjectFlag f) : m_bridgeAtom(f) {}

	// get a pointer to the first atom
	const hkpConstraintAtom* getAtoms() const { return &m_bridgeAtom; }

	// get the size of all atoms (we can't use sizeof(*this) because of align16 padding)
	int getSizeOfAllAtoms() const               { return hkGetByteOffsetInt(this, &m_bridgeAtom+1); }
};


	/// hkpSimpleContactConstraintAtom holds contact information for a single hkpSimpleContactConstraintData.
	///
	/// It is for internal use only and is unique in the following ways:
	///     - it is not a member of the owning hkpConstraintData, it is allocated externally
	///     - its size is dynamic and varies depending on the number of contact points in the constraint
	///     - it is a stand-alone constraint, therefore it derives from hkpConstraintAtom and cannot be followed by any other atom
struct hkpSimpleContactConstraintAtom : public hkpConstraintAtom
{
		HK_DECLARE_REFLECTION();

		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpSimpleContactConstraintAtom );

	public:
		// Size of hkpSimpleContactConstraintAtom is dynamically changed by the engine. It holds up to 256 contact points.
		// We initialize the size of the atom to what it is when no contact points are present.
		hkpSimpleContactConstraintAtom() : hkpConstraintAtom(TYPE_CONTACT) {}

		hkpSimpleContactConstraintAtom(hkFinishLoadedObjectFlag f);

		HK_FORCE_INLINE hkContactPoint* getContactPoints() const { return const_cast<hkContactPoint*>( reinterpret_cast<const hkContactPoint*>( this+1 ) ); }
		HK_FORCE_INLINE int getContactPointPropertiesStriding() const;
		HK_FORCE_INLINE hkpContactPointPropertiesStream* getContactPointPropertiesStream() const { return const_cast<hkpContactPointPropertiesStream*>( reinterpret_cast<const hkpContactPointPropertiesStream*>( hkAddByteOffsetConst( getContactPoints(), sizeof(hkContactPoint) * m_numReservedContactPoints ) ) ); }
		HK_FORCE_INLINE hkpContactPointPropertiesStream* getContactPointPropertiesStream(int i) const;

	public:
		HK_FORCE_INLINE hkpConstraintAtom* next() const { HK_ASSERT2(0x5b5a6955, false, "Not implemented. Need to compute the entire size of contact points & properties."); return HK_NULL; }
		HK_FORCE_INLINE int numSolverResults() const    { return m_numContactPoints+3; }

		HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const
		{
			int size = m_numContactPoints;

			infoOut.m_sizeOfSchemas    += HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA 
										+ (size >> 1) * HK_SIZE_OF_JACOBIAN_PAIR_CONTACT_SCHEMA 
				                        + (size & 1) * HK_SIZE_OF_JACOBIAN_SINGLE_CONTACT_SCHEMA 
										+ HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA;

			infoOut.m_maxSizeOfSchema = infoOut.m_sizeOfSchemas + (HK_SIZE_OF_JACOBIAN_3D_FRICTION_SCHEMA - HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA);
			infoOut.m_numSolverResults   += size + 2;
			infoOut.m_numSolverElemTemps += size + (2 + 1); // extra one for friction

			if ( size >= 2 )
			{
				infoOut.m_sizeOfSchemas   += HK_SIZE_OF_JACOBIAN_3D_FRICTION_SCHEMA - HK_SIZE_OF_JACOBIAN_2D_FRICTION_SCHEMA;
				infoOut.m_numSolverResults   += 1; // is that needed ?? solver results are stroed in the info struct
				infoOut.m_numSolverElemTemps += 1 + 0; // just one elem for both 2d & 3d friction anyways.
			}

			if (m_info.m_rollingFrictionMultiplier != 0.0f)
			{
				infoOut.m_sizeOfSchemas += HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA;
				infoOut.m_maxSizeOfSchema += HK_SIZE_OF_JACOBIAN_2D_ROLLING_FRICTION_SCHEMA;
				infoOut.m_numSolverResults += 2; 
				infoOut.m_numSolverElemTemps += 2 + 1;
			}
		}

	public:
		hkUint16 m_sizeOfAllAtoms;
		hkUint16 m_numContactPoints;
		hkUint16 m_numReservedContactPoints;

			// 8 bytes left from here

			// Extra storage for bodyA. Holds hkpShapeKey hierarchy, by default.
		hkUint8 m_numUserDatasForBodyA;

			// Extra storage for bodyA. Holds hkpShapeKey hierarchy, by default.
		hkUint8 m_numUserDatasForBodyB;

			// store striding in here to avoid recomputing in getContactPointPropsStriding()..
		hkUint8 m_contactPointPropertiesStriding;

			// Maximum number for contact points. The limitation is driven by buffer size on SPU, and by contactPointProperties size.
		hkUint16 m_maxNumContactPoints; 


		HK_ALIGN16(class hkpSimpleContactConstraintDataInfo m_info);

};


// Only include it after the hkpSimpleContactConstraintAtom is defined
#include <Physics/ConstraintSolver/Constraint/Contact/hkpContactPointProperties.h>

HK_FORCE_INLINE int hkpSimpleContactConstraintAtom::getContactPointPropertiesStriding() const { return m_contactPointPropertiesStriding; }
HK_FORCE_INLINE hkpContactPointPropertiesStream* hkpSimpleContactConstraintAtom::getContactPointPropertiesStream(int i) const 
{
	const hkContactPoint* endContactPoint = hkAddByteOffsetConst( getContactPoints(), sizeof(hkContactPoint) * m_numReservedContactPoints);
	const hkpContactPointPropertiesStream* beginningOfProperties = reinterpret_cast<const hkpContactPointPropertiesStream*>( endContactPoint );
	return const_cast<hkpContactPointPropertiesStream*>( hkAddByteOffsetConst(  beginningOfProperties, HK_HINT_SIZE16(i) * HK_HINT_SIZE16( getContactPointPropertiesStriding())  ) );
}



#	define HK_GET_LOCAL_CONTACT_ATOM(ATOM) ATOM
#	define HK_CONTACT_ATOM_SET_PPU(ATOM)


	/// Fully eliminates relative linear movement of bodies' pivots.
	///
	/// This is the most common atom. It is advised to place it at the end of the list of atoms to minimize results error.
	/// This atom eliminates 3 degrees of freedom and returns 3 solver results. It has no parameters.
struct hkpBallSocketConstraintAtom : public hkpConstraintAtom
{
		//+version(3)
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpBallSocketConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:

		hkpBallSocketConstraintAtom()
		:	hkpConstraintAtom(TYPE_BALL_SOCKET)
		,	m_solvingMethod(METHOD_OLD)
		,	m_bodiesToNotify(0)
		,	m_velocityStabilizationFactor(1.0f)
		,	m_maxImpulse(HK_REAL_MAX)
		,	m_inertiaStabilizationFactor(0.0f)
		{}

		hkpBallSocketConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}

			/// Return the next atom after this.
		HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }

			/// This tells how many solver-constraints this atom generates and how may solver-results slots it requires.
		HK_FORCE_INLINE int numSolverResults() const    { return 3; }

			/// This tells how much memory the system will need to store solver schemas and Jacobian's for this atom.
		HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const
		{
			const int schemaSize = hkMath::max2(3 * HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA, HK_SIZE_OF_STABLE_BALL_SOCKET_SCHEMA);
			infoOut.add(schemaSize, 3, 3);
		}

			/// Returns the value of the velocity stabilization factor.
		HK_FORCE_INLINE hkUFloat8 getVelocityStabilizationFactor() const
		{
			return m_velocityStabilizationFactor;
		}

			/// Sets the value of the velocity stabilization factor.
		HK_FORCE_INLINE void setVelocityStabilizationFactor(const hkUFloat8 velocityStabilizationFactor)
		{
			m_velocityStabilizationFactor = velocityStabilizationFactor;
		}

			/// Returns the value of the inertia stabilization factor.
		HK_FORCE_INLINE hkReal getInertiaStabilizationFactor() const
		{
			return m_inertiaStabilizationFactor;
		}

			/// Sets the value of the inertia stabilization factor. The passed value will be clamped to [0, 1].
		HK_FORCE_INLINE void setInertiaStabilizationFactor(const hkReal inertiaStabilizationFactor)
		{
			m_inertiaStabilizationFactor = hkMath::clamp(inertiaStabilizationFactor, 0.0f, 1.0f);
		}

			/// The method used by the solver to maintain this constraint. Can be one of the values in the hkpConstraintAtom::SolvingMethod enum.
			/// Note that maxImpulse and stabilization factor are not handled by the STABLE method. Also, the solverStabilizationFactor is only used by the
			/// STABLE method in an attempt to stabilize constrained body pairs with large mass ratios / long arm lengths.
		hkEnum<SolvingMethod, hkUint8> m_solvingMethod;	 //+default(hkpConstraintAtom::METHOD_OLD)

			/// Marks the body to be notified when the m_maxImpulse is breached.
		hkUint8 m_bodiesToNotify; //+default(0)

	protected:

			/// Deprecated. This sets the multiplier that's used to add bodies' velocities to the solver's Jacobians RHS.
			/// This defaults to 1.0f and results in the constraints positional constraint being slightly violated for the advantage of
			/// extra stabilization. At low (low frame rate, few solver iterations) this setting can cause uncontrollable jitter however.
			/// In such cases disable it setting the factor to lower values or zero. Note that this parameter is only used by the OLD solving mode.
		hkUFloat8 m_velocityStabilizationFactor; //+default(1.0f)

	public:

			/// Maximum impulse applied by each of the three resulting 1d bilinear constraints.
			/// When any of the three limits is breached and the constraint has runtime allocated, a constraint-impulse-limit breached callback is triggered.
		hkReal m_maxImpulse;  //+default(HK_REAL_MAX)

	protected:

			/// A scaling factor that controls how much the solver is allowed to numerically
			/// adjust the angular terms of the inverse virtual mass matrix while in the STABILIZED solving mode. Must be inside [0, 1]. A value of 0 means
			/// no adjustments are made to the mass distributions. A value closer to 1 stabilizes bodies having large mass ratios or long / disproportionate arm lengths.
			/// Note that this can generally be set to zero (i.e., disabled) if the inertia tensors have been already stabilized in the Create Constraints pipeline
			/// filter of by manually calling hkpConstraintStabilizationUtil::stabilizeRigidBodyInertia().
			/// Default value is zero.
		hkReal m_inertiaStabilizationFactor;	//+default(0.0f)
};

	/// Enforces a constant distance between the pivot points of linked bodies.
struct hkpStiffSpringConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpStiffSpringConstraintAtom );
	HK_DECLARE_REFLECTION();

public:
	hkpStiffSpringConstraintAtom() : hkpConstraintAtom(TYPE_STIFF_SPRING) {}
	hkpStiffSpringConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}

	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA, 1, 1 ); }

		/// The rest length / distance between pivot points.
	hkReal m_length;
};

	/// This specifies constraint spaces and pivot points in the local spaces of each body.
	///
	/// Pivot points are stored in the translation part of the transforms.
struct hkpSetLocalTransformsConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpSetLocalTransformsConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpSetLocalTransformsConstraintAtom()
	:	hkpConstraintAtom(TYPE_SET_LOCAL_TRANSFORMS)
	{}

	hkpSetLocalTransformsConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 0; }
	// addToConstraintInfo not needed

		/// Constraint orientation and origin/pivot point in bodyA's local space.
	hkTransform m_transformA;
		/// Constraint orientation and origin/pivot point in bodyB's local space.
	hkTransform m_transformB;
};

	/// This specifies pivot points in the local spaces of each body.
	///
	/// Note that this does not overwrite the constraint space's orientation.
	/// This is used when constraint orientation is irrelevant, e.g., in hkpBallAndSocketConstraintData.
struct hkpSetLocalTranslationsConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpSetLocalTranslationsConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpSetLocalTranslationsConstraintAtom() : hkpConstraintAtom(TYPE_SET_LOCAL_TRANSLATIONS) {}
	hkpSetLocalTranslationsConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 0; }
	// addToConstraintInfo not needed

		/// Pivot point in bodyA's local space.
	hkVector4 m_translationA;
		/// Pivot point in bodyB's local space.
	hkVector4 m_translationB;
};

	/// This specifies constraint spaces in the local spaces of each body.
	///
	/// Note that this does not overwrite the pivot points.
	/// This is used when the constraint space must be reoriented for some atoms in more complex hkConstraintDatas, e.g., in the hkpWheelConstraintData.
struct hkpSetLocalRotationsConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpSetLocalRotationsConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpSetLocalRotationsConstraintAtom() : hkpConstraintAtom(TYPE_SET_LOCAL_ROTATIONS) {}
	hkpSetLocalRotationsConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 0; }
	// addToConstraintInfo not needed

		/// Constraint orientation in bodyA's local space.
	hkRotation m_rotationA;
		/// Constraint orientation in bodyB's local space.
	hkRotation m_rotationB;
};

struct hkpOverwritePivotConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpOverwritePivotConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpOverwritePivotConstraintAtom() : hkpConstraintAtom(TYPE_OVERWRITE_PIVOT), m_copyToPivotBFromPivotA(true) { }
	hkpOverwritePivotConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 0; }

	hkUint8 m_copyToPivotBFromPivotA;
};

	/// Eliminates relative linear velocity of bodies' pivot points along one specified axis.
	///
	/// This is used when relative linear movement is only partly constrained as it is in e.g., prismatic or point-to-plane constraints.
struct hkpLinConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpLinConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpLinConstraintAtom() : hkpConstraintAtom(TYPE_LIN) {}
	hkpLinConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_BILATERAL_SCHEMA, 1, 1 ); }

		/// Specifies the index of the axis of the bodyB's constraint base, that will be constrained.
	hkUint8 m_axisIndex;
};

	/// Softens/controls relative linear velocity of bodies' pivot points along one specified axis.
	///
	/// This results in a spring-like reaction, it's used in the hkpWheelConstraintData.
struct hkpLinSoftConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpLinSoftConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpLinSoftConstraintAtom() : hkpConstraintAtom(TYPE_LIN_SOFT) {}
	hkpLinSoftConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_BILATERAL_USER_TAU_SCHEMA, 1, 1 ); }

		/// Specifies the index of the axis of the bodyB's constraint base, that will be constrained.
	hkUint8 m_axisIndex;
		/// Specifies a custom value for the tau parameter used by the solver.
	hkReal m_tau;
		/// Specifies a custom value for the damping parameter used by the solver.
	hkReal m_damping;
};

	/// Limits allowed relative distance between bodies' pivot points along one specified axis.
	///
	/// This allows unconstrained movement within the specified range, and applies hard limits at its ends.
struct hkpLinLimitConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpLinLimitConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpLinLimitConstraintAtom() : hkpConstraintAtom(TYPE_LIN_LIMIT) {}
	hkpLinLimitConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_LINEAR_LIMIT_SCHEMA, 1, 1 ); }

		/// The index of the axis of the bodyB's constraint base, that will be limited.
	hkUint8 m_axisIndex;
		/// Minimum distance along the axis (may be negative).
	hkReal m_min;
		/// Maximum distance along the axis (may be negative).
	hkReal m_max;
};

	/// Eliminates two degrees of freedom of angular movement and allows relative rotation along a specified axis only.
	///
	/// Angular-constraint atoms are often combined with linear-constraint atoms, e.g., this atoms combined with the ball-and-socket
	/// atom forms a hkpHingeConstraintData.
struct hkp2dAngConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkp2dAngConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkp2dAngConstraintAtom() : hkpConstraintAtom(TYPE_2D_ANG) {}
	hkp2dAngConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 2; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( 2 * HK_SIZE_OF_JACOBIAN_1D_ANGULAR_SCHEMA, 2, 2 ); }

		/// Specifies the index of the unconstrained axis of relative rotation in bodyB's constraint base.
	hkUint8 m_freeRotationAxis;
};


	/// Eliminates one, two, or three degrees of freedom of angular movement.
	///
	/// Note: this is only tested for eliminating three degrees of freedom.
struct hkpAngConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpAngConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpAngConstraintAtom() : hkpConstraintAtom(TYPE_ANG) {}
	hkpAngConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return m_numConstrainedAxes; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( m_numConstrainedAxes * HK_SIZE_OF_JACOBIAN_1D_ANGULAR_SCHEMA, m_numConstrainedAxes, m_numConstrainedAxes ); }

		/// Index of the first axis to constrain, in bodyA's constraint base.
	hkUint8 m_firstConstrainedAxis;

		/// Number of subsequent base axes to constrain.
	hkUint8 m_numConstrainedAxes;
};


	/// Limits allowed relative angle between bodies' rotations along one specified rotation axis.
	///
	/// This allows unconstrained movement within the specified range, and applies hard limits at its ends.
struct hkpAngLimitConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpAngLimitConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpAngLimitConstraintAtom() : hkpConstraintAtom(TYPE_ANG_LIMIT), m_isEnabled(true) {}
	hkpAngLimitConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_ANGULAR_LIMITS_SCHEMA, 1, 1 ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkUint8 m_isEnabled;

		/// The index of the axis in the bodyA's constraint base, that will be limited.
	hkUint8 m_limitAxis;

		/// Minimum angle value in radians (may be negative).
	hkReal m_minAngle;

		/// Maximum angle value in radians (may be negative).
	hkReal m_maxAngle;

		/// A stiffness factor [0..1] used by the solver; defaults to 1.0.
	hkReal m_angularLimitsTauFactor; //+default(1.0) +absmin(0) +absmax(1)
};


	/// Limits allowed relative angle between bodies' rotations along one specified rotation axis.
	///
	/// This constraint allows unconstrained movement within the specified range, and applies hard limits at its ends.
struct hkpTwistLimitConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpTwistLimitConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpTwistLimitConstraintAtom() : hkpConstraintAtom(TYPE_TWIST_LIMIT), m_isEnabled(true) {}
	hkpTwistLimitConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_ANGULAR_LIMITS_SCHEMA, 1, 1 ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkUint8 m_isEnabled;

		/// The index of the axis in the bodyA's constraint base, that will be limited.
	hkUint8 m_twistAxis;

		/// The index of a perpendicular axis used as a reference to measure the angle.
	hkUint8 m_refAxis;

		/// Minimum angle value in radians (may be negative).
	hkReal m_minAngle;

		/// Maximum angle value in radians (may be negative).
	hkReal m_maxAngle;

		/// A stiffness factor [0..1] used by the solver; defaults to 1.0.
	hkReal m_angularLimitsTauFactor; //+default(1.0) +absmin(0) +absmax(1)
};

	/// Limits allowed relative angle between bodies' rotations as measured between two chosen axes.
	///
	/// This allows unconstrained movement within the specified range, and applies hard limits at its ends.
struct hkpConeLimitConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpConeLimitConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpConeLimitConstraintAtom() : hkpConstraintAtom(TYPE_CONE_LIMIT), m_isEnabled(true), m_memOffsetToAngleOffset(0) {}
	hkpConeLimitConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_ANGULAR_LIMITS_SCHEMA, 1, 1 ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkUint8 m_isEnabled;

		/// The index of the axis in the bodyA's constraint base, that will be used as a reference vector and constrained to lie within the limit cone.
	hkUint8 m_twistAxisInA;

		/// The index of the axis in the bodyB's constraint base, that will be used as a reference and limit-cone axis.
	hkUint8 m_refAxisInB;

		/// Specifies how the angle between the two reference vectors is measured.
	enum MeasurementMode
	{
		// Do not change enumeration values! They're used in calculations.

			/// Zero-angle corresponds to situation where the two vectors are aligned.
		ZERO_WHEN_VECTORS_ALIGNED = 0,
			/// Zero-angle corresponds to situation where the two vectors are perpendicular, and (+)90-degree corresponds to vectors being aligned.
		ZERO_WHEN_VECTORS_PERPENDICULAR = 1
	};

		/// Specifies how the angle between the two reference vectors is measured.
	hkEnum<MeasurementMode, hkUint8> m_angleMeasurementMode;

		/// Memory offset to location in runtime where m_coneAngleOffset is stored.
		/// That is used to stabilize the constraint.
		/// Zero offset means, that no extra stabilization is done.
	hkUint8 m_memOffsetToAngleOffset; //+default(1)

		/// Minimum angle value in radians (may be negative).
	hkReal m_minAngle;

		/// Maximum angle value in radians (may be negative).
	hkReal m_maxAngle;

		/// A stiffness factor [0..1] used by the solver; defaults to 1.0.
	hkReal m_angularLimitsTauFactor; //+default(1.0) +absmin(0) +absmax(1)
};

	/// Applies friction torque along one, two, or three specified rotation axes.
struct hkpAngFrictionConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpAngFrictionConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpAngFrictionConstraintAtom() : hkpConstraintAtom(TYPE_ANG_FRICTION), m_isEnabled(true), m_numFrictionAxes(1) {}
	hkpAngFrictionConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return m_numFrictionAxes; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( m_numFrictionAxes * HK_SIZE_OF_JACOBIAN_1D_ANGULAR_FRICTION_SCHEMA, m_numFrictionAxes, (1+1) * m_numFrictionAxes ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkUint8 m_isEnabled;

		/// Index of the first axis to apply friction along, in bodyA's constraint base.
	hkUint8 m_firstFrictionAxis;

		/// Number of subsequent base axes to constrain.
	hkUint8 m_numFrictionAxes;

		/// Maximum allowed torque to be applied due to friction.
	hkReal m_maxFrictionTorque;
};

	/// Controls relative rotation angle between bodies around a specified rotation axes.
	///
	/// Note that motor atoms require access to external variables stored in hkpConstraintInstance's runtime.
	/// The atom accesses those variables using memory offsets (stored in the atom's members).
	/// Also when the motor is to operate in a range exceeding the [-Pi, Pi] range it must have a reference
	/// onto solver results of a corresponding hkpAngLimitConstraintAtom to retrieve the proper angle value.
struct hkpAngMotorConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpAngMotorConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpAngMotorConstraintAtom() : hkpConstraintAtom(TYPE_ANG_MOTOR) { m_isEnabled = true; m_initializedOffset = 0; m_previousTargetAngleOffset = 0; }
	hkpAngMotorConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_ANGULAR_MOTOR_SCHEMA, 1, (1+2) ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkBool m_isEnabled;

		/// The index of the axis in the bodyA's constraint base, that will be controlled.
	hkUint8 m_motorAxis;

		/// Memory offset from atom's solver results to runtime's m_initialized member.
	hkInt16 m_initializedOffset;

		/// Memory offset from atom's solver results to runtime's m_previousTargetAngle member
	hkInt16 m_previousTargetAngleOffset;

		/// This is an optional offset to solver results of an angular limit atom.
		/// The results store the actual angle from the last frame, and are needed if the motor
		/// is to allow 'screw' functionality (i.e., orientation is not represented by a cyclic
		/// [-180deg, 180deg] range, but as an unlimited number of degrees/rotations).
	hkInt16 m_correspondingAngLimitSolverResultOffset;

		/// The target angle for the motor.
	hkReal m_targetAngle;

		/// Motor; note that it is reference counted and should be handled by the owning constraint's get/set methods.
	HK_CPU_PTR(class hkpConstraintMotor*) m_motor;
};

	/// Controls relative rotation angle between bodies in three dimensions; used by the hkpRagdollConstraintData.
	///
	/// Note that motor atoms require access to external variables stored in hkpConstraintInstance's runtime.
	/// The atom accesses those variables using memory offsets (stored in the atom's members).
struct hkpRagdollMotorConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpRagdollMotorConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpRagdollMotorConstraintAtom() : hkpConstraintAtom(TYPE_RAGDOLL_MOTOR) { m_isEnabled = true; m_initializedOffset = 0; m_previousTargetAnglesOffset = 0; }
	hkpRagdollMotorConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 3; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( 3 * HK_SIZE_OF_JACOBIAN_1D_ANGULAR_MOTOR_SCHEMA, 3, (1+2) * 3 ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkBool m_isEnabled;

		/// Memory offset from atom's solver results to runtime's m_initialized member.
	hkInt16 m_initializedOffset;

		/// Memory offset from atom's solver results to runtime's m_previousTargetAngle member.
	hkInt16 m_previousTargetAnglesOffset;

		/// The target relative rotation the motors will try to match.
		/// This is the target rotation from bodyA's constraint space into the bodyB's (non-constraint) space.
	hkMatrix3 m_target_bRca;

		/// Three motors; note that they are reference counted and should be handled by the owning constraint's get/set methods.
	HK_CPU_PTR(class hkpConstraintMotor*) m_motors[3];
};

	/// Applies friction force along a specified axes.
struct hkpLinFrictionConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpLinFrictionConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpLinFrictionConstraintAtom() : hkpConstraintAtom(TYPE_LIN_FRICTION), m_isEnabled(true) {}
	hkpLinFrictionConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_FRICTION_SCHEMA, 1, (1+1) ); }

		/// Tells whether the atom should be handled by the solver.
		///
		/// Note that if it is not, the atom's corresponding hkpSolverResults are not updated.
	hkUint8 m_isEnabled;

		/// Index of the axis to apply friction along, in bodyB's constraint base.
	hkUint8 m_frictionAxis;

		/// Maximum allowed force to be applied due to friction.
	hkReal m_maxFrictionForce;
};

	/// Controls relative velocity of bodies along a specified axis.
	///
	/// Note that motor atoms require access to external variables stored in hkpConstraintInstance's runtime.
	/// The atom accesses those variables using memory offsets (stored in the atom's members).
struct hkpLinMotorConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpLinMotorConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpLinMotorConstraintAtom() : hkpConstraintAtom(TYPE_LIN_MOTOR) { m_isEnabled = true; m_initializedOffset = 0; m_previousTargetPositionOffset = 0; }
	hkpLinMotorConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_LINEAR_MOTOR_SCHEMA, 1, (1+2) ); }

		/// A flag saying whether the motor is active
	hkBool  m_isEnabled;

		/// The index of the axis in the bodyB's constraint base, that will be controlled.
	hkUint8 m_motorAxis;

		/// Memory offset from atom's solver results to runtime's m_initialized member.
	hkInt16 m_initializedOffset;

		/// Memory offset from atom's solver results to runtime's m_previousTargetPosition member.
	hkInt16 m_previousTargetPositionOffset;

		/// The target position for the motor.
	hkReal m_targetPosition;

		/// Motor; note that it is reference counted and should be handled by the owning constraint's get/set methods.
	HK_CPU_PTR(class hkpConstraintMotor*) m_motor;
};

	/// This implements a functionality of a pulley, where bodies are attached to a rope, and the rope is lead through two pulley wheels at fixed world positions.
struct hkpPulleyConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpPulleyConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpPulleyConstraintAtom() : hkpConstraintAtom(TYPE_PULLEY) { }
	hkpPulleyConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const { return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    { return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const { infoOut.add( HK_SIZE_OF_JACOBIAN_1D_PULLEY_SCHEMA, 1, 1 ); }


		/// Pulley's first fixed pivot point.
	hkVector4 m_fixedPivotAinWorld;
		/// Pulley's second fixed pivot point.
	hkVector4 m_fixedPivotBinWorld;

		/// The rest length (equal to ((BodyA's rope) + leverageOnBodyB * (BodyB's rope length)) )
	hkReal m_ropeLength;
		/// Leverage ratio: e.g., value of 2 means that bodyA's rope length changes by twice as much as bodyB's,
		/// and the constraint exerts twice as big forces upon bodyB.
	hkReal m_leverageOnBodyB;
};

	/// This implements functionality of a rack-and-pinion or also a screw.
struct hkpRackAndPinionConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpRackAndPinionConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpRackAndPinionConstraintAtom() : hkpConstraintAtom(TYPE_RACK_AND_PINION) { }
	hkpRackAndPinionConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const 
							{ return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    
							{ return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const 
							{ infoOut.add( HK_SIZE_OF_JACOBIAN_1D_BILATERAL_USER_TAU_SCHEMA, 1, 1 ); }

		/// Pinion radius. In the case of a screw constraint, this has represents the screw's pitch (thread width),
		/// which is the distance along the screw's axis covered by one full rotation.
		///
		/// To reverse the direction of linear movement (and, at the same time, flip the generated point where force
		/// is applied) you can reverse the shift axis or use negative pinionRadius/screwPitch.
	hkReal m_pinionRadiusOrScrewPitch;
		/// This marks the atom as operating in the screw mode.
	hkBool m_isScrew;
		/// Memory offset from atom's solver results to runtime's m_initialAngleOffset member.
		/// The member is set to non-zero value to mark runtime as initialized.
	hkInt8 m_memOffsetToInitialAngleOffset;
		/// Memory offset from atom's solver results to runtime's m_prevAngle member.
	hkInt8 m_memOffsetToPrevAngle;
		/// Memory offset from atom's solver results to runtime's m_revolutionCounter member.
	hkInt8 m_memOffsetToRevolutionCounter;
};

	/// This implements functionality of cog wheels.
struct hkpCogWheelConstraintAtom : public hkpConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR( HK_MEMORY_CLASS_CONSTRAINT, hkpCogWheelConstraintAtom );
	HK_DECLARE_REFLECTION();

	hkpCogWheelConstraintAtom() : hkpConstraintAtom(TYPE_COG_WHEEL) { }
	hkpCogWheelConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}
	HK_FORCE_INLINE hkpConstraintAtom* next() const 
							{ return const_cast<hkpConstraintAtom*>( static_cast<const hkpConstraintAtom*>(this+1) ); }
	HK_FORCE_INLINE int numSolverResults() const    
							{ return 1; }
	HK_FORCE_INLINE void addToConstraintInfo(hkpConstraintInfo& infoOut) const 
							{ infoOut.add( HK_SIZE_OF_JACOBIAN_1D_BILATERAL_USER_TAU_SCHEMA, 1, 1 ); }

		/// Radius of the cog wheel A.
	hkReal m_cogWheelRadiusA;
		/// Radius of the cog wheel B.
	hkReal m_cogWheelRadiusB;
		/// This is unused.
	hkBool m_isScrew; // unused
		/// Memory offset from atom's solver results to runtime's m_initialAngleOffset[2] member.
		/// The first element is set to non-zero value to mark runtime as initialized.
	hkInt8 m_memOffsetToInitialAngleOffset;
		/// Memory offset from atom's solver results to runtime's m_prevAngle[2] member.
	hkInt8 m_memOffsetToPrevAngle;
		/// Memory offset from atom's solver results to runtime's m_revolutionCounter[2] member.
	hkInt8 m_memOffsetToRevolutionCounter;
};







	//
	//	Atom Modifiers
	//
struct hkpModifierConstraintAtom : public hkpConstraintAtom
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpModifierConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:
			// adds the constraintInfo of one modifier to cinfo and returns the hkpConstraintAtom::CallbackRequest
		int addModifierDataToConstraintInfo( hkpConstraintInfo& cinfo, hkUint8& usedFlagsOut ) const;

			// adds its constraintInfo of all linked modifiers to cinfo and returns the hkpConstraintAtom::CallbackRequest
		static int HK_CALL addAllModifierDataToConstraintInfo( hkpModifierConstraintAtom* firstModifier, hkpConstraintInfo& cinfo, hkUint8& usedFlagsOut );

	protected:

		hkpModifierConstraintAtom(enum AtomType type, int size) : hkpConstraintAtom(type), m_modifierAtomSize( hkUint16(size)) {}

	public:

		hkpModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpConstraintAtom(f) {}


	public:

		HK_ALIGN16( hkUint16		  m_modifierAtomSize );
		hkUint16                      m_childSize;
		HK_CPU_PTR(hkpConstraintAtom*) m_child;
		hkUint32 m_pad[2];
};

	//	************************ Soft Contact **************************
	//	************************ Soft Contact **************************
	//	************************ Soft Contact **************************

struct hkpSoftContactModifierConstraintAtom : public hkpModifierConstraintAtom
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpSoftContactModifierConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:

		hkpSoftContactModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_SOFT_CONTACT, sizeof(hkpSoftContactModifierConstraintAtom)),  m_tau(0.1f), m_maxAcceleration( 20.0f) { }
		hkpSoftContactModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

		void collisionResponseBeginCallback( const hkContactPoint& cp, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);
		void collisionResponseEndCallback(   const hkContactPoint& cp, hkReal impulseApplied, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);
		HK_FORCE_INLINE int numSolverResults() const { return 0; }

		static const hkUint16 ADDITIONAL_SCHEMA_SIZE = 0;

		int getConstraintInfo( hkpConstraintInfo& info ) const	{ return hkpConstraintAtom::CALLBACK_REQUEST_NONE; }

	public:

		hkReal m_tau;

			/// The maximum acceleration, the solver will apply
		hkReal m_maxAcceleration;
};

	//	************************ Mass Changer **************************
	//	************************ Mass Changer **************************
	//	************************ Mass Changer **************************

struct hkpMassChangerModifierConstraintAtom : public hkpModifierConstraintAtom
{
	//+version(1)
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpMassChangerModifierConstraintAtom );

	public:

		hkpMassChangerModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_MASS_CHANGER, sizeof(hkpMassChangerModifierConstraintAtom)) {}
		hkpMassChangerModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

		HK_FORCE_INLINE int numSolverResults() const { return 0; }

		void collisionResponseBeginCallback( const hkContactPoint& cp, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);
		void collisionResponseEndCallback(   const hkContactPoint& cp, hkReal impulseApplied, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);

		static const hkUint16 ADDITIONAL_SCHEMA_SIZE = 2 * HK_SIZE_OF_JACOBIAN_SET_MASS_SCHEMA  + HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA;
		
		int getConstraintInfo( hkpConstraintInfo& info ) const
		{
			info.m_sizeOfSchemas += ADDITIONAL_SCHEMA_SIZE;
			return hkpConstraintAtom::CALLBACK_REQUEST_NONE;
		}

	public:

		hkVector4 m_factorA;
		hkVector4 m_factorB;
};


//	************************ Center of Mass Modifier **************************
//	************************ Center of Mass Modifier **************************
//	************************ Center of Mass Modifier **************************

struct hkpCenterOfMassChangerModifierConstraintAtom : public hkpModifierConstraintAtom
{
		HK_DECLARE_REFLECTION();
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpMassChangerModifierConstraintAtom );

	public:

		hkpCenterOfMassChangerModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_CENTER_OF_MASS_CHANGER, sizeof(hkpCenterOfMassChangerModifierConstraintAtom)) {}
		hkpCenterOfMassChangerModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

		HK_FORCE_INLINE int numSolverResults() const { return 0; }

		void collisionResponseBeginCallback( const hkContactPoint& cp, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);
		void collisionResponseEndCallback(   const hkContactPoint& cp, hkReal impulseApplied, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);

		static const hkUint16 ADDITIONAL_SCHEMA_SIZE = 2 * HK_SIZE_OF_JACOBIAN_SET_CENTER_OF_MASS_SCHEMA  + HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA;

		int getConstraintInfo( hkpConstraintInfo& info ) const
		{
			info.m_sizeOfSchemas += ADDITIONAL_SCHEMA_SIZE;
			return hkpConstraintAtom::CALLBACK_REQUEST_NONE;;
		}

	public:

			// Two displacements in local space of each of the bodies
		hkVector4 m_displacementA;
		hkVector4 m_displacementB;
};




	//	************************ Viscous Surface **************************
	//	************************ Viscous Surface **************************
	//	************************ Viscous Surface **************************

struct hkpViscousSurfaceModifierConstraintAtom : public hkpModifierConstraintAtom
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpViscousSurfaceModifierConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:

		hkpViscousSurfaceModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_VISCOUS_SURFACE, sizeof(hkpViscousSurfaceModifierConstraintAtom)) { }

		hkpViscousSurfaceModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

		static const hkUint16 ADDITIONAL_SCHEMA_SIZE = 0;

		int getConstraintInfo( hkpConstraintInfo& info ) const	{ return hkpConstraintAtom::CALLBACK_REQUEST_NONE; }

		HK_FORCE_INLINE int numSolverResults() const { return 0; }
};




	//	************************ Moving Surface **************************
	//	************************ Moving Surface **************************
	//	************************ Moving Surface **************************

struct hkpMovingSurfaceModifierConstraintAtom : public hkpModifierConstraintAtom
{
		HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpMovingSurfaceModifierConstraintAtom );
		HK_DECLARE_REFLECTION();

	public:

		hkpMovingSurfaceModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_MOVING_SURFACE, sizeof(hkpMovingSurfaceModifierConstraintAtom)) { }
		hkpMovingSurfaceModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

		HK_FORCE_INLINE int numSolverResults() const { return 0; }

		static const hkUint16 ADDITIONAL_SCHEMA_SIZE = 2 * HK_SIZE_OF_JACOBIAN_ADD_VELOCITY_SCHEMA + HK_SIZE_OF_JACOBIAN_HEADER_SCHEMA;

		int getConstraintInfo( hkpConstraintInfo& info ) const
		{
			info.m_sizeOfSchemas += ADDITIONAL_SCHEMA_SIZE;
			return hkpConstraintAtom::CALLBACK_REQUEST_NONE;
		}

		void collisionResponseBeginCallback( const hkContactPoint& cp, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);
		void collisionResponseEndCallback(   const hkContactPoint& cp, hkReal impulseApplied, struct hkpSimpleConstraintInfoInitInput& inA, struct hkpBodyVelocity& velA, hkpSimpleConstraintInfoInitInput& inB, hkpBodyVelocity& velB);

		hkVector4& getVelocity() { return m_velocity; }
		const hkVector4& getVelocity() const { return m_velocity; }

	public:

 		hkVector4 m_velocity;
};


	//	************************ Ignore Constraint **************************
	//	************************ Ignore Constraint **************************
	//	************************ Ignore Constraint **************************

struct hkpIgnoreModifierConstraintAtom : public hkpModifierConstraintAtom
{
	HK_DECLARE_NONVIRTUAL_CLASS_ALLOCATOR(HK_MEMORY_CLASS_CONSTRAINT, hkpIgnoreModifierConstraintAtom );
	HK_DECLARE_REFLECTION();

public:

	hkpIgnoreModifierConstraintAtom() : hkpModifierConstraintAtom(TYPE_MODIFIER_IGNORE_CONSTRAINT, sizeof(hkpIgnoreModifierConstraintAtom)) { }
	hkpIgnoreModifierConstraintAtom(hkFinishLoadedObjectFlag f) : hkpModifierConstraintAtom(f) {}

	HK_FORCE_INLINE int numSolverResults() const { return 0; }

	int getConstraintInfo( hkpConstraintInfo& info ) const;
};





#endif // HK_DYNAMICS2_CONSTRAINT_ATOM_H

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
