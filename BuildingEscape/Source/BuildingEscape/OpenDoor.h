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

public:
	UOpenDoor();
  void OpenDoor();
  void CloseDoor();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
  // Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
  UPROPERTY(EditAnywhere)
  float OpenAngle = 0.0f;

  UPROPERTY(EditAnywhere)
  float CloseAngle = 90.0f;

  UPROPERTY(EditAnywhere)
  ATriggerBox* PressurePlate = nullptr; // set in editor to a trigger box

  UPROPERTY(EditAnywhere)
  float DoorCloseDelay = 1.0f;

  float LastDoorOpenTime;

  AActor* Owner = nullptr;
  FString ObjectName;

  bool bIsDoorOpen = true;

  float GetTotalMassOfActorsOnPlate();
	
};
