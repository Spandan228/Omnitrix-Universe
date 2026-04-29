#include "omnitrix/Particle.h"
#include <cmath>
#include <cstdlib>


ParticleSystem::ParticleSystem() {}

void ParticleSystem::emit(sf::Vector2f pos, sf::Color color, int count,
                          bool rising, float size) {
  for (int i = 0; i < count; i++) {
    ParticleData p;
    p.position = pos;

    float vx = ((rand() % 100) - 50) / 1.0f;
    float vy = ((rand() % 100) - 50) / 1.0f;

    if (rising) {
      vx /= 2.0f;
      vy = -(rand() % 100 + 50); // fast upwards
    }

    p.velocity = sf::Vector2f(vx, vy);
    p.maxLifetime = 0.5f + (rand() % 50) / 100.0f;
    p.lifetime = p.maxLifetime;
    p.color = color;
    p.baseSize = size;
    p.currentSize = size;
    particles.push_back(p);
  }
}

void ParticleSystem::emitExplosion(sf::Vector2f pos, sf::Color color, int count,
                                   float speed, float size) {
  for (int i = 0; i < count; i++) {
    ParticleData p;
    p.position = pos;

    float angle = (rand() % 360) * 3.14159f / 180.0f;
    float actualSpeed = speed * (0.5f + (rand() % 50) / 100.0f);

    p.velocity = sf::Vector2f(std::cos(angle) * actualSpeed,
                              std::sin(angle) * actualSpeed);
    p.maxLifetime = 0.5f + (rand() % 100) / 100.0f;
    p.lifetime = p.maxLifetime;
    p.color = color;
    p.baseSize = size;
    p.currentSize = size;
    particles.push_back(p);
  }
}

void ParticleSystem::update(float dt) {
  for (auto it = particles.begin(); it != particles.end();) {
    it->lifetime -= dt;
    if (it->lifetime <= 0) {
      it = particles.erase(it);
    } else {
      it->position += it->velocity * dt;
      it->currentSize =
          it->baseSize * (it->lifetime / it->maxLifetime); // shrink over time
      it++;
    }
  }
}

void ParticleSystem::render(sf::RenderWindow &window) {
  for (const auto &p : particles) {
    sf::CircleShape circle(p.currentSize);
    circle.setOrigin(p.currentSize, p.currentSize);
    circle.setPosition(p.position);
    sf::Color c = p.color;
    c.a = static_cast<sf::Uint8>(255.0f * (p.lifetime / p.maxLifetime));
    circle.setFillColor(c);
    // Use additive blending for glowing effect
    window.draw(circle, sf::BlendAdd);
  }
}
void ParticleSystem::draw(sf::RenderWindow &window) { render(window); }
