// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"

#include "ECSObserver.h"
#include "Engine/EngineBaseTypes.h"
#include "Engine/GameInstance.h"
#include "UEEnTTRegistry.h"

#include "ECSGameInstanceInterface.generated.h"


class IInstancedSystem;
struct FActorPtrComponent;
struct FSyncTransformToECS;
struct FSyncTransformToActor;


/**
* Delegate to bind systems.
* @param float			Delta time for this frame
* @param FRegistry&		The registry from which the system is called
*/
DECLARE_MULTICAST_DELEGATE_TwoParams(FSystemTickFunctionDelegate, float, FRegistry&);


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/** Tick function for systems */
struct UNREALENGINEECS_API FSystemTickFunction : public FTickFunction
{
	FSystemTickFunction();
	
	void SetTickGroup(ETickingGroup TickingGroup);

	FSystemTickFunctionDelegate Systems;
	FRegistry* Registry = nullptr;
	
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

	TObserver<const FActorPtrComponent, FTransform, FSyncTransformToECS> ObserverSyncTransformToECS;
	TObserver<const FActorPtrComponent, const FTransform, FSyncTransformToActor> ObserverSyncTransformToActor;

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
	static FRegistry& GetRegistry(UObject* ObjectRef);

	FRegistry& GetRegistry();
	const FRegistry& GetRegistry() const;

	/** Add a system as a free function (= function pointer), which is ticked in the provided ticking group */
	FDelegateHandle AddSystem(TBaseStaticDelegateInstance<void (float, FRegistry&)>::FFuncPtr InFunc,
							  ESystemTickingGroup TickingGroup = ESystemTickingGroup::PrePhysics);

	/** Add a system as a Ufunction, which is ticked in the provided ticking group */
	template <typename UserClass>
	FDelegateHandle AddSystem(UserClass* InUserObject, typename TMemFunPtrType<false, UserClass, void (float, FRegistry&)>::Type InFunc,
							  ESystemTickingGroup TickingGroup = ESystemTickingGroup::PrePhysics);

	/** Add a system as a TFunction or lambda, which is ticked in the provided ticking group */
	FDelegateHandle AddSystem(TFunction<void (float, FRegistry&)> Lambda,
							  ESystemTickingGroup TickingGroup = ESystemTickingGroup::PrePhysics);

	/** Remove a system */
	bool RemoveSystem(const FDelegateHandle& Handle);

protected:
	/** Must be called in OnWorldChanged() */
	void HandleWorldChange(UWorld* OldWorld, UWorld* NewWorld);

	
	//---------- Variables ----------//	
protected:
	FRegistry Registry;

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
