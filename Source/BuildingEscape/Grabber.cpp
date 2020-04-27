// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be
	// ticked every frame.  You can turn these features off to improve
	// performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

// Player pawn must have a Physics Handle
void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has UGrabber component, but no UPhysicsHandleComponent set"), *GetOwner()->GetName());
	}
}

// Attach object in reach to Player's physics handle
void UGrabber::Grab()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		// PhysicsHandle is a pointer, accessing method (->) if null can cause crash
		if (!PhysicsHandle)
		{
			return;
		}

		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayersReach());
	}
}

void UGrabber::Release()
{
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// If an object is attached to Player's physics handle
	if (PhysicsHandle->GrabbedComponent)
	{
		// Move the object Player is holding.
		PhysicsHandle->SetTargetLocation(GetPlayersReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;

	// Raycast out to reach distance
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetPlayersWorldPosition(), GetPlayersReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	return Hit;
}

FVector UGrabber::GetPlayersReach() const
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	// Draw a line from player, showing reach
	// DrawDebugLine(GetWorld(), PlayerViewPointLocation, LineTraceEnd, FColor(0, 255, 0), false, 0.f, 0, 5.f);

	return LineTraceEnd;
}

FVector UGrabber::GetPlayersWorldPosition() const
{
	// Get players viewpoint
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}