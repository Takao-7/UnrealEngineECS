
#include "UEEnTTRegistry.h"

#include "ECSObserver.h"
#include "UEEnTTEntity.h"

FRegistry::FRegistry()
{
	SingletonEntity = EnTTRegistry.create();
}

//////////////////////////////////////////////////
FEntity FRegistry::CreateEntity()
{
	return FEntity(EnTTRegistry.create(), *this);
}

//////////////////////////////////////////////////
FEntity FRegistry::GetSingletonEntity()
{
	return FEntity(SingletonEntity, *this);
}

//////////////////////////////////////////////////
template <typename ... Component, typename ... Exclude>
TView<TExclude<Exclude...>, Component...> FRegistry::View(TExclude<Exclude...>) const
{
	return EnTTRegistry.view<Component, Exclude>(TExclude<Exclude...>());
}

template <typename ... Component, typename ... Exclude>
TView<TExclude<Exclude...>, Component...> FRegistry::View(TExclude<Exclude...>)
{
	return EnTTRegistry.view<Component..., Exclude...>(TExclude<Exclude...>());
}

//////////////////////////////////////////////////
template <typename ... Owned, typename ... Observed, typename ... Exclude>
TGroup<TExclude<Exclude...>, TGet<Observed...>, Owned...> FRegistry::Group(TGet<Observed...>, TExclude<Exclude...>)
{
	return EnTTRegistry.group<Owned, Exclude>(TGet<Observed...>(), TExclude<Exclude...>());
}

template <typename ... Owned, typename ... Observed, typename ... Exclude>
TGroup<TExclude<Exclude...>, TGet<Observed...>, Owned...> FRegistry::Group(TGet<Observed...>, TExclude<Exclude...>) const
{
	return EnTTRegistry.group<Owned, Exclude>(TGet<Observed...>(), TExclude<Exclude...>());
}

template <typename ... Owned, typename ... Exclude>
TGroup<TExclude<Exclude...>, TGet<>, Owned...> FRegistry::Group(TExclude<Exclude...>)
{
	return EnTTRegistry.group<Owned, Exclude>(TExclude<Exclude...>());
}

template <typename ... Owned, typename ... Exclude>
TGroup<TExclude<Exclude...>, TGet<>, Owned...> FRegistry::Group(TExclude<Exclude...>) const
{
	return EnTTRegistry.group<Owned, Exclude>(TExclude<Exclude...>());
}

//////////////////////////////////////////////////
void FRegistry::ConnectObserver(TObserver<>& Observer)
{
	Observer.Observer.connect(EnTTRegistry, );
}
