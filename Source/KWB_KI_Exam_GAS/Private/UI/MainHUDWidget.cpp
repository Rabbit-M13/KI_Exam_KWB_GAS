// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainHUDWidget.h"
#include "UI/BarWidget.h"

void UMainHUDWidget::SetMana(int32 Current, int32 Max)
{
	// UE_LOG(LogTemp, Error, TEXT("SetMana!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"));

	if (ManaBar) ManaBar->SetBarValues(Current, Max);

	UE_LOG(LogTemp, Warning, TEXT("SetMana called: %d/%d, ManaBar=%s"),
		Current, Max, ManaBar ? TEXT("Valid") : TEXT("NULL"));
}