// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "Components/AudioComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	InitialRotation = GetOwner()->GetActorRotation();
	CurrentRotation = InitialRotation;

	TargetRotation.Pitch += CurrentRotation.Pitch;
	TargetRotation.Yaw += CurrentRotation.Yaw;
	TargetRotation.Roll += CurrentRotation.Roll;

	FindPressurePlate();
	FindAudioComponent();
}

void UOpenDoor::FindPressurePlate()
{
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has UOpenDoor component, but no PressurePlate set"), *GetOwner()->GetName());
	}
}

void UOpenDoor::FindAudioComponent()
{
	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has UOpenDoor component, but no UAudioComponent set"), *GetOwner()->GetName());
	}
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (TotalMassOfActors() > MassToOpenDoor)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() >= DoorLastOpened + DoorClosedDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, TargetRotation.Pitch, DeltaTime, DoorOpenSpeed);
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, DoorOpenSpeed);
	CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, TargetRotation.Roll, DeltaTime, DoorOpenSpeed);

	GetOwner()->SetActorRotation(CurrentRotation);

	CloseDoorSoundIsPlayed = false;
	if (!AudioComponent)
	{
		return;
	}
	if (!OpenDoorSoundIsPlayed)
	{
		AudioComponent->Play();
		OpenDoorSoundIsPlayed = true;
	}
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, InitialRotation.Pitch, DeltaTime, DoorCloseSpeed);
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialRotation.Yaw, DeltaTime, DoorCloseSpeed);
	CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, InitialRotation.Roll, DeltaTime, DoorCloseSpeed);

	GetOwner()->SetActorRotation(CurrentRotation);

	OpenDoorSoundIsPlayed = false;
	if (!AudioComponent)
	{
		return;
	}
	if (!CloseDoorSoundIsPlayed)
	{
		AudioComponent->Play();
		CloseDoorSoundIsPlayed = true;
	}
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}