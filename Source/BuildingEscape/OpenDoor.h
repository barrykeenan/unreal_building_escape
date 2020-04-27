// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"

#include "OpenDoor.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UOpenDoor();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OpenDoor(float DeltaTime);
	void CloseDoor(float DeltaTime);
	float TotalMassOfActors() const;

	// Tracks if sound has been played
	bool OpenDoorSoundIsPlayed = false;
	bool CloseDoorSoundIsPlayed = true;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	void FindPressurePlate();
	void FindAudioComponent();

	FRotator InitialRotation;
	FRotator CurrentRotation;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 50.f;

	UPROPERTY(EditAnywhere)
	FRotator TargetRotation{0.0f, 90.0f, 0.0f};

	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate;

	float DoorLastOpened = 0.0f;

	UPROPERTY(EditAnywhere)
	float DoorClosedDelay = 0.5f;	 // secs

	UPROPERTY(EditAnywhere)
	float DoorOpenSpeed = 2.0f;

	UPROPERTY(EditAnywhere)
	float DoorCloseSpeed = 5.0f;

	UPROPERTY()
	UAudioComponent* AudioComponent = nullptr;
};
