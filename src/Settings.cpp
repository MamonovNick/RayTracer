#include "Settings.h"
#include <iostream>
#include <fstream>

void PSet::set_str(std::string* s)
{
  s[0] = "ResH";
  s[1] = "ResW";
  s[2] = "ViewAngH";
  s[3] = "ViewAngW";
  s[4] = "Antialiasing";
  s[5] = "AlphaProc";
  s[6] = "Planets";
	s[7] = "Ratio";
	s[8] = "Mass";
	s[9] = "CameraPosX";
	s[10] = "CameraPosY";
	s[11] = "CameraPosZ";
	s[12] = "ParallelOMP";
	s[13] = "Planet1PosX";
	s[14] = "Planet1PosY";
	s[15] = "Planet1PosZ";
	s[16] = "Planet2PosX";
	s[17] = "Planet2PosY";
	s[18] = "Planet2PosZ";
}

void PSet::set_parametrs(SCamera* s_cam, SSettings* s_set, SCharacter* s_ch, CScene* sc, char* file)
{
  std::string s;
	char c;
  std::string s_c[19];
  int rnum, i;
	float rnum2;
	std::ifstream f(file);
	if(!f)
		std::cout<<"Invalid config path! Using default parameters.\r\n"<<std::endl;
	set_str(s_c);

  while (f)
  {
		f>>s>>c;
		if(s == s_c[0])
		{
			f>>rnum;
			s_cam->m_resolution.y = rnum;
			continue;
		}
		if(s == s_c[1])
		{
			f>>rnum;
			s_cam->m_resolution.x = rnum;
			continue;
		}
		if(s == s_c[2])
		{
			f>>rnum;
			s_cam->m_viewAngle[1] = rnum;
			continue;
		}
		if(s == s_c[3])
		{
			f>>rnum;
			s_cam->m_viewAngle[0] = rnum;
			continue;
		}
		if(s == s_c[4])
		{
			f>>rnum;
			s_set->m_aa = rnum;
			continue;
		}
		if(s == s_c[5])
		{
			f>>rnum;
			s_set->m_alpha = rnum;
			continue;
		}
		if(s == s_c[6])
		{
			f>>rnum;
			s_set->m_planets = rnum;
			continue;
		}
		if(s == s_c[7])
		{
			f>>rnum2;
			s_ch->m_ratio = rnum2;
			continue;
		}
		if(s == s_c[8])
		{
			f>>rnum2;
			s_ch->m_mass = rnum2;
			continue;
		}

		if(s == s_c[9])
		{
			f>>rnum2;
			s_cam->m_pos.x = rnum2;
			continue;
		}
		if(s == s_c[10])
		{
			f>>rnum2;
			s_cam->m_pos.y = rnum2;
			continue;
		}
		if(s == s_c[11])
		{
			f>>rnum2;
			s_cam->m_pos.z = rnum2;
			continue;
		}
		if(s == s_c[12])
		{
			f>>rnum;
			s_set->m_omp = rnum;
			continue;
		}
		if(s == s_c[13])
		{
			f>>rnum2;
			sc->plan1.x = rnum2;
			continue;
		}
		if(s == s_c[14])
		{
			f>>rnum2;
			sc->plan1.y = rnum2;
			continue;
		}
		if(s == s_c[15])
		{
			f>>rnum2;
			sc->plan1.z = rnum2;
			continue;
		}
		if(s == s_c[16])
		{
			f>>rnum2;
			sc->plan2.x = rnum2;
			continue;
		}
		if(s == s_c[17])
		{
			f>>rnum2;
			sc->plan2.y = rnum2;
			continue;
		}
		if(s == s_c[18])
		{
			f>>rnum2;
			sc->plan2.z = rnum2;
			continue;
		}
  }
	proc_cam(s_cam);
}

void PSet::proc_cam(SCamera* s_cam)
{
	s_cam->m_forward = - glm::normalize(s_cam->m_pos);

	if((s_cam->m_forward.x == 0)&&(s_cam->m_forward.y == 0))
			s_cam->m_right = glm::vec3(1, 0, 0);
	else
		s_cam->m_right = glm::vec3(s_cam->m_forward.y, -s_cam->m_forward.x, 0);
	s_cam->m_right = glm::normalize(s_cam->m_right);

	s_cam->m_up = glm::cross(s_cam->m_right, s_cam->m_forward);
	s_cam->m_up = glm::normalize(s_cam->m_up);
	
	s_cam->m_up *= s_cam->m_resolution.y / 2;
	s_cam->m_right *= s_cam->m_resolution.x / 2;

	s_cam->m_forward *= s_cam->m_resolution.y / 2 / glm::tan(glm::radians(s_cam->m_viewAngle[1]));
}