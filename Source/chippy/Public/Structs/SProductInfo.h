#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/StaticMesh.h"
#include "SProductInfo.generated.h"

USTRUCT(BlueprintType)
struct FColorInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	FColorInfo();

	FColorInfo(FString inName, FColor inColor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColor RGBA = FColor::White;

	bool operator==(const FColorInfo& OtherProduct) const
	{
		return Name == OtherProduct.Name &&
			RGBA == OtherProduct.RGBA;
	}
};

inline FColorInfo::FColorInfo()
{
}

inline FColorInfo::FColorInfo(FString inName, FColor inColor)
	: FTableRowBase()
	  , Name(inName)
	  , RGBA(inColor)
{
}

USTRUCT(BlueprintType)
struct CHIPPY_API FProductInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	FProductInfo();
	FProductInfo(FName inName, TSoftObjectPtr<UStaticMesh> inMesh, TSoftObjectPtr<UTexture2D> inTexture2D,
	             FColorInfo inColor);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FColorInfo Color{"White", FColor::White};

	bool operator==(const FProductInfo& OtherProduct) const
	{
		return Name == OtherProduct.Name &&
			Mesh == OtherProduct.Mesh &&
			Texture == OtherProduct.Texture &&
			Color == OtherProduct.Color;
	}
};

inline FProductInfo::FProductInfo()
{
}

inline FProductInfo::FProductInfo(FName inName, TSoftObjectPtr<UStaticMesh> inMesh,
                                  TSoftObjectPtr<UTexture2D> inTexture2D,
                                  FColorInfo inColor)
	: FTableRowBase()
	  , Name(inName.ToString())
	  , Mesh(inMesh)
	  , Texture(inTexture2D)
	  , Color(inColor)
{
}
