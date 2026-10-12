#include <SDL.h>
#include <SDL_opengl.h>

#include <cstdlib>
#include <iostream>

static GLuint compile_shader(GLenum type, const char *source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (ok != GL_TRUE) {
        GLint length = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            char *log = new char[static_cast<size_t>(length)];
            glGetShaderInfoLog(shader, length, nullptr, log);
            std::cerr << "Shader compile failed: " << log << '\n';
            delete[] log;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static GLuint make_program() {
    const char *vertex_source = R"(
        #version 330 core
        layout (location = 0) in vec2 position;
        void main() { gl_Position = vec4(position, 0.0, 1.0); }
    )";
    const char *fragment_source = R"(
        #version 330 core
        out vec4 color;
        void main() { color = vec4(0.30, 0.72, 1.0, 1.0); }
    )";

    GLuint vertex = compile_shader(GL_VERTEX_SHADER, vertex_source);
    GLuint fragment = compile_shader(GL_FRAGMENT_SHADER, fragment_source);
    if (!vertex || !fragment) {
        if (vertex) glDeleteShader(vertex);
        if (fragment) glDeleteShader(fragment);
        return 0;
    }

    GLuint program = glCreateProgram();
    glAttachShader(program, vertex);
    glAttachShader(program, fragment);
    glLinkProgram(program);
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    GLint ok = GL_FALSE;
    glGetProgramiv(program, GL_LINK_STATUS, &ok);
    if (ok != GL_TRUE) {
        GLint length = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        if (length > 1) {
            char *log = new char[static_cast<size_t>(length)];
            glGetProgramInfoLog(program, length, nullptr, log);
            std::cerr << "Program link failed: " << log << '\n';
            delete[] log;
        }
        glDeleteProgram(program);
        return 0;
    }
    return program;
}

int main(int, char **) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << '\n';
        return EXIT_FAILURE;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    SDL_Window *window = SDL_CreateWindow(
        "Sodium — Linux renderer prototype",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        960, 600, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << '\n';
        SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_GLContext context = SDL_GL_CreateContext(window);
    if (!context) {
        std::cerr << "SDL_GL_CreateContext failed: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }
    SDL_GL_SetSwapInterval(1);

    const GLuint program = make_program();
    if (!program) {
        SDL_GL_DeleteContext(context);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return EXIT_FAILURE;
    }

    const float vertices[] = { 0.0f, 0.65f, -0.65f, -0.55f, 0.65f, -0.55f };
    GLuint vao = 0, vbo = 0;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT ||
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }

        int width = 1, height = 1;
        SDL_GL_GetDrawableSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.055f, 0.065f, 0.085f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        SDL_GL_SwapWindow(window);
    }

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}
