#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>

#include <unistd.h>

#define USE_GLFW

void platform_sleep(int millis)
{
    usleep(millis * 1000);
}

#define ARRAY_LEN(xs) (sizeof(xs) / sizeof(xs[0]))

typedef enum {
    DED = 0,
    ALIVE = 1,
} Cell;

#define WIDTH 5
#define HEIGHT 5

GLFWwindow * window;
const int W_WIDTH = 640;
const int W_HEIGHT = 480;

typedef struct {
    Cell cells[HEIGHT][WIDTH];
} Board;

typedef struct {
    int y, x;
} Coord;

void println_coord(Coord coord)
{
    printf("(%d, %d)\n", coord.y, coord.x);
}

int modi(int x, int y)
{
    return (x % y + y) % y;
}

int to_update_state = 0;

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

static Coord neighbor_index_dirs[8] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

void board_display(const Board *board, FILE *stream)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            fputc(' ', stream);
            fputc(board->cells[y][x] == ALIVE ? '#' : '.', stream);
        }
        fputc('\n', stream);
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

int board_neighbors(const Board *board, int y, int x)
{
    int result = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy != 0 || dx != 0) {
                if (board->cells[modi(y + dy, HEIGHT)][modi(x + dx, WIDTH)] == ALIVE) {
                    result += 1;
                }
            }
        }
    }
    return result;
}

void board_next_gen(const Board *prev, Board *next)
{
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            int neighbors = board_neighbors(prev, y, x);
            if (prev->cells[y][x] == ALIVE) {
                next->cells[y][x] = neighbors == 2 || neighbors == 3 ? ALIVE : DED;
            } else {                // DED
                next->cells[y][x] = neighbors == 3 ? ALIVE : DED;
            }
        }
    }
}

void board_prev_cell(const Board *next, Board *prev, int y, int x)
{
    assert(next->cells[y][x] == ALIVE);
    prev->cells[y][x] = DED;
#define N ARRAY_LEN(neighbor_index_dirs)
    for (int i = 0; i < N; ++i) {
        for (int j = i + 1; j < N; ++j) {
            for (int k = j + 1; k < N; ++k) {
                // TODO: it's probably looks like an algorithm that computes the binomial coeficient
                // https://cp-algorithms.com/combinatorics/binomial-coefficients.html
                // https://en.wikipedia.org/wiki/Binomial_coefficient
            }
        }
    }
#undef N
}

Board board[2] = {
    {
        .cells = {
            {0, 0, 1, 0, 0},
            {1, 0, 1, 0, 0},
            {0, 1, 1, 0, 0},
            {0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0},
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

int main(int argc, char *argv[])
{
#ifdef USE_GLFW
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
#else
#define GLIDER_CYCLES 4
#define STEPS (GLIDER_CYCLES * 5)
    board_display(&board[fb], stdout);
    fputs("------------------------------\n", stdout);

    for (int i = 0; i < STEPS; ++i) {
        int bb = 1 - fb;
        board_next_gen(&board[fb], &board[bb]);
        fb = bb;
        board_display(&board[fb], stdout);
        fputs("------------------------------\n", stdout);
    }
#endif

    return 0;
}

// 0  1 2
// 0 -1 1
// index = 0 + 1
