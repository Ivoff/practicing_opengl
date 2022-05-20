#include <mesh/mesh.hpp>

Mesh::Mesh(std::vector<Vertex> vertexes, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
    m_vertexes = vertexes;
    m_indices = indices;
    m_textures = textures;

    m_Initialization();
}

void Mesh::m_Initialization()
{
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
        glActiveTexture(GL_TEXTURE0);
        shader->m_setUniform(uniform_name, i);        

        m_textures[i].m_Activate(GL_TEXTURE0 + i);
        m_textures[i].m_Bind();
    }
    
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, (void*)0);
    glBindVertexArray(0);
}