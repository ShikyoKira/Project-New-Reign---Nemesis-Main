#pragma once

#include "base/importer.h"

#include "animdata/linked/animdata-linkedproject.h"

#include "animdata/animdatasinglefilebuilder.h"

namespace nemesis
{
	namespace animdata
    {
        struct SingleFile;

        struct AnimDataProjectFactory : nemesis::Importer<nemesis::animdata::LinkedProject>
        {
        private:
            nemesis::File nfile;
            VecNstr templines;

        public:
            AnimDataProjectFactory(Vec<SPtr<nemesis::animdata::LinkedProject>>& list);

            void AddLine(const nemesis::Line& line) override;
            void ParseLines(const VecNstr& lines) override;

            //UPtr<nemesis::animdata::SingleFile> GenerateSingleFile(const nemesis::File& file);
        };
    }
}
