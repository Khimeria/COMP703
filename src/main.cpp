#include <project/main.hpp>
#include "project/light.h"
#include <project/gameObject/model_go.h>
#include <project/gameObject/primitive_go.h>
#include "project/renderManager.h"
#include "project/world.h"
#include "project/playerController.h"

//-----------------------------------
const GLint WIDTH = 800, HEIGHT = 600;

const char *PROGRAM_NAME = "BossFight";

int main(int argc, char** argv)
{
    // init video system
    if( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
        fprintf(stderr,"Failed to initialize SDL Video!\n");
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    if (( IMG_Init(flags) & flags) != flags) {
        std::cout << "IMG_Init: Failed to init required jpg and png support: Error-> " << IMG_GetError() << "\n";
        SDL_Quit();
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
    glEnable(GL_DEPTH_TEST);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 20);
    //glEnable(GL_FRAMEBUFFER_SRGB);

    //camera creation
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 3.0f);
    auto camera = KhEngine::Camera(cameraPos, cameraTarget);

    //create world
    KhEngine::World world(camera,WIDTH,HEIGHT,45.0f);

    //create shader
    KhEngine::Shader* ourShader = KhEngine::RenderManager::GetShader("src/shaders/model/model.vsh", "src/shaders/model/model.fsh");
    //create models
    //KhEngine::Model ourModel("models/Scene/Scene.obj");
    //KhEngine::Model ourModel("models/14-girl-obj/girl OBJ.obj");
    //KhEngine::Model ourModel("models/backpack/backpack.obj");
    //KhEngine::Model ourModel2("models/ForMargo (1)/Meshes/SM_Tree_Large_01.OBJ");

    //create GameObjects
    KhEngine::Cube pCube;
    world.addGameObject(pCube);
    KhEngine::Cube sCube;
    world.addGameObject(sCube);
    KhEngine::TerrainGameObject terrain(*ourShader);
    world.addGameObject(terrain);
    KhEngine::GoblinGameObject goblin(*ourShader);
    world.addGameObject(goblin);

    KhEngine::PlayerController player(goblin, -world.Forward);

    //setup player and camera
    player.setSpeed(50.0f);
    camera.setSpeed(50.0f);

    int mouseX, mouseY;
    player.bindMouseInput(mouseX, mouseY);
    camera.bindMouseInput(mouseX, mouseY);
    //KhEngine::PlayerController player(goblin, -camera.Forward);
    //player.mask = glm::vec3(1.0f,0.0f,-1.0f);

    camera.setPosition(glm::vec3(0.0f, 20.0f, 60.0f));

    camera.BindTo(player);
    //camera.Unbind();

    //add Lights
    KhEngine::DirectLight dLight{};
    dLight.Color = glm::vec3(0.5f, 0.5f, 1.0f);
    dLight.Direction = glm::vec3(0.0f, -1.0f, -0.3f);
    dLight.Ambient = glm::vec3(0.07f);
    dLight.Diffuse = glm::vec3(0.15f);
    dLight.Specular = glm::vec3(0.35);

    world.addDirectLight(dLight);

    KhEngine::PointLight pLight{};
    pLight.Color = glm::vec3(0.8f, 0.7f, 0.0f);
    pLight.Position = glm::vec3(0.0f, -1.0f, -0.3f);
    pLight.Ambient = glm::vec3(0.1f);
    pLight.Diffuse = glm::vec3(0.25f);
    pLight.Specular = glm::vec3(0.55f);
    pLight.Constant = 1.0f;
    pLight.Linear = 0.022f;
    pLight.Quadratic = 0.0019;

    world.addPointLight(pLight);

    KhEngine::SpotLight sLight{};
    sLight.Color = glm::vec3(0.5f, 0.5f, 1.0f);
    sLight.Position = glm::vec3(0.0f, 1.0f, -1.0f);
    sLight.Direction = glm::vec3(0.0f, -1.0f, 0.0f);
    sLight.Ambient = glm::vec3(0.2f);
    sLight.Diffuse = glm::vec3(0.5f);
    sLight.Specular = glm::vec3(0.75f);
    sLight.Constant = 1.0f;
    sLight.Linear = 0.022f;
    sLight.Quadratic = 0.0019;
    sLight.CutOff = cos(glm::radians(12.5f));
    sLight.OuterCutOff = cos(glm::radians(17.5f));

    world.addSpotLight(sLight);

    //--- infinite loop with event queue processing
    SDL_Event event;
    float deltaTime = 0.0f;
    float lastFrame = (float)SDL_GetTicks()/1000;

    int cursorState = 0;
    KhEngine::setCursorMode(window, cursorState);

    //game loop
    while(EXIT_FAILURE)
    {
        SDL_GetRelativeMouseState(&mouseX, &mouseY);
        //input loop
        while( SDL_PollEvent( &event ))
        {
            switch( event.type )
            {
                case SDL_QUIT:
                    exit(EXIT_SUCCESS);
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            cursorState = 1-cursorState;
                            KhEngine::setCursorMode(window, cursorState);
                            break;
                        case SDLK_SPACE:
                            if(camera.IsBinded)
                            {
                                camera.Unbind();
                            }
                            else
                                camera.BindTo(player);
                            break;
                    }
                    break;
                case SDL_MOUSEWHEEL:
                {
                    world.fov -= (float)event.wheel.y;
                    if (world.fov < 1.0f)
                        world.fov = 1.0f;
                    if (world.fov > 45.0f)
                        world.fov = 45.0f;
                }
                    break;

            }

        } // -- while event in queue

        // Clear the colorbuffer
        //glClearColor(0.4f, 0.4f, 0.7f, 1.0f);
        glClearColor(0.04f, 0.04f, 0.07f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw OpenGL
        float currentFrame = (float)SDL_GetTicks()/1000;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        player.tick(deltaTime);
        camera.tick(deltaTime);

        pCube.transform.Position = pLight.Position = glm::vec3(20*sin(currentFrame), 1.0f, 20*cos(currentFrame));

        sLight.Position.x = 10*sin(currentFrame * 2.0f);
        sLight.Position.y = sin(currentFrame * 0.7f);
        sLight.Position.z = 5*sin(currentFrame * 1.3f);
        sCube.transform.Position = sLight.Position;

        //draw
        world.tick(deltaTime);

        SDL_GL_SwapWindow(window);
    }

    world.Destroy();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}



void KhEngine::setCursorMode(SDL_Window* window, int state) {
    SDL_ShowCursor(state);
    //SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
    SDL_SetRelativeMouseMode((SDL_bool)(1-state));
}

void KhEngine::loadTexture(GLuint *textureID, std::string directory, std::string path)
{
    //Load image at speci1fied path

    auto cdw = std::filesystem::current_path().parent_path();
    SDL_Surface* loadedSurface = IMG_Load((cdw/directory/path).c_str());
    if( loadedSurface == NULL )
    {
        printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
        return;
    }
    else
    {
        KhEngine::flip_surface(loadedSurface);
        glGenTextures(1, textureID);
        glBindTexture(GL_TEXTURE_2D, *textureID);

        int Mode = GL_RGB;
        switch (loadedSurface->format->BytesPerPixel)
        {
            case 4:
            case 24:
                if (loadedSurface->format->Rmask == 0x000000ff)
                    Mode = GL_RGBA;
                else      Mode = GL_BGRA;
                break;
            case 3:
                if (loadedSurface->format->Rmask == 0x000000ff)
                    Mode = GL_RGB;
                else   Mode = GL_BGR;
                break;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, Mode, loadedSurface->w, loadedSurface->h, 0, Mode, GL_UNSIGNED_BYTE, loadedSurface->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);

        //Get rid of old loaded surface
        //SDL_FreeSurface( loadedSurface ); this version of sdl crash
    }

}

void KhEngine::flip_surface(SDL_Surface* surface)
{
    SDL_LockSurface(surface);

    int pitch = surface->pitch; // row size
    char* temp = new char[pitch]; // intermediate buffer
    char* pixels = (char*) surface->pixels;

    for(int i = 0; i < surface->h / 2; ++i) {
        // get pointers to the two rows to swap
        char* row1 = pixels + i * pitch;
        char* row2 = pixels + (surface->h - i - 1) * pitch;

        // swap rows
        memcpy(temp, row1, pitch);
        memcpy(row1, row2, pitch);
        memcpy(row2, temp, pitch);
    }

    delete[] temp;

    SDL_UnlockSurface(surface);
}

