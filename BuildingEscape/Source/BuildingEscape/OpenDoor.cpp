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

// Set door starting rotation
void UOpenDoor::OpenDoor() {
    UE_LOG(LogTemp, Warning, TEXT("OpenDoor()"));

    FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);

    Owner->SetActorRotation(NewRotation);
    UE_LOG(LogTemp, Warning, TEXT("Set %s rotation to %s."), *ObjectName, *NewRotation.ToString());

    bIsDoorOpen = true;
}

// Set door closing rotation
void UOpenDoor::CloseDoor() {
    UE_LOG(LogTemp, Warning, TEXT("CloseDoor()"));

    FRotator NewRotation = FRotator(0.0f, CloseAngle, 0.0f);

    Owner->SetActorRotation(NewRotation);
    UE_LOG(LogTemp, Warning, TEXT("Set %s rotation to %s."), *ObjectName, *NewRotation.ToString());
    bIsDoorOpen = false;

}



// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// Poll if Actor is in pressure plate
  // TODO make into a parameter that can be edited in the editor
  if (GetTotalMassOfActorsOnPlate() > 50.f) {
    UE_LOG(LogTemp, Warning, TEXT("Actor is on pressure plate, Opening door."));
    OpenDoor();
    LastDoorOpenTime = GetWorld()->GetTimeSeconds();
  }

  // Check if it's time to close the door
  if (bIsDoorOpen && GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay) {
    CloseDoor();
  }

  
}

float UOpenDoor::GetTotalMassOfActorsOnPlate() {
  float TotalMass = 0.f;

  // Find all the overlapping actors
  TArray<AActor*> OverlappingActors;
  if (!PressurePlate) { return TotalMass; }
  PressurePlate->GetOverlappingActors(OUT OverlappingActors);

  // Iterate through them adding their masses
  for (const auto& Actor : OverlappingActors)
  {
    TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    UE_LOG(LogTemp, Warning, TEXT("%s on pressure plate"), *Actor->GetName())
  }

  return TotalMass;
}
