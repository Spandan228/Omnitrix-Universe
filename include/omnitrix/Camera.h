#ifndef CAMERA_H
#define CAMERA_H

#include <SFML/Graphics.hpp>

class Camera {
public:
  Camera(sf::FloatRect bounds);

  void update(float dt, sf::Vector2f targetPos);
  void addTrauma(float amount);
  void shake(float intensity);
  sf::View getView() const;
  void setZoom(float factor);

private:
  sf::View view;
  float trauma;
  float traumaDecay;
  float maxShift;
  float maxAngle;
  float zoomFactor;
};

#endif
