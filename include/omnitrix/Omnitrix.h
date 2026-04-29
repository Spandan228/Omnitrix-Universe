#ifndef OMNITRIX_H
#define OMNITRIX_H

#include <string>

class Omnitrix {
public:
  Omnitrix();
  void update(float dt);

  void attemptTransform(const std::string &alienName);
  void revert();
  void startRecharge();

  bool isTransformed() const;
  bool isOverheated() const;
  float getEnergy() const;
  float getMaxEnergy() const;
  float getCooldownTimer() const;
  std::string getCurrentAlien() const;
  bool isRecharging() const;
  void setCheat(bool active);

private:
  float energy;
  float maxEnergy;
  bool transformed;
  bool overheated;
  bool recharging;
  float overheatTimer;
  float rechargeRate;
  std::string currentAlien;

  float cooldownTimer; // Time left before can transform again if not overheated
  bool cheatActive;
};

#endif
