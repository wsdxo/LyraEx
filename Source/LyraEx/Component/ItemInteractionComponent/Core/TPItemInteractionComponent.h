#pragma once
#include  "Components/ActorComponent.h"
#include "TPItemInteractionComponent.generated.h"

class UAbilitySystemComponent;
class ATPItemActorBase;
DEFINE_LOG_CATEGORY_STATIC(LogTPItemInteractionComponent, Log, All);

UCLASS(ClassGroup="Custom",Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class UTPItemInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTPItemInteractionComponent();
public:
	UFUNCTION(BlueprintCallable,Category="Third Person ItemInteractionComponent")
	ATPItemActorBase* GetInteractingItem() const {return InteractingItem.Get();}
	
	//与物品交互
	UFUNCTION(BlueprintCallable,Category="Third Person ItemInteractionComponent")
	void StartTriggerSelectingItem();

	UFUNCTION(BlueprintCallable,Category="Third Person ItemInteractionComponent")
	void EndTriggerSelectingItem();
	
	UFUNCTION(BlueprintNativeEvent,BlueprintPure,Category="Third Person ItemInteractionComponent")
	bool CanStartInteraction()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintPure,Category="Third Person ItemInteractionComponent")
	bool CanEndInteraction()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintPure,Category="Third Person ItemInteractionComponent")
	bool CanStartTriggerInteractingItem()const;

	UFUNCTION(BlueprintNativeEvent,BlueprintPure,Category="Third Person ItemInteractionComponent")
	bool CanEndTriggerInteractingItem()const;

	//服务器广播播放蒙太奇
	UFUNCTION(NetMulticast,Unreliable,BlueprintCallable,Category="Third Person ItemInteractionComponent")
	void PlayMontageMulticast(UAnimMontage* InMontage,float InPlayRate=1.f,FName StartSelectionName=NAME_None);

	//服务器使用
	void ServerTriggerItem(ATPItemActorBase* NewTriggerItem,bool bForceInHand=false);
protected:
	virtual bool CanStartInteraction_Implementation() const;

	virtual bool CanEndInteraction_Implementation() const;

	virtual bool CanStartTriggerInteractingItem_Implementation() const;

	virtual bool CanEndTriggerInteractingItem_Implementation() const;
protected:
	//当前选中场景中交互物品
	UPROPERTY(BlueprintReadOnly,Category="Third Person ItemInteractionComponent")
	TWeakObjectPtr<ATPItemActorBase>SelectingItem;

	UFUNCTION()
	virtual void OnRep_InteractingItem();

	UFUNCTION(Server,Unreliable)
	void StartTriggerSelectingItemOnServer(ATPItemActorBase* InSelectingItem);

	//当结束交互后需要立刻与另一物体交互时，bIsPutPack为true
	UFUNCTION(Server,Unreliable)
	void EndTriggerInteractingItemOnServer(bool bIsPutPack);

	//处理开始交互逻辑，bForceInHand是否需要强制手持物品
	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemInteractionComponent")
	void OnSelectingItemTriggerStart(ATPItemActorBase* InSelectingItem,bool bForceInHand);
	virtual void OnSelectingItemTriggerStart_Implementation(ATPItemActorBase* InSelectingItem,bool bForceInHand);

	//处理结束交互逻辑
	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemInteractionComponent")
	void OnSelectingItemTriggerEnd(ATPItemActorBase* InSelectingItem,bool bForceInHand);
	virtual void OnSelectingItemTriggerEnd_Implementation(ATPItemActorBase* InSelectingItem,bool bForceInHand);

	UFUNCTION(BlueprintPure,Category="Third Person ItemInteractionComponent")
	UAbilitySystemComponent* GetAbilitySystemComponentByOwner() const;
protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	//正在交互的对象
	UPROPERTY(ReplicatedUsing=OnRep_InteractingItem,BlueprintReadOnly,Category="Third Person ItemInteractionComponent")
	TWeakObjectPtr<ATPItemActorBase> InteractingItem;
};
