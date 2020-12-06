
#include "ECSObserver.h"
#include "UEEnTTEntity.h"
#include "UEEnTTRegistry.h"

template <typename ... Component>
void TObserver<Component...>::Each(TFunction<void(FEntity Entity)> Func)
{
	Observer.each([this, Func](auto Entity)
    {
        Func(FEntity(Entity, *Registry));
    });
}

template <typename ... Component>
void TObserver<Component...>::Each(TFunction<void(FEntity Entity)> Func) const
{
	Observer.each([this, Func](auto Entity)
    {
        Func(FEntity(Entity, *Registry));
    });
}

//////////////////////////////////////////////////
template <typename ... Component>
void TObserver<Component...>::Connect(const FRegistry& Registry)
{
    this->Registry = &Registry;
    Registry.ConnectObserver(*this);
}
