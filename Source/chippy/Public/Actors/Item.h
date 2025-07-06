// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Structs/SProductInfo.h"
#include "Interfaces/Interactable.h"
#include "Item.generated.h"


UCLASS()
class CHIPPY_API AItem : public AActor, public IInteractable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	virtual void Init(FProductInfo inAssignedProduct, FColor inAssignedColor = FColor::White);

	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_Color)
	FColor MaterialColor;

	UFUNCTION()
	virtual void OnRep_Color();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* AssignedProductMesh;

public:
	UPROPERTY()
	FProductInfo AssignedProduct;
	/** Returns InteractionComponent subobject **/
	FORCEINLINE USphereComponent* GetSphereComponent() const { return SphereComponent; }
};
