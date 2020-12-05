
#include "UEEnTTSystem.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "UEEnTTComponents.h"
#include "UEEnTTRegistry.h"

DECLARE_CYCLE_STAT(TEXT("Copy transforms from ECS to actors"), STAT_CopyTransformToActor, STATGROUP_ECS);
DECLARE_CYCLE_STAT(TEXT("Copy transforms from actors to ECS"), STAT_CopyTransformToECS, STATGROUP_ECS);


//////////////////////////////////////////////////
void ISystem::TryUpdate(float DeltaTime, FRegistry& Registry)
{
	if (UpdateInterval <= .0f)
	{
		Update(DeltaTime, Registry);
		return;
	}

	TimeSinceLastUpdate += DeltaTime;
	if (TimeSinceLastUpdate >= UpdateInterval)
	{
		TimeSinceLastUpdate = .0f;
		Update(DeltaTime, Registry);
	}
}

//////////////////////////////////////////////////
void SCopyTransformToActor::Update(float DeltaTime, FRegistry& Registry) const
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToActor);
	
	auto View = Registry.View<FActorPtrComponent, FTransform, FTransformSync>();
	for (auto Entity : View)
	{
		auto& SyncComp = View.get<FTransformSync>(Entity);
		if (SyncComp.SyncType == ESyncType::ECS_To_Actor || SyncComp.SyncType == ESyncType::BothWays)
		{
			auto& ActorRef = View.get<FActorPtrComponent>(Entity);
			auto& Transform = View.get<FTransform>(Entity);
			ActorRef.Pointer->SetActorTransform(Transform, SyncComp.bSweep, nullptr, SyncComp.TeleportType);			
		}
	}
}

void SCopyTransformToECS::Update(float DeltaTime, FRegistry& Registry) const
{
	SCOPE_CYCLE_COUNTER(STAT_CopyTransformToECS);
	
	auto View = Registry.View<FActorPtrComponent, FTransform, FTransformSync>();
	for (auto Entity : View)
	{
		auto& SyncComp = View.get<FTransformSync>(Entity);
		if (SyncComp.SyncType == ESyncType::Actor_To_ECS || SyncComp.SyncType == ESyncType::BothWays)
		{
			auto& ActorRef = View.get<FActorPtrComponent>(Entity);
			auto& Transform = View.get<FTransform>(Entity);
			Transform = ActorRef.Pointer->GetActorTransform();
		}
	}
}
