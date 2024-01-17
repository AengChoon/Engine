#pragma once
#include "Drawable.h"
#include "IndexBuffer.h"

template<typename T>
class DrawableBase : public Drawable
{
protected:
	static bool IsStaticInitialized()
	{
		return !StaticBindables.empty();
	}

	static void AddStaticBindable(std::unique_ptr<Bindable> InBindable)
	{
		StaticBindables.emplace_back(std::move(InBindable));
	}

	void AddStaticIndexBuffer(std::unique_ptr<IndexBuffer> InIndexBuffer)
	{
		MyIndexBuffer = InIndexBuffer.get();
		StaticBindables.emplace_back(std::move(InIndexBuffer));
	}

	void SetIndexBufferFromStaticBindables()
	{
		auto StaticIndexBuffer = std::find_if(StaticBindables.begin(), StaticBindables.end(), [](const std::unique_ptr<Bindable>& StaticBindable)
		{
			return dynamic_cast<IndexBuffer*>(StaticBindable.get());
		});

		MyIndexBuffer = static_cast<IndexBuffer*>((*StaticIndexBuffer).get());
	}

private:
	[[nodiscard]] const std::vector<std::unique_ptr<Bindable>>& GetStaticBindables() const noexcept override
	{
		return StaticBindables;
	}

private:
	static inline std::vector<std::unique_ptr<Bindable>> StaticBindables;
};

