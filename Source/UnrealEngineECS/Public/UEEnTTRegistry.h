// Copyright @Paul Larrass 2020

#pragma once
#include <entity/registry.hpp>


template<typename... Args>
using TView = entt::basic_view<entt::entity, Args...>;

template<typename... Excludes>
using TExclude = entt::exclude_t<Excludes...>;

/**
 * Wrapper struct for the EnTT Registry with helper functions.
 * The registry holds and manages all components. 
 */
struct UNREALENGINEECS_API FRegistry
{
	friend struct FEntity;

	
	//---------- Functions ----------//
public:
	~FRegistry();
	
	/** Create a new entity */
	struct FEntity CreateEntity();

	/** Create the singleton entity and bind it to this registry */
	void CreateSingletonEntity();

	/** Returns the singleton entity. This is an entity which is used for singleton components, e.g. components, which only exists once */
	static FEntity& GetSingletonEntity();

	template<typename... Components, typename... Excludes>
	TView<TExclude<Excludes...>, Components...> View(TExclude<Excludes...> = {});


	//---------- Variables ----------//
private:
	entt::registry EnTTRegistry;
};
