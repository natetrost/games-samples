#include "game_asset_manager.hpp"
#include "filesystem_manager.h"
#include "debug_manager.h"

using namespace base_game_framework;

GameAssetManager::GameAssetManager() : mInternals(nullptr) {
    // Headless implementation, no internals needed.
}

#if !defined(BGF_SDL3)
GameAssetManager::GameAssetManager(AAssetManager *assetManager, JavaVM *jvm, jobject android_context) : mInternals(nullptr) {
    // Should not be called on macOS
}
#endif

GameAssetManager::~GameAssetManager() {
}

void GameAssetManager::OnPause() {}
void GameAssetManager::OnResume() {}
void GameAssetManager::UpdateGameAssetManager() {}

const char *GameAssetManager::GetGameAssetErrorMessage() {
    return nullptr;
}

const char *GameAssetManager::GetGameAssetParentPackName(const char *assetName) {
    return INSTALL_ASSETPACK_NAME; // Assume everything is in install pack
}

uint64_t GameAssetManager::GetGameAssetSize(const char *assetName) {
    return FilesystemManager::GetInstance().GetPackageFileSize(assetName);
}

bool GameAssetManager::LoadGameAsset(const char *assetName, const size_t bufferSize, void *loadBuffer) {
    return FilesystemManager::GetInstance().LoadPackageFile(assetName, bufferSize, loadBuffer) > 0;
}

bool GameAssetManager::LoadGameAssetAsync(const char *assetName, const size_t bufferSize, void *loadBuffer,
                        LoadingCompleteCallback callback, void* userData) {
    // Sync load for simplicity on macOS for now
    bool success = LoadGameAsset(assetName, bufferSize, loadBuffer);
    if (callback) {
        LoadingCompleteMessage message;
        message.assetName = assetName;
        message.bytesRead = success ? bufferSize : 0;
        message.loadBuffer = loadBuffer;
        message.loadSuccessful = success;
        message.userData = userData;
        callback(&message);
    }
    return success;
}

const char **GameAssetManager::GetGameAssetPackFileList(const char *assetPackName, int *fileListSize) {
    if (fileListSize) *fileListSize = 0;
    return nullptr;
}

GameAssetManager::GameAssetStatus GameAssetManager::GetGameAssetPackStatus(const char *assetPackName) {
    return GAMEASSET_READY;
}

GameAssetManager::GameAssetPackType GameAssetManager::GetGameAssetPackType(const char *assetPackName) {
    return GAMEASSET_PACKTYPE_INTERNAL;
}

void GameAssetManager::RequestMobileDataDownloads() {}

bool GameAssetManager::RequestDownload(const char *assetPackName) {
    return true;
}

void GameAssetManager::RequestDownloadCancellation(const char *assetPackName) {}

bool GameAssetManager::RequestRemoval(const char *assetPackName) {
    return true;
}

GameAssetManager::GameAssetStatus GameAssetManager::GetDownloadStatus(const char *assetPackName,
                                  float *completionProgress, uint64_t *totalPackSize) {
    if (completionProgress) *completionProgress = 1.0f;
    if (totalPackSize) *totalPackSize = 0;
    return GAMEASSET_READY;
}
