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

int Model::getSamples() const noexcept { return _samples; }

int Model::getChunkSize() const noexcept { return _chunkSize; }

bool Model::getTest() const noexcept { return _test; }

void Model::setImage(const int width, const int height)
{
    _currentImage = Image(width, height);
}

void Model::setSamples(const int samples) { _samples = samples; }

void Model::setChunkSize(const int chunkSize) { _chunkSize = chunkSize; }

void Model::setTest(const bool test) { _test = test; }

void Model::renderScene()
{
    // GenerateUV(_currentImage);
    RenderScene(_currentImage, _samples, _chunkSize, _test);
}
