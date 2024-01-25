#include "Drawable.h"
#include "Bindable.h"
#include "IndexBuffer.h"

void Drawable::Update(float InDeltaTime) noexcept
{
}

void Drawable::Draw(const Graphics& InGraphics) const
{
	for (const auto& Bindable : Bindables)
	{
		Bindable->Bind(InGraphics);
	}

	for (const auto& StaticBindable : GetStaticBindables())
	{
		StaticBindable->Bind(InGraphics);
	}

	InGraphics.DrawIndexed(MyIndexBuffer->GetCount());
}

void Drawable::AddInstanceBindable(std::unique_ptr<Bindable> InBindable)
{
	Bindables.emplace_back(std::move(InBindable));
}

void Drawable::AddInstanceIndexBuffer(std::unique_ptr<IndexBuffer> InIndexBuffer) noexcept
{
	MyIndexBuffer = InIndexBuffer.get();
	Bindables.emplace_back(std::move(InIndexBuffer));
}