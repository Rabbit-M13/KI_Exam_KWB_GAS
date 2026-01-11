// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUDWidget.generated.h"

class UBarWidget;
/**
 * 
 */
UCLASS()
class KWB_KI_EXAM_GAS_API UMainHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetMana(int32 Current, int32 Max);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBarWidget> ManaBar;
};
