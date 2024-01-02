#include <iostream>
#include <SDL.h>
#include <glew.h>

const GLint WIDTH = 800, HEIGHT = 600;

const char *PROGRAM_NAME = "BossFight";

//-----------------------------------
// Function prototypes
void SetupSDL();

//-----------------------------------
int main(int argc, char** argv)
{
    SetupSDL();
    return EXIT_SUCCESS;
}


//-----------------------------------
// Setup SDL and OpenGL
void SetupSDL(void)
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
        return exit(EXIT_FAILURE);
    }

    glViewport(0, 0, WIDTH, HEIGHT);

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

        SDL_GL_SwapWindow(window);
    } // -- infinite loop

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return exit(EXIT_SUCCESS);
}