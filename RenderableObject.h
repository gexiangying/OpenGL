#pragma once
class Renderer;
class RenderableObject
{
public:
	RenderableObject();
	virtual ~RenderableObject();

	virtual void draw(const Renderer *renderer) = 0;
};

