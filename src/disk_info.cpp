#include "disk_info.h"
#include <exception>
#include <fstream>
#include <iostream>
#include "log.h"

int CommonInfo::parse(std::string &line)
{
    /*
     * remove start and end spaces
     */
    int start_index = line.find_first_not_of(" ");
    int end_index = line.find_last_not_of(" ");
    //std::cout << line << std::endl;
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
        file = val_1.substr(1, val_1.find_last_of("\"") - 1);
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
    int line_cnt = 0;
    tracks.clear();

    std::ifstream in_file;
    in_file.exceptions (std::ifstream::badbit);
    in_file.open(file_name);
    if (in_file.is_open() == false)
    {
        std::cerr << "Can not open: " << file_name << std::endl;
        return -EINVAL;
    }
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
            line_cnt++;
            try
            {
                info->parse(line);
            }
            catch (std::exception &e)
            {
                log(LEVEL_ERROR, "DiskInfo", "Failed parse line %d for %s, Reason: %s",
                        line_cnt, file_name.c_str(), e.what());
            }
        }
    }
    in_file.close();
    return process_time();
}

int DiscInfo::process_time(void)
{
    for (int i = 0; i < tracks.size(); i++)
    {
        TrackInfo *current_track = &tracks[i];
        TrackInfo *next_track = i + 1 < tracks.size() ? &tracks[i + 1] : nullptr;
        current_track->start_time =
            parse_time_index(current_track->time_index[0]);
        if (current_track->start_time < 0 || next_track == nullptr)
            continue;
        int next_start_time = parse_time_index(next_track->time_index[0]);
        if (next_start_time < 0)
            continue;
        current_track->duration = next_start_time - current_track->start_time;
    }
    return 0;
}

int DiscInfo::parse_time_index(const std::string &index)
{
    std::vector<std::string> tokens;
    std::istringstream iss(index);
    std::string subs;

    while(getline(iss, subs, ':'))
    {
        tokens.push_back(subs);
    }
    if (tokens.size() < 3)
    {
        log(LEVEL_ERROR, "Err parse time index: %s", index.c_str());
        return -EINVAL;
    }
    return stoi(tokens[0]) * 60 + stoi(tokens[1]);
}
