#ifndef GAME_H
#define GAME_H

#include "omnitrix/Alien.h"
#include "omnitrix/Camera.h"
#include "omnitrix/Enemy.h"
#include "omnitrix/Omnitrix.h"
#include "omnitrix/Particle.h"
#include "omnitrix/UI.h"
#include <SFML/Graphics.hpp>
#include <vector>

enum class GameState { MENU, LOADING, PLAYING, GAMEOVER, PAUSED, INSTRUCTIONS };

class Game {
public:
  Game();
  void run();

private:
  void processEvents();
  void update(float dt);
  void render();

  // Legendary Systems: Kinetic Feedback & Juice
  void applyHitStop(float duration);
  void addCameraTrauma(float amount);
  void updateJuice(float dt);

  // Core Loop Helpers
  void resetGame();
  void updateMenu(float dt);
  void renderMenu();
  void updateLoading(float dt);
  void renderLoading();
  void updatePlaying(float dt);
  void renderPlaying();
  void updatePaused(float dt);
  void renderPaused();
  void updateGameOver(float dt);
  void renderGameOver();
  void updateInstructions(float dt);
  void renderInstructions();
  void spawnEnemy();

  sf::RenderWindow window;
  sf::View uiView;

  GameState state;
  float menuTitleTimer;

  // Entities & Managers
  Alien player;
  std::vector<Enemy> enemies;
  ParticleSystem particles;
  Omnitrix omnitrix;
  Camera camera;

  // Kinetic Juice State
  float hitStopTimer;
  float cameraTrauma; // 0.0 to 1.0

  // Gameplay State
  bool isTransforming;
  float transformTimer;
  float flashAlpha;
  float spawnTimer;
  int playerHp;
  int playerMaxHp;

  // UI Assets
  sf::Font font;
  UI ui;

  // Loading Screen Assets
  float loadingTimer;
  float loadingProgress;
  sf::RectangleShape loadingBarBg;
  sf::RectangleShape loadingBarFg;
  std::vector<std::string> loadingTips;
  int currentTipIndex;
  float tipTimer;
  sf::Text loadingTipText;
  sf::Text loadingProgressText;
  sf::Texture loadingBgTex;
  sf::Sprite loadingBgSprite;

  // AAA Menu & Background Assets
  sf::Texture menuBgTex;
  sf::Sprite menuBgSprite;
  sf::Text menuTitleText;
  sf::RectangleShape menuButtons[3];
  sf::Text buttonTexts[3];
  sf::Vector2f stars[100];
  sf::RectangleShape gridLine;

  // Shader & Texture Assets
  sf::Texture enemyNormalTex;
  sf::Texture enemyScoutTex;
  sf::Texture enemyTankTex;
  sf::Shader blurShaderX, blurShaderY;
  sf::RenderTexture blurBuffer;
  sf::Texture pauseTexture; // Permanent texture for pause screen
  sf::Sprite sceneSprite;   // Captures current screen for blur
  sf::RectangleShape pauseOverlay;
  sf::Text pauseTitleText;
  sf::Text instructionItems[8];

  // Game Over Assets
  sf::RectangleShape gameOverButtons[3];
  sf::Text gameOverButtonTexts[3];

  // Selection Wheel
  bool selectionWheelActive;
  int selectionIndex;
  float wheelRotation;
  sf::Vector2i lastMousePos;

  // Cheats & FX
  std::string cheatBuffer;
  bool isCheatActive;
  bool isGodMode;
  float regenRate;
  sf::RectangleShape screenFlash;

  sf::Clock gameClock;
};

#endif
