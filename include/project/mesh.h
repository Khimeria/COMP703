//
// Created by Marharyta Haichuk on 09/01/2024.
//

#ifndef M_BOSSFIGHT_MESH_H
#define M_BOSSFIGHT_MESH_H

#include <glm.hpp>
#include <project/shader.h>

namespace KhEngine
{
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
        void Draw(Shader &shader);
    private:
        //  render data
        GLuint VAO, VBO, EBO;

        void setupMesh();
    };
}



#endif //M_BOSSFIGHT_MESH_H
