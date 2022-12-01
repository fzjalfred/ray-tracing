// Termm--Fall 2022

#include "Material.hpp"

Material::Material()
{}

Material::~Material()
{
    delete m_texture;
}


bool Material::bindTexture(const std::string path) {
    m_texture = new ImageTexture(path);
    return true;
}