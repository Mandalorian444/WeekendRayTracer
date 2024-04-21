#include "Model.h"

#include <Image/Generators/GenUV.h>
#include <Renderer/Renderer.h>


Model* Model::_instance = new Model();

Model* Model::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Model();
    }
    return _instance;
}

const Image* Model::getImage() const noexcept { return &_currentImage; }

void Model::renderScene()
{
    // GenerateUV(_currentImage);
    RenderScene(_currentImage);
}
