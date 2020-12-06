// Copyright @Paul Larrass 2020

#pragma once
#include <entity/registry.hpp>
#include <entt.hpp>

template<typename... Component>
class TObserver;


//////////////////////////////////////////////////
template<typename... Args>
using TView = entt::basic_view<entt::entity, Args...>;

/**
 * @brief Alias for exclusion lists.
 * @tparam Excludes List of types.
 */
template<typename... Excludes>
using TExclude = entt::exclude_t<Excludes...>;

/**
 * @brief Alias for lists of observed components.
 * @tparam Type List of types.
 */
template<typename... Type>
using TGet = entt::type_list<Type...>;

template<typename... Args>
using TGroup = entt::basic_group<entt::entity, Args...>;

template<typename... Args>
using TCollector = entt::basic_collector<>;

inline constexpr entt::basic_collector<> ECSCollector{};

//////////////////////////////////////////////////
/**
 * Wrapper struct for the EnTT Registry with helper functions.
 * The registry holds and manages all components. 
 */
struct UNREALENGINEECS_API FRegistry
{	
	friend struct FEntity;
	friend class IECSGameInstanceInterface;
	
	
	//---------- Functions ----------//
public:
	FRegistry();
	
	/** Create a new entity */
	struct FEntity CreateEntity();

	/** Returns the singleton entity FOR THIS registry. This is an entity which is used for singleton components, e.g. components,
	 * which only exists once */
	FEntity GetSingletonEntity();

	
	/**
	 * (Copied from entt::registry.hpp)
	 * 
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
    [[nodiscard]] TView<TExclude<Exclude...>, Component...> View(TExclude<Exclude...> = {}) const;

	/*! @copydoc View */
	template<typename... Component, typename... Exclude>
	[[nodiscard]] TView<TExclude<Exclude...>, Component...> View(TExclude<Exclude...> = {});


	/**
	 * (Copied from entt::registry.hpp)
	 * 
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
	 * @tparam Observed Types of components observed by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Observed, typename... Exclude>
    [[nodiscard]] TGroup<TExclude<Exclude...>, TGet<Observed...>, Owned...> Group(TGet<Observed...>, TExclude<Exclude...> = {});

	/**
	 * (Copied from entt::registry.hpp)
	 * 
	 * @brief Static version of Group(). Returns a group for the given components.
	 *
	 * @sa group
	 *
	 * @tparam Owned Types of components owned by the group.
	 * @tparam Observed Types of components observed by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Observed, typename... Exclude>
    [[nodiscard]] TGroup<TExclude<Exclude...>, TGet<Observed...>, Owned...> Group(TGet<Observed...>, TExclude<Exclude...> = {}) const;

	/**
	 * (Copied from entt::registry.hpp)
	 * 
	 * @brief Returns a group for the given components.
	 *
	 * @sa group
	 *
	 * @tparam Owned Types of components owned by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Exclude>
    [[nodiscard]] TGroup<TExclude<Exclude...>, TGet<>, Owned...> Group(TExclude<Exclude...> = {});

	/**
	 * (Copied from entt::registry.hpp)
	 * 
	 * @brief Returns a group for the given components.
	 *
	 * @sa group
	 *
	 * @tparam Owned Types of components owned by the group.
	 * @tparam Exclude Types of components used to filter the group.
	 * @return A newly created group.
	 */
	template<typename... Owned, typename... Exclude>
    [[nodiscard]] TGroup<TExclude<Exclude...>, TGet<>, Owned...> Group(TExclude<Exclude...> = {}) const;

	void ConnectObserver(TObserver<>& Observer);
	

	//---------- Variables ----------//
private:
	entt::registry EnTTRegistry;
	entt::entity SingletonEntity = entt::null;
};
