#include <errno.h>
#include <iostream>
#include "disk_info.h"
#include <vector>

void print_cd_info(DiscInfo *disk)
{
    std::cout << "Disk Title: " << disk->title << std::endl;
    std::cout << "Disk Performer: " << disk->performer << std::endl;
    std::cout << "Num of tracks: " << disk->tracks.size() << std::endl;
    std::cout << "Track Info:" << std::endl;
    for (auto i = disk->tracks.begin(); i != disk->tracks.end(); i++)
    {
        auto track = *i;
        std::cout << "--Track: " << track.index << std::endl;
        std::cout << "\rtitle: " << track.title << std::endl;
        std::cout << "\rperformer: " << track.performer << std::endl;
        for (auto j = track.time_index.begin();
                j != track.time_index.end(); j++)
        {
            std::cout << "\rtime index: " << *j << std::endl;
        }
    }
}

//----------main process----------------
int main(int argc, char *argv[])
{
    DiscInfo diskinfo;
    if (argc < 2)
    {
        printf("Usage: %s <cue_file>\n", argv[0]);
        return -EINVAL;
    }
    diskinfo.parse_file(argv[1]);
    print_cd_info(&diskinfo);
    return 0;
}
