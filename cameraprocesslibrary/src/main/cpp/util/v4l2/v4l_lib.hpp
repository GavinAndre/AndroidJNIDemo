#include <iostream>
#include <dirent.h>
#include <regex>
#include "../android_log.h"

int getVideoId() {
    std::string dirname = "/dev/";
    std::vector<std::string> files;
    int id = 0;
    DIR *dp;
    struct dirent *dirp;
    //    cout<<dirname<<endl;
    if ((dp = opendir(dirname.c_str())) == NULL){
        LOGE("Can't open %s", dirname.c_str());
        return -1;
    }

    std::regex reg_dev("video+[0-9]\d*", std::regex::icase);
    while ((dirp = readdir(dp)) != NULL) {
        // regex_match()匹配
        if (regex_match(dirp->d_name, reg_dev)) {
            LOGI("getVideoDevice %s", dirp->d_name);
            files.push_back(
                    (std::basic_string<char, std::char_traits<char>, std::allocator<char>> &&) dirp->d_name);
        }
    }
    closedir(dp);
    std::sort(files.begin(), files.end());
    for (auto file : files) {
        LOGI("file %s", file.c_str());
    }
    auto videoName = (std::basic_string<char, std::char_traits<char>, std::allocator<char>> &&) files.back();

    LOGI("files.back() %s", videoName.c_str());

    id = atoi(videoName.substr(5, videoName.length() - 5).c_str());

    LOGI("getVideoId %d", id);
    return id;
}
