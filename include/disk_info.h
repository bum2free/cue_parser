#ifndef __DISK_INFO_H__
#define __DISK_INFO_H__
#include <string>
#include <sstream>
#include <vector>

class TrackInfo;
class CommonInfo
{
public:
    virtual ~CommonInfo() = default;
    int parse(std::string &line);

    std::string title;
    std::string performer;
    std::string file;
protected:
    std::vector<TrackInfo> *p_tracks;
private:
    void string_split(std::vector<std::string> &subs, std::string &str,
            char splitter);
};

class TrackInfo : public CommonInfo
{
public:
    TrackInfo(std::string &file, std::string &index,
            std::vector<TrackInfo> *p_tracks);
    std::string index;
    std::vector<std::string> time_index;
    int start_time = -1; //in second
    int duration = 0;
};

class DiscInfo : public CommonInfo
{
public:
    DiscInfo()
    {
        p_tracks = &tracks;
    }
    int parse_file(const std::string &file_name);
    std::vector<TrackInfo> tracks;
private:
    int process_time(void);
    int parse_time_index(const std::string &index);
};
#endif
