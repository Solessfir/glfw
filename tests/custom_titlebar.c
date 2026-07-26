//========================================================================
// Custom title bar test
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

#define GLAD_GL_IMPLEMENTATION
#include <glad/gl.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

static int scaled(GLFWwindow* window, int value)
{
    float yscale;
    glfwGetWindowContentScale(window, NULL, &yscale);
    return (int) (value * yscale + 0.5f);
}

static int hit_test_callback(GLFWwindow* window, int xpos, int ypos)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    if (!glfwGetWindowAttrib(window, GLFW_MAXIMIZED) &&
        glfwGetWindowAttrib(window, GLFW_RESIZABLE))
    {
        const int border = scaled(window, 6);
        const int left = xpos < border;
        const int right = xpos >= width - border;
        const int top = ypos < border;
        const int bottom = ypos >= height - border;

        if (top && left)
            return GLFW_HIT_TEST_RESIZE_TOP_LEFT;
        if (top && right)
            return GLFW_HIT_TEST_RESIZE_TOP_RIGHT;
        if (bottom && left)
            return GLFW_HIT_TEST_RESIZE_BOTTOM_LEFT;
        if (bottom && right)
            return GLFW_HIT_TEST_RESIZE_BOTTOM_RIGHT;
        if (left)
            return GLFW_HIT_TEST_RESIZE_LEFT;
        if (right)
            return GLFW_HIT_TEST_RESIZE_RIGHT;
        if (top)
            return GLFW_HIT_TEST_RESIZE_TOP;
        if (bottom)
            return GLFW_HIT_TEST_RESIZE_BOTTOM;
    }

    const int titlebarHeight = scaled(window, 40);
    if (ypos >= titlebarHeight)
        return GLFW_HIT_TEST_CLIENT;

    const int buttonWidth = scaled(window, 46);
    if (xpos >= width - buttonWidth)
        return GLFW_HIT_TEST_CLOSE_BUTTON;
    if (xpos >= width - buttonWidth * 2)
        return GLFW_HIT_TEST_MAXIMIZE_BUTTON;
    if (xpos >= width - buttonWidth * 3)
        return GLFW_HIT_TEST_MINIMIZE_BUTTON;
    if (xpos < titlebarHeight)
        return GLFW_HIT_TEST_SYSTEM_MENU;

    return GLFW_HIT_TEST_CAPTION;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) scancode;
    (void) mods;

    if (action != GLFW_PRESS)
        return;

    if (key == GLFW_KEY_ESCAPE)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    else if (key == GLFW_KEY_T)
    {
        const int titlebar = glfwGetWindowAttrib(window, GLFW_TITLEBAR);
        glfwSetWindowAttrib(window, GLFW_TITLEBAR, !titlebar);
    }
}

static void fill_rect(GLFWwindow* window,
                      int xpos, int ypos, int width, int height,
                      float red, float green, float blue)
{
    int windowWidth, windowHeight;
    int framebufferWidth, framebufferHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    const float xscale = (float) framebufferWidth / (float) windowWidth;
    const float yscale = (float) framebufferHeight / (float) windowHeight;

    glScissor((int) (xpos * xscale),
              framebufferHeight - (int) ((ypos + height) * yscale),
              (int) (width * xscale),
              (int) (height * yscale));
    glClearColor(red, green, blue, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}

static void draw_window(GLFWwindow* window)
{
    int width, height;
    int framebufferWidth, framebufferHeight;
    double cursorX, cursorY;
    glfwGetWindowSize(window, &width, &height);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glfwGetCursorPos(window, &cursorX, &cursorY);

    glViewport(0, 0, framebufferWidth, framebufferHeight);
    glEnable(GL_SCISSOR_TEST);

    fill_rect(window, 0, 0, width, height, 0.10f, 0.11f, 0.13f);

    const int titlebarHeight = scaled(window, 40);
    const int buttonWidth = scaled(window, 46);
    fill_rect(window, 0, 0, width, titlebarHeight, 0.16f, 0.17f, 0.20f);

    const int region = hit_test_callback(window, (int) cursorX, (int) cursorY);
    fill_rect(window, width - buttonWidth * 3, 0, buttonWidth, titlebarHeight,
              region == GLFW_HIT_TEST_MINIMIZE_BUTTON ? 0.30f : 0.21f,
              region == GLFW_HIT_TEST_MINIMIZE_BUTTON ? 0.31f : 0.22f,
              region == GLFW_HIT_TEST_MINIMIZE_BUTTON ? 0.34f : 0.25f);
    fill_rect(window, width - buttonWidth * 2, 0, buttonWidth, titlebarHeight,
              region == GLFW_HIT_TEST_MAXIMIZE_BUTTON ? 0.30f : 0.21f,
              region == GLFW_HIT_TEST_MAXIMIZE_BUTTON ? 0.31f : 0.22f,
              region == GLFW_HIT_TEST_MAXIMIZE_BUTTON ? 0.34f : 0.25f);
    fill_rect(window, width - buttonWidth, 0, buttonWidth, titlebarHeight,
              region == GLFW_HIT_TEST_CLOSE_BUTTON ? 0.78f : 0.21f,
              region == GLFW_HIT_TEST_CLOSE_BUTTON ? 0.16f : 0.22f,
              region == GLFW_HIT_TEST_CLOSE_BUTTON ? 0.14f : 0.25f);

    glDisable(GL_SCISSOR_TEST);
}

static void error_callback(int error, const char* description)
{
    (void) error;
    fprintf(stderr, "Error: %s\n", description);
}

int main(void)
{
    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_TITLEBAR, GLFW_FALSE);

    GLFWwindow* window = glfwCreateWindow(960, 540, "Custom Title Bar", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetWindowSizeLimits(window, 330, 240, GLFW_DONT_CARE, GLFW_DONT_CARE);
    glfwSetWindowHitTestCallback(window, hit_test_callback);
    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSwapInterval(1);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        draw_window(window);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    exit(EXIT_SUCCESS);
}
