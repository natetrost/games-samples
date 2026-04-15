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

#ifndef BASEGAMEFRAMEWORK_PLATFORM_UTIL_SDL3_H_
#define BASEGAMEFRAMEWORK_PLATFORM_UTIL_SDL3_H_

#include <thread>

struct SDL_Window;

namespace base_game_framework {

class PlatformUtilSDL3 {
 public:
  static void SetWindow(SDL_Window *window) { window_ = window; }
  static SDL_Window *GetWindow() { return window_; }
  
  static void SetMainThreadID(std::thread::id id) { main_thread_id_ = id; }
  static std::thread::id GetMainThreadID() { return main_thread_id_; }

 private:
  static SDL_Window *window_;
  static std::thread::id main_thread_id_;
};

} // namespace base_game_framework

#endif // BASEGAMEFRAMEWORK_PLATFORM_UTIL_SDL3_H_
