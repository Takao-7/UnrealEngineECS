
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "ThirdParty/EnTT/entt/single_include/entt/entt.hpp"
#include "ECSRegistry.generated.h"


//////////////////////////////////////////////////
/**
 * @brief Alias for exclusion lists.
 * @tparam Type List of types.
 */
template<typename... Type>
using TECSExclude = entt::exclude_t<Type...>;

/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
using TECSGet = entt::get_t<Type...>;

template<typename... Type>
using TECSView = entt::view<Type...>;

template<typename... Type>
using TECSGroup = entt::group<Type...>;

//////////////////////////////////////////////////
class UNREALENGINEECS_API IECSRegistryInterface
{
	friend struct FEntity;

public:
	/**
	 * @brief Returns the number of existing components of the given type.
	 * @tparam Component Type of component of which to return the size.
	 * @return Number of existing components of the given type.
	 */
	template<typename Component>
	[[nodiscard]] int32 Size() const;

	/**
	 * @brief Returns the number of entities created so far.
	 * @return Number of entities created so far.
	 */
	[[nodiscard]] int32 Size() const;

	/**
	 * @brief Checks whether the registry or the pools of the given components
	 * are empty.
	 *
	 * A registry is considered empty when it doesn't contain entities that are
	 * still in use.
	 *
	 * @tparam Component Types of components in which one is interested.
	 * @return True if the registry or the pools of the given components are
	 * empty, false otherwise.
	 */
	template<typename... Component>
	[[nodiscard]] bool Empty() const;

	//////////////////////////////////////////////////
	/**
	 * @brief Creates a new entity and returns it.
	 *
	 * There are two kinds of possible entity identifiers:
	 *
	 * * Newly created ones in case no entities have been previously destroyed.
	 * * Recycled ones with updated versions.
	 *
	 * @return A valid entity identifier.
	 */
	struct FEntity Create();

	/**
	 * @brief Creates a new entity and returns it.
	 *
	 * @sa create
	 *
	 * If the requested entity isn't in use, the suggested identifier is created
	 * and returned. Otherwise, a new one will be generated for this purpose.
	 *
	 * @param Hint A desired entity identifier.
	 * @return A valid entity identifier.
	 */
	[[nodiscard]] struct FEntity Create(FEntity Hint);

	//////////////////////////////////////////////////
	/**
	 * @brief Destroys an entity.
	 *
	 * When an entity is destroyed, its version is updated and the identifier
	 * can be recycled at any time.
	 *
	 * @sa remove_all
	 *
	 * @param Entity A valid entity identifier.
	 */
	void Destroy(FEntity Entity);

	//////////////////////////////////////////////////
	/**
	 * @brief Returns a view for the given components.
	 *
	 * This kind of objects are created on the fly and share with the registry
	 * its internal data structures.<br/>
	 * Feel free to discard a view after the use. Creating and destroying a view
	 * is an incredibly cheap operation because they do not require any type of
	 * initialization.<br/>
	 * As a rule of thumb, storing a view should never be an option.
	 *
	 * Views do their best to iterate the smallest set of candidate entities.
	 * In particular:
	 *
	 * * Single component views are incredibly fast and iterate a packed array
	 *   of entities, all of which has the given component.
	 * * Multi component views look at the number of entities available for each
	 *   component and pick up a reference to the smallest set of candidates to
	 *   test for the given components.
	 *
	 * Views in no way affect the functionalities of the registry nor those of
	 * the underlying pools.
	 *
	 * @note
	 * Multi component views are pretty fast. However their performance tend to
	 * degenerate when the number of components to iterate grows up and the most
	 * of the entities have all the given components.<br/>
	 * To get a performance boost, consider using a group instead.
	 *
	 * @tparam Component Type of components used to construct the view.
	 * @tparam Exclude Types of components used to filter the view.
	 * @return A newly created view.
	 */
	template<typename... Component, typename... Exclude>
	[[nodiscard]] TECSView<TECSExclude<Exclude...>, Component...> View(TECSExclude<Exclude...> = {}) const
	{
		return Registry.view<Component...>(TECSExclude<Exclude...>());
	}

	/*! @copydoc View */
	template<typename... Component, typename... Exclude>
	[[nodiscard]] TECSView<TECSExclude<Exclude...>, Component...> View(TECSExclude<Exclude...> Excludes = {})
	{
		return Registry.view<Component...>(TECSExclude<Exclude...>());
	}

	//////////////////////////////////////////////////
	/**
	 * @brief Returns a group for the given components.
	 *
	 * This kind of objects are created on the fly and share with the registry
	 * its internal data structures.<br/>
	 * Feel free to discard a group after the use. Creating and destroying a
	 * group is an incredibly cheap operation because they do not require any
	 * type of initialization, but for the first time they are requested.<br/>
	 * As a rule of thumb, storing a group should never be an option.
	 *
	 * Groups support exclusion lists and can own types of components. The more
	 * types are owned by a group, the faster it is to iterate entities and
	 * components.<br/>
	 * However, groups also affect some features of the registry such as the
	 * creation and destruction of components, which will consequently be
	 * slightly slower (nothing that can be noticed in most cases).
	 *
	 * @note
	 * Pools of components that are owned by a group cannot be sorted anymore.
	 * The group takes the ownership of the pools and arrange components so as
	 * to iterate them as fast as possible.
	 *
	 * @tparam Owned Types of components owned by the group.
	 * @tparam Get Types of components observed by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Get, typename... Exclude>
	[[nodiscard]] TECSGroup<TECSExclude<Exclude...>, TECSGet<Get...>, Owned...> Group(TECSGet<Get...>, TECSExclude<Exclude...> = {})
	{
		return Registry.group<Owned...>(TECSGet<Get...>(), TECSExclude<Exclude...>());
	}

	/**
	 * @brief Returns a group for the given components.
	 *
	 * @sa group
	 *
	 * @tparam Owned Types of components owned by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Exclude>
	[[nodiscard]] TECSGroup<TECSExclude<Exclude...>, TECSGet<>, Owned...> Group(TECSExclude<Exclude...> = {})
	{
		return Registry.group<Owned...>(TECSExclude<Exclude...>());
	}

private:
	entt::registry Registry;
};

//////////////////////////////////////////////////
template <typename Component>
int32 IECSRegistryInterface::Size() const
{
	return Registry.size<Component>();
}

inline int32 IECSRegistryInterface::Size() const
{
	return Registry.size();
}

//////////////////////////////////////////////////
template <typename ... Component>
bool IECSRegistryInterface::Empty() const
{
	return Registry.empty<Component...>();
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
/**
 * Game instance registry.
 * This registry is valid throughout the entire game.
 */
UCLASS(BlueprintType)
class UNREALENGINEECS_API UECSRegistry : public UGameInstanceSubsystem, public IECSRegistryInterface
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Return a reference to the game instance registry. This is safe to call after the game instance has been initialized */
	static UECSRegistry& GetRegistry();
	
private:
	static UECSRegistry* RegistryPtr;
};
