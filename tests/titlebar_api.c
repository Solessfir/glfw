//========================================================================
// Custom title bar API regression test
// Copyright (c) Camilla Löwy <elmindreda@glfw.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

#define REQUIRE(condition) \
    do \
    { \
        if (!(condition)) \
        { \
            fprintf(stderr, "Requirement failed at line %d: %s\n", \
                    __LINE__, #condition); \
            glfwTerminate(); \
            exit(EXIT_FAILURE); \
        } \
    } while (0)

static int first_hit_test(GLFWwindow* window, int xpos, int ypos)
{
    (void) window;
    (void) xpos;
    (void) ypos;
    return GLFW_HIT_TEST_CAPTION;
}

static int second_hit_test(GLFWwindow* window, int xpos, int ypos)
{
    (void) window;
    (void) xpos;
    (void) ypos;
    return GLFW_HIT_TEST_CLIENT;
}

int main(void)
{
    glfwInitHint(GLFW_PLATFORM, GLFW_PLATFORM_NULL);
    REQUIRE(glfwInit());

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(640, 480, "Title Bar API", NULL, NULL);
    REQUIRE(window);
    REQUIRE(glfwGetWindowAttrib(window, GLFW_TITLEBAR) == GLFW_TRUE);
    glfwDestroyWindow(window);

    glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);
    window = glfwCreateWindow(640, 480, "Title Bar API", NULL, NULL);
    REQUIRE(window);
    REQUIRE(glfwGetWindowAttrib(window, GLFW_TITLEBAR) == GLFW_FALSE);

    REQUIRE(glfwSetWindowHitTestCallback(window, first_hit_test) == NULL);
    REQUIRE(glfwSetWindowHitTestCallback(window, second_hit_test) == first_hit_test);
    REQUIRE(glfwSetWindowHitTestCallback(window, NULL) == second_hit_test);

    glfwSetWindowAttrib(window, GLFW_TITLEBAR, GLFW_TRUE);
    REQUIRE(glfwGetWindowAttrib(window, GLFW_TITLEBAR) == GLFW_TRUE);
    glfwSetWindowAttrib(window, GLFW_TITLEBAR, GLFW_FALSE);
    REQUIRE(glfwGetWindowAttrib(window, GLFW_TITLEBAR) == GLFW_FALSE);

    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;
}
