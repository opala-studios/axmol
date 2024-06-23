#include "OPDownloader.hpp"

#include "../Debug/OPDebug.hpp"
#include "../Helpers/OPString.hpp"
#include "../Misc/OPCocosThread.hpp"
#include "../Misc/OPDirectory.hpp"
#include "../Misc/OPPath.hpp"
#include "../Misc/OPZip.hpp"

USING_NS_CC;

OPDownloader* OPDownloader::shared()
{
    static OPDownloader* instance = new OPDownloader();
    return instance;
}

OPDownloader::OPDownloader()
{
    network::DownloaderHints hints;
    hints.countOfMaxProcessingTasks = 6;
    hints.tempFileNameSuffix = "_downloading";
    hints.timeoutInSeconds = 5;

    _downloader = new network::Downloader(hints);

    _downloader->setOnTaskProgress([this](const network::DownloadTask& task) {

        std::string identifier = task.identifier;
        OPCocosThread::invoke("Download progress", [this, identifier, &task]() {
            auto it = _downloaders.find(identifier);
            if (it == _downloaders.end()) {
                return;
            }

            float percent = 0;

            if (task.progressInfo.totalBytesReceived > 0) {
                percent = (float)task.progressInfo.totalBytesReceived / (float)task.progressInfo.totalBytesExpected;
            }

            if (_downloaders[identifier].progressHandler && percent < 1) {
                _downloaders[identifier].progressHandler(percent);
            }
        });
    });

    _downloader->setOnFileTaskSuccess([this](const network::DownloadTask& task) {
        OP_LOG_VERB("Download success. ID: " + task.identifier + "path: " + task.storagePath);

        std::string identifier = task.identifier;
        OPCocosThread::invoke("Download success", [this, identifier, &task]() {
            auto it = _downloaders.find(identifier);
            if (it == _downloaders.end()) {
                return;
            }

            auto downloadTask = _downloaders[identifier];
            if (downloadTask.successHandler) {
                downloadTask.successHandler(task.storagePath);
            }

            finishDownload(downloadTask.identifier);
        });
    });

    _downloader->setOnTaskError([this](const network::DownloadTask& task, int code, int codeInternal, const std::string& message) {
        OP_LOG_ERROR("Download failed. ID: " + task.identifier + "Code = " + std::to_string(codeInternal) + ". Reason = " + message);

        std::string identifier = task.identifier;
        OPCocosThread::invoke("Download error", [this, identifier, code, message]() {
            auto it = _downloaders.find(identifier);
            if (it == _downloaders.end()) {
                return;
            }

            auto downloadTask = _downloaders[identifier];

            if (downloadTask.errorHandler) {
                downloadTask.errorHandler(code, message);
            }

            finishDownload(downloadTask.identifier);
        });
    });
}

std::string OPDownloader::downloadFile(const std::string& url,
                                    const std::string& filePath,
                                    const SEL_DownloadFileSuccessHandler& success,
                                    const SEL_DownloadErrorHandler& error,
                                    const SEL_DownloadProgressHandler& progress)
{
    auto directory = OPPath::removeLastComponent(filePath);
    if (!FileUtils::getInstance()->isDirectoryExist(directory)) {
        FileUtils::getInstance()->createDirectory(directory);
    }

    auto identifier = _currentId++;

    OP_LOG("Start downloading id: " + std::to_string(identifier) + " url: " + url);

    auto strIdentifier = std::to_string(identifier);
    _downloaders[strIdentifier] = OPDownloadTask{strIdentifier, _downloader->createDownloadFileTask(url, filePath,"",strIdentifier), progress, success, error};

    return strIdentifier;
}

std::string OPDownloader::downloadZipFolder(const std::string& url,
                                         const std::string& folderPath,
                                         const SEL_DownloadZipFolderSuccessHandler& success,
                                         const SEL_DownloadErrorHandler& error,
                                         const SEL_DownloadProgressHandler& progress)
{
    auto downloadPath = OPDirectory::getTemp("kit_download_" + OPString::uuid());
    return downloadFile(
        url,
        downloadPath,
        [downloadPath, folderPath, success, error](const std::string& filePath) {
            OP_LOG_VERB("Zip file downloaded at:" + filePath);

            auto unzipSuccess = OPZip::unzip(filePath, folderPath);
            FileUtils::getInstance()->removeFile(downloadPath);

            if (unzipSuccess) {
                if (success) {
                    success(folderPath);
                }
            } else {
                if (error) {
                    error(-1, "An error has occurred when unzipping file.");
                }
            }
        },
        error,
        progress);
}

void OPDownloader::cancelDownload(const std::string& identifier)
{
    OP_LOG("Cancel download with id = " + identifier);

    auto it = _downloaders.find(identifier);
    if (it == _downloaders.end()) {
        OP_LOG_WARN("Could not cancel download. It was not found.");
        return;
    }

    it->second.task->cancel();

    finishDownload(identifier);
}

void OPDownloader::finishDownload(const std::string& identifier)
{
    auto it = _downloaders.find(identifier);
    if (it == _downloaders.end()) {
        OP_LOG_WARN("Could not cancel download. It was not found.");
        return;
    }

    _downloaders.erase(it);

    OP_LOG("Download removed: it was finished or cancelled");
}
