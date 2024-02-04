#pragma once
#include "DynamicVertex.h"
#include "IndexedTriangleList.h"

class Sphere
{
public:
	static IndexedTriangleList Make(DV::VertexLayout InLayout = {});
	static IndexedTriangleList MakeTessellated(DV::VertexLayout&& InLayout, int InLatitudeDiv, int InLongitudeDiv);
};