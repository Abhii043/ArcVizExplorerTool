// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "FloorGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AFloorGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorGenerator();

	UPROPERTY()
	UProceduralMeshComponent* FloorComponent;

	UPROPERTY()
	FVector FloorDimensions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	// Called every frame
	void GenerateFloor(const FVector& Dimensions);
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset);

};
