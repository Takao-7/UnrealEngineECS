// Copyright @Paul Larrass 2020

#pragma once
#include <entity/fwd.hpp>
#include <entity/entity.hpp>


/** Entity struct. Because of it's small size, you don't need to pass it around by reference, instead just copy it */
struct UNREALENGINEECS_API FEntity
{
    friend struct FRegistry;
    
public:
    FEntity(){}
    FEntity(entt::entity Handle, struct FRegistry& Registry);


    //---------- Functions ----------//
public:
    /** Add a component and pass through it's constructor arguments */
    template<typename Component, typename... Args>
    Component& AddComponent(Args&&... args);

    /** Add the given component to this entity, if it doesn't have it and use the provided parameters for the constructor.
     * Otherwise override the component's variable with the given parameters and return the existing component. */
    template<typename Component, typename... Args>
    Component& AddComponent_Checked(bool bOverrideExistingComponent, Args&&... args);

    /** Removes the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    void RemoveComponent();

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    Component& GetComponent();

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename Component>
    Component& GetComponent() const;

    /** Returns the given component from this entity. Asserts when we don't have the component */
    template<typename... Component>
    decltype(auto) GetComponents();

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

    /* The registry where this entity belongs to. We can use a raw pointer here, because entities are only valid to use while their
     * registry is valid. Furthermore, FEntity can only be created with a valid reference to an registry */
    FRegistry* OwningRegistry = nullptr;
};
