#ifndef __APPLE__
#ifndef __GPU_INFO_VULKAN_H__
#define __GPU_INFO_VULKAN_H__

#include "gpu_info.h"

#ifdef __linux__
/* Prefer system capabilities header when available; otherwise provide
 * minimal fallbacks so the code compiles on systems without libcap-dev. */
#if defined(__has_include)
#  if __has_include(<sys/capability.h>)
#    include <sys/capability.h>
#  else
     typedef void* cap_t;
     typedef int cap_value_t;
     typedef enum { CAP_EFFECTIVE = 0 } cap_flag_t;
     typedef enum { CAP_CLEAR = 0, CAP_SET = 1 } cap_flag_value_t;
     #ifndef CAP_PERFMON
     #define CAP_PERFMON 38
     #endif
#  endif
#else
  /* Fallback if __has_include is unavailable */
  typedef void* cap_t;
  typedef int cap_value_t;
  typedef enum { CAP_EFFECTIVE = 0 } cap_flag_t;
  typedef enum { CAP_CLEAR = 0, CAP_SET = 1 } cap_flag_value_t;
  #ifndef CAP_PERFMON
  #define CAP_PERFMON 38
  #endif
#endif
#endif

/* Detect Vulkan headers availability */
#if defined(__has_include)
#  if __has_include(<vulkan/vulkan.h>)
#    include <vulkan/vulkan.h>
#    define VK_HEADERS_AVAILABLE 1
#  else
#    define VK_HEADERS_AVAILABLE 0
#  endif
#else
  /* Assume available on toolchains without __has_include */
#  define VK_HEADERS_AVAILABLE 1
#  include <vulkan/vulkan.h>
#endif

typedef struct {
  void* vk_handle;
  void* cap_handle;
  uint16_t verbose;

  /* VkInstance type only when headers are present */
#if VK_HEADERS_AVAILABLE
  VkInstance vk;
#else
  void* vk;
#endif
  int num_devices;

#ifdef __linux__
  cap_t (*cap_get_proc)(void);

  int (*cap_get_bound)(cap_value_t);
  int (*cap_set_flag)(cap_t, cap_flag_t, int, const cap_value_t *, cap_flag_value_t);
  int (*cap_set_proc)(cap_t);
  int (*cap_free)(cap_t);
#endif

#if VK_HEADERS_AVAILABLE
  void (*vkGetPhysicalDeviceProperties)(
    VkPhysicalDevice                            physicalDevice,
    VkPhysicalDeviceProperties*                 pProperties);
  VkResult (*vkEnumerateDeviceExtensionProperties)(
      VkPhysicalDevice                            physicalDevice,
      const char*                                 pLayerName,
      uint32_t*                                   pPropertyCount,
      VkExtensionProperties*                      pProperties);
  VkResult (*vkCreateInstance)(
      const VkInstanceCreateInfo*                 pCreateInfo,
      const VkAllocationCallbacks*                pAllocator,
      VkInstance*                                 pInstance);
  VkResult (*vkEnumeratePhysicalDevices)(
      VkInstance                                  instance,
      uint32_t*                                   pPhysicalDeviceCount,
      VkPhysicalDevice*                           pPhysicalDevices);
  void (*vkGetPhysicalDeviceMemoryProperties2)(
      VkPhysicalDevice                            physicalDevice,
      VkPhysicalDeviceMemoryProperties2*          pMemoryProperties);
  void (*vkDestroyInstance)(
      VkInstance                                  instance,
      const VkAllocationCallbacks*                pAllocator);
#else
  void (*vkGetPhysicalDeviceProperties)(void*, void*);
  void* (*vkEnumerateDeviceExtensionProperties)(void*, const char*, uint32_t*, void*);
  void* (*vkCreateInstance)(const void*, const void*, void*);
  void* (*vkEnumeratePhysicalDevices)(void*, uint32_t*, void*);
  void (*vkGetPhysicalDeviceMemoryProperties2)(void*, void*);
  void (*vkDestroyInstance)(void*, const void*);
#endif
} vk_handle_t;

typedef struct vk_init_resp
{
  char *err; // If err is non-null handle is invalid
  int num_devices;
  vk_handle_t ch;
} vk_init_resp_t;

void vk_init(char* vk_lib_path, char* cap_lib_path, vk_init_resp_t *resp);
void vk_check_vram(vk_handle_t rh, int i, mem_info_t *resp);
int vk_check_flash_attention(vk_handle_t rh, int i);
void vk_release(vk_handle_t rh);

#endif
#endif
