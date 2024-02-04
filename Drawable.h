#pragma once
#include "EngineWin.h"
#include <DirectXMath.h>
#include <memory>
#include <vector>

class Graphics;
class Bindable;
class IndexBuffer;

class Drawable
{
public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	Drawable(Drawable&&) = delete;
	Drawable& operator=(const Drawable&) = delete;
	Drawable& operator=(Drawable&&) = delete;
	virtual ~Drawable();

	[[nodiscard]] virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;

	void Bind(std::shared_ptr<Bindable> InBindable);
	void Draw(const Graphics& InGraphics) const;

private:
	const IndexBuffer* BoundIndexBuffer = nullptr;
	std::vector<std::shared_ptr<Bindable>> Bindables;
};
