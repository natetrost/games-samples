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

#ifndef BASEGAMEFRAMEWORK_PLATFORM_TYPES_SDL3_H_
#define BASEGAMEFRAMEWORK_PLATFORM_TYPES_SDL3_H_

struct SDL_Window;

namespace base_game_framework {

struct PlatformEventLoopData {
  // SDL3 event loop might not need specific data held here if we use global polling,
  // but we can store the window pointer if needed.
  SDL_Window *window = nullptr;
};

struct PlatformInitParameters {
  SDL_Window *window = nullptr;
};

} // namespace base_game_framework

#endif // BASEGAMEFRAMEWORK_PLATFORM_TYPES_SDL3_H_
