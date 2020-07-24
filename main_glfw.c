#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

#include "./rgol.h"

void platform_sleep(int millis)
{
    usleep(millis * 1000);
}

int to_update_state = 0;

GLFWwindow * window;
const int W_WIDTH = 640;
const int W_HEIGHT = 480;


void key_callback(GLFWwindow* window, int key, int scancode,
                  int action, int mods)
{
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
      to_update_state = 1;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
      glfwTerminate();
      exit(0);
    }
}

void board_display_glfw(const Board *board)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_QUADS);
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            if(board->cells[y][x] == ALIVE) {
              float size_x = (1.9f)/WIDTH;
              float size_y = (1.9f)/WIDTH;
              float top_left_x = (float)(x*2)/WIDTH - 1.0f;
              float top_left_y = -((float)(y*2)/WIDTH - 1.0f);
              glVertex3f(top_left_x, top_left_y, 0.0f);
              glColor3f(1.0f, 0.0f, 0.0f);
              glVertex3f(top_left_x+size_x, top_left_y, 0.0f);
              glColor3f(1.0f, 0.0f, 0.0f);
              glVertex3f(top_left_x+size_x, top_left_y-size_y, 0.0f);
              glColor3f(1.0f, 0.0f, 0.0f);
              glVertex3f(top_left_x, top_left_y-size_y, 0.0f);
              glColor3f(1.0f, 0.0f, 0.0f);
            }
        }
    }
    glEnd();
    glfwSwapBuffers(window);
}

Board board[2] = {
    {
        .cells = {
            {0, 0, 1},
            {1, 0, 1},
            {0, 1, 1},
        }
    },
    {}
};

void terminate_glfw() {
    glfwTerminate();
}

int fb = 0;

void window_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    board_display_glfw(&board[fb]);
}

void init_glfw() {
  glfwInit();
  window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "GOL", NULL, NULL);
  glfwSetWindowSizeCallback(window, window_size_callback);
  glfwSetKeyCallback(window, key_callback);
  glfwMakeContextCurrent(window);
}

int main()
{
    init_glfw();
    board_display_glfw(&board[fb]);
    while (!glfwWindowShouldClose(window)) {
        int bb = 1 - fb;
        board_display_glfw(&board[fb]);
        while (1) {
          glfwPollEvents();
          platform_sleep(10);
          if(to_update_state == 1) {
            to_update_state = 0;
            break;
          }
        }
        board_next_gen(&board[fb], &board[bb]);
        fb = bb;
    }
    terminate_glfw();
    return 0;
}
