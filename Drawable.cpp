#include "Drawable.h"
#include "Bindable.h"
#include "IndexBuffer.h"

Drawable::~Drawable() = default;

void Drawable::Bind(std::shared_ptr<Bindable> InBindable)
{
	if (typeid(*InBindable) == typeid(IndexBuffer))
	{
		assert(BoundIndexBuffer == nullptr && "Binding multiple indexbuffers!");
		BoundIndexBuffer = static_cast<const IndexBuffer*>(InBindable.get());
	}

	Bindables.push_back(std::move(InBindable));
}

void Drawable::Draw(const Graphics& InGraphics) const
{
	for (const auto& Bindable : Bindables)
	{
		Bindable->Bind(InGraphics);
	}

	InGraphics.DrawIndexed(BoundIndexBuffer->GetCount());
}