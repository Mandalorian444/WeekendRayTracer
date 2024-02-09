#include "RenderView.h"

#include <imgui.h>

#include <Model.h>


void RenderView::onImguiRender()
{
    if (ImGui::Begin("RenderView", &_shouldClose))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::MenuItem("Render UV"))
            {
                Model::getInstance()->renderScene();
            }
        }
        if (ImGui::Button("UV"))
        {
            Model::getInstance()->renderScene();
        }

        const Image* image = _getTexture();
        if (image)
        {
            _texture.reInitTexture(image);
            ImGui::Image(
                (void*)_texture.getID(),
                ImVec2{
                    static_cast<float>(_texture.getWidth()),
                    static_cast<float>(_texture.getHeight())
                }
            );
        }
    }
    ImGui::End();
}

bool RenderView::shouldClose() const noexcept { return _shouldClose; }
