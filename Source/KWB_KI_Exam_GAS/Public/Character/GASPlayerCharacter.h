// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/GASCharacterBase.h"
#include "UI/MainHUDWidget.h"
#include "AbilitySystemComponent.h"
#include "GASPlayerCharacter.generated.h"

class UPlayerAttributeSet;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class KWB_KI_EXAM_GAS_API AGASPlayerCharacter : public AGASCharacterBase
{
	GENERATED_BODY()
	
public:
	AGASPlayerCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void InitializeAbilityActorInfo() override;
	void CreateHUD();
	void BindMana();
	void RefreshMana();

	void OnManaChanged(const FOnAttributeChangeData& Data);

	void OnMaxManaChanged(const FOnAttributeChangeData& Data);

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Init")
	TSubclassOf<UGameplayEffect> InitAttributesEffect;

	UPROPERTY(Transient)
	bool bInitAttributesApplied = false;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UMainHUDWidget> MainHUDClass;

	UPROPERTY(Transient)
	TObjectPtr<UMainHUDWidget> MainHUD;

	UPROPERTY(Transient)
	bool bAbilitiesGranted = false;

	UPROPERTY(EditDefaultsOnly, Category = "GAS|Abilities")
	TSubclassOf<UGameplayAbility> FireballAbilityClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS|Attributes")
	TObjectPtr<UPlayerAttributeSet> PlayerAttributeSet;


protected:
	bool bManaBound = false;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void Input_FireballPressed();

	void GrantAbilities();

	// GE 이니셜라이즈
	void ApplyInitAttributes();

};
