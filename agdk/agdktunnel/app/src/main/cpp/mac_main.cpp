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
#include "tunnel_engine.hpp"
#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char *argv[]) {
  if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
    std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
    return 1;
  }

  // Create a window with Vulkan support
  SDL_Window *window = SDL_CreateWindow("AGDKTunnel", 1280, 720, SDL_WINDOW_VULKAN);
  if (!window) {
    std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
    SDL_Quit();
    return 1;
  }

  base_game_framework::PlatformInitParameters init_params{window};
  base_game_framework::BaseGameFramework_Init(init_params);

  // Note: TunnelEngine currently expects an android_app pointer.
  // We pass nullptr for now and will need to modify TunnelEngine/NativeEngine
  // to handle this on non-Android platforms.
  TunnelEngine *engine = new TunnelEngine(nullptr);
  
  engine->GameLoop();
  
  delete engine;

  base_game_framework::BaseGameFramework_Destroy();
  
  SDL_DestroyWindow(window);
  SDL_Quit();
  
  return 0;
}
