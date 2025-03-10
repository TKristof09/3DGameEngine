#ifndef FORWARD_AMBIENT_H
#define FORWARD_AMBIENT_H

#include "../Shader.h"
#include "../Material.h"

class RenderingEngine;

class ForwardAmbient : public Shader
{
public:
	ForwardAmbient();
	~ForwardAmbient() = default;

	static ForwardAmbient* GetInstance()
	{
		static ForwardAmbient instance;
		return &instance;
	}

	void UpdateUniforms(const Transform& transform,/* const Camera& camera,*/ const Material& material,
	                    RenderingEngine* renderingEngine, bool animated = false) const override;
};


#endif // !FORWARD_AMBIENT_H
