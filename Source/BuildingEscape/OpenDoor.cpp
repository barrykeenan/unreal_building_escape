// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

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

	InitialRotation = GetOwner()->GetActorRotation();
	CurrentRotation = InitialRotation;

	TargetRotation.Pitch += CurrentRotation.Pitch;
	TargetRotation.Yaw += CurrentRotation.Yaw;
	TargetRotation.Roll += CurrentRotation.Roll;

	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has OpenDoor component, but no PressurePlate set"), *GetOwner()->GetName());
	}

	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PressurePlate && PressurePlate->IsOverlappingActor(ActorThatOpens))
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
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentRotation.Pitch = FMath::FInterpTo(CurrentRotation.Pitch, InitialRotation.Pitch, DeltaTime, DoorCloseSpeed);
	CurrentRotation.Yaw = FMath::FInterpTo(CurrentRotation.Yaw, InitialRotation.Yaw, DeltaTime, DoorCloseSpeed);
	CurrentRotation.Roll = FMath::FInterpTo(CurrentRotation.Roll, InitialRotation.Roll, DeltaTime, DoorCloseSpeed);

	GetOwner()->SetActorRotation(CurrentRotation);
}