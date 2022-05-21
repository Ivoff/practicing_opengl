#include <model/model.hpp>

Model::Model()
{
    m_model_mat = glm::mat4(1.0f);
    m_normal_mat = glm::mat3(1.0f);
    m_scale = 1.0f;    
}

Model::Model(std::string path)
{
    m_LoadModel(path);

    m_model_mat = glm::mat4(1.0f);
    m_normal_mat = glm::mat3(1.0f);
    m_scale = 1.0f;
}

void Model::m_LoadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
    {
        printf("Error while creating scene for %s model\n", path.c_str());
        printf("%s\n", importer.GetErrorString());
        std::exit(0);
    }    

    m_directory = path.substr(0, path.find_last_of('/')+1);    
    m_ProcessNode(scene->mRootNode, scene);
}

void Model::m_Draw(ShaderProgram* shader)
{
    int meshes_len = m_meshes.size();
    
    for (int i = 0; i < meshes_len; i += 1)
    {
        m_meshes[i].m_Draw(shader);
    }
}

void Model::m_ProcessNode(aiNode* node, const aiScene* scene)
{
    for (int i = 0; i < node->mNumMeshes; i += 1)
    {
        m_meshes.push_back(
            m_ProcessMesh(
                scene->mMeshes[node->mMeshes[i]], 
                scene
            )            
        );
    }

    for (int i = 0; i < node->mNumChildren; i += 1)
    {
        m_ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::m_ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    Mesh output_mesh;
    Vertex vertex;        

    for (int i = 0; i < mesh->mNumVertices; i += 1)
    {
        vertex.m_position.x = mesh->mVertices[i].x;
        vertex.m_position.y = mesh->mVertices[i].y;
        vertex.m_position.z = mesh->mVertices[i].z;

        vertex.m_normal.x = mesh->mNormals[i].x;
        vertex.m_normal.y = mesh->mNormals[i].y;
        vertex.m_normal.z = mesh->mNormals[i].z;

        if (mesh->mTextureCoords[0])
        {
            vertex.m_tex_coord.x = mesh->mTextureCoords[0][i].x;
            vertex.m_tex_coord.y = mesh->mTextureCoords[0][i].y;
        }
        else
        {
            vertex.m_tex_coord = glm::vec2(0.0f, 0.0f);
        }

        output_mesh.m_vertexes.push_back(vertex);        
    }

    for (int i = 0; i < mesh->mNumFaces; i += 1)
    {
        aiFace face = mesh->mFaces[i];
        
        for (int j = 0; j < face.mNumIndices; j += 1)
        {
            output_mesh.m_indices.push_back(face.mIndices[j]);
        }
    }

    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* ai_material = scene->mMaterials[mesh->mMaterialIndex];        
        
        std::vector<Texture> diffuse_maps = m_LoadMaterialTexture(ai_material, aiTextureType_DIFFUSE, "diffuse");
        if (diffuse_maps.size() == 0)
        {
            Texture diffuse_tex;
            diffuse_tex = diffuse_tex.BlackTex();
            diffuse_tex.m_type = "diffuse";
            output_mesh.m_textures.push_back(diffuse_tex);
        }
        else
        {
            output_mesh.m_textures.insert(output_mesh.m_textures.end(), diffuse_maps.begin(), diffuse_maps.end());
        }        

        std::vector<Texture> specular_maps = m_LoadMaterialTexture(ai_material, aiTextureType_SPECULAR, "specular");
        if (specular_maps.size() == 0)
        {
            Texture specular_tex;
            specular_tex = specular_tex.BlackTex();
            specular_tex.m_type = "diffuse";
            output_mesh.m_textures.push_back(specular_tex);
        }
        else
        {
            output_mesh.m_textures.insert(output_mesh.m_textures.end(), specular_maps.begin(), specular_maps.end());
        }        

        Material material = Material(glm::vec3(1.0f), glm::vec3(1.0f), 10.0f);
        aiColor3D ai_color;
        aiString ai_string;
        float shininess = 0.0f;

        ai_material->Get(AI_MATKEY_COLOR_DIFFUSE, ai_color);
        if (!ai_color.IsBlack())
        {
            material.m_diffuse.x = ai_color.r;
            material.m_diffuse.y = ai_color.g;
            material.m_diffuse.z = ai_color.b;
            ai_color = aiColor3D(0.0f);
        }

        ai_material->Get(AI_MATKEY_COLOR_SPECULAR, ai_color);
        if (!ai_color.IsBlack())
        {
            material.m_specular.x = ai_color.r;
            material.m_specular.y = ai_color.g;
            material.m_specular.z = ai_color.b;
            ai_color = aiColor3D(0.0f);
        }

        ai_material->Get(AI_MATKEY_SHININESS, shininess);
        if (shininess)
        {
            material.m_shininess = shininess;
        }

        ai_material->Get(AI_MATKEY_NAME, ai_string);
        if (ai_string.length)
        {
            material.m_name = std::string(ai_string.C_Str());
        }

        output_mesh.m_material = material;
    }
    else {
        output_mesh.m_material.m_diffuse = glm::vec3(1.0f);
        output_mesh.m_material.m_specular = glm::vec3(1.0f);
        output_mesh.m_material.m_shininess = 10.0f;
        output_mesh.m_material.m_name = "Null";
    }

    output_mesh.m_Initialization();    
    return output_mesh;
}

std::vector<Texture> Model::m_LoadMaterialTexture(aiMaterial* material, aiTextureType type, std::string type_name)
{
    std::vector<Texture> textures;    

    for (int i = 0; i < material->GetTextureCount(type); i += 1)
    {
        aiString str;        
        material->GetTexture(type, i, &str);
        std::string sub_path = std::string(str.C_Str());        
        
        for(int j = 0; j < sub_path.size(); j += 1)
        {
            if (sub_path[j] == '\\')
            {
                sub_path[j] = '/';
            }
        }
        
        std::string texture_path = m_directory + sub_path;
        // printf("Trying to load:\n\tTexture(%s)\n", texture_path.c_str());

        bool skip = false;
        int loaded_len = m_textures_loaded.size();

        for (int j = 0; j < loaded_len; j += 1)
        {
            if (m_textures_loaded[j].m_path == texture_path)
            {                
                skip = true;
                textures.push_back(m_textures_loaded[j]);
                break;
            }
        }

        if (!skip)
        {
            Texture texture = Texture(texture_path, GL_TEXTURE_2D);
            texture.m_type = type_name;
            texture.m_path = texture_path;
            m_textures_loaded.push_back(texture);
        }        
    }

    return textures;
}

glm::mat3& Model::m_GetNormalMat()
{
    m_normal_mat = glm::mat3(glm::transpose(glm::inverse(m_model_mat)));
    return m_normal_mat;
}

void Model::m_PrintMeshInfo(aiMesh* mesh, const aiScene* scene)
{
    static int i = 0;
    printf("Mesh %d\n", i++);
    printf("\tVertices: %d\n", mesh->mNumVertices);
    printf("\tFaces: %d\n", mesh->mNumFaces);
    printf("\n");
}