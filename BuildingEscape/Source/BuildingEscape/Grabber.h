// Copyright 2018 TestGames

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"


#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  //AActor* Owner;

  // Distance that the character can reach in front of them
  float Reach = 100.f;

  UPhysicsHandleComponent* PhysicsHandle = nullptr;

  UInputComponent* InputComponent = nullptr;

  void FindPhysicsHandleComponent();

  void SetupInputComponent();

  // Ray-cast and grab what's in reach
  void Grab();

  // Release what's been grabbed
  void Release();

  // Return hit for first physics body in reach
  FHitResult GetFirstPhysicsBodyInReach() const;

  FVector GetReachLineStart() const;
  FVector GetReachLineEnd() const;
};
