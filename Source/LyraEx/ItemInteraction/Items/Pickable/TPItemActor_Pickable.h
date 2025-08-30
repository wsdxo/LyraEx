#pragma once
#include "LyraEx/ItemInteraction/Items/Core/TPItemActorBase.h"
#include "LyraEx/ItemInteraction/ItemInteractionType.h"
#include "TPItemActor_Pickable.generated.h"

class UItemPickableDefinition;

UCLASS(Abstract)
class ATPItemActor_Pickable:public ATPItemActorBase
{
	GENERATED_BODY()

protected:
	ATPItemActor_Pickable();
	//拾取物种类
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="TPItemActor_Pickable")
	ETPItemPickableType PickableType;
	//物品基础配置
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="TPItemActor_Pickable")
	TSubclassOf<UItemPickableDefinition> ItemDefinition;

public:
	UFUNCTION(BlueprintPure,Category="TPItemActor_Pickable")
	ETPItemPickableType GetPickableType() const {return PickableType;}

	UFUNCTION(BlueprintPure,Category="TPItemActor_Pickable")
	UItemPickableDefinition* GetItemDefinition() const;

protected:
	virtual void OnStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent, bool bForceInHand) override;
	virtual void OnEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent, bool bIsPutPack) override;

protected:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="TPItemActor_Pickable")
	void PickupItem(UTPItemInteractionComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh);
	virtual void PickupItem_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh);

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable,Category="TPItemActor_Pickable")
	void ThrowItem(UTPItemInteractionComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh,bool bIsPutBack);
	virtual void ThrowItem_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,UPrimitiveComponent* InItemMesh, bool bIsPutBack);
};

