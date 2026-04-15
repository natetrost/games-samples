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

#include "platform_event_loop.h"
#include "debug_manager.h"
#include "game_controller_manager.h"
#include "user_input_manager.h"
#include "system_event_manager.h"
#include "display_manager.h"
#include "platform_util_sdl3.h"
#include "platform_system_event_dispatch.h"
#include "platform_user_input_dispatch.h"
#include "platform_keycodes.h"
#include <SDL3/SDL.h>

namespace base_game_framework {

std::unique_ptr<PlatformEventLoop> PlatformEventLoop::instance_ = nullptr;

PlatformEventLoop &PlatformEventLoop::GetInstance() {
  if (!instance_) {
    instance_ = std::unique_ptr<PlatformEventLoop>(new PlatformEventLoop());
  }
  return *instance_;
}

void PlatformEventLoop::ShutdownInstance() {
  PlatformEventLoop::instance_.reset();
}

PlatformEventLoop::PlatformEventLoop() {
  platform_data_ = std::unique_ptr<PlatformEventLoopData>(new PlatformEventLoopData());
  platform_data_->window = PlatformUtilSDL3::GetWindow();
}

PlatformEventLoop::~PlatformEventLoop() {
  platform_data_.reset();
}

void PlatformEventLoop::PollEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    switch (event.type) {
      case SDL_EVENT_QUIT:
        PlatformSystemEventDispatch::DispatchLifecycleEvent(SystemEventManager::kLifecycleQuit);
        break;
      case SDL_EVENT_WINDOW_MINIMIZED:
      case SDL_EVENT_WINDOW_FOCUS_LOST:
        PlatformSystemEventDispatch::DispatchFocusEvent(SystemEventManager::kSentToBackground);
        break;
      case SDL_EVENT_WINDOW_RESTORED:
      case SDL_EVENT_WINDOW_FOCUS_GAINED:
        PlatformSystemEventDispatch::DispatchFocusEvent(SystemEventManager::kMadeForeground);
        break;
      case SDL_EVENT_WINDOW_RESIZED:
        DisplayManager::GetInstance().HandlePlatformDisplayChange(
            DisplayManager::kDisplay_Change_Window_Resized);
        break;
      case SDL_EVENT_KEY_DOWN:
      case SDL_EVENT_KEY_UP: {
        KeyEvent key_event;
        key_event.key_action = (event.type == SDL_EVENT_KEY_DOWN) ? kKeyEvent_Down : kKeyEvent_Up;
        key_event.key_code = AKEYCODE_UNKNOWN;
        
        switch (event.key.scancode) {
          case SDL_SCANCODE_W: key_event.key_code = AKEYCODE_W; break;
          case SDL_SCANCODE_A: key_event.key_code = AKEYCODE_A; break;
          case SDL_SCANCODE_S: key_event.key_code = AKEYCODE_S; break;
          case SDL_SCANCODE_D: key_event.key_code = AKEYCODE_D; break;
          case SDL_SCANCODE_RETURN: key_event.key_code = AKEYCODE_ENTER; break;
          case SDL_SCANCODE_ESCAPE: key_event.key_code = AKEYCODE_ESCAPE; break;
          case SDL_SCANCODE_UP: key_event.key_code = AKEYCODE_W; break;
          case SDL_SCANCODE_LEFT: key_event.key_code = AKEYCODE_A; break;
          case SDL_SCANCODE_DOWN: key_event.key_code = AKEYCODE_S; break;
          case SDL_SCANCODE_RIGHT: key_event.key_code = AKEYCODE_D; break;
          default: break;
        }
        
        key_event.key_modifiers = kKeyModifier_None;
        key_event.key_repeat_count = 0;
        key_event.event_time = event.key.timestamp;
        key_event.down_time = event.key.timestamp;
        
        PlatformUserInputDispatch::DispatchKeyEvent(key_event);
        break;
      }
      case SDL_EVENT_MOUSE_BUTTON_DOWN:
      case SDL_EVENT_MOUSE_BUTTON_UP: {
        TouchEvent touch_event;
        touch_event.touch_action = (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) ? kTouch_Down : kTouch_Up;
        touch_event.touch_id = 0;
        touch_event.touch_x = static_cast<int32_t>(event.button.x);
        touch_event.touch_y = static_cast<int32_t>(event.button.y);
        PlatformUserInputDispatch::DispatchTouchEvent(touch_event);
        break;
      }
      case SDL_EVENT_MOUSE_MOTION: {
        if (event.motion.state & SDL_BUTTON_MASK(SDL_BUTTON_LEFT)) {
          TouchEvent touch_event;
          touch_event.touch_action = kTouch_Moved;
          touch_event.touch_id = 0;
          touch_event.touch_x = static_cast<int32_t>(event.motion.x);
          touch_event.touch_y = static_cast<int32_t>(event.motion.y);
          PlatformUserInputDispatch::DispatchTouchEvent(touch_event);
        }
        break;
      }
      default: break;
    }
  }
}

// Implement the dispatchers here
bool PlatformUserInputDispatch::DispatchKeyEvent(const KeyEvent &key_event) {
  return UserInputManager::GetInstance().OnKeyEvent(key_event);
}

bool PlatformUserInputDispatch::DispatchTouchEvent(const TouchEvent &touch_event) {
  return UserInputManager::GetInstance().OnTouchEvent(touch_event);
}

void PlatformSystemEventDispatch::DispatchFocusEvent(const SystemEventManager::FocusEvent focus_event) {
  SystemEventManager::GetInstance().OnFocusEvent(focus_event);
}

void PlatformSystemEventDispatch::DispatchLifecycleEvent(const SystemEventManager::LifecycleEvent lifecycle_event) {
  SystemEventManager::GetInstance().OnLifecycleEvent(lifecycle_event);
}

void PlatformSystemEventDispatch::DispatchMemoryEvent(const SystemEventManager::MemoryWarningEvent memory_warning_event) {
  SystemEventManager::GetInstance().OnMemoryEvent(memory_warning_event);
}

void PlatformSystemEventDispatch::DispatchReadSaveStateEvent(const SystemEventManager::SaveState &save_state) {
  SystemEventManager::GetInstance().OnReadSaveStateEvent(save_state);
}

} // namespace base_game_framework
