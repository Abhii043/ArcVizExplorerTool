// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "RoofGenerator.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoofGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoofGenerator();

	UPROPERTY()
	UProceduralMeshComponent* RoofComponent;

	UPROPERTY()
	FVector RoofDimensions;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

public:	
	// Called every frame
	void GenerateRoof(const FVector& Dimensions);
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset);

};
