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
/* Holds a weak object pointer to an actor. Added by the UECS_BridgeComponent to it's entity */
struct FActorPtrComponent
{    
    FActorPtrComponent(AActor* Actor);

    TWeakObjectPtr<AActor> Pointer;

    const AActor* operator->() const
    {
        return Pointer.Get();
    }

    AActor* operator->()
    {
        return Pointer.Get();
    }
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
    
    ~FRelationship()
    {
        
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
    FEntity First = FEntity::NullEntity;

    /* When we are attached to an entity, this is our previous sibling in our parent's list of children */
    FEntity Prev = FEntity::NullEntity;

    /* When we are attached to an entity, this is our next sibling in our parent's list of children */
    FEntity Next = FEntity::NullEntity;

    /* The entity that we are attached, too */
    FEntity Parent = FEntity::NullEntity;
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

struct FSyncTransformToECS
{
    /* Flag set by the actor component to indicate that the actor's transform changed and should be synced to ECS.
     * This is done because the ECS can run multi-threaded and we want to ensure that the transform is only synced to the ECS before
     * any system has updated */
    bool bSyncTransform = false;
};

USTRUCT(BlueprintType)
struct FSyncTransformToActor
{
    GENERATED_BODY()

    /* When synchronising the transform from ECS to actor, should we sweep the actor to the new transform? */
    UPROPERTY(EditDefaultsOnly)
    bool bSweep = false;

    UPROPERTY(EditDefaultsOnly)
    ETeleportType TeleportType = ETeleportType::None;

    /* Flag which indicates if the transform should be synced from ECS to the actor */
    bool bSyncTransform = true;
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
    virtual void BeginPlay() override;
    virtual void RegisterComponentWithECS() override {};
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/** Synchronises this owner's transform with the ECS */
UCLASS(ClassGroup = "ECS", BlueprintType, meta = (BlueprintSpawnableComponent, DisplayName = "ECS Transform Component"))
class UNREALENGINEECS_API UECS_SyncTransformComponent : public UActorComponent, public IECSComponentWrapper
{
    GENERATED_BODY()

public:
    virtual void RegisterComponentWithECS() override;
    virtual void UpdateECSComponent_Implementation() override;

private:
    void OnRootComponentTransformChanged(USceneComponent* UpdatedComponent, EUpdateTransformFlags UpdateTransformFlags,
                                         ETeleportType Teleport);

protected:
    UPROPERTY(EditDefaultsOnly, Category = "ECS")
    ESyncType SyncType = ESyncType::BothWays;

    /* Default parameters for this component. When changed after BeginPlay(), you have to call UpdateECSComponent() in order to copy the
    * new values to the ECS component. @see UpdateDefaultsFromECS() */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (EditCondition = "SyncType == ESyncType::BothWays || SyncType == ECS_To_Actor"),
              Category = "ECS")
    FSyncTransformToActor DefaultValues;

private:
    FDelegateHandle TransformChangedHandle;
};
