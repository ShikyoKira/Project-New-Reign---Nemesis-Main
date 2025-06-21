#pragma once

namespace nemesis
{
    struct Fixup
    {
        unsigned int Source;
        unsigned int Destination;
    };

    struct LocalFixup : Fixup
    {
        LocalFixup() {}
        LocalFixup(unsigned int source, unsigned int destination)
        {
            Source      = source;
            Destination = destination;
        }
    };

    struct GlobalFixup : Fixup
    {
        unsigned int DestinationSectionIndex;

        GlobalFixup() {}
        GlobalFixup(unsigned int source, unsigned int index, unsigned int destination)
        {
            Source                  = source;
            DestinationSectionIndex = index;
            Destination             = destination;
        }
    };

    struct VirtualFixup : Fixup
    {
        unsigned int DestinationSectionIndex;

        VirtualFixup() {}
        VirtualFixup(unsigned int source, unsigned int index, unsigned int destination)
        {
            Source                  = source;
            DestinationSectionIndex = index;
            Destination             = destination;
        }
    };
}