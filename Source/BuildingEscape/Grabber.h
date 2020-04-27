// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#include "Grabber.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UGrabber();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	float Reach = 100.f;

	// Anything with a U in front, should have UPROPERTY macro, for unreal cleanup purposes
	UPROPERTY()
	UPhysicsHandleComponent* PhysicsHandle = nullptr;	 // don't know if PhysicsHandleComponent has initialised on owner actor yet

	UPROPERTY()
	UInputComponent* InputComponent = nullptr;

	virtual void Grab();
	virtual void Release();
	virtual void FindPhysicsHandle();
	virtual void SetupInputComponent();

	FHitResult GetFirstPhysicsBodyInReach() const;
	FVector GetPlayersWorldPosition() const;
	FVector GetPlayersReach() const;
};
