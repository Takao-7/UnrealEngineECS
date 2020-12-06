
#include "UEEnTTSystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ECSObserver.h"
#include "UEEnTTComponents.h"


DECLARE_CYCLE_STAT(TEXT("Copy transforms from ECS to actors"), STAT_CopyTransformToActor, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("Copy transforms from actors to ECS"), STAT_CopyTransformToECS, STATGROUP_ECS);


//////////////////////////////////////////////////
void IInstancedSystem::TryTickSystem(float DeltaTime, FRegistry& Registry)
{
	if (UpdateInterval <= .0f)
	{
		TickSystem(DeltaTime, Registry);
		return;
	}

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		TimeSinceLastUpdate = .0f;
		TickSystem(DeltaTime, Registry);
	}
}

//////////////////////////////////////////////////
void ECSCoreSystems::CopyTransformToECS(FRegistry& Registry, TObserver<const FActorPtrComponent, FTransform, FSyncTransformToECS>& Observer)
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToECS);

	Observer.Each([](FEntity Entity)
	{
		// Get the components from the entity
		auto&& [Actor, Transform, SyncComp] = Entity.GetComponents<const FActorPtrComponent, FTransform, FSyncTransformToECS>();

		Transform = Actor->GetActorTransform();
		SyncComp.bSyncTransform = false;
	});
		
	//auto View = Registry.View<const FActorPtrComponent, FTransform, const FTransformSync>();
	// for (auto&& [Entity, Actor, Transform, SyncComp] : View)
	// {
	// 	if (SyncComp.SyncType == ESyncType::Actor_To_ECS || SyncComp.SyncType == ESyncType::BothWays)
	// 	{
	// 		Transform = Actor->GetActorTransform();
	// 	}
	// }
}

void ECSCoreSystems::CopyTransformToActor(FRegistry& Registry, TObserver<const FActorPtrComponent, const FTransform,
										  FSyncTransformToActor>& Observer)
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToActor);

	Observer.Each([](FEntity Entity)
	{
		auto&& [Actor, Transform, SyncComp] = Entity.GetComponents<const FActorPtrComponent, const FTransform, FSyncTransformToActor>();
		Actor->SetActorTransform(Transform, SyncComp.bSweep, nullptr, SyncComp.TeleportType);
	});
	
	// auto View = Registry.View<const FActorPtrComponent, const FTransform, const FTransformSync>();
	// for (auto&& [Entity, Actor, Transform, SyncComp] : View.each())
	// {
	// 	if (SyncComp.SyncType == ESyncType::ECS_To_Actor || SyncComp.SyncType == ESyncType::BothWays)
	// 	{
	// 		Actor->SetActorTransform(Transform, SyncComp.bSweep, nullptr, SyncComp.TeleportType);
	// 	}
	// }
}
