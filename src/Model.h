#pragma once

#include <Image/Image.h>


class Model
{
private:
    Image _currentImage;
    static Model* _instance;

public:
    static Model* getInstance();

    const Image* getImage() const noexcept;

    void renderScene();
};
