#include <scene/scene.hpp>

void Scene::destroy()
{    
    current_program->m_destroy();
}