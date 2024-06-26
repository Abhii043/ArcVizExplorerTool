// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <ProceduralMeshComponent.h>
#include "RoadConstructor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ARoadConstructor : public AActor
{
	GENERATED_BODY()
	
public:	
	ARoadConstructor();

	UPROPERTY(VisibleAnywhere, Category = "RoadConstructor")
	UProceduralMeshComponent* ProceduralMeshComponent;

	UPROPERTY(EditDefaultsOnly,Category = "RoadConstructor")
	UMaterialInterface* Material;

	UPROPERTY()
	FVector RoadDimensions{};

	UFUNCTION(BlueprintCallable , Category = "RoadConstructor")
	void GenerateRoad(const FVector& Dimensions , const FVector& LocationOffset);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<FVector> Vertices;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<int32> Triangles;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<FVector> Normals;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<FVector2D> UVs;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<FProcMeshTangent> Tangents;

	UPROPERTY(VisibleDefaultsOnly, Category = "RoadConstructor")
	TArray<FLinearColor> Colors; 

	int AddNewVertex(const FVector& VertexCordinates);

	int AddUV(const FVector2D& UVCordinates);

	void DrawTriangleFromVertex(int32 Vertex0, int32 Vertex1, int32 Vertex2);
};
