#pragma once

#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "WeaponAbilityInputConfig.generated.h"

class UInputAction;

USTRUCT(BlueprintType)
struct FWeaponInputAction
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction=nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag InputTag;
};

UCLASS(BlueprintType,Const)
class UWeaponAbilityInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable,Category="AbilityInputConfig")
	const UInputAction* FindAbilityInputActionForTag(const FGameplayTag& InputTag)const;

public:
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="AbilityInputConfig")
	TArray<FWeaponInputAction>AbilityInputActions;
};
