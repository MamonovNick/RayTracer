#include "Scene.h"

void CScene::set_param(float hr, float ratio)
{
	sphrad = hr;
	diskrad = hr * ratio;
	plnorm = glm::vec3(0,0,1);
	plpos = glm::vec3(1,2,0);
	pl1_r = 9e9;
	pl2_r = 8e9;
}

void CScene::load_texture_disk(std::string fileName)
{
	CImage* pImage = LoadImageFromFile(fileName);

  if(pImage->GetBPP() == 32)
  {
		xRes = pImage->GetWidth();
		yRes = pImage->GetHeight();
		pixels.resize(xRes*yRes);

    auto pData = (unsigned char*)pImage->GetBits();
    auto pCurrentLine = pData;
	  int imageDisplacement = 0;
		int textureDisplacement = 0;

    int pitch = pImage->GetPitch();
    for(int i = 0; i < pImage->GetHeight(); i++) // Image lines
    {
      for(int j = 0; j < pImage->GetWidth(); j++) // Pixels in line
      {
				glm::uvec4 c;
				c.b = pCurrentLine[imageDisplacement + j * 4];
        c.g = pCurrentLine[imageDisplacement + j * 4 + 1];
        c.r = pCurrentLine[imageDisplacement + j * 4 + 2];
        c.a = pCurrentLine[imageDisplacement + j * 4 + 3];
				pixels[textureDisplacement + j] = c;
      }
	  imageDisplacement += pitch;
		textureDisplacement += xRes;
    }
  }
}

void CScene::load_texture_pan(std::string fileName)
{
	CImage* pImage = LoadImageFromFile(fileName);

	xRes_p = pImage->GetWidth();
	yRes_p = pImage->GetHeight();
	pixels_p.resize(xRes_p*yRes_p);

  auto pData = (unsigned char*)pImage->GetBits();
  auto pCurrentLine = pData;
	int imageDisplacement = 0;
	int textureDisplacement = 0;

  int pitch = pImage->GetPitch();

  for(int i = 0; i < pImage->GetHeight(); i++) // Image lines
  {
	  for(int j = 0; j < pImage->GetWidth(); j++) // Pixels in line
    {
			glm::uvec3 c;
				c.b = pCurrentLine[imageDisplacement + j * 3];
				c.g = pCurrentLine[imageDisplacement + j * 3 + 1];
				c.r = pCurrentLine[imageDisplacement + j * 3 + 2];
				pixels_p[textureDisplacement + j] = c;
    }
	  imageDisplacement += pitch;
		textureDisplacement += xRes_p;
  }
}

void CScene::load_texture_planet(std::string fileName1, std::string fileName2)
{
	//planet1
	CImage* pImage = LoadImageFromFile(fileName1);
	xRes_pl1 = pImage->GetWidth();
	yRes_pl1 = pImage->GetHeight();
	pixels_pl1.resize(xRes_pl1*yRes_pl1);

  auto pData = (unsigned char*)pImage->GetBits();
  auto pCurrentLine = pData;
	int imageDisplacement = 0;
	int textureDisplacement = 0;

  int pitch = pImage->GetPitch();

  for(int i = 0; i < pImage->GetHeight(); i++) // Image lines
  {
	  for(int j = 0; j < pImage->GetWidth(); j++) // Pixels in line
    {
			glm::uvec3 c;
				c.b = pCurrentLine[imageDisplacement + j * 3];
				c.g = pCurrentLine[imageDisplacement + j * 3 + 1];
				c.r = pCurrentLine[imageDisplacement + j * 3 + 2];
				pixels_pl1[textureDisplacement + j] = c;
    }
	  imageDisplacement += pitch;
		textureDisplacement += xRes_pl1;
  }

	//planet2
	pImage = LoadImageFromFile(fileName2);
	xRes_pl2 = pImage->GetWidth();
	yRes_pl2 = pImage->GetHeight();
	pixels_pl2.resize(xRes_pl2*yRes_pl2);

  auto pData2 = (unsigned char*)pImage->GetBits();
  auto pCurrentLine2 = pData2;
	imageDisplacement = 0;
	textureDisplacement = 0;

  pitch = pImage->GetPitch();

  for(int i = 0; i < pImage->GetHeight(); i++) // Image lines
  {
	  for(int j = 0; j < pImage->GetWidth(); j++) // Pixels in line
    {
			glm::uvec3 c;
				c.b = pCurrentLine2[imageDisplacement + j * 3];
				c.g = pCurrentLine2[imageDisplacement + j * 3 + 1];
				c.r = pCurrentLine2[imageDisplacement + j * 3 + 2];
				pixels_pl2[textureDisplacement + j] = c;
    }
	  imageDisplacement += pitch;
		textureDisplacement += xRes_pl2;
  }
}

bool CScene::TestPlane(glm::vec3* a, glm::vec3* b, glm::vec3* c, glm::vec3* col, bool* bl, float* f)
{
	if((a->z > 0)&&(b->z > 0)||(a->z < 0)&&(b->z < 0))
		return false;
	float t = - ((glm::dot(*a, plnorm) - glm::dot(plnorm, plpos))/ glm::dot(plnorm, *c));
	float tx = c->x * t;
	float ty = c->y * t;
	float tz = c->z * t;
	glm::vec3 pos;
	pos.x = a->x + tx;
	pos.y = a->y + ty;
	pos.z = a->z + tz;
	float pr = glm::length(pos);
	if((pr <= diskrad))
	{
		bool l = TakePixel_disk(col, &pos, f);
		*bl = true;
		return l;
	}
	else
		return false;
}

bool CScene::TestSphere(glm::vec3* ray_pos, glm::vec3* newpos, glm::vec3* ray_dir)
{
	if (glm::length(*newpos) > sphrad)
		return false;
	else 
		return true;
}

bool CScene::TakePixel_disk(glm::vec3* col, glm::vec3* pos, float* alpha)
{
	float r = 2 * diskrad;
	glm::vec2 tpos;
	tpos.x = pos->x;
	tpos.y = pos->y;
	tpos.x += diskrad;
	tpos.y += diskrad;
	tpos *= 1 / r;
	tpos.x *= (xRes - 1);
	tpos.y *= (yRes - 1);
	tpos.x = glm::round(tpos.x);
	tpos.y = glm::round(tpos.y);
	if (pixels[(yRes - 1 - tpos.y) * xRes + tpos.x].a > 0)
	{
		col->b = pixels[(yRes - 1 - tpos.y) * (xRes) + tpos.x].b;
		col->g = pixels[(yRes - 1 - tpos.y) * (xRes) + tpos.x].g;
		col->r = pixels[(yRes - 1 - tpos.y) * (xRes) + tpos.x].r;
		*alpha = pixels[(yRes - 1 - tpos.y) * xRes + tpos.x].a / 255.0f;
		return true;
	}
	else
		return false;
}

void CScene::TestPanoram(glm::vec3* a, glm::vec3* b)
{
	float f = std::atan2(a->x, a->y);
	float o = std::asin(a->z);
	glm::uint H = yRes_p;
	f = f + Pi;
	f = f * H;
	f = f / Pi;
	o = o + Pi/2;
	o = o * H;
	o = o / Pi;
	f = glm::round(f);
	o = glm::round(o);
	
	b->b = pixels_p[(yRes_p - 1 - o) * (xRes_p) + f].b;
	b->g = pixels_p[(yRes_p - 1 - o) * (xRes_p) + f].g;
	b->r = pixels_p[(yRes_p - 1 - o) * (xRes_p) + f].r;
}

bool CScene::TestPlanets(glm::vec3* ray_pos, glm::vec3* at, glm::vec3* bt, glm::vec3* color)
{
	float f, o, b, c, d, sqrtd, t1, t2, min_t, max_t, t;

	if (glm::distance(*bt, plan1) > pl1_r)
	{
		if (glm::distance(*bt, plan2) > pl2_r)
		{
			return false;
		}
		else
		{
			glm::vec3 k = *ray_pos - plan2;
			b = 2 * glm::dot(k, *at);
			c = k.x*k.x + k.y*k.y + k.z*k.z - pl2_r * pl2_r;
			d = b * b - 4*c;
			sqrtd = glm::sqrt(d);
			t1 = -b + sqrtd;
			t2 = -b - sqrtd;
			t1 = t1 / 2;
			t2 = t2 / 2;
			min_t = glm::min(t1,t2);
			max_t = glm::max(t1,t2);
			t = (min_t >= 0) ? min_t : max_t;
			*at *= t;
			glm::vec3 nt = *at + *ray_pos;
			nt = nt - plan2;
			nt = glm::normalize(nt);
			f = std::atan2(nt.x, nt.y);
			o = std::asin(nt.z);
			glm::uint H = yRes_pl2;
			f = f + Pi;
			f = f * H;
			f = f / Pi;
			o = o + Pi/2;
			o = o * H;
			o = o / Pi;
			f = glm::round(f);
			o = glm::round(o);

			color->b = pixels_pl2[(yRes_pl2 - 1 - o) * (xRes_pl2) + f].b;
			color->g = pixels_pl2[(yRes_pl2 - 1 - o) * (xRes_pl2) + f].g;
			color->r = pixels_pl2[(yRes_pl2 - 1 - o) * (xRes_pl2) + f].r;
			return true;
		}
	}
	else
	{
		glm::vec3 k = *ray_pos - plan1;
		b = 2 * glm::dot(k, *at);
		c = k.x*k.x + k.y*k.y + k.z*k.z - pl1_r * pl1_r;
		d = b * b - 4*c;
		sqrtd = glm::sqrt(d);
		t1 = -b + sqrtd;
		t2 = -b - sqrtd;
		t1 = t1 / 2;
		t2 = t2 / 2;
		min_t = glm::min(t1,t2);
		max_t = glm::max(t1,t2);
		t = (min_t >= 0) ? min_t : max_t;
		*at *= t;
		glm::vec3 nt = *at + *ray_pos;
		nt = nt - plan1;
		nt = glm::normalize(nt);
		f = std::atan2(nt.x, nt.y);
		o = std::asin(nt.z);
		glm::uint H = yRes_pl1;
		f = f + Pi;
		f = f * H;
		f = f / Pi;
		o = o + Pi/2;
		o = o * H;
		o = o / Pi;
		f = glm::round(f);
		o = glm::round(o);

		color->b = pixels_pl1[(yRes_pl1 - 1 - o) * (xRes_pl1) + f].b;
		color->g = pixels_pl1[(yRes_pl1 - 1 - o) * (xRes_pl1) + f].g;
		color->r = pixels_pl1[(yRes_pl1 - 1 - o) * (xRes_pl1) + f].r;
		return true;
	}
}

CImage* CScene::LoadImageFromFile(std::string fileName)
{
  CImage* pImage = new CImage;

  if(SUCCEEDED(pImage->Load(fileName.c_str())))
    return pImage;
  else
  {
    delete pImage;
    return NULL;
  }
}