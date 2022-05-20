#ifndef MODEL_H
#define MODEL_H

#include <shader/shader.hpp>
#include <mesh/mesh.hpp>
#include <texture/texture.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/mat4x4.hpp>

#include <string>
#include <vector>

struct Model
{
    std::vector<Mesh> m_meshes;
    std::vector<Texture> m_textures_loaded;
    std::string m_directory;
    glm::mat4 m_model_mat;
    float m_scale;

    Model();
    Model(std::string path);
    void m_Draw(ShaderProgram* shader);
    void m_LoadModel(std::string path);
    void m_ProcessNode(aiNode* node, const aiScene* scene);
    Mesh m_ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> m_LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string type_name);
};

#endif