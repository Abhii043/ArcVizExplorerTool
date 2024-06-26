#include "WallGenerator.h"

AWallGenerator::AWallGenerator()
{
	PrimaryActorTick.bCanEverTick = true;

	WallSceneComponent = CreateDefaultSubobject<USceneComponent>("WallSceneComponent");
	RootComponent = WallSceneComponent;
}

void AWallGenerator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWallGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWallGenerator::DestroyWall()
{
	for (UStaticMeshComponent* Component : ComponentsArray) {
		if (Component) {
			Component->DestroyComponent();
			Component = nullptr;
		}
	}
	ComponentsArray.Empty();

	if (!WallGeneratorActorMap.IsEmpty()) {
		for (auto& MapComponent : WallGeneratorActorMap) {
			MapComponent.Value.ProceduralMesh->DestroyComponent();
			MapComponent.Value.ProceduralMesh = nullptr;
		}
	}
}

void AWallGenerator::CheckReducedSegments(const int32& Segments)
{
	Indexs.Empty();

	if(!WallGeneratorActorMap.IsEmpty()){
		for (auto& MapComponent : WallGeneratorActorMap) {
			if (MapComponent.Key > (Segments - 1)) {
				Indexs.Add(MapComponent.Key);
			}
		}
		for (int i{}; i < Indexs.Num(); ++i) {
			WallGeneratorActorMap.FindAndRemoveChecked(Indexs[i]);
		}
	}
}

void AWallGenerator::SetDoorsAndPRoceduralMesh(const int32& Segments)
{
	if (!WallGeneratorActorMap.IsEmpty()) {
		if(!ComponentsArray.IsEmpty()){
			for (auto& MapComponent : WallGeneratorActorMap) {
				if (MapComponent.Key <= (Segments - 1)) {
					if (ComponentsArray[MapComponent.Key]) {
						FVector MeshLocation = ComponentsArray[MapComponent.Key]->GetRelativeLocation();
						FVector WallDimensions = WallStaticMesh->GetBounds().GetBox().GetSize();
						MeshLocation.Z -= (WallDimensions.Z/2);

						ComponentsArray[MapComponent.Key]->SetStaticMesh(MapComponent.Value.StaticMesh);
						ComponentsArray[MapComponent.Key]->SetRelativeRotation(FRotator(0, 90, 0));
						ComponentsArray[MapComponent.Key]->SetRelativeLocation(MeshLocation);


						float DoorHeight = MapComponent.Value.StaticMesh->GetBounds().GetBox().GetSize().Z;
						FVector DoorLocation = ComponentsArray[MapComponent.Key]->GetRelativeLocation();

						MapComponent.Value.ProceduralMesh = NewObject<UProceduralMeshComponent>(this);
						FVector CubeLocation = FVector(DoorLocation.X, DoorLocation.Y, DoorLocation.Z + DoorHeight + ((WallDimensions.Z - DoorHeight) / 2));
						FVector CubeDimensions = FVector(WallDimensions.X, WallDimensions.Y, WallDimensions.Z - DoorHeight);

						MapComponent.Value.ProceduralMesh->SetupAttachment(WallSceneComponent);
						MapComponent.Value.ProceduralMesh->RegisterComponentWithWorld(GetWorld());

						GenerateCube(CubeDimensions, FVector::ZeroVector, MapComponent.Value.ProceduralMesh);
						MapComponent.Value.ProceduralMesh->SetRelativeLocation(CubeLocation);
						MapComponent.Value.ProceduralMesh->SetVisibility(true);
					}
				}

			}
		}
	}
}

void AWallGenerator::GenerateWall(const int32& SegmentsNo)
{
	DestroyWall();

	CheckReducedSegments(SegmentsNo);

	if(WallStaticMesh){WallHeight = WallStaticMesh->GetBounds().GetBox().GetSize().Z;}

	float MeshLength{};
	float MeshHeight{};

	if (WallStaticMesh) {
		MeshLength = WallStaticMesh->GetBounds().GetBox().GetSize().X;
		MeshHeight = WallStaticMesh->GetBounds().GetBox().GetSize().Z;
	}

	for (int i{}; i < SegmentsNo; i++) {
		UStaticMeshComponent* WallStaticMeshComponent = NewObject<UStaticMeshComponent>(this);
		WallStaticMeshComponent->SetupAttachment(WallSceneComponent);
		WallStaticMeshComponent->RegisterComponentWithWorld(GetWorld());
		if (WallStaticMesh) {
			WallStaticMeshComponent->SetStaticMesh(WallStaticMesh);
			WallStaticMeshComponent->SetRelativeLocation(FVector(i * MeshLength, WallStaticMeshComponent->GetRelativeLocation().Y, MeshHeight/2));
		}
		ComponentsArray.Add(WallStaticMeshComponent);
	}

	SetDoorsAndPRoceduralMesh(SegmentsNo);
}

void AWallGenerator::GenerateCube(const FVector& Dimensions, const FVector& LocationOffset, UProceduralMeshComponent* CubeComponent)
{
	TArray<FVector> Vertices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FProcMeshTangent> Tangents;
	TArray<FLinearColor> Colors;

	// Bottom Face
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 0 - - -
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 1 - + -
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 2 + - -
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 3 + + -

	// Top Face
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 4 - - +
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 5 - + +
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 + - +
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 + + +

	// Front Face
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 4 - - - 0
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 5 - + - 1
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 6 - - + 4
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 7 - + + 5

	// Back Face
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 8 + - - 2
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 9 + + - 3
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 10 + - + 6
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 11 + + + 7

	// Left Face
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 12 + - - 2
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 13 - - - 0
	Vertices.Add(FVector{ Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 14 + - + 6
	Vertices.Add(FVector{ Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 15 - - + 4

	// Right Face
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 16 + + - 3
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, -Dimensions.Z / 2 } + LocationOffset); // 17 - + - 1
	Vertices.Add(FVector{ -Dimensions.X / 2, -Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 18 + + + 7
	Vertices.Add(FVector{ -Dimensions.X / 2, Dimensions.Y / 2, Dimensions.Z / 2 } + LocationOffset); // 19 - + + 5

	// Bottom Face Triangle
	TArray<int32> Triangles{ 0, 1, 2 ,1, 3, 2 ,4, 5, 6 ,5, 7, 6 ,8, 9, 10,9, 11, 10 ,12, 13, 14 ,13, 15, 14 ,16, 17, 18 ,17, 19, 18 ,20, 21, 22 ,21, 23, 22 };

	for (int32 i = 0; i < Vertices.Num(); i += 4) {
		UVs.Add(FVector2D{ 0.0, 1.0 });
		UVs.Add(FVector2D{ 1.0, 1.0 });
		UVs.Add(FVector2D{ 0.0, 0.0 });
		UVs.Add(FVector2D{ 1.0, 0.0 });
	}

	for (int32 i = 0; i < Vertices.Num(); i++) {
		if (i >= 0 && i <= 3) {
			Normals.Add(FVector{ 0.0, 0.0, -1.0 });
		}
		else if (i >= 4 && i <= 7) {
			Normals.Add({ 0.0, 0.0, 1.0 });
		}
		else if (i >= 8 && i <= 11) {
			Normals.Add({ 0.0, -1.0, 0.0 });
		}
		else if (i >= 12 && i <= 15) {
			Normals.Add({ 0.0, 1.0, 0.0 });
		}
		else if (i >= 16 && i <= 19) {
			Normals.Add({ 1.0, 0.0, 0.0 });
		}
		else {
			Normals.Add({ -1.0, 0.0, 0.0 });
		}
	}

	CubeComponent->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, Colors, Tangents, true);
	CubeComponent->SetMaterial(0,ProceduralMeshMaterial);
	
}

void AWallGenerator::ApplyMaterialToWallActor(UMaterialInterface* WallMaterial)
{
	UMaterialInstanceDynamic* DynamicWallMaterial1 = UMaterialInstanceDynamic::Create(WallMaterial, this);
	UMaterialInstanceDynamic* DynamicWallMaterial2 = UMaterialInstanceDynamic::Create(WallMaterial, this);
	if (DynamicWallMaterial1 && DynamicWallMaterial2) {
		FVector WallDimensions = WallStaticMesh->GetBounds().GetBox().GetSize();


		for (int i{} ;  i<ComponentsArray.Num() ; ++i) {
			if (WallGeneratorActorMap.Contains(i)) {
				float TileX = (WallDimensions.X) / (WallDimensions.Z - 212);
				float TileY = 1;
				DynamicWallMaterial1->SetScalarParameterValue("TileX", TileX);
				DynamicWallMaterial1->SetScalarParameterValue("TileY", TileY);

				WallGeneratorActorMap[i].ProceduralMesh->SetMaterial(0, DynamicWallMaterial1);
			}
			else {
				float TileX = (WallDimensions.X) / (WallDimensions.Z - 212);
				float TileY = (WallDimensions.Z) / (WallDimensions.Z - 212);
				DynamicWallMaterial2->SetScalarParameterValue("TileX", TileX);
				DynamicWallMaterial2->SetScalarParameterValue("TileY", TileY);

				ComponentsArray[i]->SetMaterial(0, DynamicWallMaterial2);
			}
		}
	}

}

