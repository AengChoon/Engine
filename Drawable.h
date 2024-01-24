#pragma once
#include "EngineWin.h"
#include <DirectXMath.h>
#include <memory>
#include "Graphics.h"

class Bindable;
class IndexBuffer;

class Drawable
{
	template<typename T>
	friend class DrawableBase;

public:
	Drawable() = default;
	Drawable(const Drawable&) = delete;
	Drawable(Drawable&&) = delete;
	Drawable& operator=(const Drawable&) = delete;
	Drawable& operator=(Drawable&&) = delete;
	virtual ~Drawable() = default;

	virtual void Update(float InDeltaTime) noexcept = 0;
	[[nodiscard]] virtual DirectX::XMMATRIX GetTransformMatrix() const noexcept = 0;

	void Draw(const Graphics& InGraphics) const;

protected:
	void AddInstanceBindable(std::unique_ptr<Bindable> InBindable);
	void AddInstanceIndexBuffer(std::unique_ptr<IndexBuffer> InIndexBuffer) noexcept;

private:
	[[nodiscard]] virtual const std::vector<std::unique_ptr<Bindable>>& GetStaticBindables() const noexcept = 0;

private:
	const IndexBuffer* MyIndexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> Bindables;
};
