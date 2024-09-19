#include "swapChain.h"


void create_swap_chain(VkPhysicalDevice physicalDevice, VkDevice logicalDevice, VkSurfaceKHR surface, GLFWwindow* window, VkSwapchainKHR* swapChain) {
    SwapChainSupportDetails details = query_swap_chain_specs(physicalDevice, surface);

    VkSurfaceFormatKHR surfaceFormat = choose_swap_surface_format(details.formats);
    VkPresentModeKHR presentMode = choose_swap_present_mode(details.presentModes);
    VkExtent2D extent = choose_swap_extent(&details.capabilities, window);

    u32 imageCount = details.capabilities.minImageCount + 1;
    if (details.capabilities.maxImageCount > 0 && imageCount > details.capabilities.maxImageCount) {
        imageCount = details.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = surface,
        .minImageCount = imageCount,
        .imageFormat = surfaceFormat.format,
        .imageColorSpace = surfaceFormat.colorSpace,
        .imageExtent = extent,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,      //VK_IMAGE_USAGE_TRANSFER_DST_BIT for post-processing
        .imageSharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .preTransform = details.capabilities.currentTransform,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .presentMode = presentMode,
        .clipped = VK_TRUE,
        .oldSwapchain = VK_NULL_HANDLE
    };

    QueueFamilyIndices indices = __find_queue_families(physicalDevice, surface);
    u32 queueFamilyIndices[] = {indices.graphicsFamily.value, indices.presentFamily.value};

    //If we have two different queue families, we use concurrent sharing mode
    //This avoids us from having to do ownership transfers
    if (indices.graphicsFamily.value != indices.presentFamily.value) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else {
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0;
        createInfo.pQueueFamilyIndices = NULL;
    }

    ASSERT(vkCreateSwapchainKHR(logicalDevice, &createInfo, NULL, swapChain) == VK_SUCCESS, "Failed to create swap chain");

}

SwapChainSupportDetails query_swap_chain_specs(VkPhysicalDevice device, VkSurfaceKHR surface) {
    SwapChainSupportDetails details;
    details.capabilities = (VkSurfaceCapabilitiesKHR){0};

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);

    u32 formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, NULL);

    if (formatCount != 0) {
        da_init(details.formats, formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats);
        da_set_size(details.formats, formatCount);
    }

    u32 presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, NULL);

    if (presentModeCount != 0) {
        da_init(details.presentModes, presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes);
        da_set_size(details.presentModes, presentModeCount);
    }
    return details;
}

VkSurfaceFormatKHR choose_swap_surface_format(VkSurfaceFormatKHR* availableFormats){
    for (int i = 0; i < da_size(availableFormats); i++) {
        if (availableFormats[i].format == VK_FORMAT_B8G8R8A8_SRGB && availableFormats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
            return availableFormats[i];
        }
    }
    return availableFormats[0];
}

VkPresentModeKHR choose_swap_present_mode(VkPresentModeKHR* availablePresentModes) {
    for (int i = 0; i < da_size(availablePresentModes); i++) {
        if (availablePresentModes[i] == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentModes[i];
        }
    }
    return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D choose_swap_extent(const VkSurfaceCapabilitiesKHR* capabilities, GLFWwindow* window) {
    if (capabilities->currentExtent.width != UINT32_MAX){
        return capabilities->currentExtent;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    VkExtent2D actualExtent = {
        .width = (u32)width,
        .height = (u32)height
    };

    actualExtent.width = CLAMP(actualExtent.width, capabilities->minImageExtent.width, capabilities->maxImageExtent.width);
    actualExtent.height = CLAMP(actualExtent.height, capabilities->minImageExtent.height, capabilities->maxImageExtent.height);

    return actualExtent;
}

void destroy_swap_chain_details(SwapChainSupportDetails* details) {
    da_free(details->formats);
    da_free(details->presentModes);
}