#pragma once

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <memory>


using namespace glm;


class Texture
{
public:
    typedef std::shared_ptr<Texture> ptr;
    
    Texture() = default;
    virtual ~Texture() = default;
    virtual vec3 sample(const float &u, const float &v, const vec3 &p) const = 0;
};

class ImageTexture : public Texture
{
private:
    unsigned char *m_data;
    int m_width, m_height, m_channel;
    
public:
    typedef std::shared_ptr<ImageTexture> ptr;
    
    ImageTexture() = default;
    ImageTexture(const std::string &path);
    virtual ~ImageTexture() = default;
    
    virtual vec3 sample(const float &u, const float &v, const vec3 &p) const;
    
};

