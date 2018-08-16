// Copyright 2018 TestGames

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"

#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

  void OpenDoor();
  void CloseDoor();

public:	
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
  UPROPERTY(EditAnywhere)
  float OpenAngle = 0.0f;

  UPROPERTY(EditAnywhere)
  ATriggerBox* PressurePlate;

  UPROPERTY(EditAnywhere)
  float DoorCloseDelay = 1.0f;

  float LastDoorOpenTime;
  AActor* ActorThatOpens;
  AActor* Owner;

  FString ObjectName;
	
};
