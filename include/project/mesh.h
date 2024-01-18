//
// Created by Marharyta Haichuk on 09/01/2024.
//

#ifndef M_BOSSFIGHT_MESH_H
#define M_BOSSFIGHT_MESH_H

#include <project/shader.h>
#include <utility>
#include <string>
#include <detail/type_vec3.hpp>
#include <detail/type_vec2.hpp>

#define MAX_BONE_INFLUENCE 4

namespace KhEngine
{
    struct TextureCoord
    {
        glm::vec2 texture_diffuse_coord;
        glm::vec2 texture_specular_coord;
        glm::vec2 texture_normal_coord;
        glm::vec2 texture_height_coord;
        glm::vec2 texture_emission_coord;
        glm::vec2 texture_shininess_coord;
    };

    struct Material {
        glm::vec3 Diffuse;
        glm::vec3 Specular;
        glm::vec3 Ambient;
        glm::vec3 Emissive;
        float Shininess;
    };

    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        float useTextures;
        glm::vec3  color_diffuse;
        glm::vec3  color_specular;
        glm::vec3  color_normal;
        glm::vec3  color_height;
        glm::vec3  color_emission;
        glm::vec3  color_shininess;
        float shininess;
    };

    struct Texture {
        unsigned int id;
        std::string type;
        std::string path;
    };

    class Mesh {
    public:
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;

        Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);
        void Draw(Shader &shader);
        void Destroy();
    private:
        GLuint VAO, VBO, EBO;

        void setupMesh();

    };
}



#endif //M_BOSSFIGHT_MESH_H
