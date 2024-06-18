// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WallGenerator.generated.h"

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

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	USceneComponent* WallSceneComponent;

	UPROPERTY()
	TArray<UStaticMeshComponent*> ComponentsArray;

	UFUNCTION()
	void DestroyWall();
};
