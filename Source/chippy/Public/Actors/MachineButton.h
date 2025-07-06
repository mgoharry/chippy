// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/InterpToMovementComponent.h>
#include "Item.h"
#include "MachineButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnMachineButtonActivated, FProductInfo);


UCLASS()
class CHIPPY_API AMachineButton : public AItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMachineButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UStaticMeshComponent* ButtonMesh;

	UPROPERTY()
	class AColoringMachine* MachineRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void ResetPressTimer();

	FTimerHandle PressHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	UInterpToMovementComponent* MovementComponent;

public:
	virtual void Interact(AchippyCharacter* InteractingCharacter) override;

	virtual void Init(FProductInfo inAssignedProduct, FColor inAssignedColor = FColor::White);

	virtual void Init(FColor inAssignedColor = FColor::White);

	FOnMachineButtonActivated MachineButtonActivatedDelegate;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	UMaterialInstanceDynamic* AssignedMeshMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ButtonDynamicMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_ButtonColor)
	FLinearColor ButtonMaterialColor;

	UPROPERTY(ReplicatedUsing = OnRep_AssignedMeshColor)
	FLinearColor AssignedMeshMaterialColor;

	UFUNCTION()
	void OnRep_AssignedMeshColor();

	UFUNCTION()
	void OnRep_ButtonColor();
	void CreateDynamicMaterial();


	UFUNCTION()
	void AssignProductMeshToPreview(TSoftObjectPtr<UStaticMesh> inMesh);

	UFUNCTION()
	void RemoveProductPreviewMesh() const;
};
