
#include "UEEnTTSystem.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UEEnTTComponents.h"


DECLARE_CYCLE_STAT(TEXT("Copy transforms from ECS to actors"), STAT_CopyTransformToActor, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("Copy transforms from actors to ECS"), STAT_CopyTransformToECS, STATGROUP_ECS);


//////////////////////////////////////////////////
void IInstancedSystem::TryTickSystem(float DeltaTime, entt::registry& Registry)
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
void ECSCoreSystems::CopyTransformToECS(entt::registry& Registry, entt::observer& Observer)
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToECS);

	Observer.each([&Registry](const entt::entity Entity)
	{
		auto&& [Actor, Transform, SyncComp] = Registry.get<FActorPtrComponent, FTransform, FSyncTransformToECS>(Entity);
		
		Transform = Actor->GetActorTransform();
		SyncComp.bSyncTransform = false;
	});
}

void ECSCoreSystems::CopyTransformToActor(entt::registry& Registry, entt::observer& Observer)
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToActor);

	Observer.each([&Registry](const entt::entity Entity)
	{
		auto&& [Actor, Transform, SyncComp] = Registry.get<FActorPtrComponent, FTransform, FSyncTransformToActor>(Entity);
		if (SyncComp.bSyncTransform)
		{
			Actor->SetActorTransform(Transform, SyncComp.bSweep, nullptr, SyncComp.TeleportType);			
		}
	});
}
