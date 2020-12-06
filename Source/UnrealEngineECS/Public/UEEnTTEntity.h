// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "UEEnTTRegistry.h"

#include <entity/fwd.hpp>
#include <entity/entity.hpp>

#include "UEEnTTEntity.generated.h"


/** Entity struct. Because of it's small size, you don't need to pass it around by reference, instead just copy it */
USTRUCT(BlueprintType)
struct UNREALENGINEECS_API FEntity
{
    GENERATED_BODY()
    
    friend struct FRegistry;
    
public:
    FEntity(){}
    FEntity(entt::entity Handle, struct FRegistry& Registry);


    //---------- Functions ----------//
public:
    /** Add a component and pass through it's constructor arguments */
    template<typename Component, typename... Args>
    Component& AddComponent(Args&&... args);

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
        return OwningRegistry->EnTTRegistry.emplace_or_replace<Component, Args...>(EntityHandle, std::forward<Args>(args)...);
    }

    /** Removes the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    void RemoveComponent();
    
    /**
     * Removes the given component from this entity, if we have it
     * @eturn Did we had the component?
     */
    template<typename Component>
    bool RemoveComponentChecked();

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    Component& GetComponent();

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    Component& GetComponent() const;

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename... Component>
    decltype(auto) GetComponents()
    {
        checkf(HasAllComponent<Component...>(), TEXT("We don't have all components with these classes"));
        return OwningRegistry->EnTTRegistry.get<Component...>(EntityHandle);
    }

    /** Does this entity has a component from class T? */
    template<typename Component>
    bool HasComponent() const;

    /** Do we have any of the given components? */
    template<typename... Components>
    bool HasAnyComponent() const;

    /** Do we have all of the given components? */
    template<typename... Components>
    bool HasAllComponent() const;


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
    FRegistry* OwningRegistry = nullptr;
};
