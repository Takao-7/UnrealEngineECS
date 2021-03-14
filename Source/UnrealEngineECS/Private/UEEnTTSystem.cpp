
#include "UEEnTTSystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UEEnTTComponents.h"
#include "Engine/World.h"


//////////////////////////////////////////////////
void FECSSystemTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
										 const FGraphEventRef& MyCompletionGraphEvent)
{
	if (Target != nullptr)
	{
		Target->RunSystem(DeltaTime, CurrentThread);
	}
}

//////////////////////////////////////////////////
FString FECSSystemTickFunction::DiagnosticMessage()
{
	return Target->GetFullName() + TEXT("[ECS TickSystem]");
}

FName FECSSystemTickFunction::DiagnosticContext(bool bDetailed)
{
	if (bDetailed)
	{
		FString ContextString = FString::Printf(TEXT("%s/%s"), *GetParentNativeClass(Target->GetClass())->GetName(),
												*Target->GetClass()->GetName());
		return FName(*ContextString);
	}
	else
	{
		return GetParentNativeClass(Target->GetClass())->GetFName();
	}
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
void UECSSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UWorld* World = GetWorld())
	{
		RegisterTickFunction(World);
	}
}

void UECSSystem::Deinitialize()
{
	Super::Deinitialize();	
	TickFunction.UnRegisterTickFunction();
	TickFunction.Target = nullptr;
}

//////////////////////////////////////////////////
void UECSSystem::RegisterTickFunction(UWorld* World)
{	
	ULevel* Level = World->PersistentLevel;
	TickFunction.RegisterTickFunction(Level);
	TickFunction.Target = this;
}
