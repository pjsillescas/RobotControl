// Fill out your copyright notice in the Description page of Project Settings.

#include "DronePawn.h"
#include "Runtime/Engine/Classes/GameFramework/PawnMovementComponent.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

void ADronePawn::InitializeArrows(const FObjectInitializer& OI)
{
	ArrowFront = OI.CreateDefaultSubobject <UArrowComponent>(this, TEXT("ArrowFront"));
	ArrowBack = OI.CreateDefaultSubobject <UArrowComponent>(this, TEXT("ArrowBack"));
	ArrowLeft = OI.CreateDefaultSubobject <UArrowComponent>(this, TEXT("ArrowLeft"));
	ArrowRight = OI.CreateDefaultSubobject <UArrowComponent>(this,TEXT("ArrowRight"));

	FVector FrontPosition(-40, 0, 0);
	FVector LeftPosition(0, 40, 0);
	FVector BackPosition(40, 0, 0);
	FVector RightPosition(0, -40, 0);

	FRotator Rotator(90, 0, 0);
	FQuat Rotation = FQuat(Rotator);
	FTransform FrontTransform(Rotation, FrontPosition);
	ArrowFront->SetRelativeTransform(FrontTransform);

	FTransform LeftTransform(Rotation, LeftPosition);
	ArrowLeft->SetRelativeTransform(LeftTransform);

	FTransform BackTransform(Rotation, BackPosition);
	ArrowBack->SetRelativeTransform(BackTransform);

	FTransform RightTransform(Rotation, RightPosition);
	ArrowRight->SetRelativeTransform(RightTransform);
}

void ADronePawn::SetMass(const float& NewMass)
{
	FBodyInstance* BodyInst = Mesh->GetBodyInstance();

	if (BodyInst == nullptr) return;
	//~~~~~~~~~~~~~~~~~~~~~~~~

	// New Scale 
	BodyInst->MassScale = NewMass;

	// Trigger Update! 
	BodyInst->UpdateMassProperties();
}

// Sets default values
ADronePawn::ADronePawn(const FObjectInitializer& OI): Super(OI)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("Mesh"));
	
	RootComponent = Mesh;
	Mesh->SetSimulatePhysics(true);
	
	
	InitializeArrows(OI);
	
	ArrowFront->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ArrowLeft->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ArrowBack->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ArrowRight->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	/*
	FloatingPawnMovement = OI.CreateDefaultSubobject<UFloatingPawnMovement>(this,TEXT("FloatingPawnMovement"));
	FloatingPawnMovement->UpdatedComponent = RootComponent;
	*/

	DragResistance = 0.5;

	DroneVelocity.Set(0, 0, 0);

	MinTorque = 0;
	MaxTorque = 3.5316;
	ArmLength = 0.086;
	Mass = 0.18;
	MInertia.Set(0,0,0);
	SetMass(Mass);
}

// Called when the game starts or when spawned
void ADronePawn::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ADronePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADronePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FVector ADronePawn::GetWeight()
{
	UWorld* World = GetWorld();
	
	FVector gravityVector(0,0,-World->GetGravityZ());
	return Mesh->GetMass() * gravityVector;
}

FVector ADronePawn::AddFrictionForces(FVector Velocity)
{
	FVector AirResistance = Velocity;
	AirResistance.Normalize();
	AirResistance = AirResistance*(-1 * Velocity.SizeSquared() * DragResistance);
	return AirResistance;
}

void ADronePawn::GetRotorForces(FVector& FrontForce, FVector& BackForce, FVector& LeftForce, FVector& RightForce)
{
	FVector HoverForce = ArrowFront->GetComponentTransform().GetRotation().GetForwardVector() * GetWeight().Size() * 0.25 + AddFrictionForces(DroneVelocity);
	
	FrontForce = HoverForce;
	BackForce = HoverForce;
	LeftForce = HoverForce;
	RightForce = HoverForce;
}