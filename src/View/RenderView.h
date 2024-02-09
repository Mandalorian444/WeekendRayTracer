#pragma once

#include <functional>

#include <Image/Image.h>
#include <View/Texture.h>
#include <View/Viewable.h>


class RenderView : public Viewable
{
private:
    bool _shouldClose = false;
    Texture _texture;

    std::function<const Image*(void)> _getTexture;

public:
    RenderView(const std::function<const Image*(void)> getTextureCallback)
      : _getTexture(getTextureCallback)
    {}

    virtual void onImguiRender() override;
    virtual bool shouldClose() const noexcept override;
};
