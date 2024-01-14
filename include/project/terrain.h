//
// Created by Marharyta Haichuk on 14/01/2024.
//

#ifndef M_BOSSFIGHT_TERRAIN_H
#define M_BOSSFIGHT_TERRAIN_H

#include "model_go.h"

namespace KhEngine
{
    class TerrainGameObject: public ModelGameObject
    {
    public:
        TerrainGameObject(Shader& shader): ModelGameObject("models/Scene/Scene.obj", shader)
        {
        }
    };
}
#endif //M_BOSSFIGHT_TERRAIN_H
