#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>
#include <vector>

struct ParticleData {
  sf::Vector2f position;
  sf::Vector2f velocity;
  float lifetime;
  float maxLifetime;
  sf::Color color;
  float baseSize;
  float currentSize;
};

class ParticleSystem {
public:
  ParticleSystem();
  void emit(sf::Vector2f pos, sf::Color color, int count = 1,
            bool rising = false, float size = 2.0f);
  void emitExplosion(sf::Vector2f pos, sf::Color color, int count = 50,
                     float speed = 100.0f, float size = 4.0f);
  void update(float dt);
  void render(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);

private:
  std::vector<ParticleData> particles;
};

#endif
