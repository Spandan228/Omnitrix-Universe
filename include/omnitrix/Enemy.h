#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>

enum class EnemyType { Normal, Scout, Tank };

class Enemy {
public:
  Enemy(sf::Vector2f startPos, const sf::Texture &tex,
        EnemyType type = EnemyType::Normal);

  void update(float dt, sf::Vector2f playerPos);
  void render(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);

  void takeDamage(int amount);
  bool isDead() const;
  sf::FloatRect getBounds() const;
  int getHealth() const;
  int getMaxHealth() const;
  sf::Vector2f getPosition() const;

private:
  sf::Sprite sprite;
  sf::RectangleShape fallbackShape;
  int hp;
  int maxHp;
  float speed;
  float attackCooldown;

  float walkTimer;
  float baseScale;

  float pulseTime;
};

#endif
