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
        int samples = Model::getInstance()->getSamples();
        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::InputInt("Samples", &samples))
        {
            Model::getInstance()->setSamples(samples);
        }
        ImGui::SameLine();
        ImGui::SetNextItemWidth(100.0f);
        int chunkSize = Model::getInstance()->getChunkSize();
        if (ImGui::InputInt("Chunk Size", &chunkSize))
        {
            Model::getInstance()->setChunkSize(chunkSize);
        }
        ImGui::SameLine();
        bool test = Model::getInstance()->getTest();
        if(ImGui::Checkbox("Test", &test))
        {
            Model::getInstance()->setTest(test);
        }
        ImGui::SameLine();
        if (ImGui::Button("UV"))
        {
            ImVec2 contentRegion = ImGui::GetContentRegionAvail();
            Model::getInstance()->setImage(contentRegion.x, contentRegion.y);
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
                },
                ImVec2{0, 1},
                ImVec2{1, 0}
            );
        }
    }
    ImGui::End();
}

bool RenderView::shouldClose() const noexcept { return _shouldClose; }
