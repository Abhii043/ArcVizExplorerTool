// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "WallGenerator.generated.h"

USTRUCT()
struct FPair {
	GENERATED_BODY()

	UPROPERTY()
	UStaticMesh* StaticMesh; 

	UPROPERTY()
	UProceduralMeshComponent* ProceduralMesh;
};

UCLASS()
class ARCHVIZEXPLORER_API AWallGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	AWallGenerator();

	UFUNCTION()
	void GenerateWall(const int32& SegmentsNo);

	UPROPERTY(EditDefaultsOnly , category = "WallGenerator")
	UStaticMesh* WallStaticMesh;

	UPROPERTY()
	float WallHeight; 

	UPROPERTY()
	TArray<UStaticMeshComponent*> ComponentsArray;

	UPROPERTY()
	TMap<int32, FPair> WallGeneratorActorMap;

	UFUNCTION()
	void GenerateCube(const FVector& Dimensions, const FVector& LocationOffset, UProceduralMeshComponent* CubeComponent);

	UFUNCTION()
	void ApplyMaterialToWallActor(UMaterialInterface* WallMaterial);

	UPROPERTY()
	TArray<int32> Indexs;

	UPROPERTY()
	int32 NoOfSegments{};

	UPROPERTY()
	UMaterialInterface* WallMaterial_{};

	UPROPERTY(EditAnywhere, category = "WallGenerator")
	UMaterialInterface* ProceduralMeshMaterial;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	USceneComponent* WallSceneComponent; 

	UFUNCTION()
	void DestroyWall();

	UFUNCTION()
	void CheckReducedSegments(const int32& Segments);

	UFUNCTION()
	void SetDoorsAndPRoceduralMesh(const int32& Segments);
};
