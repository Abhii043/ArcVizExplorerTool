// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateActor.generated.h"

UCLASS()
class ARCHVIZEXPLORER_API ATemplateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATemplateActor();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY()
	USceneComponent* SceneComponent;

};
