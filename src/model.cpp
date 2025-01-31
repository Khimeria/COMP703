//
// Created by Marharyta Haichuk on 09/01/2024.
//
#include "project/model.h"

namespace KhEngine
{
    Model::Model(std::string path)
    {
        Model::loadModel(std::move(path));
    }

    void Model::Draw(Shader &shader)
    {
        for(auto & mesh : meshes)
            mesh.Draw(shader);
    }

    void Model::loadModel(std::string relativePath)
    {
        Assimp::Importer import;
        auto cdw = std::filesystem::current_path().parent_path();
        const aiScene *scene = import.ReadFile(cdw/relativePath, aiProcess_Triangulate | aiProcess_FlipUVs);

        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return;
        }
        directory = relativePath.substr(0, relativePath.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    void Model::processNode(aiNode *node, const aiScene *scene)
    {
        // process all the node's meshes (if any)
        for(GLuint i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }
        // then do the same for each of its children
        for(GLuint i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        for(GLuint i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            // process vertex positions, normals and texture coordinates
            glm::vec3 vector;
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;

            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;

            if (scene->mNumMaterials > mesh->mMaterialIndex)
            {
                const auto& mat = scene->mMaterials[mesh->mMaterialIndex];
                auto material = loadMaterial(mat);

                vertex.shininess = material.Shininess;

                vertex.useTextures = mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 ? 1.0f : 0.0f;
                if(vertex.useTextures != 1.0f)
                {
                    vertex.color_diffuse = material.Diffuse;
                    vertex.color_specular = material.Specular;
                    vertex.color_height = material.Ambient;
                    vertex.color_normal = material.Diffuse;
                    vertex.color_emission = material.Emissive;
                }
                else
                {
                    auto uvChannelsNum = mesh->GetNumUVChannels();
                    aiVector3D coord;
                    if(uvChannelsNum > 0)
                    {
                        if (mesh->HasTextureCoords(aiTextureType_NONE)) {
                            coord = mesh->mTextureCoords[aiTextureType_NONE][i];
                            vertex.color_diffuse.x = coord.x;
                            vertex.color_diffuse.y = coord.y;
                            vertex.color_height.x = coord.x;
                            vertex.color_height.y = coord.y;
                            vertex.color_specular.x = coord.x;
                            vertex.color_specular.y = coord.y;
                            vertex.color_emission.x = coord.x;
                            vertex.color_emission.y = coord.y;
                            vertex.color_normal.x = coord.x;
                            vertex.color_normal.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_DIFFUSE)) {
                            coord = mesh->mTextureCoords[aiTextureType_DIFFUSE][i];
                            vertex.color_diffuse.x = coord.x;
                            vertex.color_diffuse.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_AMBIENT)) {
                            coord = mesh->mTextureCoords[aiTextureType_AMBIENT][i];
                            vertex.color_height.x = coord.x;
                            vertex.color_height.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_HEIGHT)) {
                            coord = mesh->mTextureCoords[aiTextureType_HEIGHT][i];
                            vertex.color_normal.x = coord.x;
                            vertex.color_normal.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_EMISSIVE)) {
                            coord = mesh->mTextureCoords[aiTextureType_EMISSIVE][i];
                            vertex.color_emission.x = coord.x;
                            vertex.color_emission.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_SPECULAR)) {
                            coord = mesh->mTextureCoords[aiTextureType_SPECULAR][i];
                            vertex.color_specular.x = coord.x;
                            vertex.color_specular.y = coord.y;
                        }
                        if (mesh->HasTextureCoords(aiTextureType_SHININESS)) {
                            coord = mesh->mTextureCoords[aiTextureType_SHININESS][i];
                            vertex.color_shininess.x = coord.x;
                            vertex.color_shininess.y = coord.y;
                        }
                    }
                }

                if(mesh->mBones){
                    //todo: for animation
                }
            }


            //calc tangent space
            // positions
            glm::vec3 pos1(-1.0,  1.0, 0.0);
            glm::vec3 pos2(-1.0, -1.0, 0.0);
            glm::vec3 pos3( 1.0, -1.0, 0.0);
            glm::vec3 pos4( 1.0,  1.0, 0.0);
            // texture coordinates
            glm::vec2 uv1(0.0, 1.0);
            glm::vec2 uv2(0.0, 0.0);
            glm::vec2 uv3(1.0, 0.0);
            glm::vec2 uv4(1.0, 1.0);
            // normal vector
            glm::vec3 nm(0.0, 0.0, 1.0);

            glm::vec3 edge1 = pos2 - pos1;
            glm::vec3 edge2 = pos3 - pos1;
            glm::vec2 deltaUV1 = uv2 - uv1;
            glm::vec2 deltaUV2 = uv3 - uv1;

            float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

            vertex.Tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
            vertex.Tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
            vertex.Tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

            vertex.Bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
            vertex.Bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
            vertex.Bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

            vertices.push_back(vertex);
        }
        // process indices
        for(GLuint i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            for(GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        // 1. diffuse maps
        std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        // 2. specular maps
        std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        // 3. normal maps
        std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        // 4. height maps
        std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::vector<Texture> emissionMaps = loadMaterialTextures(material, aiTextureType_EMISSIVE, "texture_emission");
        textures.insert(textures.end(), emissionMaps.begin(), emissionMaps.end());

        // return a mesh object created from the extracted mesh data

        return Mesh(vertices, indices, textures);
    }

    std::vector<Texture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
    {
        std::vector<Texture> textures;
        auto num = mat->GetTextureCount(type);
        for(GLuint i = 0; i < num; i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            bool skip = false;
            for(auto & j : textures_loaded)
            {
                if(std::strcmp(j.path.data(), str.C_Str()) == 0)
                {
                    textures.push_back(j);
                    skip = true;
                    break;
                }
            }
            if(!skip)
            {   // if texture hasn't been loaded already, load it
                Texture texture;
                loadTexture(&texture.id, directory, str.C_Str());
                texture.type = typeName;
                texture.path = str.C_Str();
                textures.push_back(texture);
                textures_loaded.push_back(texture); // add to loaded textures
            }
        }
        return textures;
    }

    void Model::Destroy() {
        for (auto &mesh: meshes) {
            mesh.Destroy();
        }
    }

    Material Model::loadMaterial(aiMaterial *mat)
    {
        Material material;
        aiColor3D color(0.f, 0.f, 0.f);
        float shininess;

        mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        material.Diffuse = glm::vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
        material.Ambient = glm::vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
        material.Specular = glm::vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_COLOR_EMISSIVE, color);
        material.Emissive = glm::vec3(color.r, color.b, color.g);

        mat->Get(AI_MATKEY_SHININESS, shininess);
        material.Shininess = shininess;

        return material;
    }

}
