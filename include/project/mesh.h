//
// Created by Marharyta Haichuk on 09/01/2024.
//

#ifndef M_BOSSFIGHT_MESH_H
#define M_BOSSFIGHT_MESH_H

#include <glm.hpp>
#include <project/shader.h>

#define MAX_BONE_INFLUENCE 4

namespace KhEngine
{
    struct Vertex {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
        //bone indexes which will influence this vertex
        int m_BoneIDs[MAX_BONE_INFLUENCE];
        //weights from each bone
        float m_Weights[MAX_BONE_INFLUENCE];
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
        void Destroy();
    private:
        //  render data
        GLuint VAO, VBO, EBO;

        void setupMesh();
    };
}



#endif //M_BOSSFIGHT_MESH_H
