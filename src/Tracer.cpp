#include "Tracer.h"
#include <omp.h>
#include <stdio.h>

using namespace glm;

float CTracer::CalcRadius(float M)
{
  return 2 * M * G / C / C ;
}

void CTracer::MakeRay(vec3* oldpos, vec3* olddir, float M, SRay* ray)
{ 
	vec3 av = vec3(-ray->m_start.x, -ray->m_start.y, -ray->m_start.z);
	float sqr_r = ray->m_start.x * ray->m_start.x + ray->m_start.y * ray->m_start.y + ray->m_start.z * ray->m_start.z;
	vec3 v = ray->m_dir;

	*oldpos = ray->m_start;
	*olddir = ray->m_dir;

	float a = G * M / sqr_r;
	float dt = 10;
	av = normalize(av);
	av *= a;
	av *= dt;

	ray->m_dir += av;
	ray->m_dir = normalize(ray->m_dir);
	ray->m_dir *= C;
	v *= dt;
	av *= dt/2.0f;
	ray->m_start += v + av;
}

bool CTracer::TestRay(vec3* oldpos, vec3* color, int ratio, SRay* ray, bool* alb, float* alf)
{
	vec3 pos;
	vec3 vecpos = normalize(ray->m_start - *oldpos);
	*alb = false;

	if (m_pScene->TestSphere(oldpos, &ray->m_start, &vecpos))
	{
		*color = vec3(0,0,0);
		return true;
	}
	else
	{
		if ((set->m_planets)&&(m_pScene->TestPlanets(oldpos, &vecpos, &ray->m_start, color)))
			return true;
		else
		{
			if (m_pScene->TestPlane(oldpos, &ray->m_start, &vecpos, color, alb, alf))
				return true;
			else
				if(ratio > IterStop)
				{
					m_pScene->TestPanoram(&vecpos, color);
					return true;
				}
		}
	}
	return false;
}

vec3 CTracer::TraceRay(SRay* ray)
{
	vec3 sumcol(0,0,0);
  vec3 color(0, 0, 0);
  vec3 oldpos(0, 0, 0);
	vec3 olddir(0, 0, 0);
	bool alb;
	float alf = 0;
	float alf2 = 0;
	float alft = 0;

	int ratio = 0;

  while(true)
  {
		ratio++;
		MakeRay(&oldpos, &olddir, charc->m_mass, ray);
		if (set->m_alpha)
		{
			if (TestRay(&oldpos, &color, ratio, ray, &alb, &alf))
				if (!alb)
				{
					sumcol = color;
					break;
				}
				else
				{
					color *= alf;
					sumcol += color;

					vec3 col2(0,0,0);
					vec3 sumcol2(0,0,0);

					while (true)
					{
						ratio++;
						MakeRay(&oldpos, &olddir, charc->m_mass, ray);
						if (TestRay(&oldpos, &color, ratio, ray, &alb, &alf2))
							if(!alb)
							{
								color *= (1 - alft);
								sumcol2 += color;
								break;
							}
							else
							{
								alft = alf2;
								color *= alf2;
								sumcol2 += color;
							}
					}

					sumcol2 *= (1 - alf);
					sumcol += sumcol2;
					break;
				}
		}
		else
			if (TestRay(&oldpos, &color, ratio, ray, &alb, &alf))
			{
				sumcol = color;
				break;
			}
  }
  return sumcol;
}

void CTracer::RenderImage()
{
	m_camera->m_pixels.resize(m_camera->m_resolution.x * m_camera->m_resolution.y);
	int Res = m_camera->m_resolution.x * m_camera->m_resolution.y;
	int xRes = m_camera->m_resolution.x;
	int yRes = m_camera->m_resolution.y;

	if(set->m_omp == 1)
	{
		#pragma omp parallel
		{
		#pragma omp for
		for(int i = 0; i < Res; i++)
		{
			SRay ray;
			vec3 rt, ut, col_pix(0,0,0);
			int k = i / xRes;
			int j = i % xRes;

			if (set->m_aa == 1)
			{
				for(int p = 1; p <= 3; p+=2)
					for(int q = 1; q <= 3; q+=2)
					{
						ut = m_camera->m_up;
						ut *= ((k + p*0.25f) / yRes - 0.5f);
						rt = m_camera->m_right;
						rt *= ((j + q*0.25f) / xRes - 0.5f);

						ray.m_start = m_camera->m_pos;
						ray.m_dir = normalize(m_camera->m_forward + rt + ut);
						ray.m_dir *= C;
						col_pix += TraceRay(&ray);
					}
			}

			ut = m_camera->m_up;
			ut *= ((k + 0.5f) / yRes - 0.5f);
			rt = m_camera->m_right;
			rt *= ((j + 0.5f) / xRes - 0.5f);

			ray.m_start = m_camera->m_pos;
			ray.m_dir = normalize(m_camera->m_forward + rt + ut);
			ray.m_dir *= C;
			col_pix += TraceRay(&ray);
			col_pix /= 1.0f + set->m_aa*4;
			m_camera->m_pixels[i] = col_pix;
		}
		}
	}
	else
	{
		for(int i = 0; i < Res; i++)
		{
			SRay ray;
			vec3 rt, ut, col_pix(0,0,0);
			int k = i / xRes;
			int j = i % xRes;

			if (set->m_aa == 1)
			{
				for(int p = 1; p <= 3; p+=2)
					for(int q = 1; q <= 3; q+=2)
					{
						ut = m_camera->m_up;
						ut *= ((k + p*0.25f) / yRes - 0.5f);
						rt = m_camera->m_right;
						rt *= ((j + q*0.25f) / xRes - 0.5f);

						ray.m_start = m_camera->m_pos;
						ray.m_dir = normalize(m_camera->m_forward + rt + ut);
						ray.m_dir *= C;
						col_pix += TraceRay(&ray);
					}
			}
			ut = m_camera->m_up;
			ut *= ((k + 0.5f) / yRes - 0.5f);
			rt = m_camera->m_right;
			rt *= ((j + 0.5f) / xRes - 0.5f);

			ray.m_start = m_camera->m_pos;
			ray.m_dir = normalize(m_camera->m_forward + rt + ut);
			ray.m_dir *= C;
			col_pix += TraceRay(&ray);
			col_pix /= 1.0f + set->m_aa*4;
			m_camera->m_pixels[i] = col_pix;
		}
	}
}

void CTracer::SaveImageToFile(std::string fileName)
{
  CImage image;

  int width = m_camera->m_resolution[0];
  int height = m_camera->m_resolution[1];

  image.Create(width, height, 24);
    
	int pitch = image.GetPitch();
	unsigned char* imageBuffer = (unsigned char*)image.GetBits();

	if (pitch < 0)
	{
		imageBuffer += pitch * (height - 1);
		pitch = -pitch;
	}

	int i, j;
	int imageDisplacement = 0;
	int textureDisplacement = 0;

	for (i = 0; i < height; i++)
	{
    for (j = 0; j < width; j++)
    {
      glm::vec3 color = m_camera->m_pixels[textureDisplacement + j];
      imageBuffer[imageDisplacement + j * 3] = color.b;
      imageBuffer[imageDisplacement + j * 3 + 1] = color.g;
      imageBuffer[imageDisplacement + j * 3 + 2] = color.r;
    }

		imageDisplacement += pitch;
		textureDisplacement += width;
	}

	image.Save(fileName.c_str());
	image.Destroy();
}