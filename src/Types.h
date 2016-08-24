#pragma once

#include "atlimage.h"
#include "glm/glm.hpp"
#include <vector>
#include <string>

#define G 6.674e-11
#define C 3e+8
#define eps 1e-8
#define IterStop 300
#define Pi 3.14159265358979323846264

struct SRay
{
  glm::vec3 m_start;
  glm::vec3 m_dir;
};

struct SCamera
{
  glm::vec3 m_pos;          // Camera position
  glm::vec3 m_forward;      // Orthonormal basis
  glm::vec3 m_right;
  glm::vec3 m_up;

  glm::vec2 m_viewAngle;    // View angles, rad
  glm::uvec2 m_resolution;  // Image resolution: w, h

  std::vector<glm::vec3> m_pixels;  // Pixel array
};

struct SSettings
{
  int m_aa;
  int m_postproc;
  int m_alpha;
  int m_stereo;
  int m_planets;
	int m_omp;
};

struct SCharacter
{
  float m_mass;
  float m_ratio;
};