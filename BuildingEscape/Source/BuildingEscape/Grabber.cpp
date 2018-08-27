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

  //Owner = GetOwner();
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
  if (PhysicsHandle) {

  }
  else {
    UE_LOG(LogTemp, Error, TEXT("UPhysicsHandleComponent missing on %s "), *GetOwner()->GetName());
  }
}

void UGrabber::SetupInputComponent() {
  /// Look for Input Component (only appears at runtime)
  InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
  if (InputComponent) {
    /// Bind the input axis
    /// On Press
    InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
    /// On Release
    InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
  }
  else {
    UE_LOG(LogTemp, Error, TEXT("UInputComponent missing on %s "), *GetOwner()->GetName());
  }
}

void UGrabber::Grab() {
  UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));

  /// Line trace to try to reach any actors with physics body collision channel set
  FHitResult HitResult = GetFirstPhysicsBodyInReach();
  UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
  AActor* ActorHit = HitResult.GetActor();

  /// if we hit something, then attach a physics handle
  if (ActorHit) {
    // TODO attach physics handle
    PhysicsHandle->GrabComponent(
      ComponentToGrab,
      NAME_None,
      ComponentToGrab->GetOwner()->GetActorLocation(),
      true
    );
  }
}

void UGrabber::Release() {
  UE_LOG(LogTemp, Warning, TEXT("Grab released"));

  PhysicsHandle->ReleaseComponent();
}



// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  // TODO refactor as this is used twice

  // Get the player viewpoint
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;


  // if the physics handle is attached
  if (PhysicsHandle->GrabbedComponent) {
    // move the object we're holding
    PhysicsHandle->SetTargetLocation(LineTraceEnd);

  }



	
}


FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
  // Get the player viewpoint
  FVector PlayerViewPointLocation;
  FRotator PlayerViewPointRotation;
  GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
    OUT PlayerViewPointLocation,
    OUT PlayerViewPointRotation
  );

  FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

  /**** DEBUG

  // Log
  //UE_LOG(LogTemp, Warning, TEXT("Location %s - Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString());

  /// Draw a red trace in the world to visualize

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
  */

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
  else {
    UE_LOG(LogTemp, Warning, TEXT("Line trace NOT hit: %s"), *(ActorHit->GetName()));
  }

  return Hit;
}
