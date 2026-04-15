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

#include "../vulkan/platform_util_vulkan.h"
#include "debug_manager.h"
#include "platform_util_sdl3.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <vector>
#include <cstring>

namespace base_game_framework {

bool PlatformUtilVulkan::ActivateSwapchain(VkPhysicalDevice physical_device,
                                           VkDevice device,
                                           VkSwapchainKHR swapchain,
                                           VkQueue queue,
                                           uint32_t queue_family_index,
                                           DisplayManager::DisplaySwapInterval swap_interval) {
  // No Swappy on macOS.
  return true;
}

void PlatformUtilVulkan::DeactivateSwapchain(VkDevice device, VkSwapchainKHR swapchain) {
  // No Swappy on macOS.
}

VkResult PlatformUtilVulkan::PresentSwapchain(VkQueue queue, const VkPresentInfoKHR *present_info) {
  return vkQueuePresentKHR(queue, present_info);
}

VkSurfaceKHR PlatformUtilVulkan::CreateSurface(VkInstance instance) {
  VkSurfaceKHR surface = VK_NULL_HANDLE;
  SDL_Window *window = PlatformUtilSDL3::GetWindow();
  if (window == nullptr) {
    DebugManager::Log(DebugManager::kLog_Channel_Default,
                      DebugManager::kLog_Level_Error,
                      "PlatformUtilVulkan",
                      "CreateSurface failed: SDL_Window is null");
    return VK_NULL_HANDLE;
  }
  
  if (!SDL_Vulkan_CreateSurface(window, instance, nullptr, &surface)) {
    DebugManager::Log(DebugManager::kLog_Channel_Default,
                      DebugManager::kLog_Level_Error,
                      "PlatformUtilVulkan",
                      "SDL_Vulkan_CreateSurface failed: %s", SDL_GetError());
  }
  return surface;
}

void PlatformUtilVulkan::DestroySurface(VkInstance instance, VkSurfaceKHR surface) {
  if (surface != VK_NULL_HANDLE) {
    vkDestroySurfaceKHR(instance, surface, nullptr);
  }
}

void PlatformUtilVulkan::InitMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT &create_info) {
  create_info = {};
  create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
}

VkPhysicalDevice PlatformUtilVulkan::GetBestDevice(
    const std::vector<VkPhysicalDevice> &rendering_devices) {
  return rendering_devices[0];
}

void PlatformUtilVulkan::GetRefreshRates(VkPhysicalDevice physical_device,
                                         VkDevice device,
                                         VkSwapchainKHR swapchain,
                                         VkQueue queue,
                                         uint32_t queue_family_index,
                                         std::vector<DisplayManager::DisplaySwapInterval> &
                                         swap_intervals) {
  // Default to 60 FPS on desktop if we can't query it easily.
  swap_intervals.clear();
  swap_intervals.push_back(DisplayManager::kDisplay_Swap_Interval_60FPS);
}

std::vector<const char *> PlatformUtilVulkan::GetRequiredDeviceExtensions(
    VkPhysicalDevice physical_device) {
  std::vector<const char *> device_extensions;
  device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
  return device_extensions;
}

std::vector<const char *> PlatformUtilVulkan::GetRequiredInstanceExtensions(
    bool enable_validation_layers, bool use_physical2) {
  std::vector<const char *> instance_extensions;

  uint32_t count = 0;
  const char *const *sdl_extensions = SDL_Vulkan_GetInstanceExtensions(&count);
  for (uint32_t i = 0; i < count; ++i) {
    instance_extensions.push_back(sdl_extensions[i]);
  }

  if (enable_validation_layers) {
    instance_extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
  }
  
  return instance_extensions;
}

void PlatformUtilVulkan::GetScreenResolutions(const VkSurfaceCapabilitiesKHR &capabilities,
                                              std::vector<DisplayManager::DisplayResolution> &display_resolutions) {
  display_resolutions.clear();
  
  int width = 0;
  int height = 0;
  SDL_Window *window = PlatformUtilSDL3::GetWindow();
  if (window) {
    SDL_GetWindowSize(window, &width, &height);
  } else {
    width = capabilities.currentExtent.width;
    height = capabilities.currentExtent.height;
  }

  int dpi = 96; // Default DPI
  
  display_resolutions.push_back(DisplayManager::DisplayResolution(width,
                                                                  height,
                                                                  dpi,
                                                                  DisplayManager::kDisplay_Orientation_Landscape));
}

std::vector<const char *> PlatformUtilVulkan::GetValidationLayers() {
  std::vector<const char *> validation_layers;
  validation_layers.push_back("VK_LAYER_KHRONOS_validation");
  return validation_layers;
}

bool PlatformUtilVulkan::GetValidationLayersAvailable() {
  uint32_t layer_count;
  vkEnumerateInstanceLayerProperties(&layer_count, nullptr);
  std::vector<VkLayerProperties> available_layers(layer_count);
  vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

  for (const auto &layerProperties : available_layers) {
    if (strcmp("VK_LAYER_KHRONOS_validation", layerProperties.layerName) == 0) {
      return true;
    }
  }
  return false;
}

uint32_t PlatformUtilVulkan::GetVulkanApiVersion() {
  return VK_API_VERSION_1_1; // MoltenVK supports at least 1.1
}

bool PlatformUtilVulkan::HasNativeWindow() {
  return (PlatformUtilSDL3::GetWindow() != nullptr);
}

} // namespace base_game_framework
