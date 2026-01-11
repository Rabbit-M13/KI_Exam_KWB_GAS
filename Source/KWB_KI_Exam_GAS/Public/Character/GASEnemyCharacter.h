#pragma once
#include "CoreMinimal.h"
#include "GASCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "GASEnemyCharacter.generated.h"

class UEnemyAttributeSet;
class UWidgetComponent;
class UGameplayEffect;
class UBarWidget;

UCLASS()
class KWB_KI_EXAM_GAS_API AGASEnemyCharacter : public AGASCharacterBase
{
	GENERATED_BODY()

public:
	AGASEnemyCharacter();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> HealthWidgetComp;

	UPROPERTY()
	TObjectPtr<UBarWidget> HealthBar;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UEnemyAttributeSet> EnemyAS;

	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitAttributesEffect; // GE_Enemy_InitAttributes

	bool bBound = false;

	void ApplyInitAttributes();
	void CacheBar();
	void BindHealth();
	void RefreshHealth();

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
};
