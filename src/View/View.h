#pragma once

#include <memory>
#include <vector>

#include <GLFW/glfw3.h>

#include <View/Viewable.h>
#include <Image/Image.h>


class View : public Viewable
{
private:
    bool _shouldClose = false;
    GLFWwindow* _window;
    std::vector<std::unique_ptr<Viewable>> _views;

    bool _initOpenGL();
    void _init();

    bool _shutdownOpenGL();
    void _shutdown();

    static const Image* _getCurrentImage();

public:
    View();
    ~View();

    void onRender();
    void onImguiRender() override;
    bool shouldClose() const noexcept override;

    void addView(std::unique_ptr<Viewable> view);
};
