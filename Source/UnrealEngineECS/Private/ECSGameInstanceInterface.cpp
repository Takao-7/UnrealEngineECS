// Copyright @Paul Larrass 2020

#include "ECSGameInstanceInterface.h"

#include "UEEnTTComponents.h"
#include "Engine/World.h"

#include "UEEnTTSystem.h"


FSystemTickFunction::FSystemTickFunction()
{
	SetTickFunctionEnable(true);
}

void FSystemTickFunction::SetTickGroup(ETickingGroup TickingGroup)
{
	TickGroup = TickingGroup;
	EndTickGroup = TickingGroup;
}

//////////////////////////////////////////////////
void FSystemTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
									  const FGraphEventRef& MyCompletionGraphEvent)
{
	Systems.Broadcast(DeltaTime, *Registry);
}

//////////////////////////////////////////////////
FString FSystemTickFunction::DiagnosticMessage()
{
	return TEXT("ECS: System ticking function");
}

FName FSystemTickFunction::DiagnosticContext(bool bDetailed)
{
	return FName(TEXT("ECS ticking function"));
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
FPrePhysicsSystemTickFunction::FPrePhysicsSystemTickFunction() : FSystemTickFunction()
{
	SetTickGroup(TG_PrePhysics);
}

void FPrePhysicsSystemTickFunction::ExecuteTick(float DeltaTime, ELevelTick TickType, ENamedThreads::Type CurrentThread,
                                                const FGraphEventRef& MyCompletionGraphEvent)
{
	// Copy transforms to the ECS
	ECSCoreSystems::CopyTransformToECS(*Registry, ObserverSyncTransformToECS);
	
	Systems.Broadcast(DeltaTime, *Registry);

	// Copy transforms back to actors
	ECSCoreSystems::CopyTransformToActor(*Registry, ObserverSyncTransformToActor);
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
IECSGameInstanceInterface::IECSGameInstanceInterface()
{
	PrePhysicsTickFunction.Registry = &Registry;
	DuringPhysicsTickFunction.Registry = &Registry;
	PostPhysicsTickFunction.Registry = &Registry;
	
	// Connect observers for our sync transform systems. 
	PrePhysicsTickFunction.ObserverSyncTransformToECS.connect(Registry,
		entt::collector.update<FSyncTransformToECS>().where<FTransform, FActorPtrComponent>());
	PrePhysicsTickFunction.ObserverSyncTransformToActor.connect(Registry,
		entt::collector.update<FTransform>().where<FSyncTransformToActor, FActorPtrComponent>());
}

//////////////////////////////////////////////////
entt::registry& IECSGameInstanceInterface::GetRegistry(UObject* ObjectRef)
{
	UWorld* World = ObjectRef->GetWorld();
	return reinterpret_cast<IECSGameInstanceInterface*>(World->GetGameInstance())->Registry;
}

entt::registry& IECSGameInstanceInterface::GetRegistry()
{
	return Registry;
}

const entt::registry& IECSGameInstanceInterface::GetRegistry() const
{
	return Registry;
}

//////////////////////////////////////////////////
FDelegateHandle IECSGameInstanceInterface::AddSystem(TBaseStaticDelegateInstance<void (float, entt::registry&)>::FFuncPtr InFunc,
													 ESystemTickingGroup TickingGroup)
{
	switch (TickingGroup)
	{
	case ESystemTickingGroup::PrePhysics:
		return PrePhysicsTickFunction.Systems.AddStatic(InFunc);
	case ESystemTickingGroup::DuringPhysics: 
		return DuringPhysicsTickFunction.Systems.AddStatic(InFunc);
	case ESystemTickingGroup::PostPhysics: 
	default:
		return PostPhysicsTickFunction.Systems.AddStatic(InFunc);
	}
}

template <typename UserClass>
FDelegateHandle IECSGameInstanceInterface::AddSystem(UserClass* InUserObject,
													 typename TMemFunPtrType<false, UserClass, void(float, entt::registry&)>::Type InFunc,
													 ESystemTickingGroup TickingGroup)
{
	switch (TickingGroup)
	{
	case ESystemTickingGroup::PrePhysics:
		return PrePhysicsTickFunction.Systems.AddUObject(InUserObject, InFunc);
	case ESystemTickingGroup::DuringPhysics: 
		return DuringPhysicsTickFunction.Systems.AddUObject(InUserObject, InFunc);
	case ESystemTickingGroup::PostPhysics: 
	default:
        return PostPhysicsTickFunction.Systems.AddUObject(InUserObject, InFunc);
	}
}

FDelegateHandle IECSGameInstanceInterface::AddSystem(TFunction<void(float, entt::registry&)> Lambda, ESystemTickingGroup TickingGroup)
{
	switch (TickingGroup)
	{
	case ESystemTickingGroup::PrePhysics:
		return PrePhysicsTickFunction.Systems.AddLambda(Lambda);
	case ESystemTickingGroup::DuringPhysics: 
		return DuringPhysicsTickFunction.Systems.AddLambda(Lambda);
	case ESystemTickingGroup::PostPhysics: 
	default:
        return PostPhysicsTickFunction.Systems.AddLambda(Lambda);
	}
}

//////////////////////////////////////////////////
bool IECSGameInstanceInterface::RemoveSystem(const FDelegateHandle& Handle)
{
	if (PrePhysicsTickFunction.Systems.Remove(Handle))
	{
		return true;
	}
	if (DuringPhysicsTickFunction.Systems.Remove(Handle))
	{
		return true;
	}
	if (PostPhysicsTickFunction.Systems.Remove(Handle))
	{
		return true;
	}

	return false;
}

//////////////////////////////////////////////////
void IECSGameInstanceInterface::HandleWorldChange(UWorld* OldWorld, UWorld* NewWorld)
{
	PrePhysicsTickFunction.UnRegisterTickFunction();
	DuringPhysicsTickFunction.UnRegisterTickFunction();
	PostPhysicsTickFunction.UnRegisterTickFunction();

	// Setup and register tick functions for systems
	if (NewWorld != nullptr)
	{
		DuringPhysicsTickFunction.SetTickGroup(ETickingGroup::TG_DuringPhysics);
		PostPhysicsTickFunction.SetTickGroup(ETickingGroup::TG_PostPhysics);
		
		ULevel* PersistentLevel = NewWorld->PersistentLevel;
		PrePhysicsTickFunction.RegisterTickFunction(PersistentLevel);
		DuringPhysicsTickFunction.RegisterTickFunction(PersistentLevel);
		PostPhysicsTickFunction.RegisterTickFunction(PersistentLevel);		
	}	
}

//////////////////////////////////////////////////
//////////////////////////////////////////////////
void UECSGameInstance::OnWorldChanged(UWorld* OldWorld, UWorld* NewWorld)
{
	Super::OnWorldChanged(OldWorld, NewWorld);
	HandleWorldChange(OldWorld, NewWorld);
}
