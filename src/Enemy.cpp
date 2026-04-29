#include "omnitrix/Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(sf::Vector2f pos, const sf::Texture &tex, EnemyType type) {
  if (type == EnemyType::Scout) {
    hp = 20;
    maxHp = 20;
    speed = 180.0f;
    fallbackShape.setSize(sf::Vector2f(20, 20));
    fallbackShape.setFillColor(sf::Color(255, 200, 200, 150));
  } else if (type == EnemyType::Tank) {
    hp = 200;
    maxHp = 200;
    speed = 50.0f;
    fallbackShape.setSize(sf::Vector2f(60, 60));
    fallbackShape.setFillColor(sf::Color(100, 0, 0, 150));
  } else { // Normal enemy
    hp = 50;
    maxHp = 50;
    speed = 100.0f;
    fallbackShape.setSize(sf::Vector2f(40, 40));
    fallbackShape.setFillColor(sf::Color(200, 0, 0, 150));
  }

  if (tex.getSize().x > 0) {
    sprite.setTexture(tex, true);
    sprite.setOrigin(tex.getSize().x / 2.0f, tex.getSize().y / 2.0f);
    float scale = (type == EnemyType::Tank)
                      ? 0.25f
                      : (type == EnemyType::Scout ? 0.1f : 0.18f);
    baseScale = scale;
    sprite.setScale(scale, scale);
  } else {
    baseScale = 1.0f;
  }

  walkTimer = 0.0f;

  fallbackShape.setOrigin(fallbackShape.getSize().x / 2.0f,
                          fallbackShape.getSize().y / 2.0f);
  fallbackShape.setPosition(pos);
  sprite.setPosition(pos);
  attackCooldown = 0.0f;

  pulseTime = 0.0f;
}

void Enemy::update(float dt, sf::Vector2f playerPos) {
  if (hp <= 0)
    return;

  sf::Vector2f currentPos = getPosition();
  sf::Vector2f dir = playerPos - currentPos;
  float dist = std::sqrt(dir.x * dir.x + dir.y * dir.y);

  if (dist > 0) {
    dir /= dist;
    if (dist > 40.0f) { // move towards player if not too close
      sf::Vector2f moveVec = dir * speed * dt;
      sprite.move(moveVec);
      fallbackShape.move(moveVec);

      // Walk Animation (Swaying and Bobbing)
      walkTimer += dt * 8.0f;
      float tilt = std::sin(walkTimer) * 8.0f;
      float bob = 1.0f + std::abs(std::sin(walkTimer)) * 0.08f;

      sprite.setRotation(tilt);
      sprite.setScale(baseScale, baseScale * bob);
    } else {
      sprite.setRotation(0.0f);
      sprite.setScale(baseScale, baseScale);
    }
  }

  pulseTime += dt;

  if (attackCooldown > 0)
    attackCooldown -= dt;
}

void Enemy::render(sf::RenderWindow &window) {
  if (hp > 0) {
    if (sprite.getTexture()) {
      window.draw(sprite);
    } else {
      window.draw(fallbackShape);
    }
  }
}

void Enemy::takeDamage(int amount) {
  hp -= amount;
  if (hp < 0)
    hp = 0;
}

bool Enemy::isDead() const { return hp <= 0; }

sf::FloatRect Enemy::getBounds() const {
  if (sprite.getTexture())
    return sprite.getGlobalBounds();
  return fallbackShape.getGlobalBounds();
}

int Enemy::getHealth() const { return hp; }
int Enemy::getMaxHealth() const { return maxHp; }

sf::Vector2f Enemy::getPosition() const {
  if (sprite.getTexture())
    return sprite.getPosition();
  return fallbackShape.getPosition();
}
void Enemy::draw(sf::RenderWindow &window) { render(window); }
