#pragma once

namespace nemesis
{
	namespace animdata
    {
        enum ReadingState
        {
            None,
            Project,
            ProjectActive,
            ProjectSize,
            BehaviorCounter,
            Behavior,
            MovementDataActive,
            ClipName,
            ClipCode,
            PlaybackSpeed,
            CropStart,
            CropEnd,
            EventCounter,
            EventName,
            MovementCode,
            Duration,
            AnimationData,
            MovementDataSize,
            MovementData,
            RotationDataSize,
            RotationData
        };
    }
}
