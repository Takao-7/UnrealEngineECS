// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "ECSIncludes.h"
#include "UEEnTTEntity.generated.h"


/** Entity struct. Because of it's small size, you don't need to pass it around by reference, instead just copy it */
USTRUCT(BlueprintType)
struct UNREALENGINEECS_API FEntity
{
    GENERATED_BODY()
    
    friend class entt::registry;
    
public:
    FEntity(){}
    FEntity(entt::entity Handle, entt::registry& Registry);


    //---------- Functions ----------//
public:
    /** Add a component and pass through it's constructor arguments */
    template<typename Component, typename... Args>
	Component& AddComponent(Args&&... args)
    {
    	checkf(!HasComponent<Component>(), TEXT("We already have a component with that class"));
    	return OwningRegistry->emplace<Component>(EntityHandle, std::forward<Args>(args)...);
    }

    /**
     * @brief Assigns or replaces the given component for an entity.
     *
     * @tparam Component Type of component to assign or replace.
     * @tparam Args Types of arguments to use to construct the component.
     * @param args Parameters to use to initialize the component.
     * @return A reference to the newly created component.
     */
    template<typename Component, typename... Args>
    decltype(auto) AddOrReplaceComponent(Args&&... args)
    {
        return OwningRegistry->emplace_or_replace<Component, Args...>(EntityHandle, std::forward<Args>(args)...);
    }

    /** Removes the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
	void RemoveComponent()
    {
    	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
    	OwningRegistry->remove<Component>(EntityHandle);
    }
    
    /**
     * Removes the given component from this entity, if we have it
     * @eturn Did we had the component?
     */
    template<typename Component>
	bool RemoveComponentChecked()
    {
    	return OwningRegistry->remove_if_exists<Component>(EntityHandle) > 0;
    }

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
	Component& GetComponent()
    {
    	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
    	return OwningRegistry->get<Component>(EntityHandle);
    }

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
	Component& GetComponent() const
    {
    	checkf(HasComponent<Component>(), TEXT("We don't have a component with that class"));
    	return OwningRegistry->get<Component>(EntityHandle);
    }

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename... Component>
    decltype(auto) GetComponents()
    {
        checkf(HasAllComponent<Component...>(), TEXT("We don't have all components with these classes"));
        return OwningRegistry->get<Component...>(EntityHandle);
    }

    /** Do we have any of the given components? */
    template<typename... Component>
	bool HasComponent() const
    {
    	return OwningRegistry->any<Component...>(EntityHandle);
    }

    /** Do we have all of the given components? */
    template<typename... Component>
	bool HasAllComponent() const
    {
    	return OwningRegistry->has<Component...>(EntityHandle);
    }


    //---------- Operators ----------//
public:
    explicit operator bool() const;
    bool operator==(const FEntity& Other) const;
    bool operator!=(const FEntity& Other) const;
    FEntity& operator=(const entt::entity& OtherHandle);


    //---------- Variables ----------//    
public:
    static const FEntity NullEntity;

private:
    /* uint32 identifier for this entity. Note that 0 is a legal entity! */
    entt::entity EntityHandle = entt::null;

    /* The registry where this entity belongs to. Note that this is a raw pointer, however the main registry is (should be) in the game
     * instance object, which get's only destroyed when the is closed */
    entt::registry* OwningRegistry = nullptr;
};
