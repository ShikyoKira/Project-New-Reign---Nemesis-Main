#pragma once

#include "core/statemachine.h"
#include "core/linkedpreprocessline.h"

#include "animdata/ReadingState.h"

#include "animdata/linked/animdata-linkedproject.h"

#include "base/importer.h"

namespace nemesis
{
	namespace animdata
    {
        struct AnimDataProjectListBuilder : protected nemesis::Importer<nemesis::LinkedPreprocessLine>,
                                            public nemesis::StateMachine<AnimDataProjectListBuilder,
                                                                         nemesis::animdata::ReadingState,
                                                                         void,
                                                                         const nemesis::Line&>
                                            
        {
        private:
            enum State
            {
                None,
                Movement,
                NewProject
            };

            SPtr<nemesis::animdata::LinkedProject> project;
            static UMap<nemesis::animdata::ReadingState, StateFunc> transition_map;
            VecNstr tempdata;

            bool IsNewProject(const nemesis::Line& line);
            void AddMovementData();
            void AddNewProject();

            void AddProjectSizeLine(const nemesis::Line& line);
            void AddBehaviorCounterLine(const nemesis::Line& line);
            void AddBehaviorLine(const nemesis::Line& line);
            void AddMovementDataActiveLine(const nemesis::Line& line);
            void AddAnimationDataLine(const nemesis::Line& line);
            void AddMovementDataSizeLine(const nemesis::Line& line);
            void AddMovementDataLine(const nemesis::Line& line);

            void Build();

        protected:
            void ParseLines(const VecNstr& lines) override;

        public:
            AnimDataProjectListBuilder(nemesis::File& file,
                                       nemesis::animdata::ReadingState start_state
                                       = ReadingState::ProjectSize);

            void AppendLine(const nemesis::Line& line);
            SPtr<nemesis::animdata::LinkedProject> ToProject() noexcept;
        };
    }
}
