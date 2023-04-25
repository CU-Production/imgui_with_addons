#pragma once
#include "imvk_gpu.h"
#include "imvk_pipeline.h"
#include "immat.h"
#include <string>

namespace ImGui
{
class VKSHADER_API Star_vulkan
{
public:
    Star_vulkan(int gpu = 0);
    ~Star_vulkan();
    void SetParam(float _edgeStrength);

    double effect(const ImMat& src, ImMat& dst, float playTime, ImPixel& colour);

private:
    const VulkanDevice* vkdev   {nullptr};
    Option opt;
    Pipeline* pipe              {nullptr};
    VkCompute * cmd             {nullptr};

private:
    void upload_param(const VkMat& src, VkMat& dst, float playTime, ImPixel& colour);
};
} // namespace ImGui
