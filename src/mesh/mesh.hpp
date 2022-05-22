#ifndef MESH_HPP
#define MESH_HPP

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>

#include <texture/texture.hpp>
#include <shader/shader.hpp>
#include <material/material.hpp>

struct Vertex
{
    glm::vec3 m_position;
    glm::vec3 m_normal;
    glm::vec2 m_tex_coord;
    glm::vec3 m_tangent;
    glm::vec3 m_bitangent;
};

struct Mesh
{
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;
    std::vector<Vertex> m_vertexes;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    Material m_material;

    Mesh(){};
    Mesh(std::vector<Vertex> vertexes, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material);    
    void m_Draw(ShaderProgram* shader);
    void m_Initialization();

    void m_PrintVertexes();
    void m_PrintIndices();
    void m_PrintTextures(std::vector<Texture>* textures);
    void m_PrintMaterials();
};

#endif