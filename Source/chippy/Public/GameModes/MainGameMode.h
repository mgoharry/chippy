// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameFramework/GameModeBase.h"
#include "PlayerControllers/ChippyPlayerController.h"
#include "MainGameMode.generated.h"

/**
 * Main game mode class for the game
 */
UCLASS()
class CHIPPY_API AMainGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Constructor
	AMainGameMode();

	UPROPERTY()
	TArray<AChippyPlayerController*> PlayerControllers;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//UPROPERTY(Replicated)
	int32 CurrentBudget;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TArray<FColor> AvailableColors;

	UPROPERTY(EditDefaultsOnly, Category = "Machine")
	TObjectPtr<UDataTable> ProductsDataTable;

	void GenerateOrder();

	void CheckOrderValidity();

	void UpdateBudget();
};
