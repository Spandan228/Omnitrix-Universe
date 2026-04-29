#include "omnitrix/Game.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

Game::Game()
    : window(sf::VideoMode(1920, 1080), "Omnitrix Universe"),
      uiView(sf::FloatRect(0, 0, 1920, 1080)),
      camera(sf::FloatRect(0, 0, 1920, 1080)) {
  window.setFramerateLimit(60);
  srand(static_cast<unsigned int>(time(nullptr)));

  state = GameState::MENU;
  loadingProgress = 0.0f;
  loadingTimer = 0.0f;
  menuTitleTimer = 0.0f;
  currentTipIndex = 0;
  tipTimer = 0.0f;
  hitStopTimer = 0.0f;
  cameraTrauma = 0.0f;
  isGodMode = false;

  loadingTips = {
      "Tip: Press 'R' to forcefully recharge the Omnitrix.",
      "Tip: Heatblast's fireballs pierce through multiple small enemies.",
      "Tip: XLR8 is almost invincible while moving at top speed.",
      "Tip: Diamondhead's shards can clear whole waves if timed right.",
      "Tip: Use the Mouse Wheel to quickly cycle through aliens.",
      "Tip: Overheating leaves you vulnerable in Human form!"};

  if (menuBgTex.loadFromFile("assets/textures/ui/menu_bg.png")) {
    menuBgSprite.setTexture(menuBgTex);
    menuBgSprite.setScale(1920.0f / menuBgTex.getSize().x,
                          1080.0f / menuBgTex.getSize().y);
    loadingBgSprite = menuBgSprite;
    loadingBgSprite.setColor(
        sf::Color(180, 180, 255, 200)); // Lighter & Blueish for Loading
  }

  loadingTipText.setFont(font);
  loadingTipText.setCharacterSize(28);
  loadingTipText.setFillColor(sf::Color::White);

  loadingProgressText.setFont(font);
  loadingProgressText.setCharacterSize(36);
  loadingProgressText.setFillColor(sf::Color::Green);

  ui.load();

  for (int i = 0; i < 100; i++) {
    stars[i] = sf::Vector2f(rand() % 2000 - 1000, rand() % 2000 - 1000);
  }
  gridLine.setFillColor(sf::Color(0, 255, 0, 40));
  gridLine.setSize(sf::Vector2f(2000, 1));

  font.loadFromFile("assets/fonts/arial.ttf");

  for (int i = 0; i < 3; i++) {
    menuButtons[i].setSize(sf::Vector2f(320, 60));
    menuButtons[i].setFillColor(sf::Color(0, 100, 0, 100));
    menuButtons[i].setOutlineThickness(2);
    menuButtons[i].setOutlineColor(sf::Color(0, 255, 0, 150));
    menuButtons[i].setOrigin(160, 30);
    menuButtons[i].setPosition(960, 600 + i * 90);

    buttonTexts[i].setFont(font);
    buttonTexts[i].setCharacterSize(24);
    buttonTexts[i].setFillColor(sf::Color::White);
  }

  // Home Screen Title
  menuTitleText.setFont(font);
  menuTitleText.setCharacterSize(120);
  menuTitleText.setString("OMNITRIX UNIVERSE");
  menuTitleText.setFillColor(sf::Color::Green);
  menuTitleText.setOutlineColor(sf::Color::White);
  menuTitleText.setOutlineThickness(3);
  sf::FloatRect mtBounds = menuTitleText.getLocalBounds();
  menuTitleText.setOrigin(mtBounds.left + mtBounds.width / 2.0f,
                          mtBounds.top + mtBounds.height / 2.0f);
  menuTitleText.setPosition(960, 250);

  loadingBarBg.setSize(sf::Vector2f(400, 30));
  loadingBarBg.setFillColor(sf::Color(50, 50, 50));
  loadingBarBg.setPosition(760, 800);
  loadingBarFg.setSize(sf::Vector2f(0, 30));
  loadingBarFg.setFillColor(sf::Color::Green);
  loadingBarFg.setPosition(760, 800);

  pauseOverlay.setSize(sf::Vector2f(1920, 1080));
  pauseOverlay.setFillColor(sf::Color(0, 20, 20, 150));

  pauseTitleText.setFont(font);
  pauseTitleText.setCharacterSize(80);
  pauseTitleText.setString("MISSION PAUSED");
  pauseTitleText.setFillColor(sf::Color::Cyan);
  pauseTitleText.setOutlineColor(sf::Color::White);
  pauseTitleText.setOutlineThickness(2);
  sf::FloatRect ptBounds = pauseTitleText.getLocalBounds();
  pauseTitleText.setOrigin(ptBounds.left + ptBounds.width / 2.0f,
                           ptBounds.top + ptBounds.height / 2.0f);
  pauseTitleText.setPosition(960, 400);

  // Initialize Instruction Items
  std::vector<std::string> instructions = {
      "MOVEMENT: WASD ONLY",
      "COMBAT: SPACE BAR (Auto-Targeting)",
      "SELECTION WHEEL: HOLD 'V'",
      "CYCLE ALIENS: ARROW KEYS or MOUSE",
      "REVERT TO HUMAN: PRESS 'R'",
      "PAUSE MISSION: ESCAPE",
      "CHEAT CODE: TYPE '1010' FOR GOD MODE",
      "PRESS ESCAPE TO RETURN TO MENU"};

  for (int i = 0; i < 8; i++) {
    instructionItems[i].setFont(font);
    instructionItems[i].setCharacterSize(35);
    instructionItems[i].setString(instructions[i]);
    instructionItems[i].setFillColor(i == 6 ? sf::Color::Yellow
                                            : sf::Color::Cyan);
    sf::FloatRect b = instructionItems[i].getLocalBounds();
    instructionItems[i].setOrigin(b.left + b.width / 2.0f,
                                  b.top + b.height / 2.0f);
    instructionItems[i].setPosition(960, 300 + i * 70);
  }

  isTransforming = false;
  transformTimer = 0.0f;
  flashAlpha = 0.0f;
  selectionWheelActive = false;
  selectionIndex = 0;
  spawnTimer = 0.0f;
  isCheatActive = false;
  regenRate = 5.0f;
  cheatBuffer = "";
  wheelRotation = 0.0f;

  screenFlash.setSize(sf::Vector2f(1920, 1080));
  screenFlash.setFillColor(sf::Color(255, 255, 255, 0));

  for (int i = 0; i < 3; i++) {
    gameOverButtons[i].setSize(sf::Vector2f(280, 50));
    gameOverButtons[i].setFillColor(sf::Color(100, 0, 0));
    gameOverButtons[i].setOutlineThickness(2);
    gameOverButtons[i].setOutlineColor(sf::Color(sf::Color::Red));
    gameOverButtons[i].setOrigin(140, 25);
    gameOverButtons[i].setPosition(960, 540 + i * 80);

    gameOverButtonTexts[i].setFont(font);
    gameOverButtonTexts[i].setCharacterSize(24);
    gameOverButtonTexts[i].setFillColor(sf::Color::White);
  }
  gameOverButtonTexts[0].setString("RESTART MISSION");
  gameOverButtonTexts[1].setString("RETURN TO MENU");
  gameOverButtonTexts[2].setString("EXIT TO DESKTOP");

  const std::string blurCodeX =
      "uniform sampler2D texture;"
      "uniform float offset;"
      "void main() {"
      "  vec2 off = vec2(offset, 0.0);"
      "  vec4 sum = texture2D(texture, gl_TexCoord[0].xy) * 0.2270270270;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy + off * 1.3846153846) * "
      "0.3162162162;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy - off * 1.3846153846) * "
      "0.3162162162;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy + off * 3.2307692308) * "
      "0.0702702703;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy - off * 3.2307692308) * "
      "0.0702702703;"
      "  gl_FragColor = gl_Color * sum;"
      "}";

  const std::string blurCodeY =
      "uniform sampler2D texture;"
      "uniform float offset;"
      "void main() {"
      "  vec2 off = vec2(0.0, offset);"
      "  vec4 sum = texture2D(texture, gl_TexCoord[0].xy) * 0.2270270270;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy + off * 1.3846153846) * "
      "0.3162162162;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy - off * 1.3846153846) * "
      "0.3162162162;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy + off * 3.2307692308) * "
      "0.0702702703;"
      "  sum += texture2D(texture, gl_TexCoord[0].xy - off * 3.2307692308) * "
      "0.0702702703;"
      "  gl_FragColor = vec4(gl_Color.rgb * sum.rgb, gl_Color.a * 0.9);"
      "}";

  if (sf::Shader::isAvailable()) {
    blurShaderX.loadFromMemory(blurCodeX, sf::Shader::Fragment);
    blurShaderY.loadFromMemory(blurCodeY, sf::Shader::Fragment);
    blurShaderX.setUniform("offset", 0.005f);
    blurShaderY.setUniform("offset", 0.005f);
  }

  blurBuffer.create(1920, 1080);

  auto loadEnemyMasked = [&](sf::Texture &tex, const std::string &path) {
    sf::Image img;
    if (img.loadFromFile(path)) {
      // Robust fuzzy masking for white backgrounds
      img.createMaskFromColor(sf::Color::White);
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
    }
  };

  loadEnemyMasked(enemyNormalTex, "assets/textures/enemies/enemy_normal.png");
  loadEnemyMasked(enemyScoutTex, "assets/textures/enemies/enemy_scout.png");
  loadEnemyMasked(enemyTankTex, "assets/textures/enemies/enemy_tank.png");

  resetGame();
}

void Game::resetGame() {
  playerHp = 100;
  playerMaxHp = 100;
  spawnTimer = 2.0f;
  selectionWheelActive = false;
  selectionIndex = 0;
  isTransforming = false;
  transformTimer = 0.0f;
  flashAlpha = 0.0f;
  enemies.clear();
  player.setPosition(sf::Vector2f(960, 540));
  player.setForm("Human");
}

void Game::run() {
  sf::Clock rClock;
  while (window.isOpen()) {
    float dt = rClock.restart().asSeconds();
    processEvents();

    if (state == GameState::MENU)
      updateMenu(dt);
    else if (state == GameState::LOADING)
      updateLoading(dt);
    else if (state == GameState::PLAYING)
      update(dt);
    else if (state == GameState::GAMEOVER)
      updateGameOver(dt);
    else if (state == GameState::PAUSED)
      updatePaused(dt);
    else if (state == GameState::INSTRUCTIONS)
      updateInstructions(dt);

    render();
  }
}

void Game::applyHitStop(float duration) { hitStopTimer = duration; }
void Game::addCameraTrauma(float amount) {
  cameraTrauma = std::min(1.0f, cameraTrauma + amount);
}

void Game::updateJuice(float dt) {
  if (hitStopTimer > 0)
    hitStopTimer -= dt;
  if (cameraTrauma > 0) {
    cameraTrauma = std::max(0.0f, cameraTrauma - 0.8f * dt);
    float shake = cameraTrauma * cameraTrauma * 20.0f;
    camera.shake(shake);
  }
}

void Game::processEvents() {
  sf::Event event;
  while (window.pollEvent(event)) {
    if (event.type == sf::Event::Closed)
      window.close();
    if (state == GameState::PLAYING) {
      // Revert Check
      if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        omnitrix.revert();
      }

      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::V) {
          selectionWheelActive = true;
        }

        // Arrow Key Selection Support
        if (selectionWheelActive) {
          std::vector<std::string> unlocked = player.getUnlockedAliens();
          if (!unlocked.empty()) {
            if (event.key.code == sf::Keyboard::Left ||
                event.key.code == sf::Keyboard::Up) {
              selectionIndex =
                  (selectionIndex - 1 + unlocked.size()) % unlocked.size();
            } else if (event.key.code == sf::Keyboard::Right ||
                       event.key.code == sf::Keyboard::Down) {
              selectionIndex = (selectionIndex + 1) % unlocked.size();
            }
          }
        }

        if (event.key.code == sf::Keyboard::Escape) {
          state = GameState::PAUSED;
          buttonTexts[0].setString("RESUME MISSION");
          buttonTexts[1].setString("RETURN TO MENU");
          for (int i = 0; i < 2; i++) {
            sf::FloatRect bounds = buttonTexts[i].getLocalBounds();
            buttonTexts[i].setOrigin(bounds.left + bounds.width / 2.0f,
                                     bounds.top + bounds.height / 2.0f);
            buttonTexts[i].setPosition(menuButtons[i].getPosition());
          }
          pauseTexture.create(window.getSize().x, window.getSize().y);
          pauseTexture.update(window);
          sceneSprite.setTexture(pauseTexture, true);
        }
        if (event.key.code >= sf::Keyboard::Num0 &&
            event.key.code <= sf::Keyboard::Num9) {
          cheatBuffer += std::to_string(event.key.code - sf::Keyboard::Num0);
          if (cheatBuffer.size() > 10)
            cheatBuffer =
                cheatBuffer.substr(cheatBuffer.size() - 4); // Keep buffer short
          if (cheatBuffer.find("1010") != std::string::npos) {
            isCheatActive = !isCheatActive;        // TOGGLE
            isGodMode = isCheatActive;             // Unlocks Invincibility
            player.setCheatUnlocks(isCheatActive); // Unlocks all aliens
            cheatBuffer = "";
            addCameraTrauma(0.5f); // Impactful feedback
            particles.emit(player.getPosition(), sf::Color::Yellow, 50, true);
          }
        }
      }
      if (event.type == sf::Event::KeyReleased) {
        if (event.key.code == sf::Keyboard::V) {
          selectionWheelActive = false;
          std::vector<std::string> unlocked = player.getUnlockedAliens();
          if (selectionIndex >= 0 &&
              selectionIndex < static_cast<int>(unlocked.size())) {
            player.triggerTransformation(unlocked[selectionIndex]);
            omnitrix.attemptTransform(unlocked[selectionIndex]);
          }
        }
      }
    } else if (state == GameState::PAUSED) {
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        state = GameState::PLAYING;
      }
    } else if (state == GameState::INSTRUCTIONS) {
      if (event.type == sf::Event::KeyPressed &&
          event.key.code == sf::Keyboard::Escape) {
        state = GameState::MENU;
      }
    }

    if (event.type == sf::Event::MouseButtonPressed) {
      sf::Vector2i mousePos = sf::Mouse::getPosition(window);
      sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, uiView);
      if (state == GameState::MENU) {
        for (int i = 0; i < 3; i++) {
          if (menuButtons[i].getGlobalBounds().contains(mouseWorld)) {
            if (i == 0) {
              state = GameState::LOADING;
              loadingProgress = 0.0f;
            } else if (i == 1) {
              state = GameState::INSTRUCTIONS;
            } else if (i == 2)
              window.close();
          }
        }
      } else if (state == GameState::PAUSED) {
        for (int i = 0; i < 2; i++) {
          if (menuButtons[i].getGlobalBounds().contains(mouseWorld)) {
            if (i == 0)
              state = GameState::PLAYING;
            else if (i == 1) {
              state = GameState::MENU;
              resetGame();
            }
          }
        }
      } else if (state == GameState::GAMEOVER) {
        for (int i = 0; i < 3; i++) {
          if (gameOverButtons[i].getGlobalBounds().contains(mouseWorld)) {
            if (i == 0) {
              state = GameState::PLAYING;
              resetGame();
            } else if (i == 1) {
              state = GameState::MENU;
              resetGame();
            } else if (i == 2)
              window.close();
          }
        }
      }
    }
  }
}

void Game::spawnEnemy() {
  sf::Vector2f pos = player.getPosition();
  float angle = (rand() % 360) * 3.14159f / 180.0f;
  float dist = 1100.0f + (rand() % 300);
  pos.x += std::cos(angle) * dist;
  pos.y += std::sin(angle) * dist;
  int r = rand() % 100;
  EnemyType type = EnemyType::Normal;
  const sf::Texture *tex = &enemyNormalTex;
  if (r < 15) {
    type = EnemyType::Tank;
    tex = &enemyTankTex;
  } else if (r < 40) {
    type = EnemyType::Scout;
    tex = &enemyScoutTex;
  }
  enemies.push_back(Enemy(pos, *tex, type));
}

void Game::update(float dt) {
  if (playerHp <= 0)
    return;
  updateJuice(dt);
  if (isTransforming) {
    transformTimer -= dt;
    flashAlpha -= dt * 510.0f;
    if (flashAlpha < 0)
      flashAlpha = 0;
    screenFlash.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)flashAlpha));
    camera.update(dt, player.getPosition());
    particles.update(dt);
    if (transformTimer <= 0)
      isTransforming = false;
    return;
  }
  if (playerHp < playerMaxHp && playerHp > 0) {
    playerHp += (int)(regenRate * dt);
    if (playerHp > playerMaxHp)
      playerHp = playerMaxHp;
  }
  omnitrix.update(dt);
  omnitrix.setCheat(isCheatActive);

  // Legendary State Sync: Force player to match Omnitrix form
  if (player.getCurrentForm() != omnitrix.getCurrentAlien()) {
    player.setForm(omnitrix.getCurrentAlien());
    if (omnitrix.getCurrentAlien() == "Human") {
      addCameraTrauma(0.5f); // More impact on revert
      particles.emit(player.getPosition(), sf::Color::White, 40, false);
      particles.emit(player.getPosition(), sf::Color::Red, 20, false);
    } else {
      addCameraTrauma(0.4f);
      particles.emit(player.getPosition(), sf::Color::Green, 50, true);
    }
  }

  if (isCheatActive)
    regenRate = 50.0f; // Ultra Regen
  else
    regenRate = 5.0f;
  if (hitStopTimer > 0)
    return;
  if (!selectionWheelActive) {
    sf::Vector2f inputDir(0, 0);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
      inputDir.y -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
      inputDir.y += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
      inputDir.x -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
      inputDir.x += 1;
    player.update(dt, inputDir);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
      // Auto-Face Targeting: Find nearest enemy to aim at
      sf::Vector2f targetDir;
      if (!enemies.empty()) {
        float minDist = 999999.0f;
        sf::Vector2f nearest;
        bool found = false;
        for (auto &e : enemies) {
          if (!e.isDead()) {
            float d = std::sqrt(
                std::pow(e.getPosition().x - player.getPosition().x, 2) +
                std::pow(e.getPosition().y - player.getPosition().y, 2));
            if (d < minDist) {
              minDist = d;
              nearest = e.getPosition();
              found = true;
            }
          }
        }
        if (found && minDist < 800.0f) {
          targetDir = nearest - player.getPosition();
        } else {
          sf::Vector2i mPos = sf::Mouse::getPosition(window);
          sf::Vector2f mWorld = window.mapPixelToCoords(mPos, camera.getView());
          targetDir = mWorld - player.getPosition();
        }
      } else {
        sf::Vector2i mPos = sf::Mouse::getPosition(window);
        sf::Vector2f mWorld = window.mapPixelToCoords(mPos, camera.getView());
        targetDir = mWorld - player.getPosition();
      }
      player.attack(targetDir);
    }
  }
  player.updateProjectiles(dt);
  spawnTimer -= dt;
  if (spawnTimer <= 0) {
    spawnEnemy();
    spawnTimer = 3.0f - (rand() % 100) / 100.0f;
  }
  auto &projs = player.getProjectiles();
  for (auto &p : projs) {
    if (!p.active)
      continue;
    sf::FloatRect pb = (p.type == 0)   ? p.rectShape.getGlobalBounds()
                       : (p.type == 1) ? p.circleShape.getGlobalBounds()
                                       : p.convexShape.getGlobalBounds();
    for (auto &e : enemies) {
      if (!e.isDead() && pb.intersects(e.getBounds())) {
        e.takeDamage(p.damage);
        if (player.getCurrentForm() != "Diamondhead")
          p.active = false;
        sf::Color pc = (p.type == 0)   ? p.rectShape.getFillColor()
                       : (p.type == 1) ? p.circleShape.getFillColor()
                                       : p.convexShape.getFillColor();
        particles.emit(e.getPosition(), pc, 10, false);
        addCameraTrauma(0.2f);
        if (!p.active)
          break;
      }
    }
  }
  int ceHp = 0, ceMaxHp = 0;
  float cDist = 999999.0f;
  for (auto it = enemies.begin(); it != enemies.end();) {
    it->update(dt, player.getPosition());
    float d = std::abs(it->getPosition().x - player.getPosition().x) +
              std::abs(it->getPosition().y - player.getPosition().y);
    if (d < cDist && !it->isDead()) {
      cDist = d;
      ceHp = it->getHealth();
      ceMaxHp = it->getMaxHealth();
    }
    if (!it->isDead() && it->getBounds().intersects(player.getBounds())) {
      if (!isGodMode)
        playerHp -= 10;
      it->takeDamage(1000);
      addCameraTrauma(0.4f);
      particles.emit(player.getPosition(), sf::Color::Red, 10, false);
    }
    if (it->isDead()) {
      particles.emit(it->getPosition(), sf::Color(100, 0, 0), 10, false);
      it = enemies.erase(it);
    } else
      it++;
  }
  camera.update(dt, player.getPosition());
  particles.update(dt);

  // Selection Wheel Logic
  if (selectionWheelActive) {
    std::vector<std::string> unlocked = player.getUnlockedAliens();
    if (!unlocked.empty()) {
      sf::Vector2i mPos = sf::Mouse::getPosition(window);
      sf::Vector2f center(960, 540);
      sf::Vector2f mWorld = window.mapPixelToCoords(mPos, uiView);
      sf::Vector2f dir = mWorld - center;
      float angle = std::atan2(dir.y, dir.x) * 180.0f / 3.14159f + 90.0f;
      if (angle < 0)
        angle += 360.0f;
      float slice = 360.0f / unlocked.size();
      sf::Vector2f dirF = mWorld - center;
      float dist = std::sqrt(dirF.x * dirF.x + dirF.y * dirF.y);
      if (dist > 100.0f && mPos != lastMousePos) {
        float ang = std::atan2(dirF.y, dirF.x) * 180.0f / 3.14159f + 90.0f;
        if (ang < 0)
          ang += 360.0f;
        selectionIndex = static_cast<int>(ang / slice) % unlocked.size();
      }
      lastMousePos = mPos;
    }
  }

  ui.update(
      dt, playerHp, playerMaxHp, ceHp, ceMaxHp, omnitrix.getEnergy(),
      omnitrix.getMaxEnergy(), omnitrix.getCurrentAlien(),
      omnitrix.getCooldownTimer(), omnitrix.isOverheated(), "WASD: Move",
      isCheatActive
          ? "ULTIMATRIX MODE ACTIVE"
          : (selectionWheelActive ? "SELECT ALIEN (Mouse Direction)" : ""));
  if (playerHp <= 0)
    state = GameState::GAMEOVER;
}

void Game::updateMenu(float dt) {
  menuTitleTimer += dt;

  // Title Animation (Pulse & Color Glow)
  float scale = 1.0f + std::sin(menuTitleTimer * 2.0f) * 0.05f;
  menuTitleText.setScale(scale, scale);
  int glow = 150 + static_cast<int>(std::sin(menuTitleTimer * 3.0f) * 105);
  menuTitleText.setFillColor(sf::Color(0, glow, 0));

  // Cosmic Star Drifting
  for (int i = 0; i < 100; i++) {
    stars[i].x += 25.0f * dt; // Faster drift
    stars[i].y += 12.0f * dt;
    if (stars[i].x > 1000)
      stars[i].x = -1000;
    if (stars[i].y > 1000)
      stars[i].y = -1000;
  }

  buttonTexts[0].setString("START MISSION");
  buttonTexts[1].setString("INSTRUCTIONS");
  buttonTexts[2].setString("EXIT TO DESKTOP");

  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, uiView);

  for (int i = 0; i < 3; i++) {
    // Hover Feedback
    if (menuButtons[i].getGlobalBounds().contains(mouseWorld)) {
      menuButtons[i].setFillColor(sf::Color(0, 255, 0, 180)); // Brighter Green
      menuButtons[i].setScale(1.1f, 1.1f);
    } else {
      menuButtons[i].setFillColor(sf::Color(0, 100, 0, 100));
      menuButtons[i].setScale(1.0f, 1.0f);
    }

    sf::FloatRect bounds = buttonTexts[i].getLocalBounds();
    buttonTexts[i].setOrigin(bounds.left + bounds.width / 2.0f,
                             bounds.top + bounds.height / 2.0f);
    buttonTexts[i].setPosition(menuButtons[i].getPosition());
  }
}

void Game::updateLoading(float dt) {
  loadingProgress += dt * 0.33f;
  if (loadingProgress >= 1.0f) {
    loadingProgress = 1.0f;
    state = GameState::PLAYING;
    resetGame();
  }
  loadingBarFg.setSize(sf::Vector2f(400 * loadingProgress, 30));
  tipTimer -= dt;
  if (tipTimer <= 0) {
    currentTipIndex = (currentTipIndex + 1) % loadingTips.size();
    tipTimer = 4.0f;
  }
  loadingTipText.setString(loadingTips[currentTipIndex]);
  sf::FloatRect tipBounds = loadingTipText.getLocalBounds();
  loadingTipText.setOrigin(tipBounds.width / 2.0f, tipBounds.height / 2.0f);
  loadingTipText.setPosition(960, 880);
  int percent = static_cast<int>(loadingProgress * 100);
  loadingProgressText.setString(std::to_string(percent) + "%");
  sf::FloatRect progBounds = loadingProgressText.getLocalBounds();
  loadingProgressText.setOrigin(progBounds.width / 2.0f,
                                progBounds.height / 2.0f);
  loadingProgressText.setPosition(960, 780);
}

void Game::updatePaused(float dt) {}
void Game::updateGameOver(float dt) {
  sf::Vector2i mousePos = sf::Mouse::getPosition(window);
  sf::Vector2f mouseWorld = window.mapPixelToCoords(mousePos, uiView);

  for (int i = 0; i < 3; i++) {
    // Hover Feedback
    if (gameOverButtons[i].getGlobalBounds().contains(mouseWorld)) {
      gameOverButtons[i].setFillColor(sf::Color(200, 0, 0, 200));
      gameOverButtons[i].setScale(1.1f, 1.1f);
    } else {
      gameOverButtons[i].setFillColor(sf::Color(100, 0, 0, 150));
      gameOverButtons[i].setScale(1.0f, 1.0f);
    }

    sf::FloatRect bounds = gameOverButtonTexts[i].getLocalBounds();
    gameOverButtonTexts[i].setOrigin(bounds.left + bounds.width / 2.0f,
                                     bounds.top + bounds.height / 2.0f);
    gameOverButtonTexts[i].setPosition(gameOverButtons[i].getPosition());
  }
}

void Game::render() {
  if (state == GameState::MENU)
    renderMenu();
  else if (state == GameState::LOADING)
    renderLoading();
  else if (state == GameState::GAMEOVER)
    renderGameOver();
  else if (state == GameState::PAUSED)
    renderPaused();
  else if (state == GameState::INSTRUCTIONS)
    renderInstructions();
  else
    renderPlaying();
}

void Game::renderMenu() {
  window.clear(sf::Color::Black);
  window.setView(uiView); // CRITICAL: Standardize view for coordinate mapping
  window.draw(menuBgSprite);

  // Draw Animated Stars
  sf::CircleShape starShape(1.5f);
  starShape.setFillColor(sf::Color::White);
  for (int i = 0; i < 100; i++) {
    starShape.setPosition(stars[i] + sf::Vector2f(960, 540));
    window.draw(starShape);
  }

  window.draw(menuTitleText); // Draw Title
  for (int i = 0; i < 3; i++) {
    window.draw(menuButtons[i]);
    window.draw(buttonTexts[i]);
  }
  window.display();
}

void Game::updateInstructions(float dt) {
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
    state = GameState::MENU;
  }
}

void Game::renderInstructions() {
  window.clear(sf::Color(0, 5, 10));
  window.setView(uiView);
  window.draw(menuBgSprite);

  // Draw Stars to keep look consistent
  sf::CircleShape starShape(1.5f);
  starShape.setFillColor(sf::Color::White);
  for (int i = 0; i < 100; i++) {
    starShape.setPosition(stars[i] + sf::Vector2f(960, 540));
    window.draw(starShape);
  }

  // Draw Overlay
  sf::RectangleShape over(sf::Vector2f(1920, 1080));
  over.setFillColor(sf::Color(0, 0, 0, 180));
  window.draw(over);

  sf::Text title("MISSION INTEL & CONTROLS", font, 60);
  title.setFillColor(sf::Color::Green);
  sf::FloatRect b = title.getLocalBounds();
  title.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
  title.setPosition(960, 150);
  window.draw(title);

  for (int i = 0; i < 8; i++) {
    window.draw(instructionItems[i]);
  }

  window.display();
}

void Game::renderLoading() {
  window.clear(sf::Color::Black);
  window.setView(uiView); // Use uiView
  window.draw(loadingBgSprite);
  window.draw(loadingBarBg);
  window.draw(loadingBarFg);
  window.draw(loadingTipText);
  window.draw(loadingProgressText);
  window.display();
}

void Game::renderPlaying() {
  window.clear(sf::Color(5, 5, 10)); // Darker for AAA feel
  window.setView(camera.getView());

  // Dynamic Infinite Grid
  sf::Vector2f center = camera.getView().getCenter();
  int startX = static_cast<int>(center.x / 100) * 100 - 1200;
  int startY = static_cast<int>(center.y / 100) * 100 - 800;

  for (int x = startX; x <= startX + 2400; x += 100) {
    gridLine.setPosition(x, startY);
    gridLine.setSize(sf::Vector2f(1, 1600));
    window.draw(gridLine);
  }
  for (int y = startY; y <= startY + 1600; y += 100) {
    gridLine.setPosition(startX, y);
    gridLine.setSize(sf::Vector2f(2400, 1));
    window.draw(gridLine);
  }

  for (auto &e : enemies)
    e.draw(window);
  player.draw(window);
  particles.draw(window);
  window.setView(uiView);
  ui.draw(window);

  // Draw Selection Wheel
  if (selectionWheelActive) {
    std::vector<std::string> unlocked = player.getUnlockedAliens();
    sf::CircleShape wheel(200);
    wheel.setOrigin(200, 200);
    wheel.setPosition(960, 540);
    wheel.setFillColor(sf::Color(0, 0, 0, 150));
    wheel.setOutlineThickness(4);
    wheel.setOutlineColor(sf::Color::Green);
    window.draw(wheel);

    for (size_t i = 0; i < unlocked.size(); i++) {
      float angle = (i * 360.0f / unlocked.size() - 90.0f) * 3.14159f / 180.0f;
      sf::Vector2f pos(960 + std::cos(angle) * 140,
                       540 + std::sin(angle) * 140);

      sf::Text t(unlocked[i], font, 20);
      sf::FloatRect b = t.getLocalBounds();
      t.setOrigin(b.left + b.width / 2.0f, b.top + b.height / 2.0f);
      t.setPosition(pos);

      if (static_cast<int>(i) == selectionIndex) {
        t.setFillColor(sf::Color::Green);
        t.setScale(1.3f, 1.3f);
      } else {
        t.setFillColor(sf::Color::White);
      }
      window.draw(t);
    }
  }

  if (flashAlpha > 0)
    window.draw(screenFlash);
  window.display();
}

void Game::renderPaused() {
  window.setView(uiView);
  if (sf::Shader::isAvailable()) {
    blurBuffer.clear();
    blurBuffer.draw(sceneSprite, &blurShaderX);
    blurBuffer.display();
    sf::Sprite s(blurBuffer.getTexture());
    window.draw(s, &blurShaderY);
  } else
    window.draw(sceneSprite);
  window.draw(pauseOverlay);
  window.draw(pauseTitleText);
  for (int i = 0; i < 2; i++) {
    window.draw(menuButtons[i]);
    window.draw(buttonTexts[i]);
  }
  window.display();
}

void Game::renderGameOver() {
  window.clear(sf::Color(50, 0, 0));
  window.setView(uiView);

  // Background star drift (reusing stars)
  sf::CircleShape starShape(1.0f);
  starShape.setFillColor(sf::Color(150, 0, 0));
  for (int i = 0; i < 100; i++) {
    starShape.setPosition(stars[i] + sf::Vector2f(960, 540));
    window.draw(starShape);
  }

  for (int i = 0; i < 3; i++) {
    window.draw(gameOverButtons[i]);
    window.draw(gameOverButtonTexts[i]);
  }
  window.display();
}
