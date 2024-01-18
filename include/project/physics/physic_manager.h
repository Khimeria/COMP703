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
        float startTime{};
        Force force{};

        PhysicObject(GameObject* go, float mass);
        void addForce(float accumulation, glm::vec3 direction);
    };

    class PhysicEnvironment: public IDestroyable
    {
    public:
        std::vector<Force> environmentForces;

        PhysicEnvironment(){}
        void AddForce(float accumulation, glm::vec3 direction);
        void AddForce(Force& force);
        void RemoveForce(Force& force);
        void RemoveForce(int pos);
        template <typename T>
        void remove(std::vector<T>& vec, std::size_t pos);

        PhysicObject* AddObject(GameObject* go, float mass);

        void RemoveObject(GameObject* go, float mass);
        void calcResultantForce(PhysicObject& obj);
        void tick(float deltaTime);
        void Destroy() override;

        PhysicObject& getObject(int i);

    private:
        std::vector<PhysicObject> objects;
        float time = 0;
    };
}


#endif //M_BOSSFIGHT_PHYSIC_MANAGER_H
