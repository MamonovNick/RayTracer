#pragma once

#include "glm/glm.hpp"
#include "Types.h"
#include "Scene.h"
#include "Settings.h"

class CTracer
{
public:
  void MakeRay(glm::vec3*, glm::vec3*, float, SRay*);
  glm::vec3 TraceRay(SRay*);
  void RenderImage();
  void SaveImageToFile(std::string fileName);
  CImage* LoadImageFromFile(std::string fileName);
  float CalcRadius(float);
	bool TestRay(glm::vec3*, glm::vec3*, int, SRay*, bool*, float*);

public:
  SCamera* m_camera;
	SSettings* set;
	SCharacter* charc;
  CScene* m_pScene;
};