// Copyright @Paul Larrass 2020

#pragma once
struct FRegistry;


/** Interface for systems. They shouldn't contain any state */
class UNREALENGINEECS_API ISystem
{
public:
    virtual ~ISystem() = default;
	
    void TryUpdate(float DeltaTime, FRegistry& Registry);

	/* Called once in the game instance's BeginPlay() function */
	virtual void BeginPlay(FRegistry& Registry){}

protected:
	virtual void Update(float DeltaTime, FRegistry& Registry) const = 0;

	/* Delay between updates. Set to <= 0 to update each tick */
	float UpdateInterval = .0f;

private:	
	float TimeSinceLastUpdate = 0;
};


//////////////////////////////////////////////////
//////////////////////////////////////////////////
class UNREALENGINEECS_API SCopyTransformToActor : public ISystem
{
    virtual void Update(float DeltaTime, FRegistry& Registry) const override;
};

class UNREALENGINEECS_API SCopyTransformToECS : public ISystem
{
	virtual void Update(float DeltaTime, FRegistry& Registry) const override;
};
