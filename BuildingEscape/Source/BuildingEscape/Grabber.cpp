// Copyright 2018 TestGames

#include "Grabber.h"
#include "NameTypes.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT


// Sets default values for this component's properties
UGrabber::UGrabber()
{
  // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
  // off to improve performance if you don't need them.
  PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
  Super::BeginPlay();
  FindPhysicsHandleComponent();
  SetupInputComponent();
}

void UGrabber::FindPhysicsHandleComponent() {
  /// Look for attached phyics handle
  PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
  if (!PhysicsHandle) {
    UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing on %s "), *GetOwner()->GetName());
  }
}

void UGrabber::SetupInputComponent() {
  /// Look for Input Component (only appears at runtime)
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

  if (InputComponent) {
    InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("UInputComponent missing on %s "), *GetOwner()->GetName());
  }
}

void UGrabber::Grab() {
  /// Line trace to try to reach any actors with physics body collision channel set
  FHitResult HitResult = GetFirstPhysicsBodyInReach();
  UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
  AActor* ActorHit = HitResult.GetActor();

  /// if we hit something, then attach a physics handle
  if (ActorHit) {
    PhysicsHandle->GrabComponent(
      ComponentToGrab,
      NAME_None, // no bones needed
      ComponentToGrab->GetOwner()->GetActorLocation(),
      true
    );
  }
}

void UGrabber::Release() {
  PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  if (!PhysicsHandle) {
    return;
  }

  // if the physics handle is attached
  if (PhysicsHandle->GrabbedComponent) {
    // move the object we're holding
    PhysicsHandle->SetTargetLocation(GetReachLineEnd());
  }

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
  // Ray cast out to reach distance
  FHitResult HitResult;
  FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

  GetWorld()->LineTraceSingleByObjectType(
    OUT HitResult,
    GetReachLineStart(),
    GetReachLineEnd(),
    FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
    TraceParameters
  );

  return HitResult;
}

FVector UGrabber::GetReachLineStart() const {
  // Get the player viewpoint
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd() const {
  // Get the player viewpoint
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
