//
// Created by Marharyta Haichuk on 09/01/2024.
//

#ifndef M_BOSSFIGHT_MODEL_H
#define M_BOSSFIGHT_MODEL_H

#include <project/shader.h>
#include <project/mesh.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include "project/gameObject/game_object.h"
#include "project/main.hpp"
#include <assimp/Importer.hpp>
#include <utility>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <SDL.h>

namespace KhEngine
{
    class Model
    {
        public:
            Model(std::string path);
            void Draw(Shader &shader);
            void Destroy();

        private:
            // model data
            std::vector<Texture> textures_loaded;
            std::vector<Mesh> meshes;
            std::string directory;

            void loadModel(std::string relativePath);
            void processNode(aiNode *node, const aiScene *scene);
            Mesh processMesh(aiMesh *mesh, const aiScene *scene);
            std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
            Material loadMaterial(aiMaterial* mat);
    };
}

#endif //M_BOSSFIGHT_MODEL_H
