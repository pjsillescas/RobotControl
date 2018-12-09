// Fill out your copyright notice in the Description page of Project Settings.

#include "QuadPDAIController.h"
#include "DronePawn.h"

#include "Runtime/Engine/Classes/Engine/StaticMesh.h"

void AQuadPDAIController::BeginPlay()
{
	Pawn = Cast<ADronePawn>(GetPawn());
	UWorld* World = GetWorld();

	Gravity = World->GetGravityZ();
	Mass = Pawn->GetMesh()->GetMass();
	MinTorque = Pawn->GetMinTorque();
	MaxTorque = Pawn->GetMaxTorque();

	Kpphi = 2000;
	Kdphi = 35;

	Kptheta = 2000;
	Kdtheta = 35;

	Kppsi = 2000;
	Kdpsi = 35;

	Kpx = 35;
	Kdx = 3;

	Kpy = 35;
	Kdy = 3;

	Kpz = 2000;
	Kdz = 3;
}

void AQuadPDAIController::UpdateInputs(const struct FQuadRotorState& State, const struct FQuadRotorDesiredState& DesState,float& F, FVector& M)
{
	FVector ErrorPosition = DesState.Position - State.Position; // ep = [des_state.pos - state.pos];
	FVector ErrorVelocity = DesState.Velocity - State.Velocity; // ev = [des_state.vel - state.vel];

	//rxdes = des_state.acc(1) + Kdx * ev(1) + Kpx * ep(1);
	//rydes = des_state.acc(2) + Kdy * ev(2) + Kpy * ep(2);
	//rzdes = des_state.acc(3) + Kdz * ev(3) + Kpz * ep(3);

	FVector KdPosition(Kdx,Kdy,Kdz);
	FVector KpPosition(Kpx, Kpy, Kpz);

	FVector RDes = DesState.Acceleration + KdPosition * ErrorVelocity + KpPosition * ErrorPosition;

	//sinpsi = sin(des_state.yaw);
	//cospsi = cos(des_state.yaw);

	//float SinPsi = FMath::Sin(DesState.Rotation.Z);
	//float CosPsi = FMath::Cos(DesState.Rotation.Z);
	
	float SinPsi,CosPsi;
	FMath::SinCos(&SinPsi, &CosPsi, DesState.Yaw);

	//phides = 1 / g * (rxdes * sinpsi - rydes * cospsi);
	//thetades = 1 / g * (rxdes * cospsi + rydes * sinpsi);
	//psides = des_state.yaw;

	FVector RotationDes(1/Gravity * (RDes.X * SinPsi - RDes.Y * CosPsi, 1/ Gravity * (RDes.X * CosPsi + RDes.Y * SinPsi, DesState.Yaw)));

	FVector Rotation = State.Rotation;

	//phi = state.rot(1);
	//theta = state.rot(2);
	//psi = state.rot(3);

	//pdes = 0;
	//qdes = 0;
	//rdes = des_state.yawdot;

	FVector OmegaDes(0, 0, DesState.YawDot);

	//p = state.omega(1);
	//q = state.omega(2);
	//r = state.omega(3);
	FVector Omega = State.AngularVelocity;

	/*
	M = [Kpphi * (phides - phi) + Kdphi * (pdes - p), ...
		Kptheta * (thetades - theta) + Kdtheta * (qdes - q), ...
		Kppsi * (psides - psi) + Kdpsi * (rdes - r)]';

		F = clamp(m * (g + rzdes), minF, maxF);
	*/
	FVector KpRot(Kpphi,Kptheta,Kppsi);
	FVector KdRot(Kdphi,Kdtheta,Kdpsi);
	
	M = KpRot * (RotationDes - Rotation) + KdRot * (OmegaDes - Omega);
	F = FMath::Clamp<float>(Mass * (Gravity + RDes.Z), MinTorque, MaxTorque);
}