// Fill out your copyright notice in the Description page of Project Settings.

#include "PositionModellingActor.h"
//#include "Synopticon.h"


//FVector APositionModellingActor::PositionOffset = FVector(-12, 7, -3);


// Sets default values
APositionModellingActor::APositionModellingActor()
{
	PrimaryActorTick.bCanEverTick = false;
	ManualToolOffset = FVector(58.6, 0, 0);
	LaserToolOffset = FVector(0, 0, -4);
}

void APositionModellingActor::SetPositionAndOrientationComponent(UPositionAndOrientationComponent* NewPosOriComponent)
{
	PosOriComponent = NewPosOriComponent;
}

// Called when the game starts or when spawned
void APositionModellingActor::BeginPlay()
{
	Super::BeginPlay();

	ToolVisible = true;
	SetActorHiddenInGame(ToolVisible);
	SetActorEnableCollision(false);
}

void APositionModellingActor::SetManualModellingToolOffset(float dist)
{
	if (ManualType)
	{
		ManualToolOffset.Set(dist, 0, 0);
		if (PosOriComponent)
		{
			PosOriComponent->SetActorLocationOffset(ManualToolOffset);
		}
	}
}

FVector APositionModellingActor::GetLocationOffset() {
	if (ManualType)
	{
		return ManualToolOffset;
	}
	else 
	{
		return LaserToolOffset;
	}
}

void APositionModellingActor::SetModellingToolType(bool _Manual)
{
	ManualType = _Manual;
}

bool APositionModellingActor::IsManualModellingTool()
{
	return ManualType;
}

FVector APositionModellingActor::CalculatePoint(float distance)
{
	FTransform ToolTransform = this->GetRootComponent()->GetComponentTransform();
	
	FVector Direction = ToolTransform.Rotator().Vector();//this->GetActorRotation().Vector();
	FVector Location = ToolTransform.GetLocation();//this->GetActorLocation();

	FVector Point;
	if (ManualType)
	{
		Point = Location;
	}
	else
	{
		Point = Location + (Direction*distance);
	}
	
	return Point;
}

void APositionModellingActor::SetToolVisibility(bool on)
{
	SetActorHiddenInGame(!on);
}

void APositionModellingActor::SetShouldApplyOffset(bool ApplyOffset)
{
	PosOriComponent->SetShouldApplyOffset(ApplyOffset);
}

void APositionModellingActor::ResetPositionAndOrientation()
{
	UPositionAndOrientationComponent* PosComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (PosComponent)
	{
		PosComponent->ResetMarkerIDs();
	}
	if (ManualType)
	{
		SetManualModellingToolOffset(0);
	}
}

TArray<FVector> APositionModellingActor::CreateEllipseShape(TArray<FVector> Points, int Subdivisions)
{
	//first point is to calculate the height, ignore it for the ellipsis
	TArray<FVector> Result;
	//Result.Add(Points[0]);
	if (Subdivisions < 0) 
	{
		Subdivisions = 0;
	}
	FVector Center = (Points[1] + Points[3]) / 2;

	float a = FVector::Dist(Points[1], Points[3])/2;
	float b = FVector::Dist(Points[2], Center);
	float z = Points[1].Z;

	float Delta = 0;
	if (Subdivisions != 0)
	{
		Delta = (Points[2].X - Points[1].X)/Subdivisions;
	}

	//Result.Add(Points[1]);
	bool First = true;
	for (int i = 0; i <= Subdivisions * 2; i++) {
		float x = Points[1].X + Delta*i;
		float y = EllipseFormula(a, b, Center, x);
		int LastIndex = Result.Add(FVector(x, y + Center.Y, z));
		FVector FourthMarker;
		if (First)
		{
			First = false;
			FourthMarker = Points[0];
			//FourthMarker = (Result[LastIndex] + Result[LastIndex - 2] - Result[LastIndex - 1]);
		}
		else
		{
			FourthMarker = (Result[LastIndex] + Result[LastIndex - 1] - Result[LastIndex - 2]);
		}

		Result.Add(FourthMarker);
	}

	/*Result.Add(Points[3]);
	int LastIndex = Result.Num() - 1;
	FVector FourthMarker = (Result[LastIndex] + Result[LastIndex - 1] - Result[LastIndex - 2]);
	Result.Add(FourthMarker);*/
	return Result;
}

float APositionModellingActor::EllipseFormula(float a, float b, FVector Center, float x)
{
	return (b / a)*FMath::Sqrt(FMath::Pow(a, 2) - FMath::Pow(x - Center.X, 2)); //y = (longR/shortR)*sqrt(shortR^2 - x^2)
}


TArray<FVector> APositionModellingActor::CreateArcShape(TArray<FVector> Points, int Subdivisions)
{
	TArray<FVector> Result;
	//Result.Add(Points[0]);
	if (Subdivisions < 0)
	{
		Subdivisions = 0;
	}

	float a, b, r;
	float z = Points[1].Z;

	CalculateCircle(Points[1], Points[2], Points[3], a, b, r);
	float Delta = 0;
	if (Subdivisions != 0)
	{
		Delta = (Points[2].X - Points[1].X) / Subdivisions;
	}

	//Result.Add(Points[1]);
	bool First = true;
	for (int i = 0; i <= Subdivisions * 2; i++) {
		float x = Points[1].X + Delta*i;
		float y = CircularFormula(a, b, r, x);
		int LastIndex = Result.Add(FVector(x, y, z));
		FVector FourthMarker;
		if (First)
		{
			First = false;
			FourthMarker = Points[0];
			//FourthMarker = (Result[LastIndex] + Result[LastIndex - 2] - Result[LastIndex - 1]);
		}
		else
		{
			FourthMarker = (Result[LastIndex] + Result[LastIndex - 1] - Result[LastIndex - 2]);
		}

		Result.Add(FourthMarker);
	}

	/*Result.Add(Points[3]);
	int LastIndex = Result.Num() - 1;
	FVector FourthMarker = (Result[LastIndex] + Result[LastIndex - 1] - Result[LastIndex - 2]);
	Result.Add(FourthMarker);*/
	return Result;
}

void APositionModellingActor::CalculateCircle(FVector P1, FVector P2, FVector P3, float& a, float& b, float& r)
{
	//we disregard Z, consider P1, P2, P3 are on the same plane
	//solve this equation with three inputs: (x - a)^2 + (y - b)^2 = r^2
	float M1 = (P1.X*P1.X - P2.X*P2.X) + (P1.Y*P1.Y - P2.Y*P2.Y);
	float M2 = (P1.X*P1.X - P3.X*P3.X) + (P1.Y*P1.Y - P3.Y*P3.Y);

	a = (M1*(P1.Y - P3.Y) - M2*(P1.Y - P2.Y)) / (2*((P1.X - P2.X)*(P1.Y - P3.Y) - (P1.X - P3.X)*(P1.Y - P2.Y)));
	b = (M1 - 2*a*(P1.X - P2.X))/(2*(P1.Y - P2.Y));
	r = FMath::Sqrt(FMath::Pow(P1.X - a, 2) + FMath::Pow(P1.Y - b, 2));
}

float APositionModellingActor::CircularFormula(float a, float b, float r, float x)
{
	return FMath::Sqrt(FMath::Pow(r, 2) - FMath::Pow(x - a, 2)) + b;
}

UFUNCTION(BlueprintCallabel, Category = "ModellingTool")
void APositionModellingActor::SetLaserToolOffset(FVector _Offset)
{
	LaserToolOffset = _Offset;
	UPositionAndOrientationComponent* PosComponent = FindComponentByClass<UPositionAndOrientationComponent>();
	if (PosComponent)
	{
		PosComponent->SetActorLocationOffset(LaserToolOffset);
	}
}