#pragma once

#include <Image/Image.h>


class Model
{
private:
    Image _currentImage;
    std::size_t _samples = 2u;
    std::size_t _chunkSize = 4096u;
    bool _test = false;
    static Model* _instance;

public:
    static Model* getInstance();

    const Image* getImage() const noexcept;
    int getSamples() const noexcept;
    int getChunkSize() const noexcept;
    bool getTest() const noexcept;

    void setImage(const int width, const int height);
    void setSamples(const int samples);
    void setChunkSize(const int chunkSize);
    void setTest(const bool test);

    void renderScene();
};
