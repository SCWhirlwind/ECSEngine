#pragma once
#include "Component.h"

class RectComponent : public Component
{
public:

	Vec2 points[4];
	Vec2 sEdges[4];
	Vec2 eEdges[4];

	RectComponent()
	{
	}

	RectComponent(const Vec2& p1, const Vec2& p2, const Vec2& p3, const Vec2& p4)
	{
		points[0] = p1;
		points[1] = p2;
		points[2] = p3;
		points[3] = p4;

		sEdges[0] = p1;
		sEdges[1] = p2;
		sEdges[2] = p4;
		sEdges[3] = p3;

		eEdges[0] = p2;
		eEdges[1] = p4;
		eEdges[2] = p3;
		eEdges[3] = p1;
	}
};