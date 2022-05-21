#include <mesh/mesh.hpp>

Mesh::Mesh(std::vector<Vertex> vertexes, std::vector<unsigned int> indices, std::vector<Texture> textures, Material material)
{
    m_vertexes = vertexes;
    m_indices = indices;
    m_textures = textures;
    m_material = material;

    m_Initialization();    
}

void Mesh::m_Initialization()
{
    m_PrintMaterials();
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*m_vertexes.size(), &m_vertexes[0], GL_DYNAMIC_DRAW);    

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_position));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_normal));
    glEnableVertexAttribArray(1);    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_tex_coord));
    glEnableVertexAttribArray(2);
    
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*m_indices.size(), &m_indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::m_Draw(ShaderProgram* shader)
{
    unsigned int diffuse_counter = 0;
    unsigned int specular_counter = 0;
    int tex_len = m_textures.size();
    std::string uniform_name;

    shader->m_use();

    for (int i = 0; i < tex_len; i += 1)
    {        
        uniform_name = "texture_";

        if (m_textures[i].m_type == "specular")
        {
            uniform_name = uniform_name + "specular_" + std::to_string(specular_counter++);
        }            
        else if (m_textures[i].m_type == "diffuse")
        {
            uniform_name = uniform_name + "diffuse_" + std::to_string(diffuse_counter++);
        }
        
        shader->m_setUniform(uniform_name, i);
        shader->m_setUniform("material.diffuse", m_material.m_diffuse);
        shader->m_setUniform("material.specular", m_material.m_specular);
        shader->m_setUniform("material.shininess", m_material.m_shininess);
        
        m_textures[i].m_Activate(GL_TEXTURE0 + i);
        m_textures[i].m_Bind();        
    }    
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);    
    glBindVertexArray(0);
}

void Mesh::m_PrintVertexes()
{    
    int len = m_vertexes.size();
    printf("Printing Vertexes:\n");
    for (int i = 0; i < len; i += 1)
    {
        printf("[%d](%f, %f, %f)\n", i, m_vertexes[i].m_position.x, m_vertexes[i].m_position.y, m_vertexes[i].m_position.z);
        printf("[%d](%f, %f, %f)\n", i, m_vertexes[i].m_normal.x, m_vertexes[i].m_normal.y, m_vertexes[i].m_normal.z);
        printf("[%d](%f, %f)\n", i, m_vertexes[i].m_tex_coord.x, m_vertexes[i].m_tex_coord.y);
        printf("\n");
    }    
}

void Mesh::m_PrintIndices()
{        
    int len = m_indices.size();
    printf("Printing Indices:\n");
    for (int i = 0; i < len; i += 3)
    {
        printf("[%d](%d, %d, %d)\n", i/3, m_indices[i], m_indices[i+1], m_indices[i+2]);        
        printf("\n");
    }    
}

void Mesh::m_PrintTextures(std::vector<Texture>* textures)
{    
    int len = textures->size();
    
    for (int i = 0; i < len; i += 1)
    {
        printf("\t[%d] %s\n", i, textures->at(i).m_path.c_str());
    }
}

void Mesh::m_PrintMaterials()
{    
    printf("%s:\n", m_material.m_name.c_str());
    printf("\tdiffuse: %s\n", glm::to_string(m_material.m_diffuse).c_str());
    printf("\tspecular: %s\n", glm::to_string(m_material.m_specular).c_str());
    printf("\tshininess: %f\n", m_material.m_shininess);
    m_PrintTextures(&m_textures);
    printf("\n");
}