#include "imgui.h"
#include "imgui_helper.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_vulkan.h"
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <string>
#include <memory>
#include <SDL.h>
#include <SDL_vulkan.h>
#include "application.h"
#if IMGUI_VULKAN_SHADER
#include <ImVulkanShader.h>
#endif
#include "entry_vulkan.h"

void Application_FullScreen(bool on)
{
    ImGui_ImplSDL2_FullScreen(ImGui::GetMainViewport(), on);
}

int main(int argc, char** argv)
{
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    ApplicationWindowProperty property(argc, argv);
    Application_GetWindowProperties(property);
    // Init IME effect windows only
    ImGui_ImplSDL2_InitIme();

    std::string title = property.name;
    title += " Vulkan SDL";
    int window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_ALLOW_HIGHDPI;
    if (property.resizable) window_flags |= SDL_WINDOW_RESIZABLE;
    if (property.full_size)
    {
        SDL_DisplayMode DM;
        SDL_GetCurrentDisplayMode(0, &DM);
        SDL_Rect r;
        SDL_GetDisplayUsableBounds(0, &r);
        property.pos_x = (r.x > 0 && r.x < 100) ? r.x : r.x + FULLSCREEN_OFFSET_X;
        property.pos_y = r.y + FULLSCREEN_OFFSET_Y;
        property.width = DM.w - FULLSCREEN_WIDTH_ADJ;
        property.height = DM.h - r.y;
        property.center = false;
        window_flags |= SDL_WINDOW_BORDERLESS;
    }
    else if (property.full_screen)
    {
        window_flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }
    else
    {
        if (property.top_most)
        {
            window_flags |= SDL_WINDOW_ALWAYS_ON_TOP;
        }
        if (!property.window_border)
        {
            window_flags |= SDL_WINDOW_BORDERLESS;
        }
    }
    SDL_Window* window = SDL_CreateWindow(title.c_str(), property.center ? SDL_WINDOWPOS_CENTERED : property.pos_x, 
                                                        property.center ? SDL_WINDOWPOS_CENTERED : property.pos_y, 
                                                        property.width, property.height, window_flags);
    if (!window)
    {
        fprintf(stderr, "Failed to Create Window: %s\n", SDL_GetError());
        return -1;
    }

    // Set window icon
    if (!property.icon_path.empty())
    {
        ImGui_ImplSDL2_SetWindowIcon(window, property.icon_path.c_str());
    }

    // Hook IME effect windows only
    ImGui_ImplSDL2_HookIme(window);
    // Setup Vulkan
    uint32_t extensions_count = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensions_count, NULL);
    const char** ext = new const char*[extensions_count];
    SDL_Vulkan_GetInstanceExtensions(window, &extensions_count, ext);
    std::vector<const char*> extensions;
    for (int i = 0; i < extensions_count; i++)
        extensions.push_back(ext[i]);
    SetupVulkan(extensions);
    delete[] ext;

    // Create Window Surface
    VkSurfaceKHR surface;
    VkResult err;
    if (SDL_Vulkan_CreateSurface(window, g_Instance, &surface) == 0)
    {
        printf("Failed to create Vulkan surface.\n");
        return 1;
    }

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    auto ctx = ImGui::CreateContext();
    Application_SetupContext(ctx);
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGuiContext& g = *GImGui;
    io.Fonts->AddFontDefault();
    io.FontGlobalScale = property.scale;
    if (property.power_save) io.ConfigFlags |= ImGuiConfigFlags_EnableLowRefreshMode;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    if (property.docking) io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    if (property.viewport) io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    if (!property.auto_merge) io.ConfigViewportsNoAutoMerge = true;
    // Setup App setting file path
    auto setting_path = property.using_setting_path ? ImGuiHelper::settings_path(property.name) : "";
    auto ini_name = property.name;
    std::replace(ini_name.begin(), ini_name.end(), ' ', '_');
    setting_path += ini_name + ".ini";
    io.IniFilename = setting_path.c_str();
    auto language_path = property.language_path + ini_name + "_language.ini";
    if (property.internationalize)
    {
        io.LanguageFileName = language_path.c_str();
        g.Style.TextInternationalize = 1;
        g.LanguageName = "Default";
    }
    
    // Create Framebuffers
    int w, h;
    SDL_GetWindowSize(window, &w, &h);
    ImGui_ImplVulkanH_Window* wd = &g_MainWindowData;
    SetupVulkanWindow(wd, surface, w, h);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForVulkan(window);
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = g_Instance;
    init_info.PhysicalDevice = g_PhysicalDevice;
    init_info.Device = g_Device;
    init_info.QueueFamily = g_QueueFamily;
    init_info.Queue = g_Queue;
    init_info.PipelineCache = g_PipelineCache;
    init_info.DescriptorPool = g_DescriptorPool;
    init_info.Allocator = g_Allocator;
    init_info.MinImageCount = g_MinImageCount;
    init_info.ImageCount = wd->ImageCount;
    init_info.CheckVkResultFn = check_vk_result;
    ImGui_ImplVulkan_Init(&init_info, wd->RenderPass);

    UpdateVulkanFont(wd);

#if IMGUI_VULKAN_SHADER
    ImGui::ImVulkanShaderInit();
#endif

    Application_Initialize(&property.handle);

    // Main loop
    bool done = false;
    bool app_done = false;
    bool show = true;
    while (!app_done)
    {
        ImGui_ImplSDL2_WaitForEvent();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SHOWN)
            {
                show = true;
            }
            if (event.type == SDL_WINDOWEVENT && (event.window.event == SDL_WINDOWEVENT_HIDDEN || event.window.event == SDL_WINDOWEVENT_MINIMIZED))
            {
                show = false;
            }
            if (event.type == SDL_WINDOWEVENT && (event.window.event == SDL_WINDOWEVENT_EXPOSED || event.window.event == SDL_WINDOWEVENT_RESTORED))
            {
                show = true;
            }
            if (event.type == SDL_DROPFILE)
            {
                // TODO::Handle drop file
                // file path in event.drop.file
            }
        }
        if (!show && !(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable))
        {
            ImGui::sleep(10);
            continue;
        }

        // Resize swap chain?
        int width, height;
        SDL_GetWindowSize(window, &width, &height);
        if (g_SwapChainRebuild || width > property.width || height > property.height)
        {
            if (width > 0 && height > 0)
            {
                ImGui_ImplVulkan_SetMinImageCount(g_MinImageCount);
                ImGui_ImplVulkanH_CreateOrResizeWindow(g_Instance, g_PhysicalDevice, g_Device, &g_MainWindowData, g_QueueFamily, g_Allocator, width, height, g_MinImageCount);
                g_MainWindowData.FrameIndex = 0;
                g_SwapChainRebuild = false;
                property.width = width;
                property.height = height;
            }
        }

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        if (io.ConfigFlags & ImGuiConfigFlags_EnableLowRefreshMode)
            ImGui::SetMaxWaitBeforeNextFrame(1.0 / property.fps);

        app_done = Application_Frame(property.handle, done);

        ImGui::EndFrame();
        // Rendering
        ImGui::Render();
        FrameRendering(wd);
    }

    Application_Finalize(&property.handle);

    // Cleanup
#if IMGUI_VULKAN_SHADER
    //ImGui::ImVulkanShaderClear();
#endif
    err = vkDeviceWaitIdle(g_Device);
    check_vk_result(err);
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    CleanupVulkanWindow();
#if IMGUI_VULKAN_SHADER
    ImGui::ImVulkanShaderClear();
    CleanupVulkan(true);
#else
    CleanupVulkan();
#endif
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
