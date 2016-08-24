#include "Tracer.h"
#include "Settings.h"
#include <iostream>

void main(int argc, char** argv)
{
  CTracer tracer;
  CScene scene;
  SCamera s_cam;
	SSettings s_set;
	SCharacter s_ch;
  PSet set;

	tracer.m_camera = &s_cam;
	tracer.set = &s_set;
	tracer.charc = &s_ch;
	tracer.m_pScene = &scene;
	
	if(argc >= 2)
		set.set_parametrs(&s_cam, &s_set, &s_ch, &scene, argv[1]);
	else
		set.set_parametrs(&s_cam, &s_set, &s_ch, &scene, "default.txt");
	scene.set_param(tracer.CalcRadius(s_ch.m_mass), s_ch.m_ratio);
	scene.load_texture_disk("../img/data/disk_32.png");
	scene.load_texture_pan("../img/data/stars.jpg");
	scene.load_texture_planet("../img/data/planet1.jpg", "../img/data/planet2.jpg");
  tracer.RenderImage();
  tracer.SaveImageToFile("Result.png");
}