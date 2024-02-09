#include "View.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <Model.h>
#include <View/GLHelpers.h>
#include <View/RenderView.h>


bool View::_initOpenGL()
{
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const int success = glfwInit();
    if (!success)
    {
        //  TODO: log  the error
        std::terminate();
    }

    _window = glfwCreateWindow(1280, 720, "WRT", nullptr, nullptr);
    if (!_window)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        std::terminate();
    }
    glfwMakeContextCurrent(_window);
    glfwSwapInterval(1);

    CallGlewInit();

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void View::_init() { _initOpenGL(); }

bool View::_shutdownOpenGL()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Clean up child views (and their GLCALLs) before termination
    _views.clear();
    // Make sure window destructor called before terminate
    glfwDestroyWindow(_window);
    glfwTerminate();

    return true;
}

void View::_shutdown() {}

const Image* View::_getCurrentImage()
{
    return Model::getInstance()->getImage();
}

View::View() { _init(); }

View::~View() { _shutdown(); }

void View::onRender()
{
    GLCall(glEnable(GL_BLEND));
    const char* aboutPopupID = "AboutWRT";
    while (!_shouldClose)
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Exit"))
                {
                    _shouldClose = true;
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("RenderView"))
                {
                    _views.push_back(
                        std::make_unique<RenderView>(View::_getCurrentImage)
                    );
                }

                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                //  TODO: make an AboutView
                ImGui::MenuItem("About");

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();

             for (std::vector<std::unique_ptr<Viewable>>::iterator v =
                      _views.begin();
                  v != _views.end();)
            {
                 (*v)->onImguiRender();
                 if ((*v)->shouldClose())
                 {
                     v = _views.erase(v);
                 }
                 else
                 {
                     ++v;
                 }
             }

            ImGui::Render();
            GLCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
            GLCall(glClear(GL_COLOR_BUFFER_BIT));
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            if (glfwWindowShouldClose(_window))
            {
                _shouldClose = true;
            }

            if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                GLFWwindow* backup_current_context = glfwGetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                glfwMakeContextCurrent(backup_current_context);
            }
            glfwSwapBuffers(_window);
        }
    }
}

void View::onImguiRender()
{
    if (ImGui::Begin("WRT", &_shouldClose))
    {
        for (auto it = _views.begin(); it != _views.end();)
        {
            (*it)->onImguiRender();
            if ((*it)->shouldClose())
            {
                _views.erase(it);
                continue;
            }
            ++it;
        }
    }
    ImGui::End();
}

bool View::shouldClose() const noexcept { return _shouldClose; }

void View::addView(std::unique_ptr<Viewable> view)
{
    _views.push_back(std::move(view));
}
