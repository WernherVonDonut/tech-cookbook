#include "arrive.hpp"

#include <cmath>

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>

namespace aifg
{
namespace
{
const constexpr float kMaxSpeed = 300.0f;
const constexpr float kSatisfactionRadius = 100.0f;
const constexpr float kTimeToTarget = 0.25f;

void drawBody(const KinematicBody& body, const ALLEGRO_COLOR& color)
{
  al_draw_filled_circle(body.position_x_, body.position_y_, 10, color);
  float x = body.position_x_ + sin(body.orientation_) * 20;
  float y = body.position_y_ + cos(body.orientation_) * 20;

  float velocity_length = sqrt(body.velocity_x_ * body.velocity_x_ + body.velocity_y_ * body.velocity_y_);
  float velocity_x = (body.velocity_x_ / velocity_length) * 30;
  float velocity_y = (body.velocity_y_ / velocity_length) * 30;

  al_draw_line(body.position_x_, body.position_y_, body.position_x_ + velocity_x, body.position_y_ + velocity_y,
               al_map_rgb(255, 0, 255), 2);
  al_draw_line(body.position_x_, body.position_y_, x, y, al_map_rgb(255, 255, 255), 2);
}

} // namespace

Arrive::Arrive() : character_(600, 500, 0, 0, 0, 0), target_(200, 100, 0, 0, 0, 0) {}

void Arrive::UpdateBodies()
{
  // Reset positions
  float distance =
      sqrt(pow(character_.position_x_ - target_.position_x_, 2) + pow(character_.position_y_ - target_.position_y_, 2));
  if (distance < kSatisfactionRadius) {
    character_.velocity_x_ = 0;
    character_.velocity_y_ = 0;
    return;
  }

  // Base velocity is set to the target position
  KinematicSteeringOutput steering;
  steering.velocity_x_ = target_.position_x_ - character_.position_x_;
  steering.velocity_y_ = target_.position_y_ - character_.position_y_;

  steering.velocity_x_ = steering.velocity_x_ / kTimeToTarget;
  steering.velocity_y_ = steering.velocity_y_ / kTimeToTarget;

  if (sqrt(steering.velocity_x_ * steering.velocity_x_ + steering.velocity_y_ * steering.velocity_y_) > kMaxSpeed) {
    // Normalize the velocity to the maximum speed if going too fast
    float steering_vector_length =
        sqrt(steering.velocity_x_ * steering.velocity_x_ + steering.velocity_y_ * steering.velocity_y_);
    steering.velocity_x_ = (steering.velocity_x_ / steering_vector_length) * kMaxSpeed;
    steering.velocity_y_ = (steering.velocity_y_ / steering_vector_length) * kMaxSpeed;
  }

  // Update the orientation
  character_.orientation_ =
      character_.NewOrientation(character_.orientation_, steering.velocity_x_, steering.velocity_y_);
  steering.rotation_ = 0;

  // Update the kinematic
  character_.Update(steering, 1.0f / 60.0f);
}

void Arrive::Update(ALLEGRO_DISPLAY* display, ALLEGRO_FONT* font)
{
  UpdateBodies();
  al_clear_to_color(al_map_rgb(0, 0, 0));
  al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
  al_draw_text(font, al_map_rgb(255, 255, 255), 0, 550, 0, "white - orientation");
  al_draw_text(font, al_map_rgb(255, 255, 255), 0, 570, 0, "purple - velocity direction");
  drawBody(character_, al_map_rgb(255, 0, 0));
  drawBody(target_, al_map_rgb(0, 255, 0));
  al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_INVERSE_ALPHA);
  al_flip_display();
}
} // namespace aifg