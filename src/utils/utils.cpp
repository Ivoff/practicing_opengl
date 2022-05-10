#ifndef UTILS_CPP
#define UTILS_CPP

#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>

struct Utils
{
    static unsigned int ticks()
    {
        return glfwGetTime()*1000;
    }

    static char scan_to_ascii_code(char input)
    {
        static std::unordered_map<char, char> m_key_map = {
            {9, 27} // ESC
        };

        if (m_key_map.count(input))
            return m_key_map[input];
        else
        {
            printf("Error while converting scan to ascii code\n");
            std::exit(0);
        }        

    }
};

#endif