// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "QuadPDAIController.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTICSCONTROL_API AQuadPDAIController : public AAIController
{
	GENERATED_BODY()

	void BeginPlay();

protected:
	class ADronePawn* Pawn;

	float Gravity;
	float Mass;
	float MinTorque;
	float MaxTorque;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	float Kpphi;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdphi;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kptheta;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdtheta;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kppsi;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdpsi;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kpx;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdx;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kpy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdy;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kpz;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Kdz;

public:
	void UpdateInputs(const struct FQuadRotorState& State, const struct FQuadRotorDesiredState& DesState, float& F, FVector& M);
};
