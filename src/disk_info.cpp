#include "disk_info.h"
#include <fstream>
#include <iostream>

int CommonInfo::parse(std::string &line)
{
    /*
     * remove start and end spaces
     */
    int start_index = line.find_first_not_of(" ");
    int end_index = line.find_last_not_of(" ");
    std::string line_2 = line.substr(start_index, end_index - start_index + 1);
    //std::cout << line_2 << std::endl;

    /*
     * extract key
     */
    end_index = line_2.find_first_of(" ");
    std::string key = line_2.substr(0, end_index);
    start_index = line_2.find_first_not_of(" ", end_index);
    std::string val_1 = line_2.substr(start_index, line_2.size() - start_index); 
    //std::cout << key << ": " << val_1 << std::endl;

    /*
     * Process value:
     *
     * assume there is "" for some keys
     */
    if (key == "PERFORMER")
    {
        performer = val_1.substr(1, val_1.size() - 2);
    }
    else if (key == "TITLE")
    {
        title = val_1.substr(1, val_1.size() - 2);
    }
    else if (key == "FILE")
    {
        file = val_1.substr(1, val_1.size() - 2);
    }
    else if (key == "TRACK")
    {
        std::vector<std::string> tokens;
        string_split(tokens, val_1, ' ');
        p_tracks->push_back(TrackInfo(file, tokens[0], p_tracks));
    }
    else if (key == "INDEX")
    {
        std::vector<std::string> tokens;
        string_split(tokens, val_1, ' ');
        TrackInfo *track_info = dynamic_cast<TrackInfo *>(this);
        track_info->time_index.push_back(tokens[1]);
    }
    return 0;
}

void CommonInfo::string_split(std::vector<std::string> &subs, std::string &str,
        char splitter)
{
    std::istringstream s_line(str);
    for(std::string each; std::getline(s_line, each, splitter);
            subs.push_back(each));
}

TrackInfo::TrackInfo(std::string &file, std::string &index,
        std::vector<TrackInfo> *p_tracks) : index(index)
{
    CommonInfo::file = file;
    this->p_tracks = p_tracks;
}

int DiscInfo::parse_file(const std::string &file_name)
{
    tracks.clear();

    std::ifstream in_file;
    in_file.exceptions (std::ifstream::badbit);
    in_file.open(file_name);
    while(!in_file.eof())
    {
        std::string line;
        CommonInfo *info = nullptr;
        if (tracks.size() == 0)
        {
            info = this;
        }
        else
        {
            info = &tracks[tracks.size() - 1];
        }
        if (std::getline(in_file, line))
        {
            //std::cout << line << std::endl;
            info->parse(line);
        }
    }
    in_file.close();

}
