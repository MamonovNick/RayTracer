#pragma once

#include "Types.h"

class CScene
{
  glm::vec3 plpos; //plane position (1,2,0)
	glm::vec3 plnorm; //plane normal
	float diskrad; //radius of disk
	std::vector<glm::uvec4> pixels;
	glm::uint xRes;
	glm::uint yRes;
	
	glm::vec3 sphpos; //sphere position (0,0,0)
	float sphrad; //sphere radius

	glm::vec3 pan; //panorama
	std::vector<glm::uvec3> pixels_p;
	glm::uint xRes_p;
	glm::uint yRes_p;

public:
	glm::vec3 plan1; //planet1
	std::vector<glm::uvec3> pixels_pl1;
	glm::uint xRes_pl1;
	glm::uint yRes_pl1;
	float pl1_r;

	glm::vec3 plan2; //planet2
	std::vector<glm::uvec3> pixels_pl2;
	glm::uint xRes_pl2;
	glm::uint yRes_pl2;
	float pl2_r;

	void set_param(float, float);
	void load_texture_disk(std::string);
	void load_texture_pan(std::string);
	void load_texture_planet(std::string, std::string);
	bool TestPlane(glm::vec3*, glm::vec3*, glm::vec3*, glm::vec3*, bool*, float*);
	bool TakePixel_disk(glm::vec3*, glm::vec3*, float*);
	bool TestSphere(glm::vec3*, glm::vec3*, glm::vec3*);
	bool TestPlanets(glm::vec3*, glm::vec3*, glm::vec3*, glm::vec3*);
	void TestPanoram(glm::vec3*, glm::vec3*);
	CImage* LoadImageFromFile(std::string);
};