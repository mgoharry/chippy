// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MainHUDWidget.h"


void UMainHUDWidget::AddOrderCard(FString Name, UTexture2D* Image, FColor Color)
{
	if (UOrderCardWidget* OrderCardWidget = CreateWidget<UOrderCardWidget>(this, OrderCardWidgetClass))
	{
		if (!OrderCardWidget) return;
		OrderCardWidget->ProductInfo->SetText(FText::FromString(Name));
		OrderCardWidget->ProductImage->SetBrushFromTexture(Image);
		OrderCardWidget->ProductImage->SetBrushTintColor(Color);
		CardsVerticalBox->AddChild(OrderCardWidget);
	}
}

void UMainHUDWidget::RemoveOrderCard()
{
	CardsVerticalBox->GetChildAt(0)->RemoveFromParent();
}
