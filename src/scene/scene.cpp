#include <scene/scene.hpp>

void Scene::destroy()
{
    glDeleteVertexArrays(1, &current_vao);
    glDeleteBuffers(1, &current_ebo);
    current_program->m_destroy();
}