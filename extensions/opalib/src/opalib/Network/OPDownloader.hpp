#ifndef OPDownloader_hpp
#define OPDownloader_hpp

#include <network/CCDownloader.h>

#include <cstring>
#include <iostream>
#include <map>

class OPDownloader
{
public:
    typedef std::function<void(int code, const std::string& message)> SEL_DownloadErrorHandler;
    typedef std::function<void(float percent)> SEL_DownloadProgressHandler;
    typedef std::function<void(const std::string& filePath)> SEL_DownloadFileSuccessHandler;
    typedef std::function<void(const std::string& directory)> SEL_DownloadZipFolderSuccessHandler;

    struct OPDownloadTask {
        std::string identifier;
        std::shared_ptr<cocos2d::network::DownloadTask> task;
        SEL_DownloadProgressHandler progressHandler;
        SEL_DownloadFileSuccessHandler successHandler;
        SEL_DownloadErrorHandler errorHandler;
    };

public:
    OPDownloader();

    static OPDownloader* shared();

    void cancelDownload(const std::string& identifier);

    std::string downloadFile(const std::string& url,
                          const std::string& filePath,
                          const SEL_DownloadFileSuccessHandler& success,
                          const SEL_DownloadErrorHandler& error = nullptr,
                          const SEL_DownloadProgressHandler& progress = nullptr);

    std::string downloadZipFolder(const std::string& url,
                               const std::string& filePath,
                               const SEL_DownloadZipFolderSuccessHandler& success,
                               const SEL_DownloadErrorHandler& error = nullptr,
                               const SEL_DownloadProgressHandler& progress = nullptr);

private:
    std::map<std::string, OPDownloadTask> _downloaders;

    void finishDownload(const std::string& identifier);

    uint32_t _currentId = 1;

    cocos2d::network::Downloader* _downloader;
};

#endif /* OPDownloader_hpp */
