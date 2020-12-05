// Copyright @Paul Larrass 2020

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "UEEnTTEntity.h"
#include "ECSComponentWrapperInterface.h"

#include "UEEnTTComponents.generated.h"


//////////////////////////////////////////////////
/* Simple name component */
struct FNameComponent
{    
    FString Name = "Unnamed Entity";
};

//////////////////////////////////////////////////
/* Holds a weak object pointer to an actor */
struct FActorPtrComponent
{
    FActorPtrComponent(AActor* Actor);
    TWeakObjectPtr<AActor> Pointer;
};

//////////////////////////////////////////////////
struct FRelationship
{
    FRelationship(){}
    /**
     * @param Parent    The entity which our owner is now attached, too
     * @param Owner     The entity that owns this component
     */
    FRelationship(FEntity Parent, FEntity Owner)
    {
        this->Parent = Parent;
        
        Prev = Parent.GetComponent<FRelationship>().LastChild();
        Prev.GetComponent<FRelationship>().Next = Owner;
    }
    
    /** Call the given function for all of our children. Assumes that all children have the relationship component! */
    void ForEachChildren(TFunction<void(FEntity Child)> Function) const
    {
        auto& ParentRelationship = Parent.GetComponent<FRelationship>();
        auto CurrentEntity = ParentRelationship.First;
        
        while(CurrentEntity)
        {
            Function(CurrentEntity);
            CurrentEntity = CurrentEntity.GetComponent<FRelationship>().Next;
        }
    }

    /** Returns the last child that is directly attached to us */
    FEntity LastChild() const
    {
        FEntity Current = First;
        while (Current)
        {
            Current = Current.GetComponent<FRelationship>().Next;
        }
        return Current;
    }

    
    //---------- Variables ----------//
public:
    /* The first entity which is attached to us */
    FEntity First;

    /* When we are attached to an entity, this is our previous sibling in our parent's list of children */
    FEntity Prev;

    /* When we are attached to an entity, this is our next sibling in our parent's list of children */
    FEntity Next;

    /* The entity that we are attached, too */
    FEntity Parent;
};


//////////////////////////////////////////////////
UENUM(BlueprintType)
enum class ESyncType : uint8
{
    /* No sync at the moment */
    Disabled,
    /* Copy the data from the ECS after all system did run */
    ECS_To_Actor,
    /* Copy the data to the ECS before any system runs */
    Actor_To_ECS,
    /* Copy the data to the ECS, before any system runs and then copy the data back to the actor after all systems run */
    BothWays
};

USTRUCT()
struct FTransformSync
{
    GENERATED_BODY()

    FTransformSync(){}
    FTransformSync(ESyncType SyncType, bool bSweep, ETeleportType TeleportType)
    {
        this->SyncType = SyncType;
        this->bSweep = bSweep;
        this->TeleportType = TeleportType;
    }

    UPROPERTY(EditDefaultsOnly)
    ESyncType SyncType = ESyncType::ECS_To_Actor;

    UPROPERTY(EditDefaultsOnly)
    bool bSweep = false;

    UPROPERTY(EditDefaultsOnly)
    ETeleportType TeleportType = ETeleportType::None;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/**
 * Main component to link an actor with the ECS. Every actor that wants to use any ECS component must have this component, too.
 * Adds a FActorPtrComponent to the entity */
UCLASS(ClassGroup = "ECS", BlueprintType, meta = (BlueprintSpawnableComponent, DisplayName = "ECS Bridge Component"))
class UNREALENGINEECS_API UECS_BridgeComponent : public UActorComponent, public IECSComponentWrapper
{
    GENERATED_BODY()

public:
    UECS_BridgeComponent();
    virtual void BeginPlay() override;
    virtual void RegisterComponentWithECS() override {};
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/** Synchronises this owner's transform with the ECS */
UCLASS(ClassGroup = "ECS", BlueprintType, meta = (BlueprintSpawnableComponent, DisplayName = "ECS Transform Component"))
class UNREALENGINEECS_API UECS_TransformSyncComponent : public UActorComponent, public IECSComponentWrapper
{
    GENERATED_BODY()

public:
    UECS_TransformSyncComponent();
    virtual void RegisterComponentWithECS() override;

protected:
    /* Default parameters for this component. When changed after BeginPlay(), you have to call UpdateECSComponent() in order to copy the
    * new values to the ECS component. @see UpdateDefaultsFromECS() */
    UPROPERTY(EditDefaultsOnly, Category = "ECS")
    FTransformSync DefaultComponentParams;
};
