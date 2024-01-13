#include <project/main.hpp>
#include "project/model.h"
#include "project/light.h"

//-----------------------------------
const GLint WIDTH = 800, HEIGHT = 600;

const char *PROGRAM_NAME = "BossFight";

float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

int indices[] = {0, 1, 3, 1,2,3};

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

    KhEngine::Shader ourShader("src/shaders/model/model.vsh", "src/shaders/model/model.fsh");
    //KhEngine::Model ourModel("models/14-girl-obj/girl OBJ.obj");
    KhEngine::Model ourModel("models/Scene/Scene.obj");
    //KhEngine::Model ourModel("models/backpack/backpack.obj");
    //KhEngine::Model ourModel2("models/ForMargo (1)/Meshes/SM_Tree_Large_01.OBJ");
    KhEngine::LightSource ourLightSource(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.5f,0.5f,1.0f));
    KhEngine::LightSource ourLightSource2(glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(1.0f,1.0f,1.0f));

    glm::mat4 projection;
    float fov = 45.0f;
    projection = KhEngine::getProjection(fov);

    glEnable(GL_DEPTH_TEST);

    //camera creation
    glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    auto camera = KhEngine::Camera(cameraPos, cameraTarget);

    //--- infinite loop with event queue processing
    SDL_Event event;
    float deltaTime = 0.0f;
    float lastFrame = (float)SDL_GetTicks()/1000;

    int cursorState = 0;
    KhEngine::setCursorMode(window, cursorState);

    //game loop
    while(EXIT_FAILURE)
    {

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
                    }
                    break;
                case SDL_MOUSEWHEEL:
                {
                    fov -= (float)event.wheel.y;
                    if (fov < 1.0f)
                        fov = 1.0f;
                    if (fov > 45.0f)
                        fov = 45.0f;

                    projection = KhEngine::getProjection(fov);
                }
                    break;

            }

        } // -- while event in queue

        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // draw OpenGL
        float currentFrame = (float)SDL_GetTicks()/1000;
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        camera.tick(deltaTime);

        // get matrix's uniform location and set matrix
        auto view = camera.getViewMat4();

        glm::vec3 lightPos = glm::vec3(20*sin(currentFrame), 1.0f, 20*cos(currentFrame));
        ourLightSource.setPosition(lightPos);
        ourLightSource.setView(view);
        ourLightSource.setProjection(projection);

        ourLightSource2.setView(view);
        ourLightSource2.setProjection(projection);

        ourShader.use();

        ourShader.setVec3("material2.ambient", 1.0f, 0.5f, 0.31f);
        ourShader.setVec3("material2.specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("material2.shininess", 32.0f);
        ourShader.setFloat("material.shininess", 32.0f);

        glm::vec3 pointerLightPos;
        pointerLightPos.x = sin(currentFrame * 2.0f);
        pointerLightPos.y = sin(currentFrame * 0.7f);
        pointerLightPos.z = sin(currentFrame * 1.3f);
        ourLightSource2.setPosition(pointerLightPos);

        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);

        glm::mat3 normModel = transpose(inverse(model));
        ourShader.setMat3("normalModel", normModel);

        //directLight
        ourShader.setVec3("directLight.direction", 0.0f, -1.0f, -0.3f);
        ourShader.setVec3("directLight.color",     0.5f, 0.5f, 1.0f);
        ourShader.setVec3("directLight.ambient",   0.05f, 0.05f, 0.05f);
        ourShader.setVec3("directLight.diffuse",   0.12f, 0.12f, 0.12f); // darken diffuse light a bit
        ourShader.setVec3("directLight.specular",  0.25f, 0.25f, 0.25f);

        //pointLight
        ourShader.setVec3("pointLights[0].color", ourLightSource2.getColor());
        ourShader.setVec3("pointLights[0].position", ourLightSource2.getPosition());
        ourShader.setVec3("pointLights[0].direction", 0.0f, -1.0f, -0.3f);
        ourShader.setVec3("pointLights[0].ambient",  0.1f, 0.1f, 0.1f);
        ourShader.setVec3("pointLights[0].diffuse",  0.25f, 0.25f, 0.25f); // darken diffuse light a bit
        ourShader.setVec3("pointLights[0].specular", 0.5f, 0.5f, 0.5f);
        ourShader.setFloat("pointLights[0].constant",  1.0f);
        ourShader.setFloat("pointLights[0].linear",    0.022f);
        ourShader.setFloat("pointLights[0].quadratic", 0.0019f);

        //spotLight
        ourShader.setVec3("pointLights[0].color", ourLightSource.getColor());
        ourShader.setVec3("pointLights[0].position", ourLightSource.getPosition());
        ourShader.setVec3("pointLights[0].direction", 0.0f, -1.0f, -0.3f);
        ourShader.setVec3("pointLights[0].ambient",  0.2f, 0.2f, 0.2f);
        ourShader.setVec3("pointLights[0].diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("pointLights[0].constant",  1.0f);
        ourShader.setFloat("pointLights[0].linear",    0.022f);
        ourShader.setFloat("pointLights[0].quadratic", 0.0019f);
        ourShader.setFloat("pointLights[0].cutOff",   glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("pointLights[0].outerCutOff",   glm::cos(glm::radians(17.5f)));


        ourShader.setVec3("spotLights[0].color", ourLightSource2.getColor());
        ourShader.setVec3("spotLights[0].position", ourLightSource2.getPosition());
        ourShader.setVec3("spotLights[0].direction", 0.0f, -1.0f, -0.3f);
        ourShader.setVec3("spotLights[0].ambient",  0.2f, 0.2f, 0.2f);
        ourShader.setVec3("spotLights[0].diffuse",  0.5f, 0.5f, 0.5f); // darken diffuse light a bit
        ourShader.setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
        ourShader.setFloat("spotLights[0].constant",  1.0f);
        ourShader.setFloat("spotLights[0].linear",    0.022f);
        ourShader.setFloat("spotLights[0].quadratic", 0.0019f);
        ourShader.setFloat("spotLights[0].cutOff",   glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("spotLights[0].outerCutOff",   glm::cos(glm::radians(17.5f)));

        ourShader.setVec3("viewPos", camera.getPosition());

        ourModel.Draw(ourShader);
        //ourModel2.Draw(ourShader);
        ourLightSource.use();
        ourLightSource2.use();

        SDL_GL_SwapWindow(window);
    }

    ourModel.Destroy();

    SDL_GL_DeleteContext(context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

glm::mat4 KhEngine::getProjection(float fov) {
    return glm::perspective(glm::radians(fov), 1.0f * WIDTH / HEIGHT, 0.1f, 100.0f);
}

void KhEngine::setCursorMode(SDL_Window* window, int state) {
    SDL_ShowCursor(state);
    SDL_WarpMouseInWindow(window, WIDTH/2, HEIGHT/2);
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

