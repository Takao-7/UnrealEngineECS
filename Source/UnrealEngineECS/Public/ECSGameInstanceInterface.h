// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "ECSIncludes.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/GameInstance.h"

#include "ECSGameInstanceInterface.generated.h"


class IInstancedSystem;
struct FActorPtrComponent;
struct FSyncTransformToECS;
struct FSyncTransformToActor;


/**
* Delegate to bind systems.
* @param float				Delta time for this frame
* @param entt::registry&	The registry from which the system is called
*/
DECLARE_MULTICAST_DELEGATE_TwoParams(FSystemTickFunctionDelegate, float, entt::registry&);


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/** Tick function for systems */
struct UNREALENGINEECS_API FSystemTickFunction : public FTickFunction
{
	FSystemTickFunction();
	
	void SetTickGroup(ETickingGroup TickingGroup);

	FSystemTickFunctionDelegate Systems;
	entt::registry* Registry = nullptr;
	
private:
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
							 const FGraphEventRef& MyCompletionGraphEvent) override;
	virtual FString DiagnosticMessage() override;
	virtual FName DiagnosticContext(bool bDetailed) override;
};

/**
 * Special tick function for the pre-physics tick group, because here we have to call both the "CopyTransform" systems in the right order
 */
struct UNREALENGINEECS_API FPrePhysicsSystemTickFunction : public FSystemTickFunction
{
	FPrePhysicsSystemTickFunction();

	entt::observer ObserverSyncTransformToECS;
	entt::observer ObserverSyncTransformToActor;

private:
	virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
							 const FGraphEventRef& MyCompletionGraphEvent) override;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
UENUM()
enum class ESystemTickingGroup : uint8
{
	PrePhysics,
	DuringPhysics,
	PostPhysics
};

//////////////////////////////////////////////////
//////////////////////////////////////////////////
/**
 * Interface for the UGameInstance class.
 * The function "HandleWorldChange()" must be called in UGameInstance::OnWorldChanged().
 *
 * Add new systems with the provided functions. Map changes are handled automatically. 
 */
class UNREALENGINEECS_API IECSGameInstanceInterface
{	
public:
	IECSGameInstanceInterface();
	virtual ~IECSGameInstanceInterface() = default;
	
	/**
	 * Returns the main registry for this game instance. Will crash when the object is invalid or our game instance does not inherit
	 * from IECSGameInstanceInterface!
	 */
	static entt::registry& GetRegistry(UObject* ObjectRef);

	entt::registry& GetRegistry();
	const entt::registry& GetRegistry() const;

	/** Add a system as a free function (= function pointer), which is ticked in the provided ticking group */
	FDelegateHandle AddSystem(void (*InFunc)(float, entt::registry&), ESystemTickingGroup TickingGroup = ESystemTickingGroup::PrePhysics);

	/** Remove a system */
	bool RemoveSystem(const FDelegateHandle& Handle);

protected:
	/** Must be called in OnWorldChanged() */
	void HandleWorldChange(UWorld* OldWorld, UWorld* NewWorld);

	
	//---------- Variables ----------//	
protected:
	entt::registry Registry;

	/* Systems which run before the physics simulation */
	FPrePhysicsSystemTickFunction PrePhysicsTickFunction;

	/* Systems which run during the physics simulation */
	FSystemTickFunction DuringPhysicsTickFunction;

	/* Systems which run after the physics simulation */
	FSystemTickFunction PostPhysicsTickFunction;
};


/** Basic game instance which inherits from IECSGameInstanceInterface */
UCLASS(Blueprintable, BlueprintType, ClassGroup = "ECS")
class UNREALENGINEECS_API UECSGameInstance : public UGameInstance, public IECSGameInstanceInterface
{
	GENERATED_BODY()

public:
	virtual void OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld) override;
};
