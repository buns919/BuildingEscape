// Copyright 2018 TestGames

#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

  // Set door starting rotation
  AActor* Owner = GetOwner();
  FString ObjectName = Owner->GetName();
  FRotator NewRotation = FRotator(0.0f, 0.60f, 0.0f);

  Owner->SetActorRotation(NewRotation);

  UE_LOG(LogTemp, Warning, TEXT("Set %s rotation."), *ObjectName);
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

