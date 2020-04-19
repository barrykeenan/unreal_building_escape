// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be
	// ticked every frame.  You can turn these features off to improve
	// performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// UPhysicsHandleComponent* PhysicsHandle = nullptr;	 // don't know if PhysicsHandleComponent has initialised on owner actor
	// yet

	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has UGrabber component, but no UPhysicsHandleComponent set"), *GetOwner()->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Draw a line from player, showing reach
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, 5.f);

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	// Raycast out to reach distance
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* Grabbed = Hit.GetActor();
	if (Grabbed)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabbed: %s"), *Grabbed->GetName());
	}
}
