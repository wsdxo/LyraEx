#pragma once
#include "GameFramework/Actor.h"
#include "LyraEx/ItemInteraction/ItemInteractionType.h"
#include "TPItemActorBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTPItemActor,Log,All);

enum class ETPItemType:uint8;
class UTPItemInteractionComponent;
UCLASS(Abstract)
class ATPItemActorBase : public AActor
{
	GENERATED_BODY()

public:
	ATPItemActorBase();

protected:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Third Person ItemActorBase")
	ETPItemType TPItemType;

	UFUNCTION(BlueprintCallable,Category="Third Person ItemActorBase")
	bool StartTrigger(UTPItemInteractionComponent* ItemInteractionComponent,bool bForceInHand=false);

	UFUNCTION(BlueprintCallable,Category="Third Person ItemActorBase")
	bool EndTrigger(UTPItemInteractionComponent* ItemInteractionComponent,bool bIsPutPack);

	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemActorBase")
	bool CanStartTrigger(UTPItemInteractionComponent* ItemInteractionComponent,bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemActorBase")
	bool CanEndTrigger(UTPItemInteractionComponent* ItemInteractionComponent,bool bIsPutPack);

	UFUNCTION(BlueprintPure,Category="Third Person ItemActorBase")
	ETPItemType GetTPItemType()const {return TPItemType;}


protected:
	virtual bool CanStartTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,bool bForceInHand);
	virtual bool CanEndTrigger_Implementation(UTPItemInteractionComponent* ItemInteractionComponent,bool bIsPutPack);
	
	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemActorBase")
	void OnStartTrigger(UTPItemInteractionComponent*ItemInteractionComponent,bool bForceInHand);
	virtual void OnStartTrigger_Implementation(UTPItemInteractionComponent*ItemInteractionComponent,bool bForceInHand);

	UFUNCTION(BlueprintNativeEvent,Category="Third Person ItemActorBase")
	void OnEndTrigger(UTPItemInteractionComponent*ItemInteractionComponent,bool bIsPutPack);
	virtual void OnEndTrigger_Implementation(UTPItemInteractionComponent*ItemInteractionComponent,bool bIsPutPack);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(Replicated,BlueprintReadOnly,Category="Third Person ItemActorBase")
	TWeakObjectPtr<UTPItemInteractionComponent> InteractingComponent;
};
