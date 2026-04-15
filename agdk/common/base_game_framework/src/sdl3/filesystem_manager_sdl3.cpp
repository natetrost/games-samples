/*
 * Copyright 2026 Google LLC
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "filesystem_manager.h"
#include "debug_manager.h"
#include <SDL3/SDL.h>
#include <string>

namespace base_game_framework {

std::unique_ptr<FilesystemManager> FilesystemManager::instance_ = nullptr;

FilesystemManager &FilesystemManager::GetInstance() {
  if (!instance_) {
    instance_ = std::unique_ptr<FilesystemManager>(new FilesystemManager());
  }
  return *instance_;
}

void FilesystemManager::ShutdownInstance() {
  FilesystemManager::instance_.reset();
}

FilesystemManager::FilesystemManager() {
  char *pref_path = SDL_GetPrefPath("Google", "AGDKTunnel");
  if (pref_path) {
    root_path_internal_ = pref_path;
    root_path_external_ = pref_path;
    root_path_cache_ = pref_path;
    SDL_free(pref_path);
  } else {
    root_path_internal_ = "./";
    root_path_external_ = "./";
    root_path_cache_ = "./";
  }
}

bool FilesystemManager::GetExternalStorageEmulated() const {
  return false;
}

const std::string &FilesystemManager::GetRootPath(const RootPathType path_type) const {
  switch (path_type) {
    case kRootPathInternalStorage:return root_path_internal_;
    case kRootPathExternalStorage:return root_path_external_;
    case kRootPathCache:return root_path_cache_;
  }
  return root_path_internal_;
}

int64_t FilesystemManager::GetFreeSpace(const RootPathType path_type) const {
  return 1024 * 1024 * 1024; // 1 GB dummy
}

uint64_t FilesystemManager::GetPackageFileSize(const std::string &file_path) {
  const char *base_path = SDL_GetBasePath();
  std::string full_path = base_path ? base_path : "./";
  
  full_path += "assets/" + file_path;
  
  SDL_Log("GetPackageFileSize: file_path=%s, base_path=%s, full_path=%s", 
          file_path.c_str(), base_path ? base_path : "NULL", full_path.c_str());
  
  SDL_IOStream *io = SDL_IOFromFile(full_path.c_str(), "rb");
  if (io) {
    Sint64 size = SDL_GetIOSize(io);
    SDL_CloseIO(io);
    SDL_Log("GetPackageFileSize: found file, size=%lld", size);
    return static_cast<uint64_t>(size);
  }
  SDL_Log("GetPackageFileSize: failed to open file: %s", full_path.c_str());
  return 0;
}

uint64_t FilesystemManager::LoadPackageFile(const std::string file_path, const uint64_t buffer_size,
                                            void *load_buffer) {
  const char *base_path = SDL_GetBasePath();
  std::string full_path = base_path ? base_path : "./";
  
  full_path += "assets/" + file_path;
  
  SDL_IOStream *io = SDL_IOFromFile(full_path.c_str(), "rb");
  if (io) {
    size_t read = SDL_ReadIO(io, load_buffer, buffer_size);
    SDL_CloseIO(io);
    return static_cast<uint64_t>(read);
  }
  return 0;
}

} // namespace base_game_framework
