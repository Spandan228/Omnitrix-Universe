#include "omnitrix/Omnitrix.h"

Omnitrix::Omnitrix() {
  maxEnergy = 100.0f;
  energy = maxEnergy;
  transformed = false;
  overheated = false;
  recharging = false;
  overheatTimer = 0.0f;
  rechargeRate = 10.0f; // energy per second
  cooldownTimer = 0.0f;
  currentAlien = "Human";
  cheatActive = false;
}

void Omnitrix::update(float dt) {
  if (overheated) {
    overheatTimer -= dt;
    if (overheatTimer <= 0) {
      overheated = false;
      rechargeRate = 5.0f; // Need to recharge slowly after overheat
      startRecharge();
    }
  } else if (recharging) {
    energy += rechargeRate * dt;
    if (energy >= maxEnergy) {
      energy = maxEnergy;
      recharging = false;
    }
  } else if (transformed) {
    if (!cheatActive) {
      energy -= 10.0f * dt; // Drain energy
      if (energy <= 0) {
        energy = 0;
        transformed = false;
        overheated = true;
        overheatTimer = 5.0f; // 5 seconds of overheat
        currentAlien = "Human";
      }
    } else {
      energy = maxEnergy;
    }
  }

  if (cheatActive) {
    overheated = false;
    overheatTimer = 0.0f;
    cooldownTimer = 0.0f;
    energy = maxEnergy;
  }

  if (cooldownTimer > 0) {
    cooldownTimer -= dt;
  }
}

void Omnitrix::attemptTransform(const std::string &alienName) {
  if (overheated || recharging || cooldownTimer > 0)
    return;
  if (energy < 20.0f)
    return; // need min energy

  transformed = true;
  currentAlien = alienName;
}

void Omnitrix::revert() {
  if (transformed) {
    transformed = false;
    currentAlien = "Human";
    cooldownTimer = 3.0f; // 3 sec cooldown
  }
}

void Omnitrix::startRecharge() {
  if (!transformed && !overheated && energy < maxEnergy) {
    recharging = true;
    rechargeRate = 20.0f; // Faster normal recharge
  }
}

bool Omnitrix::isTransformed() const { return transformed; }
bool Omnitrix::isOverheated() const { return overheated; }
float Omnitrix::getEnergy() const { return energy; }
float Omnitrix::getMaxEnergy() const { return maxEnergy; }
float Omnitrix::getCooldownTimer() const {
  return overheated ? overheatTimer : cooldownTimer;
}
std::string Omnitrix::getCurrentAlien() const { return currentAlien; }
bool Omnitrix::isRecharging() const { return recharging; }

void Omnitrix::setCheat(bool active) {
  cheatActive = active;
  if (active) {
    energy = maxEnergy;
    overheated = false;
    cooldownTimer = 0.0f;
  }
}
