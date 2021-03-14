#pragma once

#include "ECSRegistry.h"
#include "UEEnTTSystem.h"
#include "ECSCoreSystems.generated.h"

/**
 * Copy transforms from actors to the ECS
 */
UCLASS()
class UECSCopyTransformToECS : public UECSSystem
{
	GENERATED_BODY()

public:
	UECSCopyTransformToECS();
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void RunSystem(float DeltaTime, ENamedThreads::Type CurrentThread) const override;

protected:
	FECSObserver Observer;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
/**
 * Copy transforms from the ECS to the linked actor
 */
UCLASS()
class UECSCopyTransformToActor : public UECSSystem
{
	GENERATED_BODY()

public:
	UECSCopyTransformToActor();
	virtual void RunSystem(float DeltaTime, ENamedThreads::Type CurrentThread) const override;
};
