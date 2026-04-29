#include "omnitrix/UI.h"
#include <cmath>
#include <iostream>

UI::UI() {
  overheated = false;
  unlockMsgTimer = 0;
  iconPulseTime = 0;
}

void UI::load() {
  if (!font.loadFromFile("assets/fonts/arial.ttf")) {
    std::cout << "Warning: Could not load assets/arial.ttf font." << std::endl;
  }

  // Health
  playerHealthBarBg.setSize(sf::Vector2f(200, 20));
  playerHealthBarBg.setFillColor(sf::Color(100, 0, 0));
  playerHealthBarBg.setPosition(20, 20);

  playerHealthBarFg.setSize(sf::Vector2f(200, 20));
  playerHealthBarFg.setFillColor(sf::Color::Red);
  playerHealthBarFg.setPosition(20, 20);

  enemyHealthBarBg.setSize(sf::Vector2f(200, 20));
  enemyHealthBarBg.setFillColor(sf::Color(100, 100, 0));
  enemyHealthBarBg.setPosition(1700, 20);

  enemyHealthBarFg.setSize(sf::Vector2f(200, 20));
  enemyHealthBarFg.setFillColor(sf::Color::Yellow);
  enemyHealthBarFg.setPosition(1700, 20);

  // Energy
  energyBarBg.setSize(sf::Vector2f(200, 10));
  energyBarBg.setFillColor(sf::Color(0, 50, 0));
  energyBarBg.setPosition(20, 50);

  energyBarFg.setSize(sf::Vector2f(200, 10));
  energyBarFg.setFillColor(sf::Color::Green);
  energyBarFg.setPosition(20, 50);

  // Texts
  playerHealthText.setFont(font);
  playerHealthText.setCharacterSize(14);
  playerHealthText.setPosition(20, 5);

  enemyHealthText.setFont(font);
  enemyHealthText.setCharacterSize(14);
  enemyHealthText.setPosition(1700, 5);

  alienNameText.setFont(font);
  alienNameText.setCharacterSize(24);
  alienNameText.setFillColor(sf::Color::Green);
  alienNameText.setPosition(20, 70);

  cooldownText.setFont(font);
  cooldownText.setCharacterSize(20);
  cooldownText.setFillColor(sf::Color::White);
  cooldownText.setPosition(20, 100);

  instructionsText.setFont(font);
  instructionsText.setCharacterSize(14);
  instructionsText.setFillColor(sf::Color::White);
  instructionsText.setPosition(20, 1040);

  unlockMessageText.setFont(font);
  unlockMessageText.setCharacterSize(30);
  unlockMessageText.setFillColor(sf::Color::Yellow);
  unlockMessageText.setPosition(700, 500);

  selectionText.setFont(font);
  selectionText.setCharacterSize(30);
  selectionText.setFillColor(sf::Color::Green);
  selectionText.setOutlineThickness(2);
  selectionText.setOutlineColor(sf::Color::Black);

  screenRedTint.setSize(sf::Vector2f(1920, 1080));
  screenRedTint.setFillColor(sf::Color(255, 0, 0, 50));
}

void UI::update(float dt, int playerHealth, int maxHealth, int enemyHealth,
                int maxEnemyHealth, float omnitrixEnergy, float maxEnergy,
                const std::string &alienName, float cooldown, bool isOverheated,
                const std::string &instructions,
                const std::string &selectionStr) {
  if (maxHealth > 0)
    playerHealthBarFg.setSize(
        sf::Vector2f(200.0f * (playerHealth / (float)maxHealth), 20));
  if (maxEnemyHealth > 0)
    enemyHealthBarFg.setSize(
        sf::Vector2f(200.0f * (enemyHealth / (float)maxEnemyHealth), 20));
  if (maxEnergy > 0)
    energyBarFg.setSize(
        sf::Vector2f(200.0f * (omnitrixEnergy / maxEnergy), 10));

  playerHealthText.setString("Player HP: " + std::to_string(playerHealth));
  enemyHealthText.setString("Enemy HP: " + std::to_string(enemyHealth));

  alienNameText.setString("Alien: " + alienName);

  if (cooldown > 0) {
    cooldownText.setString("Cooldown: " + std::to_string((int)cooldown) + "s");
  } else {
    cooldownText.setString("Omnitrix Ready");
  }

  instructionsText.setString(instructions);
  selectionText.setString(selectionStr);

  overheated = isOverheated;
  if (overheated) {
    alienNameText.setString("OVERHEATED");
    alienNameText.setFillColor(sf::Color::Red);
  } else {
    alienNameText.setFillColor(sf::Color::Green);
  }

  iconPulseTime += dt;

  if (unlockMsgTimer > 0)
    unlockMsgTimer -= dt;
}

void UI::setUnlockMessage(const std::string &msg) {
  unlockMessageText.setString(msg);
  unlockMsgTimer = 3.0f;
}

void UI::render(sf::RenderWindow &window) {
  if (overheated) {
    window.draw(screenRedTint);
  }

  window.draw(playerHealthBarBg);
  window.draw(playerHealthBarFg);
  window.draw(enemyHealthBarBg);
  window.draw(enemyHealthBarFg);
  window.draw(energyBarBg);
  window.draw(energyBarFg);

  if (font.getInfo().family != "") {
    window.draw(playerHealthText);
    window.draw(enemyHealthText);
    window.draw(alienNameText);
    window.draw(cooldownText);
    window.draw(instructionsText);

    if (unlockMsgTimer > 0) {
      window.draw(unlockMessageText);
    }

    if (selectionText.getString() != "") {
      sf::FloatRect bounds = selectionText.getLocalBounds();
      selectionText.setOrigin(bounds.left + bounds.width / 2.0f, 0);
      selectionText.setPosition(960, 50);
      window.draw(selectionText);
    }
  }
}
void UI::draw(sf::RenderWindow &window) { render(window); }
