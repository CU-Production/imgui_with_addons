#pragma once
#include <vulkan/vulkan.h>

// This header contains new structure and function declearation to fix build with old vulkan sdk

#if VK_HEADER_VERSION < 70
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SUBGROUP_PROPERTIES (VkStructureType)1000094000
typedef enum VkSubgroupFeatureFlagBits
{
    VK_SUBGROUP_FEATURE_BASIC_BIT = 0x00000001,
    VK_SUBGROUP_FEATURE_VOTE_BIT = 0x00000002,
    VK_SUBGROUP_FEATURE_ARITHMETIC_BIT = 0x00000004,
    VK_SUBGROUP_FEATURE_BALLOT_BIT = 0x00000008,
    VK_SUBGROUP_FEATURE_SHUFFLE_BIT = 0x00000010,
    VK_SUBGROUP_FEATURE_SHUFFLE_RELATIVE_BIT = 0x00000020,
    VK_SUBGROUP_FEATURE_CLUSTERED_BIT = 0x00000040,
    VK_SUBGROUP_FEATURE_QUAD_BIT = 0x00000080,
    VK_SUBGROUP_FEATURE_PARTITIONED_BIT_NV = 0x00000100,
    VK_SUBGROUP_FEATURE_FLAG_BITS_MAX_ENUM = 0x7FFFFFFF
} VkSubgroupFeatureFlagBits;
typedef VkFlags VkSubgroupFeatureFlags;
typedef struct VkPhysicalDeviceSubgroupProperties
{
    VkStructureType sType;
    void* pNext;
    uint32_t subgroupSize;
    VkShaderStageFlags supportedStages;
    VkSubgroupFeatureFlags supportedOperations;
    VkBool32 quadOperationsInAllStages;
} VkPhysicalDeviceSubgroupProperties;
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MAINTENANCE_3_PROPERTIES (VkStructureType)1000168000
#define VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_SUPPORT            (VkStructureType)1000168001
typedef struct VkPhysicalDeviceMaintenance3Properties
{
    VkStructureType sType;
    void* pNext;
    uint32_t maxPerSetDescriptors;
    VkDeviceSize maxMemoryAllocationSize;
} VkPhysicalDeviceMaintenance3Properties;
typedef struct VkDescriptorSetLayoutSupport
{
    VkStructureType sType;
    void* pNext;
    VkBool32 supported;
} VkDescriptorSetLayoutSupport;
typedef VkPhysicalDeviceMaintenance3Properties VkPhysicalDeviceMaintenance3PropertiesKHR;
typedef VkDescriptorSetLayoutSupport VkDescriptorSetLayoutSupportKHR;
typedef void(VKAPI_PTR* PFN_vkGetDescriptorSetLayoutSupportKHR)(VkDevice device, const VkDescriptorSetLayoutCreateInfo* pCreateInfo, VkDescriptorSetLayoutSupport* pSupport);
#endif // VK_HEADER_VERSION < 70

#if VK_HEADER_VERSION < 80
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_8BIT_STORAGE_FEATURES_KHR (VkStructureType)1000177000
typedef struct VkPhysicalDevice8BitStorageFeaturesKHR
{
    VkStructureType sType;
    void* pNext;
    VkBool32 storageBuffer8BitAccess;
    VkBool32 uniformAndStorageBuffer8BitAccess;
    VkBool32 storagePushConstant8;
} VkPhysicalDevice8BitStorageFeaturesKHR;
#define VK_STRUCTURE_TYPE_ATTACHMENT_DESCRIPTION_2_KHR  (VkStructureType)1000109000
#define VK_STRUCTURE_TYPE_ATTACHMENT_REFERENCE_2_KHR    (VkStructureType)1000109001
#define VK_STRUCTURE_TYPE_SUBPASS_DESCRIPTION_2_KHR     (VkStructureType)1000109002
#define VK_STRUCTURE_TYPE_SUBPASS_DEPENDENCY_2_KHR      (VkStructureType)1000109003
#define VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO_2_KHR (VkStructureType)1000109004
#define VK_STRUCTURE_TYPE_SUBPASS_BEGIN_INFO_KHR        (VkStructureType)1000109005
#define VK_STRUCTURE_TYPE_SUBPASS_END_INFO_KHR          (VkStructureType)1000109006
typedef struct VkAttachmentDescription2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkAttachmentDescriptionFlags flags;
    VkFormat format;
    VkSampleCountFlagBits samples;
    VkAttachmentLoadOp loadOp;
    VkAttachmentStoreOp storeOp;
    VkAttachmentLoadOp stencilLoadOp;
    VkAttachmentStoreOp stencilStoreOp;
    VkImageLayout initialLayout;
    VkImageLayout finalLayout;
} VkAttachmentDescription2KHR;
typedef struct VkAttachmentReference2KHR
{
    VkStructureType sType;
    const void* pNext;
    uint32_t attachment;
    VkImageLayout layout;
    VkImageAspectFlags aspectMask;
} VkAttachmentReference2KHR;
typedef struct VkSubpassDescription2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkSubpassDescriptionFlags flags;
    VkPipelineBindPoint pipelineBindPoint;
    uint32_t viewMask;
    uint32_t inputAttachmentCount;
    const VkAttachmentReference2KHR* pInputAttachments;
    uint32_t colorAttachmentCount;
    const VkAttachmentReference2KHR* pColorAttachments;
    const VkAttachmentReference2KHR* pResolveAttachments;
    const VkAttachmentReference2KHR* pDepthStencilAttachment;
    uint32_t preserveAttachmentCount;
    const uint32_t* pPreserveAttachments;
} VkSubpassDescription2KHR;
typedef struct VkSubpassDependency2KHR
{
    VkStructureType sType;
    const void* pNext;
    uint32_t srcSubpass;
    uint32_t dstSubpass;
    VkPipelineStageFlags srcStageMask;
    VkPipelineStageFlags dstStageMask;
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkDependencyFlags dependencyFlags;
    int32_t viewOffset;
} VkSubpassDependency2KHR;
typedef struct VkRenderPassCreateInfo2KHR
{
    VkStructureType sType;
    const void* pNext;
    VkRenderPassCreateFlags flags;
    uint32_t attachmentCount;
    const VkAttachmentDescription2KHR* pAttachments;
    uint32_t subpassCount;
    const VkSubpassDescription2KHR* pSubpasses;
    uint32_t dependencyCount;
    const VkSubpassDependency2KHR* pDependencies;
    uint32_t correlatedViewMaskCount;
    const uint32_t* pCorrelatedViewMasks;
} VkRenderPassCreateInfo2KHR;
typedef struct VkSubpassBeginInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    VkSubpassContents contents;
} VkSubpassBeginInfoKHR;

typedef struct VkSubpassEndInfoKHR
{
    VkStructureType sType;
    const void* pNext;
} VkSubpassEndInfoKHR;
typedef VkResult(VKAPI_PTR* PFN_vkCreateRenderPass2KHR)(VkDevice device, const VkRenderPassCreateInfo2KHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkRenderPass* pRenderPass);
typedef void(VKAPI_PTR* PFN_vkCmdBeginRenderPass2KHR)(VkCommandBuffer commandBuffer, const VkRenderPassBeginInfo* pRenderPassBegin, const VkSubpassBeginInfoKHR* pSubpassBeginInfo);
typedef void(VKAPI_PTR* PFN_vkCmdNextSubpass2KHR)(VkCommandBuffer commandBuffer, const VkSubpassBeginInfoKHR* pSubpassBeginInfo, const VkSubpassEndInfoKHR* pSubpassEndInfo);
typedef void(VKAPI_PTR* PFN_vkCmdEndRenderPass2KHR)(VkCommandBuffer commandBuffer, const VkSubpassEndInfoKHR* pSubpassEndInfo);
#endif // VK_HEADER_VERSION < 80

#if VK_HEADER_VERSION < 95
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FLOAT16_INT8_FEATURES_KHR (VkStructureType)1000082000
typedef struct VkPhysicalDeviceFloat16Int8FeaturesKHR
{
    VkStructureType sType;
    void* pNext;
    VkBool32 shaderFloat16;
    VkBool32 shaderInt8;
} VkPhysicalDeviceFloat16Int8FeaturesKHR;
#endif // VK_HEADER_VERSION < 95

#if VK_HEADER_VERSION < 97
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_BUDGET_PROPERTIES_EXT (VkStructureType)1000237000
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_MEMORY_PRIORITY_FEATURES_EXT (VkStructureType)1000238000
#define VK_STRUCTURE_TYPE_MEMORY_PRIORITY_ALLOCATE_INFO_EXT            (VkStructureType)1000238001
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_ADDRESS_FEATURES_EXT  (VkStructureType)1000244000
#define VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_EXT               (VkStructureType)1000244001
#define VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_CREATE_INFO_EXT        (VkStructureType)1000244002
#define VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT                      (VkStructureType)1000247000
#define VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_EXT         (VkBufferCreateFlagBits)0x00020000
#define VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT                  (VkBufferUsageFlagBits)0x00020000
typedef uint64_t VkDeviceAddress;
typedef struct VkPhysicalDeviceMemoryBudgetPropertiesEXT
{
    VkStructureType sType;
    void* pNext;
    VkDeviceSize heapBudget[VK_MAX_MEMORY_HEAPS];
    VkDeviceSize heapUsage[VK_MAX_MEMORY_HEAPS];
} VkPhysicalDeviceMemoryBudgetPropertiesEXT;
typedef struct VkPhysicalDeviceMemoryPriorityFeaturesEXT
{
    VkStructureType sType;
    void* pNext;
    VkBool32 memoryPriority;
} VkPhysicalDeviceMemoryPriorityFeaturesEXT;
typedef struct VkMemoryPriorityAllocateInfoEXT
{
    VkStructureType sType;
    const void* pNext;
    float priority;
} VkMemoryPriorityAllocateInfoEXT;
typedef struct VkPhysicalDeviceBufferAddressFeaturesEXT
{
    VkStructureType sType;
    void* pNext;
    VkBool32 bufferDeviceAddress;
    VkBool32 bufferDeviceAddressCaptureReplay;
    VkBool32 bufferDeviceAddressMultiDevice;
} VkPhysicalDeviceBufferAddressFeaturesEXT;
typedef struct VkBufferDeviceAddressInfoEXT
{
    VkStructureType sType;
    const void* pNext;
    VkBuffer buffer;
} VkBufferDeviceAddressInfoEXT;
typedef struct VkBufferDeviceAddressCreateInfoEXT
{
    VkStructureType sType;
    const void* pNext;
    VkDeviceSize deviceAddress;
} VkBufferDeviceAddressCreateInfoEXT;
typedef VkDeviceAddress(VKAPI_PTR* PFN_vkGetBufferDeviceAddressEXT)(VkDevice device, const VkBufferDeviceAddressInfoEXT* pInfo);
typedef enum VkValidationFeatureEnableEXT
{
    VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT = 0,
    VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT = 1,
    VK_VALIDATION_FEATURE_ENABLE_BEGIN_RANGE_EXT = VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
    VK_VALIDATION_FEATURE_ENABLE_END_RANGE_EXT = VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT,
    VK_VALIDATION_FEATURE_ENABLE_RANGE_SIZE_EXT = (VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_RESERVE_BINDING_SLOT_EXT - VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT + 1),
    VK_VALIDATION_FEATURE_ENABLE_MAX_ENUM_EXT = 0x7FFFFFFF
} VkValidationFeatureEnableEXT;
typedef enum VkValidationFeatureDisableEXT
{
    VK_VALIDATION_FEATURE_DISABLE_ALL_EXT = 0,
    VK_VALIDATION_FEATURE_DISABLE_SHADERS_EXT = 1,
    VK_VALIDATION_FEATURE_DISABLE_THREAD_SAFETY_EXT = 2,
    VK_VALIDATION_FEATURE_DISABLE_API_PARAMETERS_EXT = 3,
    VK_VALIDATION_FEATURE_DISABLE_OBJECT_LIFETIMES_EXT = 4,
    VK_VALIDATION_FEATURE_DISABLE_CORE_CHECKS_EXT = 5,
    VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT = 6,
    VK_VALIDATION_FEATURE_DISABLE_BEGIN_RANGE_EXT = VK_VALIDATION_FEATURE_DISABLE_ALL_EXT,
    VK_VALIDATION_FEATURE_DISABLE_END_RANGE_EXT = VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT,
    VK_VALIDATION_FEATURE_DISABLE_RANGE_SIZE_EXT = (VK_VALIDATION_FEATURE_DISABLE_UNIQUE_HANDLES_EXT - VK_VALIDATION_FEATURE_DISABLE_ALL_EXT + 1),
    VK_VALIDATION_FEATURE_DISABLE_MAX_ENUM_EXT = 0x7FFFFFFF
} VkValidationFeatureDisableEXT;
typedef struct VkValidationFeaturesEXT
{
    VkStructureType sType;
    const void* pNext;
    uint32_t enabledValidationFeatureCount;
    const VkValidationFeatureEnableEXT* pEnabledValidationFeatures;
    uint32_t disabledValidationFeatureCount;
    const VkValidationFeatureDisableEXT* pDisabledValidationFeatures;
} VkValidationFeaturesEXT;
#endif // VK_HEADER_VERSION < 97

#if VK_HEADER_VERSION < 101
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_FEATURES_NV   (VkStructureType)1000249000
#define VK_STRUCTURE_TYPE_COOPERATIVE_MATRIX_PROPERTIES_NV                 (VkStructureType)1000249001
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COOPERATIVE_MATRIX_PROPERTIES_NV (VkStructureType)1000249002
typedef enum VkComponentTypeNV
{
    VK_COMPONENT_TYPE_FLOAT16_NV = 0,
    VK_COMPONENT_TYPE_FLOAT32_NV = 1,
    VK_COMPONENT_TYPE_FLOAT64_NV = 2,
    VK_COMPONENT_TYPE_SINT8_NV = 3,
    VK_COMPONENT_TYPE_SINT16_NV = 4,
    VK_COMPONENT_TYPE_SINT32_NV = 5,
    VK_COMPONENT_TYPE_SINT64_NV = 6,
    VK_COMPONENT_TYPE_UINT8_NV = 7,
    VK_COMPONENT_TYPE_UINT16_NV = 8,
    VK_COMPONENT_TYPE_UINT32_NV = 9,
    VK_COMPONENT_TYPE_UINT64_NV = 10,
    VK_COMPONENT_TYPE_BEGIN_RANGE_NV = VK_COMPONENT_TYPE_FLOAT16_NV,
    VK_COMPONENT_TYPE_END_RANGE_NV = VK_COMPONENT_TYPE_UINT64_NV,
    VK_COMPONENT_TYPE_RANGE_SIZE_NV = (VK_COMPONENT_TYPE_UINT64_NV - VK_COMPONENT_TYPE_FLOAT16_NV + 1),
    VK_COMPONENT_TYPE_MAX_ENUM_NV = 0x7FFFFFFF
} VkComponentTypeNV;
typedef enum VkScopeNV
{
    VK_SCOPE_DEVICE_NV = 1,
    VK_SCOPE_WORKGROUP_NV = 2,
    VK_SCOPE_SUBGROUP_NV = 3,
    VK_SCOPE_QUEUE_FAMILY_NV = 5,
    VK_SCOPE_BEGIN_RANGE_NV = VK_SCOPE_DEVICE_NV,
    VK_SCOPE_END_RANGE_NV = VK_SCOPE_QUEUE_FAMILY_NV,
    VK_SCOPE_RANGE_SIZE_NV = (VK_SCOPE_QUEUE_FAMILY_NV - VK_SCOPE_DEVICE_NV + 1),
    VK_SCOPE_MAX_ENUM_NV = 0x7FFFFFFF
} VkScopeNV;
typedef struct VkCooperativeMatrixPropertiesNV
{
    VkStructureType sType;
    void* pNext;
    uint32_t MSize;
    uint32_t NSize;
    uint32_t KSize;
    VkComponentTypeNV AType;
    VkComponentTypeNV BType;
    VkComponentTypeNV CType;
    VkComponentTypeNV DType;
    VkScopeNV scope;
} VkCooperativeMatrixPropertiesNV;
typedef struct VkPhysicalDeviceCooperativeMatrixFeaturesNV
{
    VkStructureType sType;
    void* pNext;
    VkBool32 cooperativeMatrix;
    VkBool32 cooperativeMatrixRobustBufferAccess;
} VkPhysicalDeviceCooperativeMatrixFeaturesNV;
typedef struct VkPhysicalDeviceCooperativeMatrixPropertiesNV
{
    VkStructureType sType;
    void* pNext;
    VkShaderStageFlags cooperativeMatrixSupportedStages;
} VkPhysicalDeviceCooperativeMatrixPropertiesNV;
typedef VkResult(VKAPI_PTR* PFN_vkGetPhysicalDeviceCooperativeMatrixPropertiesNV)(VkPhysicalDevice physicalDevice, uint32_t* pPropertyCount, VkCooperativeMatrixPropertiesNV* pProperties);
#endif // VK_HEADER_VERSION < 101

#if VK_HEADER_VERSION < 121
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_COHERENT_MEMORY_FEATURES_AMD (VkStructureType)1000229000
#define VK_MEMORY_PROPERTY_DEVICE_COHERENT_BIT_AMD                     (VkMemoryPropertyFlagBits)0x00000040
#define VK_MEMORY_PROPERTY_DEVICE_UNCACHED_BIT_AMD                     (VkMemoryPropertyFlagBits)0x00000040
typedef struct VkPhysicalDeviceCoherentMemoryFeaturesAMD
{
    VkStructureType sType;
    void* pNext;
    VkBool32 deviceCoherentMemory;
} VkPhysicalDeviceCoherentMemoryFeaturesAMD;
#endif // VK_HEADER_VERSION < 121

#if VK_HEADER_VERSION < 129
#define VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_BUFFER_DEVICE_ADDRESS_FEATURES_KHR (VkStructureType)1000257000
#define VK_STRUCTURE_TYPE_BUFFER_DEVICE_ADDRESS_INFO_KHR                     (VkStructureType)1000244001
#define VK_STRUCTURE_TYPE_BUFFER_OPAQUE_CAPTURE_ADDRESS_CREATE_INFO_KHR      (VkStructureType)1000257002
#define VK_STRUCTURE_TYPE_MEMORY_OPAQUE_CAPTURE_ADDRESS_ALLOCATE_INFO_KHR    (VkStructureType)1000257003
#define VK_STRUCTURE_TYPE_DEVICE_MEMORY_OPAQUE_CAPTURE_ADDRESS_INFO_KHR      (VkStructureType)1000257004
#define VK_BUFFER_CREATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR               (VkBufferCreateFlagBits)0x00020000
#define VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR                        (VkBufferUsageFlagBits)0x00020000
#define VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_BIT_KHR                            (VkMemoryAllocateFlagBits)0x00000002
#define VK_MEMORY_ALLOCATE_DEVICE_ADDRESS_CAPTURE_REPLAY_BIT_KHR             (VkMemoryAllocateFlagBits)0x00000004
typedef struct VkPhysicalDeviceBufferDeviceAddressFeaturesKHR
{
    VkStructureType sType;
    void* pNext;
    VkBool32 bufferDeviceAddress;
    VkBool32 bufferDeviceAddressCaptureReplay;
    VkBool32 bufferDeviceAddressMultiDevice;
} VkPhysicalDeviceBufferDeviceAddressFeaturesKHR;
typedef struct VkBufferDeviceAddressInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    VkBuffer buffer;
} VkBufferDeviceAddressInfoKHR;
typedef struct VkBufferOpaqueCaptureAddressCreateInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    uint64_t opaqueCaptureAddress;
} VkBufferOpaqueCaptureAddressCreateInfoKHR;
typedef struct VkMemoryOpaqueCaptureAddressAllocateInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    uint64_t opaqueCaptureAddress;
} VkMemoryOpaqueCaptureAddressAllocateInfoKHR;
typedef struct VkDeviceMemoryOpaqueCaptureAddressInfoKHR
{
    VkStructureType sType;
    const void* pNext;
    VkDeviceMemory memory;
} VkDeviceMemoryOpaqueCaptureAddressInfoKHR;
typedef VkDeviceAddress(VKAPI_PTR* PFN_vkGetBufferDeviceAddressKHR)(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo);
typedef uint64_t(VKAPI_PTR* PFN_vkGetBufferOpaqueCaptureAddressKHR)(VkDevice device, const VkBufferDeviceAddressInfoKHR* pInfo);
typedef uint64_t(VKAPI_PTR* PFN_vkGetDeviceMemoryOpaqueCaptureAddressKHR)(VkDevice device, const VkDeviceMemoryOpaqueCaptureAddressInfoKHR* pInfo);
#endif // VK_HEADER_VERSION < 129

#if VK_HEADER_VERSION < 208
// Modify by Dicky for MacOS build
//typedef enum VkInstanceCreateFlagBits
//{
#if !defined(VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR)
#define VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR 0x00000001
#endif
#if !defined(VK_INSTANCE_CREATE_FLAG_BITS_MAX_ENUM)
#define VK_INSTANCE_CREATE_FLAG_BITS_MAX_ENUM 0x7FFFFFFF
#endif
//} VkInstanceCreateFlagBits;
// Modify by Dicky end
#endif // VK_HEADER_VERSION < 208
