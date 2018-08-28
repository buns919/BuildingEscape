// Copyright 2018 TestGames

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerBox.h"

#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	UOpenDoor();

  UPROPERTY(BlueprintAssignable)
  FDoorEvent OnOpen;

  UPROPERTY(BlueprintAssignable)
  FDoorEvent OnClose;



protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
  // Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
  UPROPERTY(EditAnywhere)
  ATriggerBox* PressurePlate = nullptr; // set in editor to a trigger box

  AActor* Owner = nullptr;
  FString ObjectName;

  UPROPERTY(EditAnywhere)
  float TriggerMass = 50.f;

  float GetTotalMassOfActorsOnPlate();
	
};
