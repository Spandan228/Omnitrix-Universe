#ifndef ALIEN_H
#define ALIEN_H

#include <SFML/Graphics.hpp>
#include <map>
#include <string>
#include <vector>

struct Projectile {
  sf::ConvexShape convexShape;
  sf::RectangleShape rectShape;
  sf::CircleShape circleShape;
  int type; // 0: Rect, 1: Circle, 2: Convex
  sf::Vector2f velocity;
  float lifetime;
  int damage;
  bool active;
};

class Alien {
public:
  Alien();

  void loadUnlocks();
  void saveUnlocks();
  void recordUsage(const std::string &alienName);
  std::string checkNewUnlocks();

  void setForm(const std::string &alienName);
  void triggerTransformation(const std::string &alienName);
  std::string getCurrentForm() const;
  void update(float dt, sf::Vector2f inputDir);
  void render(sf::RenderWindow &window);

  void attack(sf::Vector2f direction);

  void updateProjectiles(float dt);
  void renderProjectiles(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);

  std::vector<Projectile> &getProjectiles();

  sf::Vector2f getPosition() const;
  sf::FloatRect getBounds() const;
  void setPosition(sf::Vector2f pos);

  std::vector<std::string> getUnlockedAliens() const;
  void setCheatUnlocks(bool active);

private:
  std::string currentForm;
  sf::Sprite sprite;
  sf::Texture
      textures[6]; // Human, Heatblast, Four Arms, XLR8, Diamondhead, Upgrade

  // Fallback primitives for when textures fail
  sf::RectangleShape fallbackSprite;
  float pulseTime;
  float walkTimer;

  // Transformation Bash
  bool isTransforming;
  bool isReverting;
  float transformationTimer;
  float revertTimer;
  std::string targetForm;
  sf::CircleShape bashEffect;

  float speed;

  float attackCooldown;
  float currentCooldown;

  std::vector<Projectile> projectiles;

  std::map<std::string, bool> unlocked;
  std::map<std::string, int> usageCount;

  std::string getFilePath() const;
};

#endif
