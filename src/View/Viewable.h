#pragma once


class Viewable
{
public:
    virtual ~Viewable() = 0;

    virtual void onImguiRender() = 0;
    virtual bool shouldClose() const noexcept = 0;
};
