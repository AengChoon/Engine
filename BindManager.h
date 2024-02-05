#pragma once
#include <memory>
#include <unordered_map>
#include "Bindable.h"

class BindManager
{
public:
	template<typename T, typename... Params>
	[[nodiscard]] static std::shared_ptr<T> Resolve(const Graphics& InGraphics, Params&&... InParams)
	{
		static_assert(std::is_base_of_v<Bindable, T>, "T is not derived from Bindable");
		return Get().ResolveImpl<T>(InGraphics, std::forward<Params>(InParams)...);
	}

private:
	template<typename T, typename... Params>
	[[nodiscard]] std::shared_ptr<T> ResolveImpl(const Graphics& InGraphics, Params&&... InParams)
	{
		const auto Key = T::GenerateUniqueID(std::forward<Params>(InParams)...);

		if (const auto TargetIterator = SharedBindables.find(Key); TargetIterator == SharedBindables.end() || TargetIterator->second.expired())
		{
			auto SharedBindable = std::make_shared<T>(InGraphics, std::forward<Params>(InParams)...);
			SharedBindables[Key] = SharedBindable;
			return SharedBindable;
		}
		else
		{
			return std::static_pointer_cast<T>(TargetIterator->second.lock());
		}
	}

	static BindManager& Get();

private:
	std::unordered_map<std::string, std::weak_ptr<Bindable>> SharedBindables;
};
