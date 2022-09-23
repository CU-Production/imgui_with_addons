#pragma once
#include "imvk_gpu.h"
#include "imvk_pipeline.h"
#include "immat.h"

namespace ImGui 
{
class VKSHADER_API Bilateral_vulkan
{
public:
    Bilateral_vulkan(int gpu = -1);
    ~Bilateral_vulkan();

    double filter(const ImMat& src, ImMat& dst, int ksz, float sigma_spatial, float sigma_color);

public:
    const VulkanDevice* vkdev {nullptr};
    Pipeline * pipe           {nullptr};
    VkCompute * cmd           {nullptr};
    Option opt;

private:
    void upload_param(const VkMat& src, VkMat& dst, int ksz, float sigma_spatial, float sigma_color);
};
} // namespace ImGui 