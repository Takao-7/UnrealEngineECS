// Copyright @Paul Larrass 2020

#pragma once

#include "ECSIncludes.h"

struct FSyncTransformToActor;
struct FSyncTransformToECS;
struct FActorPtrComponent;


/**
 * Interface for non-static systems, which shouldn't run each tick
 *
 * Static system can be simple functions (@see ECSCoreSystems) or lambdas.
 */
class UNREALENGINEECS_API IInstancedSystem
{
public:
    virtual ~IInstancedSystem() = default;

	/** Called by the game instance to try to update this system */
    void TryTickSystem(float DeltaTime, entt::registry& Registry);

protected:
	virtual void TickSystem(float DeltaTime, entt::registry& Registry) const = 0;

public:
	/* Delay between updates. Set to <= 0 to update each tick. Can be changed freely at runtime */
	float UpdateInterval = .0f;

private:	
	float TimeSinceLastUpdate = 0;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
namespace ECSCoreSystems
{
	/**
	 * Copy transforms from actors to the ECS.
	 * Called in PrePhysics ticking group BEFORE any other system is updated.
	 * 
	 * Required components: FActorPtrComponent, FTransform, FTransformSync
	 * Required actor component: UECS_TransformSyncComponent 
	 */
	void CopyTransformToECS(entt::registry& Registry, entt::observer& Observer);
	
	/**
	 * Copy transforms from the ECS to the linked actor.
	 * Called in PrePhysics ticking group AFTER all other systems are updated (this system may move actor's so we have to update it here)
	 * 
	 * Required components: FActorPtrComponent, FTransform & FTransformSync
	 * Required actor component: UECS_TransformSyncComponent
	 */
	void CopyTransformToActor(entt::registry& Registry, entt::observer& Observer);
}
