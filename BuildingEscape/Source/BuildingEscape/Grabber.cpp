// Copyright 2018 TestGames

#include "Grabber.h"
#include "NameTypes.h"
#include "Engine/World.h"
#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;

  //Owner = GetOwner();
}


// Called when the game starts
void UGrabber::BeginPlay()
{
  Super::BeginPlay();

  UE_LOG(LogTemp, Warning, TEXT("Grabber ready."));

  /// Look for attached phyics handle
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
  if (PhysicsHandle) {

  }
  else {
    UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing on %s "), *GetOwner()->GetName());
  }

  /// Look for Input Component (only appears at runtime)
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
  if (InputComponent) {
    /// Bind the input axis
    InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("UInputComponent missing on %s "), *GetOwner()->GetName());
  }
	
}

void UGrabber::Grab() {
  UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get the player viewpoint
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  // Log
  //UE_LOG(LogTemp, Warning, TEXT("Location %s - Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

  /// Draw a red trace in the world to visualize
  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

  DrawDebugLine(
    GetWorld(),
    PlayerViewPointLocation,
    LineTraceEnd,
    FColor(255, 0, 0),
    false,
    0.f,
    0.f,
    10.f
  );

  /// Setup query parameters
  FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

  // Ray cast out to reach distance
  FHitResult Hit;
  GetWorld()->LineTraceSingleByObjectType(
    OUT Hit,
    PlayerViewPointLocation,
    LineTraceEnd,
    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
    TraceParameters
  );


  /// See what we hit
  AActor* ActorHit = Hit.GetActor();
  if (ActorHit) {
    UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *(ActorHit->GetName()));
  }
}

