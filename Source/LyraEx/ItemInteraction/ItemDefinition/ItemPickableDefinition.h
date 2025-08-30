#pragma once

#include "Engine/DataAsset.h"
#include "ItemPickableDefinition.generated.h"

UCLASS(BlueprintType,Blueprintable)
class UItemPickableDefinition : public UObject
{
	GENERATED_BODY()

public:
	UItemPickableDefinition();

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	FName ItemSlotName;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	FTransform ItemRelativeTransform;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	TObjectPtr<UAnimMontage> StartTriggerAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	TObjectPtr<UAnimMontage> EndTriggerAnimMontage;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	float ThrowItemSpeed;

	//背包物品实例类
	// UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Defaults")
	// TSubclassOf<ATPItemActor_Inventory> ItemClass;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Inventory")
	int32 ItemMaxCounts;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Inventory")
	bool bAllowInHand;
};
