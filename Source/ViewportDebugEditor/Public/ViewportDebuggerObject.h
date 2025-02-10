#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ViewportDebuggerObject.generated.h"

UCLASS()
class UViewportInfoObject : public UObject
{
	GENERATED_UCLASS_BODY()
public:
	
	UPROPERTY(Category = "Basic", VisibleAnywhere, meta = (DisplayName = "Active Viewport", DisplayPriority = 1))
	FString ActiveViewInfo;

	UPROPERTY(Category = "Basic", VisibleAnywhere, meta = (DisplayName = "Map Name", DisplayPriority = 1))
	FString ActiveMapName;

	UPROPERTY(Category = "Basic", VisibleAnywhere, meta = (DisplayName = "Camera Location", DisplayPriority = 2))
	FVector ViewPosition;

	UPROPERTY(Category = "Basic", VisibleAnywhere, meta = (DisplayName = "Camera Rotation", DisplayPriority = 2))
	FRotator ViewRotation;

	UPROPERTY(Category = "Basic", VisibleAnywhere, meta = (DisplayName = "FOV", DisplayPriority = 2))
	float FOV;

	UPROPERTY(Category = "Basic", EditAnywhere, meta = (DisplayName = "Camera Location", DisplayPriority = 2))
	FVector SetViewPosition;

	UPROPERTY(Category = "Basic", EditAnywhere, meta = (DisplayName = "Camera Rotation", DisplayPriority = 2))
	FRotator SetViewRotation;

	UPROPERTY(Category = "Basic", EditAnywhere, meta = (DisplayName = "FOV", DisplayPriority = 2))
	float SetFOV;
	
	FVector PawnPosition;

	FRotator PawnRotation;
};

