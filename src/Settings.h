#pragma once

#include "Scene.h"
#include "Types.h"

class PSet
{
	void set_str(std::string*);
public:
	void set_parametrs(SCamera*, SSettings*, SCharacter*, CScene*, char*);
	void proc_cam(SCamera*);
};