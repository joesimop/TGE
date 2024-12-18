#pragma once

#include "global.h"
#include "physicalDevice.h"


// Creates the swap chain
void create_swap_chain(VulkanCore* core);

// Populates the swap chain support details
SwapChainSupportDetails query_swap_chain_specs(VkPhysicalDevice device, VkSurfaceKHR surface);

// Chooses the swap chain surface format
// Right now: B8G8R8A8_SRGB and SRGB_NONLINEAR
VkSurfaceFormatKHR choose_swap_surface_format(VkSurfaceFormatKHR* availableFormats);

// Choose presentation mode
// Right now: defaults to VK_PRESENT_MODE_FIFO_KHR (vsync) if mailbox is not available
VkPresentModeKHR choose_swap_present_mode(VkPresentModeKHR* availablePresentModes);

// Choose the swap extent
// Important for matching resolution of the window surface to vulkans pixels
VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR* capabilities, GLFWwindow* window);

// Destroys the swap chain details
void destroy_swap_chain_details(SwapChainSupportDetails* details);

// Destroys all things associated with the swap chain
void destroy_swap_chain(VulkanCore* core);

void recreate_swap_chain(VulkanCore* core);

void cleanup_swap_chain(VulkanCore* core);
