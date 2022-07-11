#ifndef UTILS_CPP
#define UTILS_CPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <unordered_map>
#include <vector>

struct Utils;

#define GL_STMT(stmt) do {\
    stmt;\
    Utils::Check(#stmt, __FILE__, __LINE__);\
} while (0)

struct Utils
{
    static void Check(const char* statement, const char* file_name, int line)
    {
        GLenum error = glGetError();

        while (error != GL_NO_ERROR)
        {
            printf("\nERRROR\nFile: %s:%d\nStmt: %s\n", file_name, line, statement);

            switch(error)
            {
                case GL_INVALID_ENUM:
                    printf("GL_INVALID_ENUM\n\tAn unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
                break;
                
                case GL_INVALID_VALUE:
                    printf("GL_INVALID_VALUE\n\tA numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
                break;
                
                case GL_INVALID_OPERATION:
                    printf("GL_INVALID_OPERATION\n\tThe specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
                break;

                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    printf("GL_INVALID_FRAMEBUFFER_OPERATION\n\tThe framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
                break;

                case GL_OUT_OF_MEMORY:
                    printf("GL_OUT_OF_MEMORY\n\tThere is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
                break;

                case GL_STACK_UNDERFLOW:
                    printf("GL_STACK_UNDERFLOW\n\tAn attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
                break;

                case GL_STACK_OVERFLOW:
                    printf("GL_STACK_OVERFLOW\n\tAn attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
                break;
            }
            printf("\n");

            error = glGetError();
        }
    }

    static void GlError(std::string context, GLenum error = GL_NO_ERROR)
    {
        GLenum err = glGetError();
        
        if (error == GL_NO_ERROR && err == GL_NO_ERROR);
        {
            return;
        }

        err = error;

        printf("In: %s\n", context.c_str());
        
        switch(err)
        {
            case GL_INVALID_ENUM:
                printf("GL_INVALID_ENUM\n\tAn unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.\n");
            break;
            
            case GL_INVALID_VALUE:
                printf("GL_INVALID_VALUE\n\tA numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.\n");
            break;
            
            case GL_INVALID_OPERATION:
                printf("GL_INVALID_OPERATION\n\tThe specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.\n");
            break;

            case GL_INVALID_FRAMEBUFFER_OPERATION:
                printf("GL_INVALID_FRAMEBUFFER_OPERATION\n\tThe framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.\n");
            break;

            case GL_OUT_OF_MEMORY:
                printf("GL_OUT_OF_MEMORY\n\tThere is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.\n");
            break;

            case GL_STACK_UNDERFLOW:
                printf("GL_STACK_UNDERFLOW\n\tAn attempt has been made to perform an operation that would cause an internal stack to underflow.\n");
            break;

            case GL_STACK_OVERFLOW:
                printf("GL_STACK_OVERFLOW\n\tAn attempt has been made to perform an operation that would cause an internal stack to overflow.\n");
            break;
        }
    }

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