// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Widgets/MainHUDWidget.h"
#include "ChippyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CHIPPY_API AChippyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Config")
	TSubclassOf<UMainHUDWidget> MainHUDWidgetClass;

	UPROPERTY()
	UMainHUDWidget* MainHUDWidget;

	UPROPERTY()
	class AMainGameMode* MainGameMode;

	UFUNCTION(Client, Unreliable)
	void AddOrderCard(FName Name, UTexture2D* Image, FColor Color);
};
