#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H
#include <algorithm>
#include <memory>
#include <vector>
#include "Common.h"
#include "Component.h"
#include "Rect.h"

class Component;

class GameObject {
  public:
    GameObject(Common::Layer layer = Common::Layer::DEFAULT)
        : layer(layer), isDead(false) {}
    ~GameObject();

    void Update(float dt);
    void RhythmUpdate();
    void RhythmReset();

    void RenderOrder(Common::Layer layer) const;

    bool CanEnd() const;

    bool IsDead() const { return isDead; }
    void RequestDelete() { isDead = true; }
    void AddComponent(Component *cpt);
    void RemoveComponent(Component *cpt);

    Component *GetComponent(const std::string &type) const;
    void Start();
    void NotifyCollision(std::shared_ptr<GameObject> other);
    void CopyPosition(const GameObject &go);

    double AngleRad() const { return angleDeg * M_PI / 180; }

    template <class T>
    T GetComponent() const {
        for (Component *component : components)
            if (T t = dynamic_cast<T>(component)) return t;

        return nullptr;
    }

    Rect box;
    bool worldReference = true;
    bool fromPlayer = false;
    bool started = false;
    double angleDeg = 0;
    Common::Layer layer = Common::Layer::DEFAULT;

  private:
    std::vector<Component *> components;
    bool isDead;
};

#endif  // GAMEOBJECT_H
