#include "omnitrix/Camera.h"
#include <cmath>

Camera::Camera(sf::FloatRect bounds) {
  view.reset(bounds);
  trauma = 0.0f;
  traumaDecay = 1.0f; // dictates how fast shake decays
  maxShift = 20.0f;
  maxAngle = 5.0f;
  zoomFactor = 1.0f;
}

void Camera::update(float dt, sf::Vector2f targetPos) {
  // Smooth follow
  sf::Vector2f currentCenter = view.getCenter();
  sf::Vector2f newCenter =
      currentCenter + (targetPos - currentCenter) * 5.0f * dt;
  view.setCenter(newCenter);

  // Screen shake from trauma
  if (trauma > 0) {
    float shake = trauma * trauma;
    float offsetX = maxShift * shake * ((rand() % 100 - 50) / 50.0f);
    float offsetY = maxShift * shake * ((rand() % 100 - 50) / 50.0f);
    float angle = maxAngle * shake * ((rand() % 100 - 50) / 50.0f);

    view.move(offsetX, offsetY);
    view.setRotation(angle);

    trauma -= traumaDecay * dt;
    if (trauma < 0)
      trauma = 0;
  } else {
    view.setRotation(0);
  }
}

void Camera::addTrauma(float amount) {
  trauma += amount;
  if (trauma > 1.0f)
    trauma = 1.0f;
}

void Camera::setZoom(float factor) {
  if (factor != zoomFactor) {
    // adjust zoom relative to old zoom
    view.zoom(factor / zoomFactor);
    zoomFactor = factor;
  }
}

void Camera::shake(float intensity) {
  float offsetX = ((rand() % 100 - 50) / 50.0f) * intensity;
  float offsetY = ((rand() % 100 - 50) / 50.0f) * intensity;
  view.move(offsetX, offsetY);
}

sf::View Camera::getView() const { return view; }
