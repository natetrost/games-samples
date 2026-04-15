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

#include "basegameframework_init.h"
#include "debug_manager.h"
#include "display_manager.h"
#include "filesystem_manager.h"
#include "platform_event_loop.h"
#include "platform_util_sdl3.h"
#include "system_event_manager.h"
#include "user_input_manager.h"
#include <thread>

namespace base_game_framework {

void BaseGameFramework_Init(const PlatformInitParameters &init_params) {
  PlatformUtilSDL3::SetMainThreadID(std::this_thread::get_id());
  PlatformUtilSDL3::SetWindow(init_params.window);

  DebugManager::GetInstance();
  FilesystemManager::GetInstance();
  PlatformEventLoop::GetInstance();
  SystemEventManager::GetInstance();
  UserInputManager::GetInstance();
  DisplayManager::GetInstance();
}

void BaseGameFramework_Destroy() {
  DisplayManager::ShutdownInstance();
  UserInputManager::ShutdownInstance();
  SystemEventManager::ShutdownInstance();
  PlatformEventLoop::ShutdownInstance();
  FilesystemManager::ShutdownInstance();
  DebugManager::ShutdownInstance();
}

} // namespace base_game_framework
