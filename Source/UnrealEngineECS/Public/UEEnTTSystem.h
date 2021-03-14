// Copyright @Paul Larrass 2020

#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineBaseTypes.h"
#include "Subsystems/GameInstanceSubsystem.h"

#include "UEEnTTSystem.generated.h"

USTRUCT()
struct FECSSystemTickFunction : public FTickFunction
{
	GENERATED_BODY()

	UPROPERTY()
	class UECSSystem* Target = nullptr;

	/** 
	* Abstract function actually execute the tick. 
	* @param DeltaTime 					Frame time to advance, in seconds
	* @param TickType 					Kind of tick for this frame
	* @param CurrentThread 				Thread we are executing on, useful to pass along as new tasks are created
	* @param MyCompletionGraphEvent 	Completion event for this task. Useful for holding the completion of this task until certain child
	*									tasks are complete.
	*/
	UNREALENGINEECS_API virtual void ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
                                                 const FGraphEventRef& MyCompletionGraphEvent) override;

	/** Abstract function to describe this tick. Used to print messages about illegal cycles in the dependency graph */
	UNREALENGINEECS_API virtual FString DiagnosticMessage() override;
	UNREALENGINEECS_API virtual FName DiagnosticContext(bool bDetailed) override;
};

template<>
struct TStructOpsTypeTraits<FECSSystemTickFunction> : public TStructOpsTypeTraitsBase2<FECSSystemTickFunction>
{
	enum
	{
		WithCopy = false
    };
};

//////////////////////////////////////////////////
/**
 * Interface for systems.
 * Systems run each tick (or at a given interval).
 * Set the ticking related parameters through the tick function (@see TickFunction) in the constructor
 */
UCLASS(Abstract)
class UNREALENGINEECS_API UECSSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual ~UECSSystem() = default;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	/** Main function for systems. This is called each tick (or how long the tick function is set to) */
	virtual void RunSystem(float DeltaTime, ENamedThreads::Type CurrentThread) const {};	
	
protected:
	void RegisterTickFunction(UWorld* World);
	
	
	//---------- Variables ----------//
public:
	FECSSystemTickFunction TickFunction;
	class IECSRegistryInterface* Registry = nullptr;
};
