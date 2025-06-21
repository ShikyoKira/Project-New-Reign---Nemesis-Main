#include "Havok/hkpWorldCinfo.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWorldCinfo,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWorldCinfo::Class; });

nemesis::hkpWorldCinfo::hkpWorldCinfo() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpWorldCinfo::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWorldCinfo::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.Pad(16);                                   // offset: 8/16 size: 8/0

    serializer.WriteValue("gravity", m_gravity); // offset: 16/16 size: 16/16 align: 16/16
    serializer.WriteValue("broadPhaseQuerySize", m_broadPhaseQuerySize); // offset: 32/32 size: 4/4 align: 4/4
    serializer.WriteValue("contactRestingVelocity",
                          m_contactRestingVelocity); // offset: 36/36 size: 4/4 align: 4/4
    serializer.WriteValue("broadPhaseBorderBehaviour",
                          m_broadPhaseBorderBehaviour); // offset: 40/40 size: 1/1 align: 1/1
    serializer.WriteValue("mtPostponeAndSortBroadPhaseBorderCallbacks",
                          m_mtPostponeAndSortBroadPhaseBorderCallbacks); // offset: 41/41 size: 1/1 align: 1/1
    serializer.Skip(6);                                                  // offset: 42/42 size: 6/6

    serializer.WriteObject("broadPhaseWorldAabb",
                           m_broadPhaseWorldAabb); // offset: 48/48 size: 32/32 align: 16/16

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        if (cur_ver <= HavokVersion::HK_2010_2_0)
        {
            serializer.WriteValue("useKdTree", m_useKdTree);             // offset: 80/80 size: 1/1 align: 1/1
            serializer.WriteValue("useMultipleTree", m_useMultipleTree); // offset: 81/81 size: 1/1 align: 1/1
            serializer.WriteValue("treeUpdateType", m_treeUpdateType);   // offset: 82/82 size: 1/1 align: 1/1
        }
    }
    else
    {
        serializer.WriteValue("useKdTree", m_useKdTree);
    }

    if (cur_ver <= HavokVersion::HK_2011_2_0)
    {
        serializer.WriteValue("autoUpdateKdTree",
                              m_autoUpdateKdTree); // offset: 83/83 size: 1/1 align: 1/1
    }

    serializer.Pad(4);

    serializer.WriteValue("collisionTolerance", m_collisionTolerance); // offset: 84/84 size: 4/4 align: 4/4
    serializer.WriteObject("collisionFilter", m_collisionFilter);      // offset: 88/88 size: 4/8 align: 4/8
    serializer.WriteObject("convexListFilter", m_convexListFilter);    // offset: 92/96 size: 4/8 align: 4/8
    serializer.WriteValue("expectedMaxLinearVelocity",
                          m_expectedMaxLinearVelocity); // offset: 96/104 size: 4/4 align: 4/4
    serializer.WriteValue("sizeOfToiEventQueue",
                          m_sizeOfToiEventQueue); // offset: 100/108 size: 4/4 align: 4/4
    serializer.WriteValue("expectedMinPsiDeltaTime",
                          m_expectedMinPsiDeltaTime); // offset: 104/112 size: 4/4 align: 4/4
    serializer.Pad(serializer.GetPointerSize());      // offset: 108/116 size: 0/4

    serializer.WriteObject("memoryWatchDog", m_memoryWatchDog); // offset: 108/120 size: 4/8 align: 4/8
    serializer.WriteValue("broadPhaseNumMarkers",
                          m_broadPhaseNumMarkers); // offset: 112/128 size: 4/4 align: 4/4
    serializer.WriteValue("contactPointGeneration",
                          m_contactPointGeneration); // offset: 116/132 size: 1/1 align: 1/1
    serializer.WriteValue("allowToSkipConfirmedCallbacks",
                          m_allowToSkipConfirmedCallbacks); // offset: 117/133 size: 1/1 align: 1/1

    if (cur_ver == HavokVersion::HK_2010_2_0)
    {
        serializer.WriteValue("useHybridBroadphase",
                              m_useHybridBroadphase); // offset: 118/134 size: 1/1 align: 1/1
        serializer.Skip(1);                           // offset: 119/135 size: 1/1
    }
    else
    {
        serializer.Skip(2);
    }

    serializer.WriteValue("solverTau", m_solverTau);               // offset: 120/136 size: 4/4 align: 4/4
    serializer.WriteValue("solverDamp", m_solverDamp);             // offset: 124/140 size: 4/4 align: 4/4
    serializer.WriteValue("solverIterations", m_solverIterations); // offset: 128/144 size: 4/4 align: 4/4
    serializer.WriteValue("solverMicrosteps", m_solverMicrosteps); // offset: 132/148 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_700)
    {
        serializer.WriteValue("maxConstraintViolation",
                              m_maxConstraintViolation); // offset: 136/152 size: 4/4 align: 4/4
    }

    serializer.WriteValue("forceCoherentConstraintOrderingInSolver",
                          m_forceCoherentConstraintOrderingInSolver); // offset: 140/156 size: 1/1 align: 1/1
    serializer.Skip(3);                                               // offset: 141/157 size: 3/3

    serializer.WriteValue("snapCollisionToConvexEdgeThreshold",
                          m_snapCollisionToConvexEdgeThreshold); // offset: 144/160 size: 4/4 align: 4/4
    serializer.WriteValue("snapCollisionToConcaveEdgeThreshold",
                          m_snapCollisionToConcaveEdgeThreshold); // offset: 148/164 size: 4/4 align: 4/4
    serializer.WriteValue("enableToiWeldRejection",
                          m_enableToiWeldRejection); // offset: 152/168 size: 1/1 align: 1/1
    serializer.WriteValue("enableDeprecatedWelding",
                          m_enableDeprecatedWelding); // offset: 153/169 size: 1/1 align: 1/1
    serializer.Skip(2);                               // offset: 154/170 size: 2/2

    serializer.WriteValue("iterativeLinearCastEarlyOutDistance",
                          m_iterativeLinearCastEarlyOutDistance); // offset: 156/172 size: 4/4 align: 4/4
    serializer.WriteValue("iterativeLinearCastMaxIterations",
                          m_iterativeLinearCastMaxIterations); // offset: 160/176 size: 4/4 align: 4/4
    serializer.WriteValue("deactivationNumInactiveFramesSelectFlag0",
                          m_deactivationNumInactiveFramesSelectFlag0); // offset: 164/180 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationNumInactiveFramesSelectFlag1",
                          m_deactivationNumInactiveFramesSelectFlag1); // offset: 165/181 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationIntegrateCounter",
                          m_deactivationIntegrateCounter); // offset: 166/182 size: 1/1 align: 1/1
    serializer.WriteValue("shouldActivateOnRigidBodyTransformChange",
                          m_shouldActivateOnRigidBodyTransformChange); // offset: 167/183 size: 1/1 align: 1/1
    serializer.WriteValue("deactivationReferenceDistance",
                          m_deactivationReferenceDistance); // offset: 168/184 size: 4/4 align: 4/4
    serializer.WriteValue("toiCollisionResponseRotateNormal",
                          m_toiCollisionResponseRotateNormal); // offset: 172/188 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        if (cur_ver > HavokVersion::HK_2011_1_0)
        {
            serializer.WriteValue(cur_ver > HavokVersion::HK_2011_3_0 ? "useCompoundSpuElf"
                                                                      : "BETA_useCompoundSpuElf",
                                  m_useCompoundSpuElf);
            serializer.Skip(3);
        }
        else if (cur_ver == HavokVersion::HK_2011_1_0)
        {
            serializer.WriteValue("BETA_useSplitCollisionPipeline", m_BETA_useSplitCollisionPipeline);
            serializer.Skip(3);
        }

        serializer.WriteValue("maxSectorsPerMidphaseCollideTask",
                              m_maxSectorsPerMidphaseCollideTask); // offset: 176/192 size: 4/4 align: 4/4
        serializer.WriteValue("maxSectorsPerNarrowphaseCollideTask",
                              m_maxSectorsPerNarrowphaseCollideTask); // offset: 180/196 size: 4/4 align: 4/4
        serializer.WriteValue("processToisMultithreaded",
                              m_processToisMultithreaded); // offset: 184/200 size: 1/1 align: 1/1
        serializer.Skip(3);                                // offset: 185/201 size: 3/3

        serializer.WriteValue("maxEntriesPerToiMidphaseCollideTask",
                              m_maxEntriesPerToiMidphaseCollideTask); // offset: 188/204 size: 4/4 align: 4/4
        serializer.WriteValue(
            "maxEntriesPerToiNarrowphaseCollideTask",
            m_maxEntriesPerToiNarrowphaseCollideTask); // offset: 192/208 size: 4/4 align: 4/4
    }
    else
    {
        serializer.WriteValue("maxSectorsPerCollideTask", m_maxSectorsPerCollideTask);
        serializer.WriteValue("processToisMultithreaded", m_processToisMultithreaded);
        serializer.Skip(3);

        serializer.WriteValue("maxEntriesPerToiCollideTask", m_maxEntriesPerToiCollideTask);
    }

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.WriteValue(
            "maxNumToiCollisionPairsSinglethreaded",
            m_maxNumToiCollisionPairsSinglethreaded); // offset: 196/212 size: 4/4 align: 4/4
    }

    serializer.WriteValue(
        "numToisTillAllowedPenetrationSimplifiedToi",
        m_numToisTillAllowedPenetrationSimplifiedToi); // offset: 200/216 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToi",
                          m_numToisTillAllowedPenetrationToi); // offset: 204/220 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToiHigher",
                          m_numToisTillAllowedPenetrationToiHigher); // offset: 208/224 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToiForced",
                          m_numToisTillAllowedPenetrationToiForced);   // offset: 212/228 size: 4/4 align: 4/4
    serializer.WriteValue("enableDeactivation", m_enableDeactivation); // offset: 216/232 size: 1/1 align: 1/1
    serializer.WriteValue("simulationType", m_simulationType);         // offset: 217/233 size: 1/1 align: 1/1
    serializer.WriteValue("enableSimulationIslands",
                          m_enableSimulationIslands); // offset: 218/234 size: 1/1 align: 1/1
    serializer.Skip(1);                               // offset: 219/235 size: 1/1

    serializer.WriteValue("minDesiredIslandSize",
                          m_minDesiredIslandSize); // offset: 220/236 size: 4/4 align: 4/4
    serializer.WriteValue("processActionsInSingleThread",
                          m_processActionsInSingleThread); // offset: 224/240 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_700)
    {
        serializer.WriteValue(
            "allowIntegrationOfIslandsWithoutConstraintsInASeparateJob",
            m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob); // offset: 225/241 size: 1/1 align: 1/1
        serializer.Skip(2);                                               // offset: 226/242 size: 2/2
    }
    else
    {
        serializer.Skip(3);
    }

    serializer.WriteValue("frameMarkerPsiSnap", m_frameMarkerPsiSnap); // offset: 228/244 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_660)
    {
        serializer.WriteValue("fireCollisionCallbacks",
                              m_fireCollisionCallbacks); // offset: 232/248 size: 1/1 align: 1/1
    }

    serializer.Pad(16); // offset: 233/249 size: 7/7
    // class size: 240/256 align: 16/16
}

void nemesis::hkpWorldCinfo::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.Pad(16);                                       // offset: 8/16 size: 8/0

    deserializer.ReadValue("gravity", m_gravity); // offset: 16/16 size: 16/16 align: 16/16
    deserializer.ReadValue("broadPhaseQuerySize",
                           m_broadPhaseQuerySize); // offset: 32/32 size: 4/4 align: 4/4
    deserializer.ReadValue("contactRestingVelocity",
                           m_contactRestingVelocity); // offset: 36/36 size: 4/4 align: 4/4
    deserializer.ReadValue("broadPhaseBorderBehaviour",
                           m_broadPhaseBorderBehaviour); // offset: 40/40 size: 1/1 align: 1/1
    deserializer.ReadValue(
        "mtPostponeAndSortBroadPhaseBorderCallbacks",
        m_mtPostponeAndSortBroadPhaseBorderCallbacks); // offset: 41/41 size: 1/1 align: 1/1
    deserializer.Skip(6);                              // offset: 42/42 size: 6/6

    deserializer.ReadObject("broadPhaseWorldAabb",
                            m_broadPhaseWorldAabb); // offset: 48/48 size: 32/32 align: 16/16

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver > HavokVersion::HK_660)
    {
        if (cur_ver <= HavokVersion::HK_2010_2_0)
        {
            deserializer.ReadValue("useKdTree", m_useKdTree); // offset: 80/80 size: 1/1 align: 1/1
            deserializer.ReadValue("useMultipleTree",
                                   m_useMultipleTree);                  // offset: 81/81 size: 1/1 align: 1/1
            deserializer.ReadValue("treeUpdateType", m_treeUpdateType); // offset: 82/82 size: 1/1 align: 1/1
        }
    }
    else
    {
        deserializer.ReadValue("useKdTree", m_useKdTree);
    }

    if (cur_ver <= HavokVersion::HK_2011_2_0)
    {
        deserializer.ReadValue("autoUpdateKdTree",
                               m_autoUpdateKdTree); // offset: 83/83 size: 1/1 align: 1/1
    }

    deserializer.Pad(4);

    deserializer.ReadValue("collisionTolerance", m_collisionTolerance); // offset: 84/84 size: 4/4 align: 4/4
    deserializer.ReadObject("collisionFilter", m_collisionFilter);      // offset: 88/88 size: 4/8 align: 4/8
    deserializer.ReadObject("convexListFilter", m_convexListFilter);    // offset: 92/96 size: 4/8 align: 4/8
    deserializer.ReadValue("expectedMaxLinearVelocity",
                           m_expectedMaxLinearVelocity); // offset: 96/104 size: 4/4 align: 4/4
    deserializer.ReadValue("sizeOfToiEventQueue",
                           m_sizeOfToiEventQueue); // offset: 100/108 size: 4/4 align: 4/4
    deserializer.ReadValue("expectedMinPsiDeltaTime",
                           m_expectedMinPsiDeltaTime); // offset: 104/112 size: 4/4 align: 4/4
    deserializer.Pad(deserializer.GetPointerSize());   // offset: 108/116 size: 0/4

    deserializer.ReadObject("memoryWatchDog", m_memoryWatchDog); // offset: 108/120 size: 4/8 align: 4/8
    deserializer.ReadValue("broadPhaseNumMarkers",
                           m_broadPhaseNumMarkers); // offset: 112/128 size: 4/4 align: 4/4
    deserializer.ReadValue("contactPointGeneration",
                           m_contactPointGeneration); // offset: 116/132 size: 1/1 align: 1/1
    deserializer.ReadValue("allowToSkipConfirmedCallbacks",
                           m_allowToSkipConfirmedCallbacks); // offset: 117/133 size: 1/1 align: 1/1

    if (cur_ver == HavokVersion::HK_2010_2_0)
    {
        deserializer.ReadValue("useHybridBroadphase",
                               m_useHybridBroadphase); // offset: 118/134 size: 1/1 align: 1/1
        deserializer.Skip(1);                          // offset: 119/135 size: 1/1
    }
    else
    {
        deserializer.Skip(2);
    }

    deserializer.ReadValue("solverTau", m_solverTau);               // offset: 120/136 size: 4/4 align: 4/4
    deserializer.ReadValue("solverDamp", m_solverDamp);             // offset: 124/140 size: 4/4 align: 4/4
    deserializer.ReadValue("solverIterations", m_solverIterations); // offset: 128/144 size: 4/4 align: 4/4
    deserializer.ReadValue("solverMicrosteps", m_solverMicrosteps); // offset: 132/148 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_700)
    {
        deserializer.ReadValue("maxConstraintViolation",
                               m_maxConstraintViolation); // offset: 136/152 size: 4/4 align: 4/4
    }

    deserializer.ReadValue("forceCoherentConstraintOrderingInSolver",
                           m_forceCoherentConstraintOrderingInSolver); // offset: 140/156 size: 1/1 align: 1/1
    deserializer.Skip(3);                                              // offset: 141/157 size: 3/3

    deserializer.ReadValue("snapCollisionToConvexEdgeThreshold",
                           m_snapCollisionToConvexEdgeThreshold); // offset: 144/160 size: 4/4 align: 4/4
    deserializer.ReadValue("snapCollisionToConcaveEdgeThreshold",
                           m_snapCollisionToConcaveEdgeThreshold); // offset: 148/164 size: 4/4 align: 4/4
    deserializer.ReadValue("enableToiWeldRejection",
                           m_enableToiWeldRejection); // offset: 152/168 size: 1/1 align: 1/1
    deserializer.ReadValue("enableDeprecatedWelding",
                           m_enableDeprecatedWelding); // offset: 153/169 size: 1/1 align: 1/1
    deserializer.Skip(2);                              // offset: 154/170 size: 2/2

    deserializer.ReadValue("iterativeLinearCastEarlyOutDistance",
                           m_iterativeLinearCastEarlyOutDistance); // offset: 156/172 size: 4/4 align: 4/4
    deserializer.ReadValue("iterativeLinearCastMaxIterations",
                           m_iterativeLinearCastMaxIterations); // offset: 160/176 size: 4/4 align: 4/4
    deserializer.ReadValue(
        "deactivationNumInactiveFramesSelectFlag0",
        m_deactivationNumInactiveFramesSelectFlag0); // offset: 164/180 size: 1/1 align: 1/1
    deserializer.ReadValue(
        "deactivationNumInactiveFramesSelectFlag1",
        m_deactivationNumInactiveFramesSelectFlag1); // offset: 165/181 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationIntegrateCounter",
                           m_deactivationIntegrateCounter); // offset: 166/182 size: 1/1 align: 1/1
    deserializer.ReadValue(
        "shouldActivateOnRigidBodyTransformChange",
        m_shouldActivateOnRigidBodyTransformChange); // offset: 167/183 size: 1/1 align: 1/1
    deserializer.ReadValue("deactivationReferenceDistance",
                           m_deactivationReferenceDistance); // offset: 168/184 size: 4/4 align: 4/4
    deserializer.ReadValue("toiCollisionResponseRotateNormal",
                           m_toiCollisionResponseRotateNormal); // offset: 172/188 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_2010_1_0)
    {
        if (cur_ver > HavokVersion::HK_2011_1_0)
        {
            deserializer.ReadValue(cur_ver > HavokVersion::HK_2011_3_0 ? "useCompoundSpuElf"
                                                                       : "BETA_useCompoundSpuElf",
                                   m_useCompoundSpuElf);
            deserializer.Skip(3);
        }
        else if (cur_ver == HavokVersion::HK_2011_1_0)
        {
            deserializer.ReadValue("BETA_useSplitCollisionPipeline", m_BETA_useSplitCollisionPipeline);
            deserializer.Skip(3);
        }

        deserializer.ReadValue("maxSectorsPerMidphaseCollideTask",
                               m_maxSectorsPerMidphaseCollideTask); // offset: 176/192 size: 4/4 align: 4/4
        deserializer.ReadValue("maxSectorsPerNarrowphaseCollideTask",
                               m_maxSectorsPerNarrowphaseCollideTask); // offset: 180/196 size: 4/4 align: 4/4
        deserializer.ReadValue("processToisMultithreaded",
                               m_processToisMultithreaded); // offset: 184/200 size: 1/1 align: 1/1
        deserializer.Skip(3);                               // offset: 185/201 size: 3/3

        deserializer.ReadValue("maxEntriesPerToiMidphaseCollideTask",
                               m_maxEntriesPerToiMidphaseCollideTask); // offset: 188/204 size: 4/4 align: 4/4
        deserializer.ReadValue(
            "maxEntriesPerToiNarrowphaseCollideTask",
            m_maxEntriesPerToiNarrowphaseCollideTask); // offset: 192/208 size: 4/4 align: 4/4
    }
    else
    {
        deserializer.ReadValue("maxSectorsPerCollideTask", m_maxSectorsPerCollideTask);
        deserializer.ReadValue("processToisMultithreaded", m_processToisMultithreaded);
        deserializer.Skip(3);

        deserializer.ReadValue("maxEntriesPerToiCollideTask", m_maxEntriesPerToiCollideTask);
    }

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadValue(
            "maxNumToiCollisionPairsSinglethreaded",
            m_maxNumToiCollisionPairsSinglethreaded); // offset: 196/212 size: 4/4 align: 4/4
    }

    deserializer.ReadValue(
        "numToisTillAllowedPenetrationSimplifiedToi",
        m_numToisTillAllowedPenetrationSimplifiedToi); // offset: 200/216 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToi",
                           m_numToisTillAllowedPenetrationToi); // offset: 204/220 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToiHigher",
                           m_numToisTillAllowedPenetrationToiHigher); // offset: 208/224 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToiForced",
                           m_numToisTillAllowedPenetrationToiForced); // offset: 212/228 size: 4/4 align: 4/4
    deserializer.ReadValue("enableDeactivation",
                           m_enableDeactivation);               // offset: 216/232 size: 1/1 align: 1/1
    deserializer.ReadValue("simulationType", m_simulationType); // offset: 217/233 size: 1/1 align: 1/1
    deserializer.ReadValue("enableSimulationIslands",
                           m_enableSimulationIslands); // offset: 218/234 size: 1/1 align: 1/1
    deserializer.Skip(1);                              // offset: 219/235 size: 1/1

    deserializer.ReadValue("minDesiredIslandSize",
                           m_minDesiredIslandSize); // offset: 220/236 size: 4/4 align: 4/4
    deserializer.ReadValue("processActionsInSingleThread",
                           m_processActionsInSingleThread); // offset: 224/240 size: 1/1 align: 1/1

    if (cur_ver > HavokVersion::HK_700)
    {
        deserializer.ReadValue(
            "allowIntegrationOfIslandsWithoutConstraintsInASeparateJob",
            m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob); // offset: 225/241 size: 1/1 align: 1/1
        deserializer.Skip(2);                                             // offset: 226/242 size: 2/2
    }
    else
    {
        deserializer.Skip(3);
    }

    deserializer.ReadValue("frameMarkerPsiSnap",
                           m_frameMarkerPsiSnap); // offset: 228/244 size: 4/4 align: 4/4

    if (cur_ver > HavokVersion::HK_660)
    {
        deserializer.ReadValue("fireCollisionCallbacks",
                               m_fireCollisionCallbacks); // offset: 232/248 size: 1/1 align: 1/1
    }

    deserializer.Pad(16); // offset: 233/249 size: 7/7
    // class size: 240/256 align: 16/16
}
