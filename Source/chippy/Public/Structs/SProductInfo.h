#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "SProductInfo.generated.h"

USTRUCT(BlueprintType)
struct CHIPPY_API FProductInfo : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor Color = FColor::White;

	FProductInfo()
		: Name("Unknown")
		  , Mesh()
		  , Color(FColor::White)
	{
	}
};
