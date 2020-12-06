
#pragma once
#include "CoreMinimal.h"
#include <entity/observer.hpp>


struct FRegistry;


template<typename... Component>
class UNREALENGINEECS_API TObserver
{
	friend struct FRegistry;
	
public:
	void Each(TFunction<void (struct FEntity Entity)> Func);
	void Each(TFunction<void (struct FEntity Entity)> Func) const;

	void Connect(const FRegistry& Registry);
	
private:
	entt::observer Observer;
	FRegistry* Registry = nullptr;
};
