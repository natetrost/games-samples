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

#include "platform_util_sdl3.h"
#include "platform_util_log.h"
#include "display_manager.h"
#include "system_event_manager.h"
#include "game_controller_manager.h"
#include "game_controller.h"
#include <cstdio>
#include <cstdarg>

namespace base_game_framework {

SDL_Window *PlatformUtilSDL3::window_ = nullptr;
std::thread::id PlatformUtilSDL3::main_thread_id_;

void PlatformUtilLog::Log(const DebugManager::DebugLogLevel log_level,
                          const char* tag,
                          const char* string) {
  printf("[%s] %s\n", tag, string);
}

void PlatformUtilLog::Log(const DebugManager::DebugLogLevel log_level,
                          const char* tag,
                          const char* format,
                          va_list specifiers) {
  printf("[%s] ", tag);
  vprintf(format, specifiers);
  printf("\n");
}

void DisplayManager::HandlePlatformDisplayChange(const DisplayChangeMessage& change_message) {
  // Stub for macOS/SDL3
}

void SystemEventManager::WriteSaveState(const SaveState &save_state) {
  // Stub for macOS/SDL3
}

GameControllerManager::GameControllerManager() {
  // Stub for macOS/SDL3
}

GameControllerManager::~GameControllerManager() {
  // Stub for macOS/SDL3
}

GameController &GameControllerManager::GetGameController(const int32_t controller_index) {
  static GameController dummy;
  return dummy;
}

} // namespace base_game_framework
