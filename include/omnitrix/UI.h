#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <string>

class UI {
public:
  UI();
  void load();
  void update(float dt, int playerHealth, int maxHealth, int enemyHealth,
              int maxEnemyHealth, float omnitrixEnergy, float maxEnergy,
              const std::string &alienName, float cooldown, bool isOverheated,
              const std::string &instructions,
              const std::string &selectionStr = "");
  void render(sf::RenderWindow &window);
  void draw(sf::RenderWindow &window);

  void setUnlockMessage(const std::string &msg);

private:
  sf::Font font;
  sf::Text playerHealthText;
  sf::RectangleShape playerHealthBarBg;
  sf::RectangleShape playerHealthBarFg;

  sf::Text enemyHealthText;
  sf::RectangleShape enemyHealthBarBg;
  sf::RectangleShape enemyHealthBarFg;

  sf::Text energyText;
  sf::RectangleShape energyBarBg;
  sf::RectangleShape energyBarFg;

  sf::Text alienNameText;
  sf::Text cooldownText;
  sf::Text instructionsText;
  sf::Text unlockMessageText;
  sf::Text selectionText;

  float iconPulseTime;

  float unlockMsgTimer;

  sf::RectangleShape screenRedTint;
  bool overheated;
};

#endif
