#pragma once

#include "animdata/AnimDataProjectListBuilder.h"

namespace nemesis
{
	namespace animdata
    {
        struct SingleFile;

        struct AnimDataSingleFileBuilder : public nemesis::StateMachine<AnimDataSingleFileBuilder,
                                                                        nemesis::animdata::ReadingState,
                                                                        void,
                                                                        const nemesis::Line&>
        {
        private:
            UPtr<nemesis::animdata::AnimDataProjectListBuilder> projectlist_buidler;
            UPtr<nemesis::animdata::SingleFile> singlefile;
            static UMap<nemesis::animdata::ReadingState, StateFunc> transition_map;

            void AddProjectLine(const nemesis::Line& line);
            void AddProjectActiveLine(const nemesis::Line& line);
            void AddBehaviorCounterLine(const nemesis::Line& line);
            void AddBehaviorLine(const nemesis::Line& line);
            void AddMovementDataActiveLine(const nemesis::Line& line);
            void AddClipNameLine(const nemesis::Line& line);
            void AddClipCodeLine(const nemesis::Line& line);
            void AddPlaybackSpeedLine(const nemesis::Line& line);
            void AddCropStartLine(const nemesis::Line& line);
            void AddCropEndLine(const nemesis::Line& line);
            void AddEventCountLine(const nemesis::Line& line);
            void AddEventLine(const nemesis::Line& line);
            void AddMovementCodeLine(const nemesis::Line& line);
            void AddDurationLine(const nemesis::Line& line);
            void AddMovementCounterLine(const nemesis::Line& line);
            void AddMovementLine(const nemesis::Line& line);
            void AddRotationCounterLine(const nemesis::Line& line);
            void AddRotationLine(const nemesis::Line& line);
            
        public:
            AnimDataSingleFileBuilder(nemesis::File& file);
            void AppendLine(const nemesis::Line& line);
        };
    }
}
