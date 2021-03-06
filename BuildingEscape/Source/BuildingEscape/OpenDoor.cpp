// Copyright 2018 TestGames

#include "OpenDoor.h"

#include "Engine/World.h"

#define OUT


// Sets default values for this component's properties
UOpenDoor::UOpenDoor() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();

  Owner = GetOwner();
  ObjectName = Owner->GetName();

  if (!PressurePlate) {
    UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *ObjectName);
  }
	
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll if Actor is in pressure plate
  if (GetTotalMassOfActorsOnPlate() > TriggerMass) {
    UE_LOG(LogTemp, Warning, TEXT("Actor is on pressure plate, opening door."));
    OnOpen.Broadcast();
  }
  else {
    UE_LOG(LogTemp, Warning, TEXT("Actor is NOT on pressure plate, closing door."));
    OnClose.Broadcast();
  }

}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
  float TotalMass = 0.f;

  if (!PressurePlate) {
    UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *ObjectName);
    return TotalMass;
  }

  // Find all the overlapping actors
  TArray<AActor*> OverlappingActors;
  PressurePlate->GetOverlappingActors(OUT OverlappingActors);

  // Iterate through them adding their masses
  for (const auto& Actor : OverlappingActors)
  {
    TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName());
  }

  return TotalMass;
}
