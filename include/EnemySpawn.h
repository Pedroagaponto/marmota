#ifndef ENEMYSPAWN_H
#define ENEMYSPAWN_H
#define INCLUDE_SDL
#include "SDL_include.h"

#include <iostream>
#include <queue>
#include <string>
#include "Component.h"
#include "InputManager.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Vec2.h"

class EnemySpawn : public Component {
  public:
    EnemySpawn(GameObject &associated, TileMap *tileMap)
        : Component(associated), tileMap(tileMap) {}

    ~EnemySpawn() {}

    void Update(float dt);
    void RhythmUpdate() {}
    void RhythmReset() {}
    void Render(Common::Layer layer) const {}

  private:
    TileMap *tileMap;
    int currentY = 0;
};

#endif  // ENEMYSPAWN_H
