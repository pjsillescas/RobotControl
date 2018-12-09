// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Runtime/Engine/Classes/Components/ArrowComponent.h"
#include "Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#include "DronePawn.generated.h"

USTRUCT(BlueprintType)
struct FQuadRotorState
{
	GENERATED_BODY()
	/*
	state: The current state of the robot with the following fields:
	state.pos = [x; y; z], state.vel = [x_dot; y_dot; z_dot],
	state.rot = [phi; theta; psi], state.omega = [p; q; r]
	*/
	FVector Position;
	FVector Velocity;

	FVector Rotation;
	FVector AngularVelocity;
};

USTRUCT(BlueprintType)
struct FQuadRotorDesiredState
{
	GENERATED_BODY()
	/*
	des_state: The desired states are:
	des_state.pos = [x; y; z], des_state.vel = [x_dot; y_dot; z_dot],
	des_state.acc = [x_ddot; y_ddot; z_ddot], des_state.yaw,
	des_state.yaw_dot
	*/
	FVector Position;
	FVector Velocity;
	FVector Acceleration;

	float Yaw;
	float YawDot;
};

UCLASS(Blueprintable)
class ROBOTICSCONTROL_API ADronePawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADronePawn(const FObjectInitializer& OI);

private:
	void InitializeArrows(const FObjectInitializer& OI);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FVector GetWeight();

	FVector DroneVelocity;

	FVector AddFrictionForces(FVector Velocity);

	FQuadRotorState State;
	class AQuadPDAIController* Controller;


	UFUNCTION(BlueprintGetter)
	FORCEINLINE FQuadRotorState GetState() { return State; };

	UFUNCTION(BlueprintSetter)
	void SetState(const FQuadRotorState& NewState) { this->State = State; };

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "Physical Model")
	float DragResistance;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	FVector MInertia;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	float MinTorque;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	float MaxTorque;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	float MotorForceConstant;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	float MotorMomentConstant;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Physical Model")
	float ArmLength;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Physical Model")
	float Mass;

	
	
	UFUNCTION(BlueprintCallable)
	void GetRotorForces(FVector& FrontForce, FVector& BackForce, FVector& LeftForce, FVector& RightForce);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent* ArrowFront;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent *ArrowLeft;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent *ArrowBack;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UArrowComponent*ArrowRight;

	UFUNCTION(BlueprintCallable)
	void GetRotorForcesByInputs(const float& U1, const FVector& U2, FVector& FrontForce, FVector& BackForce, FVector& LeftForce, FVector& RightForce);

	UFUNCTION(BlueprintCallable)
	void SetForce(UArrowComponent* Arrow, const FVector& Force);

	void AdjustThrusts(float U1, const FVector& U2);

	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	//UFloatingPawnMovement* FloatingPawnMovement;
public:
	UFUNCTION(BlueprintGetter)
	FORCEINLINE FVector GetDroneVelocity() { return DroneVelocity; };

	UFUNCTION(BlueprintSetter)
	void SetDroneVelocity(const FVector& NewVelocity) { this->DroneVelocity = NewVelocity; };
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UStaticMeshComponent* GetMesh() { return Mesh; };

	UFUNCTION(BlueprintSetter)
	void SetMesh(UStaticMeshComponent* NewMesh) { this->Mesh = NewMesh; };

	UFUNCTION(BlueprintGetter)
	FORCEINLINE UArrowComponent* GetArrowFront() { return ArrowFront; };

	UFUNCTION(BlueprintSetter)
	void SetArrowFront(UArrowComponent* Arrow1) { this->ArrowFront = Arrow1; };

	UFUNCTION(BlueprintGetter)
	FORCEINLINE UArrowComponent* GetArrowLeft() { return ArrowLeft; };

	UFUNCTION(BlueprintSetter)
	void SetArrowLeft(UArrowComponent* Arrow2) { this->ArrowLeft = Arrow2; };
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UArrowComponent* GetArrowBack() { return ArrowBack; };

	UFUNCTION(BlueprintSetter)
	void SetArrowBack(UArrowComponent* Arrow3) { this->ArrowBack = Arrow3; };
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE UArrowComponent* GetArrowRight() { return ArrowRight; };

	UFUNCTION(BlueprintSetter)
	void SetArrowRight(UArrowComponent* Arrow4) { this->ArrowRight = Arrow4; };

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetMotorForceConstant() { return MotorForceConstant; }

	UFUNCTION(BlueprintSetter)
	void SetMotorForceConstant(const float& NewMotorForceConstant) { MotorForceConstant = NewMotorForceConstant; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetMotorMomentConstant() { return MotorMomentConstant; }

	UFUNCTION(BlueprintSetter)
	void SetMotorMomentConstant(const float& NewMotorMomentConstant) { MotorMomentConstant = NewMotorMomentConstant; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetMass() { return Mass; }

	UFUNCTION(BlueprintSetter)
	void SetMass(const float& NewMass);

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetArmLength() { return ArmLength; }

	UFUNCTION(BlueprintSetter)
	void SetArmLength(const float& NewArmLength) { ArmLength = NewArmLength; }

	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetMinTorque() { return MinTorque; }

	UFUNCTION(BlueprintSetter)
	void SetMinTorque(const float& NewMinTorque) { MinTorque = NewMinTorque; }
	
	UFUNCTION(BlueprintGetter)
	FORCEINLINE float GetMaxTorque() { return MaxTorque; }

	UFUNCTION(BlueprintSetter)
	void SetMaxTorque(const float& NewMaxTorque) { MaxTorque = NewMaxTorque; }

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
