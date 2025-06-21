#include "Havok/hkpWorld.h"

REGISTER_HAVOK_POINTER_SOURCE(hkpWorld,
                              [](nemesis::HavokVersion version)
                              { return &nemesis::hkpWorld::Class; });

nemesis::hkpWorld::hkpWorld() noexcept
    : nemesis::HavokObject(Class.GetSignature())
    , nemesis::hkReferencedObject(Class)
{
}

const nemesis::hkClass* nemesis::hkpWorld::GetClass(nemesis::HavokVersion version) const
{
    return &Class;
}

void nemesis::hkpWorld::SerializeTo(nemesis::Serializer& serializer) const
{
    nemesis::hkReferencedObject::SerializeTo(serializer); // offset: 0/0 size: 8/16 align: 4/8
    serializer.WriteObject("simulation", m_simulation);   // offset: 8/16 size: 4/8 align: 4/8
    serializer.Pad(16);                                   // offset: 12/24 size: 4/8

    serializer.WriteValue("gravity", m_gravity); // offset: 16/32 size: 16/16 align: 16/16
    serializer.WriteSerializeIgnoredObject("fixedIsland",
                                           m_fixedIsland);      // offset: 32/48 size: 4/8 align: 4/8
    serializer.WriteObject("fixedRigidBody", m_fixedRigidBody); // offset: 36/56 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("activeSimulationIslands",
                                           m_activeSimulationIslands); // offset: 40/64 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "inactiveSimulationIslands", m_inactiveSimulationIslands); // offset: 52/80 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("dirtySimulationIslands",
                                           m_dirtySimulationIslands); // offset: 64/96 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("maintenanceMgr",
                                           m_maintenanceMgr); // offset: 76/112 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("memoryWatchDog",
                                           m_memoryWatchDog); // offset: 80/120 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredValue(
        "assertOnRunningOutOfSolverMemory",
        m_assertOnRunningOutOfSolverMemory); // offset: 84/128 size: 1/1 align: 1/1

    auto cur_ver = serializer.GetContentsVersion();

    if (cur_ver >= HavokVersion::HK_2011_1_0)
    {
        serializer.WriteSerializeIgnoredValue("broadPhaseType", m_broadPhaseType);
    }

    serializer.Pad(16); // offset: 85/129 size: 3/7

    serializer.WriteSerializeIgnoredObject("broadPhase", m_broadPhase); // offset: 88/136 size: 4/8 align: 4/8

    if (cur_ver < HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredObject("kdTreeManager",
                                               m_kdTreeManager);   // offset: 92/144 size: 4/8 align: 4/8
        serializer.WriteValue("autoUpdateTree", m_autoUpdateTree); // offset: 96/152 size: 1/1 align: 1/1
    }

    serializer.Pad(16); // offset: 97/153 size: 3/7

    serializer.WriteSerializeIgnoredObject("broadPhaseDispatcher",
                                           m_broadPhaseDispatcher); // offset: 100/160 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "phantomBroadPhaseListener", m_phantomBroadPhaseListener); // offset: 104/168 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "entityEntityBroadPhaseListener",
        m_entityEntityBroadPhaseListener); // offset: 108/176 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "broadPhaseBorderListener", m_broadPhaseBorderListener); // offset: 112/184 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "multithreadedSimulationJobData",
        m_multithreadedSimulationJobData); // offset: 116/192 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("collisionInput",
                                           m_collisionInput); // offset: 120/200 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("collisionFilter",
                                           m_collisionFilter); // offset: 124/208 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("collisionDispatcher",
                                           m_collisionDispatcher); // offset: 128/216 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("convexListFilter",
                                           m_convexListFilter); // offset: 132/224 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("pendingOperations",
                                           m_pendingOperations); // offset: 136/232 size: 4/8 align: 4/8
    serializer.WriteValue("pendingOperationsCount",
                          m_pendingOperationsCount); // offset: 140/240 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue(
        "pendingBodyOperationsCount", m_pendingBodyOperationsCount); // offset: 144/244 size: 4/4 align: 4/4
    serializer.WriteValue("criticalOperationsLockCount",
                          m_criticalOperationsLockCount); // offset: 148/248 size: 4/4 align: 4/4
    serializer.WriteValue("criticalOperationsLockCountForPhantoms",
                          m_criticalOperationsLockCountForPhantoms); // offset: 152/252 size: 4/4 align: 4/4
    serializer.WriteValue("blockExecutingPendingOperations",
                          m_blockExecutingPendingOperations); // offset: 156/256 size: 1/1 align: 1/1
    serializer.WriteValue("criticalOperationsAllowed",
                          m_criticalOperationsAllowed); // offset: 157/257 size: 1/1 align: 1/1
    serializer.Pad(16);                                 // offset: 158/258 size: 2/6

    serializer.WriteSerializeIgnoredObject("pendingOperationQueues",
                                           m_pendingOperationQueues); // offset: 160/264 size: 4/8 align: 4/8
    serializer.WriteValue("pendingOperationQueueCount",
                          m_pendingOperationQueueCount); // offset: 164/272 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject("multiThreadCheck",
                                           m_multiThreadCheck); // offset: 168/276 size: 12/12 align: 4/4
    serializer.WriteValue("processActionsInSingleThread",
                          m_processActionsInSingleThread); // offset: 180/288 size: 1/1 align: 1/1
    serializer.WriteValue(
        "allowIntegrationOfIslandsWithoutConstraintsInASeparateJob",
        m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob); // offset: 181/289 size: 1/1 align: 1/1
    serializer.Skip(2);                                               // offset: 182/290 size: 2/2

    serializer.WriteValue("minDesiredIslandSize",
                          m_minDesiredIslandSize); // offset: 184/292 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredObject(
        "modifyConstraintCriticalSection",
        m_modifyConstraintCriticalSection);        // offset: 188/296 size: 4/8 align: 4/8
    serializer.WriteValue("isLocked", m_isLocked); // offset: 192/304 size: 4/4 align: 4/4
    serializer.Pad(16);                            // offset: 196/308 size: 0/4

    serializer.WriteSerializeIgnoredObject(
        "islandDirtyListCriticalSection",
        m_islandDirtyListCriticalSection); // offset: 196/312 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("propertyMasterLock",
                                           m_propertyMasterLock); // offset: 200/320 size: 4/8 align: 4/8
    serializer.WriteValue("wantSimulationIslands",
                          m_wantSimulationIslands); // offset: 204/328 size: 1/1 align: 1/1

    if (cur_ver < HavokVersion::HK_2011_1_0)
    {
        serializer.WriteSerializeIgnoredValue("useHybridBroadphase",
                                              m_useHybridBroadphase); // offset: 205/329 size: 1/1 align: 1/1
        serializer.Skip(2);                                           // offset: 206/330 size: 2/2
    }
    else
    {
        serializer.Skip(3);
    }

    serializer.WriteValue("snapCollisionToConvexEdgeThreshold",
                          m_snapCollisionToConvexEdgeThreshold); // offset: 208/332 size: 4/4 align: 4/4
    serializer.WriteValue("snapCollisionToConcaveEdgeThreshold",
                          m_snapCollisionToConcaveEdgeThreshold); // offset: 212/336 size: 4/4 align: 4/4
    serializer.WriteValue("enableToiWeldRejection",
                          m_enableToiWeldRejection);               // offset: 216/340 size: 1/1 align: 1/1
    serializer.WriteValue("wantDeactivation", m_wantDeactivation); // offset: 217/341 size: 1/1 align: 1/1
    serializer.WriteValue("shouldActivateOnRigidBodyTransformChange",
                          m_shouldActivateOnRigidBodyTransformChange); // offset: 218/342 size: 1/1 align: 1/1
    serializer.Skip(1);                                                // offset: 219/343 size: 1/1

    serializer.WriteValue("deactivationReferenceDistance",
                          m_deactivationReferenceDistance); // offset: 220/344 size: 4/4 align: 4/4
    serializer.WriteValue("toiCollisionResponseRotateNormal",
                          m_toiCollisionResponseRotateNormal); // offset: 224/348 size: 4/4 align: 4/4
    serializer.WriteValue("maxSectorsPerMidphaseCollideTask",
                          m_maxSectorsPerMidphaseCollideTask); // offset: 228/352 size: 4/4 align: 4/4
    serializer.WriteValue("maxSectorsPerNarrowphaseCollideTask",
                          m_maxSectorsPerNarrowphaseCollideTask); // offset: 232/356 size: 4/4 align: 4/4
    serializer.WriteValue("processToisMultithreaded",
                          m_processToisMultithreaded); // offset: 236/360 size: 1/1 align: 1/1
    serializer.Skip(3);                                // offset: 237/361 size: 3/3

    serializer.WriteValue("maxEntriesPerToiMidphaseCollideTask",
                          m_maxEntriesPerToiMidphaseCollideTask); // offset: 240/364 size: 4/4 align: 4/4
    serializer.WriteValue("maxEntriesPerToiNarrowphaseCollideTask",
                          m_maxEntriesPerToiNarrowphaseCollideTask); // offset: 244/368 size: 4/4 align: 4/4
    serializer.WriteValue("maxNumToiCollisionPairsSinglethreaded",
                          m_maxNumToiCollisionPairsSinglethreaded); // offset: 248/372 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("simulationType",
                                          m_simulationType); // offset: 252/376 size: 4/4 align: 4/4
    serializer.WriteValue(
        "numToisTillAllowedPenetrationSimplifiedToi",
        m_numToisTillAllowedPenetrationSimplifiedToi); // offset: 256/380 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToi",
                          m_numToisTillAllowedPenetrationToi); // offset: 260/384 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToiHigher",
                          m_numToisTillAllowedPenetrationToiHigher); // offset: 264/388 size: 4/4 align: 4/4
    serializer.WriteValue("numToisTillAllowedPenetrationToiForced",
                          m_numToisTillAllowedPenetrationToiForced); // offset: 268/392 size: 4/4 align: 4/4
    serializer.WriteValue("lastEntityUid", m_lastEntityUid);         // offset: 272/396 size: 4/4 align: 4/4
    serializer.WriteValue("lastIslandUid", m_lastIslandUid);         // offset: 276/400 size: 4/4 align: 4/4
    serializer.WriteValue("lastConstraintUid", m_lastConstraintUid); // offset: 280/404 size: 4/4 align: 4/4
    serializer.WriteObject("phantoms", m_phantoms);                  // offset: 284/408 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("actionListeners",
                                           m_actionListeners); // offset: 296/424 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("entityListeners",
                                           m_entityListeners); // offset: 308/440 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("phantomListeners",
                                           m_phantomListeners); // offset: 320/456 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("constraintListeners",
                                           m_constraintListeners); // offset: 332/472 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "worldDeletionListeners", m_worldDeletionListeners); // offset: 344/488 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "islandActivationListeners", m_islandActivationListeners); // offset: 356/504 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "worldPostSimulationListeners",
        m_worldPostSimulationListeners); // offset: 368/520 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "worldPostIntegrateListeners",
        m_worldPostIntegrateListeners); // offset: 380/536 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "worldPostCollideListeners", m_worldPostCollideListeners); // offset: 392/552 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "islandPostIntegrateListeners",
        m_islandPostIntegrateListeners); // offset: 404/568 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "islandPostCollideListeners", m_islandPostCollideListeners); // offset: 416/584 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("contactListeners",
                                           m_contactListeners); // offset: 428/600 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject(
        "contactImpulseLimitBreachedListeners",
        m_contactImpulseLimitBreachedListeners); // offset: 440/616 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("worldExtensions",
                                           m_worldExtensions); // offset: 452/632 size: 12/16 align: 4/8
    serializer.WriteSerializeIgnoredObject("violatedConstraintArray",
                                           m_violatedConstraintArray); // offset: 464/648 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("broadPhaseBorder",
                                           m_broadPhaseBorder); // offset: 468/656 size: 4/8 align: 4/8
    serializer.WriteSerializeIgnoredObject("destructionWorld",
                                           m_destructionWorld); // offset: 472/664 size: 4/8 align: 4/8

    if (cur_ver < HavokVersion::HK_2011_1_0 || cur_ver >= HavokVersion::HK_2011_3_0)
    {
        serializer.WriteSerializeIgnoredObject("npWorld", m_npWorld); // offset: 476/672 size: 4/8 align: 4/8
    }

    serializer.Pad(16); // offset: 480/680 size: 0/8

    serializer.WriteObject("dynamicsStepInfo",
                           m_dynamicsStepInfo); // offset: 480/688 size: 320/320 align: 16/16
    serializer.WriteValue("broadPhaseExtents",
                          m_broadPhaseExtents); // offset: 800/1008 size: 32/32 align: 16/16
    serializer.WriteValue("broadPhaseNumMarkers",
                          m_broadPhaseNumMarkers); // offset: 832/1040 size: 4/4 align: 4/4
    serializer.WriteValue("sizeOfToiEventQueue",
                          m_sizeOfToiEventQueue); // offset: 836/1044 size: 4/4 align: 4/4
    serializer.WriteValue("broadPhaseQuerySize",
                          m_broadPhaseQuerySize); // offset: 840/1048 size: 4/4 align: 4/4
    serializer.WriteValue("broadPhaseUpdateSize",
                          m_broadPhaseUpdateSize); // offset: 844/1052 size: 4/4 align: 4/4
    serializer.WriteSerializeIgnoredValue("contactPointGeneration",
                                          m_contactPointGeneration); // offset: 848/1056 size: 1/1 align: 1/1

    if (cur_ver >= HavokVersion::HK_2011_1_0)
    {
        if (cur_ver >= HavokVersion::HK_2012_1_0)
        {
            serializer.WriteValue("m_useCompoundSpuElf", m_BETA_useSplitCollisionPipeline);
        }
        else if (cur_ver >= HavokVersion::HK_2011_2_0)
        {
            serializer.WriteValue("BETA_useCompoundSpuElf", m_BETA_useSplitCollisionPipeline);
        }
        else
        {
            serializer.WriteValue("BETA_useSplitCollisionPipeline", m_BETA_useSplitCollisionPipeline);
        }

        serializer.Skip(14);
    }
    else
    {
        serializer.Skip(15); // offset: 849/1057 size: 15/15
    }

    // class size: 864/1072 align: 16/16
}

void nemesis::hkpWorld::DeserializeFrom(nemesis::Deserializer& deserializer)
{
    nemesis::hkReferencedObject::DeserializeFrom(deserializer); // offset: 0/0 size: 8/16 align: 4/8
    deserializer.ReadObject("simulation", m_simulation);        // offset: 8/16 size: 4/8 align: 4/8
    deserializer.Pad(16);                                       // offset: 12/24 size: 4/8

    deserializer.ReadValue("gravity", m_gravity);                // offset: 16/32 size: 16/16 align: 16/16
    deserializer.ReadObject("fixedIsland", m_fixedIsland);       // offset: 32/48 size: 4/8 align: 4/8
    deserializer.ReadObject("fixedRigidBody", m_fixedRigidBody); // offset: 36/56 size: 4/8 align: 4/8
    deserializer.ReadObject("activeSimulationIslands",
                            m_activeSimulationIslands); // offset: 40/64 size: 12/16 align: 4/8
    deserializer.ReadObject("inactiveSimulationIslands",
                            m_inactiveSimulationIslands); // offset: 52/80 size: 12/16 align: 4/8
    deserializer.ReadObject("dirtySimulationIslands",
                            m_dirtySimulationIslands);           // offset: 64/96 size: 12/16 align: 4/8
    deserializer.ReadObject("maintenanceMgr", m_maintenanceMgr); // offset: 76/112 size: 4/8 align: 4/8
    deserializer.ReadObject("memoryWatchDog", m_memoryWatchDog); // offset: 80/120 size: 4/8 align: 4/8
    deserializer.ReadValue("assertOnRunningOutOfSolverMemory",
                           m_assertOnRunningOutOfSolverMemory); // offset: 84/128 size: 1/1 align: 1/1

    auto cur_ver = deserializer.GetContentsVersion();

    if (cur_ver >= HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("broadPhaseType", m_broadPhaseType);
    }

    deserializer.Pad(16); // offset: 85/129 size: 3/7

    deserializer.ReadObject("broadPhase", m_broadPhase); // offset: 88/136 size: 4/8 align: 4/8

    if (cur_ver < HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("kdTreeManager", m_kdTreeManager);  // offset: 92/144 size: 4/8 align: 4/8
        deserializer.ReadValue("autoUpdateTree", m_autoUpdateTree); // offset: 96/152 size: 1/1 align: 1/1
    }

    deserializer.Pad(16); // offset: 97/153 size: 3/7

    deserializer.ReadObject("broadPhaseDispatcher",
                            m_broadPhaseDispatcher); // offset: 100/160 size: 4/8 align: 4/8
    deserializer.ReadObject("phantomBroadPhaseListener",
                            m_phantomBroadPhaseListener); // offset: 104/168 size: 4/8 align: 4/8
    deserializer.ReadObject("entityEntityBroadPhaseListener",
                            m_entityEntityBroadPhaseListener); // offset: 108/176 size: 4/8 align: 4/8
    deserializer.ReadObject("broadPhaseBorderListener",
                            m_broadPhaseBorderListener); // offset: 112/184 size: 4/8 align: 4/8
    deserializer.ReadObject("multithreadedSimulationJobData",
                            m_multithreadedSimulationJobData);     // offset: 116/192 size: 4/8 align: 4/8
    deserializer.ReadObject("collisionInput", m_collisionInput);   // offset: 120/200 size: 4/8 align: 4/8
    deserializer.ReadObject("collisionFilter", m_collisionFilter); // offset: 124/208 size: 4/8 align: 4/8
    deserializer.ReadObject("collisionDispatcher",
                            m_collisionDispatcher);                    // offset: 128/216 size: 4/8 align: 4/8
    deserializer.ReadObject("convexListFilter", m_convexListFilter);   // offset: 132/224 size: 4/8 align: 4/8
    deserializer.ReadObject("pendingOperations", m_pendingOperations); // offset: 136/232 size: 4/8 align: 4/8
    deserializer.ReadValue("pendingOperationsCount",
                           m_pendingOperationsCount); // offset: 140/240 size: 4/4 align: 4/4
    deserializer.ReadValue("pendingBodyOperationsCount",
                           m_pendingBodyOperationsCount); // offset: 144/244 size: 4/4 align: 4/4
    deserializer.ReadValue("criticalOperationsLockCount",
                           m_criticalOperationsLockCount); // offset: 148/248 size: 4/4 align: 4/4
    deserializer.ReadValue("criticalOperationsLockCountForPhantoms",
                           m_criticalOperationsLockCountForPhantoms); // offset: 152/252 size: 4/4 align: 4/4
    deserializer.ReadValue("blockExecutingPendingOperations",
                           m_blockExecutingPendingOperations); // offset: 156/256 size: 1/1 align: 1/1
    deserializer.ReadValue("criticalOperationsAllowed",
                           m_criticalOperationsAllowed); // offset: 157/257 size: 1/1 align: 1/1
    deserializer.Pad(16);                                // offset: 158/258 size: 2/6

    deserializer.ReadObject("pendingOperationQueues",
                            m_pendingOperationQueues); // offset: 160/264 size: 4/8 align: 4/8
    deserializer.ReadValue("pendingOperationQueueCount",
                           m_pendingOperationQueueCount);            // offset: 164/272 size: 4/4 align: 4/4
    deserializer.ReadObject("multiThreadCheck", m_multiThreadCheck); // offset: 168/276 size: 12/12 align: 4/4
    deserializer.ReadValue("processActionsInSingleThread",
                           m_processActionsInSingleThread); // offset: 180/288 size: 1/1 align: 1/1
    deserializer.ReadValue(
        "allowIntegrationOfIslandsWithoutConstraintsInASeparateJob",
        m_allowIntegrationOfIslandsWithoutConstraintsInASeparateJob); // offset: 181/289 size: 1/1 align: 1/1
    deserializer.Skip(2);                                             // offset: 182/290 size: 2/2

    deserializer.ReadValue("minDesiredIslandSize",
                           m_minDesiredIslandSize); // offset: 184/292 size: 4/4 align: 4/4
    deserializer.ReadObject("modifyConstraintCriticalSection",
                            m_modifyConstraintCriticalSection); // offset: 188/296 size: 4/8 align: 4/8
    deserializer.ReadValue("isLocked", m_isLocked);             // offset: 192/304 size: 4/4 align: 4/4
    deserializer.Pad(16);                                       // offset: 196/308 size: 0/4

    deserializer.ReadObject("islandDirtyListCriticalSection",
                            m_islandDirtyListCriticalSection); // offset: 196/312 size: 4/8 align: 4/8
    deserializer.ReadObject("propertyMasterLock",
                            m_propertyMasterLock); // offset: 200/320 size: 4/8 align: 4/8
    deserializer.ReadValue("wantSimulationIslands",
                           m_wantSimulationIslands); // offset: 204/328 size: 1/1 align: 1/1

    if (cur_ver < HavokVersion::HK_2011_1_0)
    {
        deserializer.ReadValue("useHybridBroadphase",
                               m_useHybridBroadphase); // offset: 205/329 size: 1/1 align: 1/1
        deserializer.Skip(2);                          // offset: 206/330 size: 2/2
    }
    else
    {
        deserializer.Skip(3);
    }

    deserializer.ReadValue("snapCollisionToConvexEdgeThreshold",
                           m_snapCollisionToConvexEdgeThreshold); // offset: 208/332 size: 4/4 align: 4/4
    deserializer.ReadValue("snapCollisionToConcaveEdgeThreshold",
                           m_snapCollisionToConcaveEdgeThreshold); // offset: 212/336 size: 4/4 align: 4/4
    deserializer.ReadValue("enableToiWeldRejection",
                           m_enableToiWeldRejection);               // offset: 216/340 size: 1/1 align: 1/1
    deserializer.ReadValue("wantDeactivation", m_wantDeactivation); // offset: 217/341 size: 1/1 align: 1/1
    deserializer.ReadValue(
        "shouldActivateOnRigidBodyTransformChange",
        m_shouldActivateOnRigidBodyTransformChange); // offset: 218/342 size: 1/1 align: 1/1
    deserializer.Skip(1);                            // offset: 219/343 size: 1/1

    deserializer.ReadValue("deactivationReferenceDistance",
                           m_deactivationReferenceDistance); // offset: 220/344 size: 4/4 align: 4/4
    deserializer.ReadValue("toiCollisionResponseRotateNormal",
                           m_toiCollisionResponseRotateNormal); // offset: 224/348 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSectorsPerMidphaseCollideTask",
                           m_maxSectorsPerMidphaseCollideTask); // offset: 228/352 size: 4/4 align: 4/4
    deserializer.ReadValue("maxSectorsPerNarrowphaseCollideTask",
                           m_maxSectorsPerNarrowphaseCollideTask); // offset: 232/356 size: 4/4 align: 4/4
    deserializer.ReadValue("processToisMultithreaded",
                           m_processToisMultithreaded); // offset: 236/360 size: 1/1 align: 1/1
    deserializer.Skip(3);                               // offset: 237/361 size: 3/3

    deserializer.ReadValue("maxEntriesPerToiMidphaseCollideTask",
                           m_maxEntriesPerToiMidphaseCollideTask); // offset: 240/364 size: 4/4 align: 4/4
    deserializer.ReadValue("maxEntriesPerToiNarrowphaseCollideTask",
                           m_maxEntriesPerToiNarrowphaseCollideTask); // offset: 244/368 size: 4/4 align: 4/4
    deserializer.ReadValue("maxNumToiCollisionPairsSinglethreaded",
                           m_maxNumToiCollisionPairsSinglethreaded); // offset: 248/372 size: 4/4 align: 4/4
    deserializer.ReadValue("simulationType", m_simulationType);      // offset: 252/376 size: 4/4 align: 4/4
    deserializer.ReadValue(
        "numToisTillAllowedPenetrationSimplifiedToi",
        m_numToisTillAllowedPenetrationSimplifiedToi); // offset: 256/380 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToi",
                           m_numToisTillAllowedPenetrationToi); // offset: 260/384 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToiHigher",
                           m_numToisTillAllowedPenetrationToiHigher); // offset: 264/388 size: 4/4 align: 4/4
    deserializer.ReadValue("numToisTillAllowedPenetrationToiForced",
                           m_numToisTillAllowedPenetrationToiForced); // offset: 268/392 size: 4/4 align: 4/4
    deserializer.ReadValue("lastEntityUid", m_lastEntityUid);         // offset: 272/396 size: 4/4 align: 4/4
    deserializer.ReadValue("lastIslandUid", m_lastIslandUid);         // offset: 276/400 size: 4/4 align: 4/4
    deserializer.ReadValue("lastConstraintUid", m_lastConstraintUid); // offset: 280/404 size: 4/4 align: 4/4
    deserializer.ReadObject("phantoms", m_phantoms);                 // offset: 284/408 size: 12/16 align: 4/8
    deserializer.ReadObject("actionListeners", m_actionListeners);   // offset: 296/424 size: 12/16 align: 4/8
    deserializer.ReadObject("entityListeners", m_entityListeners);   // offset: 308/440 size: 12/16 align: 4/8
    deserializer.ReadObject("phantomListeners", m_phantomListeners); // offset: 320/456 size: 12/16 align: 4/8
    deserializer.ReadObject("constraintListeners",
                            m_constraintListeners); // offset: 332/472 size: 12/16 align: 4/8
    deserializer.ReadObject("worldDeletionListeners",
                            m_worldDeletionListeners); // offset: 344/488 size: 12/16 align: 4/8
    deserializer.ReadObject("islandActivationListeners",
                            m_islandActivationListeners); // offset: 356/504 size: 12/16 align: 4/8
    deserializer.ReadObject("worldPostSimulationListeners",
                            m_worldPostSimulationListeners); // offset: 368/520 size: 12/16 align: 4/8
    deserializer.ReadObject("worldPostIntegrateListeners",
                            m_worldPostIntegrateListeners); // offset: 380/536 size: 12/16 align: 4/8
    deserializer.ReadObject("worldPostCollideListeners",
                            m_worldPostCollideListeners); // offset: 392/552 size: 12/16 align: 4/8
    deserializer.ReadObject("islandPostIntegrateListeners",
                            m_islandPostIntegrateListeners); // offset: 404/568 size: 12/16 align: 4/8
    deserializer.ReadObject("islandPostCollideListeners",
                            m_islandPostCollideListeners);           // offset: 416/584 size: 12/16 align: 4/8
    deserializer.ReadObject("contactListeners", m_contactListeners); // offset: 428/600 size: 12/16 align: 4/8
    deserializer.ReadObject("contactImpulseLimitBreachedListeners",
                            m_contactImpulseLimitBreachedListeners); // offset: 440/616 size: 12/16 align: 4/8
    deserializer.ReadObject("worldExtensions", m_worldExtensions);   // offset: 452/632 size: 12/16 align: 4/8
    deserializer.ReadObject("violatedConstraintArray",
                            m_violatedConstraintArray);              // offset: 464/648 size: 4/8 align: 4/8
    deserializer.ReadObject("broadPhaseBorder", m_broadPhaseBorder); // offset: 468/656 size: 4/8 align: 4/8
    deserializer.ReadObject("destructionWorld", m_destructionWorld); // offset: 472/664 size: 4/8 align: 4/8

    if (cur_ver < HavokVersion::HK_2011_1_0 || cur_ver >= HavokVersion::HK_2011_3_0)
    {
        deserializer.ReadObject("npWorld", m_npWorld); // offset: 476/672 size: 4/8 align: 4/8
    }

    deserializer.Pad(16); // offset: 480/680 size: 0/8

    deserializer.ReadObject("dynamicsStepInfo",
                            m_dynamicsStepInfo); // offset: 480/688 size: 320/320 align: 16/16
    deserializer.ReadValue("broadPhaseExtents",
                           m_broadPhaseExtents); // offset: 800/1008 size: 32/32 align: 16/16
    deserializer.ReadValue("broadPhaseNumMarkers",
                           m_broadPhaseNumMarkers); // offset: 832/1040 size: 4/4 align: 4/4
    deserializer.ReadValue("sizeOfToiEventQueue",
                           m_sizeOfToiEventQueue); // offset: 836/1044 size: 4/4 align: 4/4
    deserializer.ReadValue("broadPhaseQuerySize",
                           m_broadPhaseQuerySize); // offset: 840/1048 size: 4/4 align: 4/4
    deserializer.ReadValue("broadPhaseUpdateSize",
                           m_broadPhaseUpdateSize); // offset: 844/1052 size: 4/4 align: 4/4
    deserializer.ReadValue("contactPointGeneration",
                           m_contactPointGeneration); // offset: 848/1056 size: 1/1 align: 1/1

    if (cur_ver >= HavokVersion::HK_2011_1_0)
    {
        if (cur_ver >= HavokVersion::HK_2012_1_0)
        {
            deserializer.ReadValue("m_useCompoundSpuElf", m_BETA_useSplitCollisionPipeline);
        }
        else if (cur_ver >= HavokVersion::HK_2011_2_0)
        {
            deserializer.ReadValue("BETA_useCompoundSpuElf", m_BETA_useSplitCollisionPipeline);
        }
        else
        {
            deserializer.ReadValue("BETA_useSplitCollisionPipeline", m_BETA_useSplitCollisionPipeline);
        }

        deserializer.Skip(14);
    }
    else
    {
        deserializer.Skip(15); // offset: 849/1057 size: 15/15
    }

    // class size: 864/1072 align: 16/16
}
