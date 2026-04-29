#include "omnitrix/Alien.h"
#include <cmath>
#include <fstream>
#include <iostream>

Alien::Alien() {
  currentForm = "Human";

  // Helper to load texture with robust fuzzy transparency mask
  auto loadMasked = [&](sf::Texture &tex, const std::string &path) {
    sf::Image img;
    if (img.loadFromFile(path)) {
      // Robust masking: createMaskFromColor for pure white
      img.createMaskFromColor(sf::Color::White);
      // Plus fuzzy masking for noise
      sf::Vector2u size = img.getSize();
      for (unsigned int x = 0; x < size.x; x++) {
        for (unsigned int y = 0; y < size.y; y++) {
          sf::Color pixel = img.getPixel(x, y);
          if (pixel.r > 230 && pixel.g > 230 && pixel.b > 230) {
            img.setPixel(x, y, sf::Color::Transparent);
          }
        }
      }
      tex.loadFromImage(img);
      std::cout << "SUCCESS: Loaded texture " << path << " (" << tex.getSize().x
                << "x" << tex.getSize().y << ")" << std::endl
                << std::flush;
    } else {
      std::cout << "ERROR: Failed to load texture: " << path << std::endl
                << std::flush;
    }
  };

  isReverting = false;
  revertTimer = 0.0f;

  // Load Textures
  loadMasked(textures[0], "assets/textures/player/player_human.png");
  loadMasked(textures[1], "assets/textures/player/heatblast.png");
  loadMasked(textures[2], "assets/textures/player/four_arms.png");
  loadMasked(textures[3], "assets/textures/player/xlr8.png");
  loadMasked(textures[4], "assets/textures/player/diamondhead.png");
  loadMasked(textures[5], "assets/textures/player/upgrade.png");

  if (textures[0].getSize().x > 0) {
    sprite.setTexture(textures[0]);
    sprite.setOrigin(textures[0].getSize().x / 2.0f,
                     textures[0].getSize().y / 2.0f);
    sprite.setScale(0.15f, 0.15f);
  }
  sprite.setPosition(400, 300);

  fallbackSprite.setSize(sf::Vector2f(40, 40));
  fallbackSprite.setOrigin(20, 20);
  fallbackSprite.setPosition(400, 300);

  pulseTime = 0.0f;
  walkTimer = 0.0f;

  isTransforming = false;
  transformationTimer = 0.0f;
  bashEffect.setRadius(0.0f);
  bashEffect.setFillColor(sf::Color(0, 255, 0, 150));
  bashEffect.setOutlineThickness(2);
  bashEffect.setOutlineColor(sf::Color::White);

  unlocked["Human"] = true;
  unlocked["Heatblast"] = true;
  unlocked["Four Arms"] = true;
  unlocked["XLR8"] = false;
  unlocked["Diamondhead"] = false;
  unlocked["Upgrade"] = false;

  // Default to human
  setForm("Human");

  loadUnlocks();
}

std::string Alien::getFilePath() const { return "unlocks.txt"; }

std::string Alien::getCurrentForm() const { return currentForm; }

void Alien::loadUnlocks() {
  std::ifstream file(getFilePath());
  if (file.is_open()) {
    std::string name;
    int count;
    while (file >> name >> count) {
      usageCount[name] = count;
    }
    file.close();
  }
  // Re-evaluate unloads based on loaded counts
  if (usageCount["Heatblast"] >= 5)
    unlocked["XLR8"] = true;
  if (usageCount["Four Arms"] >= 5)
    unlocked["Diamondhead"] = true;
  if (usageCount["XLR8"] >= 5)
    unlocked["Upgrade"] = true;
}

void Alien::saveUnlocks() {
  std::ofstream file(getFilePath());
  for (auto const &[name, count] : usageCount) {
    file << name << " " << count << "\n";
  }
  file.close();
}

void Alien::recordUsage(const std::string &alienName) {
  if (alienName != "Human") {
    usageCount[alienName]++;
    saveUnlocks();
  }
}

std::string Alien::checkNewUnlocks() {
  std::string newlyUnlocked = "";
  if (usageCount["Heatblast"] >= 5 && !unlocked["XLR8"]) {
    unlocked["XLR8"] = true;
    newlyUnlocked = "XLR8";
  } else if (usageCount["Four Arms"] >= 5 && !unlocked["Diamondhead"]) {
    unlocked["Diamondhead"] = true;
    newlyUnlocked = "Diamondhead";
  } else if (usageCount["XLR8"] >= 5 && !unlocked["Upgrade"]) {
    unlocked["Upgrade"] = true;
    newlyUnlocked = "Upgrade";
  }
  return newlyUnlocked;
}

std::vector<std::string> Alien::getUnlockedAliens() const {
  std::vector<std::string> list;
  for (auto const &[name, isUnl] : unlocked) {
    if (isUnl && name != "Human")
      list.push_back(name);
  }
  return list;
}

void Alien::setForm(const std::string &alienName) {
  bool wasAlien = (currentForm != "Human");
  currentForm = alienName;
  bool isAlien = (currentForm != "Human");

  // Reset timers on form change
  if (isAlien && !wasAlien) {
    isTransforming = true;
    transformationTimer = 0.5f;
  } else if (!isAlien && wasAlien) {
    isReverting = true;
    revertTimer = 0.5f;
  }

  int texIdx = 0;
  if (alienName == "Human")
    texIdx = 0;
  else if (alienName == "Heatblast")
    texIdx = 1;
  else if (alienName == "Four Arms")
    texIdx = 2;
  else if (alienName == "XLR8")
    texIdx = 3;
  else if (alienName == "Diamondhead")
    texIdx = 4;
  else if (alienName == "Upgrade")
    texIdx = 5;

  if (textures[texIdx].getSize().x > 0) {
    sprite.setTexture(textures[texIdx], true);
    sprite.setOrigin(textures[texIdx].getSize().x / 2.0f,
                     textures[texIdx].getSize().y / 2.0f);
    sprite.setScale(0.15f, 0.15f);
  }

  if (alienName == "Human") {
    fallbackSprite.setFillColor(sf::Color::White);
    speed = 100.0f;
    attackCooldown = 0.5f;
  } else if (alienName == "Heatblast") {
    fallbackSprite.setFillColor(sf::Color(255, 100, 0, 150));
    speed = 120.0f;
    attackCooldown = 0.3f;
  } else if (alienName == "Four Arms") {
    fallbackSprite.setFillColor(sf::Color(200, 0, 0, 150));
    speed = 80.0f;
    attackCooldown = 0.6f;
  } else if (alienName == "XLR8") {
    fallbackSprite.setFillColor(sf::Color(0, 100, 255, 150));
    speed = 400.0f;
    attackCooldown = 0.1f;
  } else if (alienName == "Diamondhead") {
    fallbackSprite.setFillColor(sf::Color(0, 255, 255, 100));
    speed = 100.0f;
    attackCooldown = 0.4f;
  } else if (alienName == "Upgrade") {
    fallbackSprite.setFillColor(sf::Color(0, 200, 50, 150));
    speed = 140.0f;
    attackCooldown = 0.3f;
  }

  if (currentForm != "Human") {
    currentCooldown = 0.0f;
  }
}

void Alien::triggerTransformation(const std::string &alienName) {
  if (isTransforming || currentForm == alienName)
    return;
  isTransforming = true;
  transformationTimer = 0.5f;
  targetForm = alienName;
}

void Alien::update(float dt, sf::Vector2f inputDir) {
  if (isTransforming) {
    transformationTimer -= dt;
    float progress = 1.0f - (transformationTimer / 0.5f);

    float bashOffset = std::sin(progress * 3.14159f) * 30.0f;
    sprite.setOrigin(sprite.getOrigin().x,
                     textures[0].getSize().y / 2.0f - bashOffset);

    if (progress > 0.5f) {
      bashEffect.setRadius((progress - 0.5f) * 200.0f);
      bashEffect.setOrigin(bashEffect.getRadius(), bashEffect.getRadius());
      bashEffect.setPosition(sprite.getPosition());
    }

    if (transformationTimer <= 0) {
      isTransforming = false;
      setForm(targetForm);
      bashEffect.setRadius(0.0f);
      int texIdx = 0;
      if (currentForm == "Human")
        texIdx = 0;
      else if (currentForm == "Heatblast")
        texIdx = 1;
      else if (currentForm == "Four Arms")
        texIdx = 2;
      else if (currentForm == "XLR8")
        texIdx = 3;
      else if (currentForm == "Diamondhead")
        texIdx = 4;
      else if (currentForm == "Upgrade")
        texIdx = 5;
      sprite.setOrigin(textures[texIdx].getSize().x / 2.0f,
                       textures[texIdx].getSize().y / 2.0f);
    }
    return;
  }

  if (isReverting) {
    revertTimer -= dt;
    if (revertTimer <= 0)
      isReverting = false;
  }

  if (inputDir.x != 0 || inputDir.y != 0) {
    float len = std::sqrt(inputDir.x * inputDir.x + inputDir.y * inputDir.y);
    inputDir /= len;
    sf::Vector2f moveVec = inputDir * speed * dt;
    sprite.move(moveVec);
    fallbackSprite.move(moveVec);

    walkTimer += dt * 10.0f;
    float tilt = std::sin(walkTimer) * 8.0f;
    float bob = 1.0f + std::abs(std::sin(walkTimer)) * 0.08f;
    sprite.setRotation(tilt);
    sprite.setScale(0.15f, 0.15f * bob);
  } else {
    sprite.setRotation(0.0f);
    sprite.setScale(0.15f, 0.15f);
  }

  pulseTime += dt;
  if (currentCooldown > 0)
    currentCooldown -= dt;
}

void Alien::render(sf::RenderWindow &window) {
  if (isTransforming) {
    float s = 0.15f + std::abs(std::sin(transformationTimer * 20.0f)) * 0.1f;
    sprite.setScale(s, s);
    if (bashEffect.getRadius() > 0)
      window.draw(bashEffect);
  }

  if (isReverting) {
    float s = 0.15f * (revertTimer / 0.5f);
    sprite.setScale(s, s);
    sf::CircleShape rev(60.0f * (0.5f - revertTimer) / 0.5f);
    rev.setOrigin(rev.getRadius(), rev.getRadius());
    rev.setPosition(sprite.getPosition());
    rev.setFillColor(sf::Color::White);
    rev.setOutlineColor(sf::Color::Red);
    rev.setOutlineThickness(2);
    window.draw(rev);
  }

  int texIdx = 0;
  if (currentForm == "Human")
    texIdx = 0;
  else if (currentForm == "Heatblast")
    texIdx = 1;
  else if (currentForm == "Four Arms")
    texIdx = 2;
  else if (currentForm == "XLR8")
    texIdx = 3;
  else if (currentForm == "Diamondhead")
    texIdx = 4;
  else if (currentForm == "Upgrade")
    texIdx = 5;

  if (textures[texIdx].getSize().x > 0) {
    window.draw(sprite);
  } else {
    window.draw(fallbackSprite);
  }
}

void Alien::attack(sf::Vector2f dir) {
  if (isTransforming)
    return;
  if (currentCooldown <= 0) {
    sf::Vector2f pos = sprite.getPosition();
    float len = std::sqrt(dir.x * dir.x + dir.y * dir.y);
    if (len > 0) {
      dir /= len;

      auto createProjectile = [&](sf::Vector2f shootDir) {
        Projectile p;
        p.active = true;
        p.velocity = shootDir;
        float angle = std::atan2(shootDir.y, shootDir.x);

        if (currentForm == "Human") {
          p.type = 0;
          p.rectShape.setSize(sf::Vector2f(10, 10));
          p.rectShape.setOrigin(5, 5);
          p.rectShape.setFillColor(sf::Color::White);
          p.velocity *= 400.0f;
          p.damage = 10;
          p.lifetime = 1.0f;
        } else if (currentForm == "Heatblast") {
          p.type = 2;
          p.convexShape.setPointCount(3);
          p.convexShape.setPoint(0, sf::Vector2f(0, -15));
          p.convexShape.setPoint(1, sf::Vector2f(-8, 8));
          p.convexShape.setPoint(2, sf::Vector2f(8, 8));
          p.convexShape.setFillColor(sf::Color::Yellow);
          p.convexShape.setOutlineThickness(2);
          p.convexShape.setOutlineColor(sf::Color(255, 100, 0));
          p.velocity *= 500.0f;
          p.damage = 25;
          p.lifetime = 1.2f;
          p.convexShape.setRotation(angle * 180.0f / 3.14159f + 90.0f);
        } else if (currentForm == "Four Arms") {
          p.type = 0;
          p.rectShape.setSize(sf::Vector2f(45, 45));
          p.rectShape.setOrigin(22.5, 22.5);
          p.rectShape.setFillColor(sf::Color(200, 30, 0));
          p.rectShape.setOutlineThickness(4);
          p.rectShape.setOutlineColor(sf::Color::Black);
          p.velocity *= 200.0f;
          p.damage = 100;
          p.lifetime = 0.6f;
        } else if (currentForm == "XLR8") {
          p.type = 0;
          p.rectShape.setSize(sf::Vector2f(50, 6));
          p.rectShape.setOrigin(25, 3);
          p.rectShape.setFillColor(sf::Color(0, 255, 255));
          p.velocity *= 1500.0f;
          p.damage = 20;
          p.lifetime = 0.4f;
          p.rectShape.setRotation(angle * 180.0f / 3.14159f);
        } else if (currentForm == "Diamondhead") {
          p.type = 2;
          p.convexShape.setPointCount(4);
          p.convexShape.setPoint(0, sf::Vector2f(20, 0));
          p.convexShape.setPoint(1, sf::Vector2f(0, 8));
          p.convexShape.setPoint(2, sf::Vector2f(-10, 0));
          p.convexShape.setPoint(3, sf::Vector2f(0, -8));
          p.convexShape.setFillColor(sf::Color::Cyan);
          p.velocity *= 600.0f;
          p.damage = 35;
          p.lifetime = 2.0f;
          p.convexShape.setRotation(angle * 180.0f / 3.14159f);
        } else if (currentForm == "Upgrade") {
          p.type = 1;
          p.circleShape.setRadius(12);
          p.circleShape.setOrigin(12, 12);
          p.circleShape.setFillColor(sf::Color(0, 255, 0));
          p.circleShape.setOutlineThickness(2);
          p.circleShape.setOutlineColor(sf::Color::White);
          p.velocity *= 500.0f;
          p.damage = 20;
          p.lifetime = 1.0f;
        }

        if (p.type == 0)
          p.rectShape.setPosition(pos);
        else if (p.type == 1)
          p.circleShape.setPosition(pos);
        else if (p.type == 2)
          p.convexShape.setPosition(pos);

        projectiles.push_back(p);
      };

      if (currentForm == "Human" || currentForm == "Four Arms" ||
          currentForm == "XLR8") {
        createProjectile(dir);
      } else if (currentForm == "Heatblast") {
        float baseAngle = std::atan2(dir.y, dir.x);
        for (int i = -1; i <= 1; i++) {
          float a = baseAngle + i * 0.3f;
          createProjectile(sf::Vector2f(std::cos(a), std::sin(a)));
        }
      } else if (currentForm == "Diamondhead") {
        for (int i = 0; i < 8; i++) {
          float a = i * (3.14159f * 2.0f / 8.0f);
          createProjectile(sf::Vector2f(std::cos(a), std::sin(a)));
        }
      } else if (currentForm == "Upgrade") {
        sf::Vector2f side(-dir.y, dir.x);
        float lenSide = std::sqrt(side.x * side.x + side.y * side.y);
        if (lenSide > 0)
          side /= lenSide;
        createProjectile(dir);
        createProjectile(
            sf::Vector2f(dir.x + side.x * 0.2f, dir.y + side.y * 0.2f));
        createProjectile(
            sf::Vector2f(dir.x - side.x * 0.2f, dir.y - side.y * 0.2f));
      }

      currentCooldown = attackCooldown;
    }
  }
}

void Alien::updateProjectiles(float dt) {
  for (auto it = projectiles.begin(); it != projectiles.end();) {
    it->lifetime -= dt;
    if (it->lifetime <= 0 || !it->active) {
      it = projectiles.erase(it);
    } else {
      if (it->type == 0)
        it->rectShape.move(it->velocity * dt);
      else if (it->type == 1)
        it->circleShape.move(it->velocity * dt);
      else if (it->type == 2)
        it->convexShape.move(it->velocity * dt);
      it++;
    }
  }
}

void Alien::renderProjectiles(sf::RenderWindow &window) {
  for (const auto &p : projectiles) {
    if (p.type == 0)
      window.draw(p.rectShape);
    else if (p.type == 1)
      window.draw(p.circleShape);
    else if (p.type == 2)
      window.draw(p.convexShape);
  }
}

void Alien::draw(sf::RenderWindow &window) {
  render(window);
  renderProjectiles(window);
}

std::vector<Projectile> &Alien::getProjectiles() { return projectiles; }
sf::Vector2f Alien::getPosition() const { return sprite.getPosition(); }
sf::FloatRect Alien::getBounds() const {
  int texIdx = 0;
  if (currentForm == "Human")
    texIdx = 0;
  else if (currentForm == "Heatblast")
    texIdx = 1;
  else if (currentForm == "Four Arms")
    texIdx = 2;
  else if (currentForm == "XLR8")
    texIdx = 3;
  else if (currentForm == "Diamondhead")
    texIdx = 4;
  else if (currentForm == "Upgrade")
    texIdx = 5;

  if (textures[texIdx].getSize().x > 0)
    return sprite.getGlobalBounds();
  return fallbackSprite.getGlobalBounds();
}

void Alien::setPosition(sf::Vector2f pos) {
  sprite.setPosition(pos);
  fallbackSprite.setPosition(pos);
}

void Alien::setCheatUnlocks(bool active) {
  if (active) {
    unlocked["Heatblast"] = true;
    unlocked["Four Arms"] = true;
    unlocked["XLR8"] = true;
    unlocked["Diamondhead"] = true;
    unlocked["Upgrade"] = true;
  } else {
    loadUnlocks();
  }
}
