//
// Created by Marharyta Haichuk on 09/01/2024.
//

#ifndef M_BOSSFIGHT_MODEL_H
#define M_BOSSFIGHT_MODEL_H

#include <project/shader.h>
#include <project/mesh.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/material.h>

namespace KhEngine
{
    class Model
    {
    public:
        Model(char *path);
        void Draw(Shader &shader);
    private:
        // model data
        std::vector<Texture> textures_loaded;
        std::vector<Mesh> meshes;
        std::string directory;

        void loadModel(std::string path);
        void processNode(aiNode *node, const aiScene *scene);
        Mesh processMesh(aiMesh *mesh, const aiScene *scene);
        std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type,
                                             std::string typeName);
    };
}

#endif //M_BOSSFIGHT_MODEL_H
