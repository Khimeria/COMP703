#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <filesystem>
#include "project/custom_shader.h"

const GLint WIDTH = 800, HEIGHT = 600;

const char *PROGRAM_NAME = "BossFight";

float vertices[] = {
        // positions          // colors           // texture coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left
};

float texCoords[] = {
        0.0f, 0.0f,  // lower-left corner
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
};

GLuint* loadTexture(std::string path );

//-----------------------------------
int main(int argc, char** argv)
{
    // init video system
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        fprintf(stderr,"Failed to initialize SDL Video!\n");
        exit(EXIT_FAILURE);
    }

    // tell system which funciton to process when exit() call is made
    atexit(SDL_Quit);

    // set opengl attributes
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,        5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,      5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,       5);
#ifdef __APPLE__
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      32);
#else
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,      16);
#endif
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,    1);

    /* Request opengl 3.2 context.
     * SDL doesn't have the ability to choose which profile at this time of writing,
     * but it should default to the core profile */
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,     SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // get a framebuffer
    auto window = SDL_CreateWindow(PROGRAM_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   WIDTH, HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    SDL_GLContext context = SDL_GL_CreateContext(window);

    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    glViewport(0, 0, WIDTH, HEIGHT);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


    CustomShader::Shader ourShader("src/shaders/test.vsh", "src/shaders/test.fsh");

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    auto texture = loadTexture("textures/container.jpg");

    //--- infinite loop with event queue processing
    SDL_Event event;
    while(EXIT_FAILURE)
    {
        while( SDL_PollEvent( &event ))
        {
            switch( event.type )
            {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                    break;
            }

        } // -- while event in queue
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw OpenGL
        ourShader.use();

        auto timeValue = float (SDL_GetTicks()/ 1000.0);
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        ourShader.setFloat("ourColor", greenValue);


        //glBindTexture(GL_TEXTURE_2D, *texture);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    } // -- infinite loop

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

GLuint* loadTexture(std::string path )
{
    //Load image at specified path
    std::filesystem::path cwd = std::filesystem::current_path().parent_path();
    SDL_Surface* loadedSurface = IMG_Load( (cwd/path).c_str() );
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return nullptr;
    }
    else
    {
        GLuint TextureID = 0;

        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);

        int Mode = GL_RGB;
        if(loadedSurface->format->BytesPerPixel == 4) {
            Mode = GL_RGBA;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, Mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Get rid of old loaded surface
        //SDL_FreeSurface( loadedSurface );
        return &TextureID;
    }

}
