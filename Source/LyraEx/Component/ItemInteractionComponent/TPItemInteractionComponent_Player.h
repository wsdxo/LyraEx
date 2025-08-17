#pragma once
#include "Core/TPItemInteractionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TPItemInteractionComponent_Player.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS(ClassGroup="Custom",Blueprintable,BlueprintType,meta=(BlueprintSpawnableComponent))
class UTPItemInteractionComponent_Player:public UTPItemInteractionComponent
{
	GENERATED_BODY()
public:
	UTPItemInteractionComponent_Player();

protected:
	//物品交互输入映射
	UPROPERTY(EditDefaultsOnly,Category="TPItemInteractionComponent_Player")
	TObjectPtr<UInputMappingContext>IMC_TPItem;
	
	//与物品交互输入
	UPROPERTY(EditDefaultsOnly,Category="TPItemInteractionComponent_Player")
	TObjectPtr<UInputAction>IA_TriggerTPItem;
	
	UPROPERTY(EditDefaultsOnly,Category="TPItemInteractionComponent_Player")
	TObjectPtr<UInputAction>IA_ThrowTPItem;

	//碰撞检测类型
	UPROPERTY(EditDefaultsOnly,Category="TPItemInteractionComponent_Player")
	TEnumAsByte<ECollisionChannel>CheckCollisionType;

	//选取物品中心偏移量
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="TPItemInteractionComponent_Player")
	FVector SphereCenterOffset;
	//选取物品范围
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="TPItemInteractionComponent_Player")
	float SelectItemRange;
	//绘制模式
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="TPItemInteractionComponent_Player")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebugTraceMode;

public:
	virtual void AddPlayerInput(UEnhancedInputComponent* PlayerInputComponent);
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void PlayerTriggerTPItem(const FInputActionValue& Value);
	virtual void PlayerThrowTPItem(const FInputActionValue& Value);

public:
	void CheckTPItemAroundPlayer();

protected:
	//附近物品列表
	UPROPERTY(BlueprintReadOnly,Category="TPItemInteractionComponent_Player")
	TArray<ATPItemActorBase*>NearbyItems;
};
