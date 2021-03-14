
#include "ECSCoreSystems.h"
#include "UEEnTTComponents.h"
#include "GameFramework/Actor.h"

DECLARE_CYCLE_STAT(TEXT("Copy transforms from ECS to actors"), STAT_CopyTransformToActor, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("Copy transforms from actors to ECS"), STAT_CopyTransformToECS, STATGROUP_ECS);


//////////////////////////////////////////////////
//////////////////////////////////////////////////
UECSCopyTransformToECS::UECSCopyTransformToECS()
{
	TickFunction.TickGroup = ETickingGroup::TG_PrePhysics;
}

//////////////////////////////////////////////////
void UECSCopyTransformToECS::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Connect an observer to the registry, which listens for entities with a transform, actor ptr and sync transform component 
	Observer.Connect(*Registry, ECS::Collector.update<FTransform>().where<FActorPtrComponent, FSyncTransformToECS>());
}

void UECSCopyTransformToECS::Deinitialize()
{
	Super::Deinitialize();
	Observer.Disconnect();
}

//////////////////////////////////////////////////
void UECSCopyTransformToECS::RunSystem(float DeltaTime, ENamedThreads::Type CurrentThread) const
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToECS);

	// auto View = Registry->View<FActorPtrComponent, FTransform, FSyncTransformToECS>();	
	// for (auto&& [Entity, Actor, Transform, SyncComp] : View.each())
	// {
	// 	Transform = Actor->GetActorTransform();
	// 	SyncComp.bSyncTransform = false;
	// }

	Observer.Each([&](const auto Entity)
	{
		auto&& [Actor, Transform, SyncComp] = Registry->GetEntTTReg().get<FActorPtrComponent, FTransform, FSyncTransformToECS>(Entity);
		Transform = Actor->GetActorTransform();
        SyncComp.bSyncTransform = false;
	});
}


//////////////////////////////////////////////////
//////////////////////////////////////////////////
UECSCopyTransformToActor::UECSCopyTransformToActor()
{
	TickFunction.TickGroup = ETickingGroup::TG_PostPhysics;
}

void UECSCopyTransformToActor::RunSystem(float DeltaTime, ENamedThreads::Type CurrentThread) const
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToActor);
	
	auto View = Registry->View<FActorPtrComponent, FTransform, FSyncTransformToActor>();
	for (auto&& [Entity, Actor, Transform, SyncComp] : View.each())
	{
		if (SyncComp.bSyncTransform)
		{
			Actor->SetActorTransform(Transform, SyncComp.bSweep, nullptr, SyncComp.TeleportType);			
		}
	}
}
