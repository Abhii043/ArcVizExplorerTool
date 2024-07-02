// Fill out your copyright notice in the Description page of Project Settings.


#include "TemplateActor.h"

// Sets default values
ATemplateActor::ATemplateActor()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = SceneComponent;
}

void ATemplateActor::BeginPlay()
{
	Super::BeginPlay();
	
}
void ATemplateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

