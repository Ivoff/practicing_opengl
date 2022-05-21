#include <shader/shader.hpp>

ShaderProgram::ShaderProgram(std::vector<GLuint> shaders):
    m_shaders{shaders}
{
    m_selected = false;
    m_id = glCreateProgram();
    for(auto some_shader : m_shaders)
        glAttachShader(m_id, some_shader);
    
    glLinkProgram(m_id);

    for(auto some_shader : m_shaders)
        glDeleteShader(some_shader);

    int link_ok{};
    char log[4096]{};
    glGetProgramiv(m_id, GL_LINK_STATUS, &link_ok);
    if (!link_ok) {
        glGetProgramInfoLog(m_id, 4096, NULL, log);
        std::cerr << "Error Program [" << m_id << "]:\n" << log << std::endl;
        std::exit(EXIT_FAILURE);
    }
}

void ShaderProgram::m_setUniform(std::string uniform_name, glm::vec4& uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniform4f(uniform, uniform_value.x, uniform_value.y, uniform_value.z, uniform_value.w);
}
void ShaderProgram::m_setUniform(std::string uniform_name, glm::vec3& uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniform3f(uniform, uniform_value.x, uniform_value.y, uniform_value.z);
}
void ShaderProgram::m_setUniform(std::string uniform_name, int uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniform1i(uniform, uniform_value);
}
void ShaderProgram::m_setUniform(std::string uniform_name, GLuint uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniform1i(uniform, uniform_value);
}
void ShaderProgram::m_setUniform(std::string uniform_name, float uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniform1f(uniform, uniform_value);
}
void ShaderProgram::m_setUniform(std::string uniform_name, glm::mat4& uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniformMatrix4fv(uniform, 1, GL_FALSE, &uniform_value[0][0]);
}

void ShaderProgram::m_setUniform(std::string uniform_name, glm::mat3& uniform_value)
{
    GLuint uniform = glGetUniformLocation(m_id, uniform_name.c_str());
    glUniformMatrix3fv(uniform, 1, GL_FALSE, &uniform_value[0][0]);
}

void ShaderProgram::m_destroy()
{
    glDeleteProgram(m_id);
}

void ShaderProgram::m_use()
{
    glUseProgram(m_id);
}

ShaderProgram::~ShaderProgram()
{
    m_destroy();
}