#ifndef UTILS_CPP
#define UTILS_CPP

#include <GLFW/glfw3.h>

struct Utils
{
    static unsigned int ticks()
    {
        return glfwGetTime()*1000;
    }
};


#endif