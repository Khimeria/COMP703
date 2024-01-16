//
// Created by Marharyta Haichuk on 16/01/2024.
//

#ifndef M_BOSSFIGHT_PHYSIC_MANAGER_H
#define M_BOSSFIGHT_PHYSIC_MANAGER_H
#include <project/gameObject/game_object.h>
#include <SDL.h>

namespace KhEngine
{
    struct Force
    {
        //float magnitude;
        float accumulation;
        glm::vec3 direction;
    };

    struct PhysicObject
    {
    public:
        GameObject* Object;
        float mass;
        float velocity;
        float startVelocity;
        float startTime;
        Force force;


        PhysicObject(GameObject* go, float mass): Object(go), mass(mass)
        {
            force.accumulation = 0.0f;
            force.direction = glm::vec3(0.0f);
            startVelocity = 0.0f;
            velocity = 0.0f;
        }

        void addForce(float accumulation, glm::vec3 direction){
            auto exist = mass * force.accumulation * force.direction;
            auto newOne = mass * accumulation * direction;
            auto resultForce = exist + newOne;

            auto dotProduct = glm::dot(glm::normalize(exist),glm::normalize(newOne));
            this->force.accumulation = (float)std::sqrt(pow(force.accumulation, 2) + pow(accumulation, 2) +
                    2 * force.accumulation * accumulation * dotProduct);
            auto test = (float)std::sqrt(resultForce.x*resultForce.x+resultForce.y*resultForce.y+resultForce.z*resultForce.z)/mass;
            if(force.accumulation == test)
                std::cout<<"WOW"<<std::endl;
            force.direction = glm::normalize(resultForce);
        }
    };

    class PhysicEnvironment
    {
    public:
        std::vector<Force> environmentForces;

        PhysicEnvironment(){
        }

        void AddForce(float accumulation, glm::vec3 direction)
        {
            Force force{accumulation,direction};
            environmentForces.push_back(force);
        }

        void AddObject(GameObject* go, float mass)
        {
            objects.emplace_back(go,mass);
        }

        void calcResultantForce(PhysicObject& obj)
        {
            auto oldForce = obj.force;
            glm::vec3 resultant = obj.mass * obj.force.accumulation * obj.force.direction;
            for(auto force: environmentForces)
            {
                resultant += obj.mass * force.accumulation * force.direction;
            }
            auto resMagnitude = std::sqrt(resultant.x * resultant.x + resultant.y * resultant.y + resultant.z * resultant.z);
            obj.force.accumulation = resMagnitude/obj.mass;
            obj.force.direction = glm::normalize(resultant);
        }

        void tick(float deltaTime){
            deltaTime /=360.0f;
            time += deltaTime;

            for(int i=0; i<objects.size();i++)
            {
                float velocityTime = time - objects[i].startTime;

                if(objects[i].Object->transform.Position.y <= 0.0f)
                {
                    //hit plane
                    float accumulation = (objects[i].velocity - objects[i].startVelocity) / velocityTime;

                    glm::vec3 planeNormal = glm::vec3(0.0f,1.0f,0.0f);
                    auto cos = glm::dot(objects[i].force.direction,planeNormal);
                    glm::vec3 reflectDir = objects[i].force.direction - 2*cos*planeNormal;

                    objects[i].addForce(0.8f * accumulation, reflectDir);

                    objects[i].startVelocity = objects[i].velocity;
                    objects[i].startTime = time;
                }

                calcResultantForce(objects[i]);

                objects[i].velocity = objects[i].startVelocity + objects[i].force.accumulation * velocityTime / objects[i].mass;

                auto r = objects[i].velocity * velocityTime * objects[i].force.direction;
                objects[i].Object->transform.Position = max(objects[i].Object->transform.Position + r, glm::vec3(0.0f));
            }
        }

    private:
        std::vector<PhysicObject> objects;
        float time = 0;
    };
}


#endif //M_BOSSFIGHT_PHYSIC_MANAGER_H
