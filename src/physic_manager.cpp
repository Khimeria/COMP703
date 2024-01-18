//
// Created by Marharyta Haichuk on 16/01/2024.
//

#include "project/physics/physic_manager.h"
namespace KhEngine{
    void KhEngine::PhysicEnvironment::AddForce(float accumulation, glm::vec3 direction)
    {
        Force force{accumulation,direction};
        environmentForces.push_back(force);
    }

    void KhEngine::PhysicEnvironment::AddForce(KhEngine::Force &force) {
        environmentForces.push_back(force);
    }

    void KhEngine::PhysicEnvironment::RemoveForce(KhEngine::Force &force) {
        for(int i =0;i<environmentForces.size();i++)
        {
            Force elem = environmentForces[i];
            if( elem.accumulation == force.accumulation &&
                elem.direction == force.direction)
            {
                RemoveForce( i);
                return;
            }
        }
    }

    void KhEngine::PhysicEnvironment::RemoveForce(int pos) {
        remove(environmentForces, pos);
    }

    KhEngine::PhysicObject *KhEngine::PhysicEnvironment::AddObject(KhEngine::GameObject *go, float mass) {
        objects.emplace_back(go,mass);
        auto item = &objects[objects.size()-1];
        return item;
    }

    void KhEngine::PhysicEnvironment::RemoveObject(KhEngine::GameObject *go, float mass) {
        for(int i = 0; i< objects.size(); i++)
        {
            if(objects[i].Object == go &&
               objects[i].mass == mass)
            {
                remove(objects,i);
                return;
            }
        }
    }

    KhEngine::PhysicObject &KhEngine::PhysicEnvironment::getObject(int i) {
        return objects.at(i);
    }

    void KhEngine::PhysicEnvironment::calcResultantForce(KhEngine::PhysicObject &obj) {
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

    void KhEngine::PhysicEnvironment::tick(float deltaTime) {
        deltaTime /= 2.0f;
        time += deltaTime;

        for(int i=0; i<objects.size();i++)
        {
            float velocityTime = time;

            if(objects[i].Object->transform.Position.y <= -1.0f)
            {
                //hit plane
                float accumulation = 0.65f * objects[i].force.accumulation;

                glm::vec3 planeNormal = glm::vec3(0.0f,1.0f,0.0f);
                auto cos = glm::dot(objects[i].force.direction,planeNormal);
                glm::vec3 reflectDir = objects[i].force.direction - 2*cos*planeNormal;

                objects[i].addForce(accumulation, reflectDir);
            }

            calcResultantForce(objects[i]);

            objects[i].velocity = objects[i].startVelocity + objects[i].force.accumulation * velocityTime / objects[i].mass;

            auto r = objects[i].velocity * deltaTime * objects[i].force.direction;
            objects[i].Object->transform.Position = max(objects[i].Object->transform.Position + r, glm::vec3(0.0f));
        }
    }

    void KhEngine::PhysicEnvironment::Destroy()
    {
        objects.clear();
        environmentForces.clear();
    }

    template<typename T>
    void KhEngine::PhysicEnvironment::remove(std::vector<T> &vec, std::size_t pos) {
        typename std::vector<T>::iterator it = vec.begin();
        std::advance(it, pos);
        vec.erase(it);
    }

    KhEngine::PhysicObject::PhysicObject(KhEngine::GameObject *go, float mass) : Object(go), mass(mass)
    {
        force.accumulation = 0.0f;
        force.direction = glm::vec3(0.0f);
        startVelocity = 0.0f;
        velocity = 0.0f;
    }

    void PhysicObject::addForce(float accumulation, glm::vec3 direction) {
        force.accumulation = accumulation;
        force.direction = direction;
    }
}


