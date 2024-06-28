// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteriorDesignActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API AInteriorDesignActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AInteriorDesignActor();

	UPROPERTY()
	int8 IdentityIndex;

	UFUNCTION()
	void SetInteriorMesh(UStaticMesh* InteriorMesh);

	UPROPERTY()
	UStaticMesh* InteriorMesh;

	UPROPERTY()
	UStaticMeshComponent* InteriorMeshComponent;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:	
	UPROPERTY()
	USceneComponent* InteriorSceneComponent;
};
