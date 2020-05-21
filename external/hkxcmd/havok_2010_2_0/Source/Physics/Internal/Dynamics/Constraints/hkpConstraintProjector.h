/* 
 * 
 * Confidential Information of Telekinesys Research Limited (t/a Havok). Not for disclosure or distribution without Havok's
 * prior written consent. This software contains code, techniques and know-how which is confidential and proprietary to Havok.
 * Level 2 and Level 3 source code contains trade secrets of Havok. Havok Software (C) Copyright 1999-2010 Telekinesys Research Limited t/a Havok. All Rights Reserved. Use of this software is subject to the terms of an end user license agreement.
 * 
 */
#ifndef HK_CONSTRAINT_PROJECTOR_H
#define	HK_CONSTRAINT_PROJECTOR_H

/// This is an utility class that can be used to project constraints pivots and limits in order to correct large positional and angular errors.
class	hkpConstraintProjector
{
public:
	/// Projection presets
	enum	Preset
	{
		HK_DEFAULT,									///< Generic projection preset.
		HK_PROJECT_PIVOTS,							///< Very basic and fast pivots only projection.
		HK_HIERARCHY,								///< Specific preset for hierarchy or rag dolls.
		HK_REPAIR_VIOLATIONS,						///< Specific preset for repairing constraints.
	};
	/// Constraint sort type
	enum	Sort
	{
		HK_SORT_NONE,								///< Do not sort constraints.
		HK_SORT_PER_ERROR,							///< Sort constraints per error.
		HK_SORT_PER_MASS,							///< Sort constraints per mass.
		HK_SORT_DETERMINISTIC						///< Deterministic sorting.
	};
public:
	Sort					m_sortMode;				///< Sorting mode.
	int						m_numPositionIterations;///< Positions iterations.
	int						m_numVelocityIterations;///< Velocities iterations.
	hkReal					m_globalTau;			///< Global error reduction parameter.
	hkReal					m_finalTau;				///< Final projection error reduction parameter.
	hkReal					m_linearTau;			///< Linear error reduction parameter.
	hkReal					m_angularTau;			///< Angular error reduction parameter.
	hkReal					m_maxLinearError;		///< Maximum linear error (0=unlimited).
	hkReal					m_maxAngularError;		///< Maximum angular error (0=unlimited).
	hkReal					m_maxLinearVelocity;	///< maximum linear velocity (0=unlimited).
	hkReal					m_maxAngularVelocity;	///< maximum angular velocity (0=unlimited).
	hkReal					m_warmupT0;				///< Fraction of previous solution used to warm up the solver at Time 0.
	hkReal					m_warmupT1;				///< Fraction of previous solution used to warm up the solver at Time 1.
	hkReal					m_linearDamping;		///< Post linear velocity damping.
	hkReal					m_angularDamping;		///< Post angular velocity damping.
	hkBool					m_finalProject;			///< Final projection step, disregard masses.
	hkBool					m_projectOnly;			///< Apply only final projection step.
	hkBool					m_doLimits;				///< Project limits.
	hkBool					m_doAxis;				///< Project axis.
public:
		/// The constructor initialize projection parameters with the given preset
	hkpConstraintProjector(Preset preset=HK_DEFAULT) { setPreset(preset); }
public:
		/// Set the preset used for constraints projection
		/// Notes: Projection parameters can be manually tuned by directly settings their values.
	void setPreset(Preset preset);
		/// Project constraints by moving rigid bodies referenced by constraints.
	void project(const hkArray<hkpConstraintInstance*>& constraints,hkpRigidBody* root=HK_NULL,hkReal timestep=0) const;
};

#endif

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
