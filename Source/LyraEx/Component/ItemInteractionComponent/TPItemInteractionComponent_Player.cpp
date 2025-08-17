#include "TPItemInteractionComponent_Player.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "LyraEx/ItemInteraction/Items/Core/TPItemActorBase.h"

UE_DISABLE_OPTIMIZATION

UTPItemInteractionComponent_Player::UTPItemInteractionComponent_Player()
{
	PrimaryComponentTick.bCanEverTick=true;
	SelectItemRange=150.f;
	SphereCenterOffset=FVector::ZeroVector;
	
	CheckCollisionType=ECC_GameTraceChannel9;
}

void UTPItemInteractionComponent_Player::AddPlayerInput(UEnhancedInputComponent* PlayerInputComponent)
{
	if (PlayerInputComponent&&IA_TriggerTPItem&&IA_ThrowTPItem)
	{
		PlayerInputComponent->BindAction(IA_TriggerTPItem,ETriggerEvent::Triggered,this,&ThisClass::PlayerTriggerTPItem);
		PlayerInputComponent->BindAction(IA_ThrowTPItem,ETriggerEvent::Triggered,this,&ThisClass::PlayerThrowTPItem);
	}
}

void UTPItemInteractionComponent_Player::BeginPlay()
{
	Super::BeginPlay();
	if (ACharacter* PlayerCharacter=Cast<ACharacter>(GetOwner()))
	{
		if (APlayerController* PlayerController=Cast<APlayerController>(PlayerCharacter->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem=ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				Subsystem->AddMappingContext(IMC_TPItem,1);
			}
		}
	}
}

void UTPItemInteractionComponent_Player::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (GetOwner()&&GetOwner()->HasLocalNetOwner())
	{
		CheckTPItemAroundPlayer();
	}
}

void UTPItemInteractionComponent_Player::PlayerTriggerTPItem(const FInputActionValue& Value)
{
}

void UTPItemInteractionComponent_Player::PlayerThrowTPItem(const FInputActionValue& Value)
{
}

void UTPItemInteractionComponent_Player::CheckTPItemAroundPlayer()
{
	if (!CanStartInteraction())
	{
		SelectingItem=nullptr;
		return;
	}
	UWorld* WorldPtr=GetWorld();
	APlayerController* PlayerControllerPtr=WorldPtr?WorldPtr->GetFirstPlayerController():nullptr;
	APawn* PlayerCharacterPtr=PlayerControllerPtr?PlayerControllerPtr->GetPawn():nullptr;

	NearbyItems.Empty();
	SelectingItem=nullptr;

	if (WorldPtr&&PlayerControllerPtr&&PlayerCharacterPtr)
	{
		FVector PlayerLocation=PlayerCharacterPtr->GetActorLocation();
		TArray<TEnumAsByte<EObjectTypeQuery>>ObjectTypes{UEngineTypes::ConvertToObjectType(CheckCollisionType)};
		TArray<AActor*>ActorsToIgnore{PlayerCharacterPtr,InteractingItem.Get()};

		TArray<FHitResult>HitResults;

		double MinItemAngle=360.f;
		if (UKismetSystemLibrary::SphereTraceMultiForObjects
			(WorldPtr,
			PlayerLocation+SphereCenterOffset,
			PlayerLocation+SphereCenterOffset,
			SelectItemRange,
			ObjectTypes,
			false,
			ActorsToIgnore,
			DrawDebugTraceMode,
			HitResults,
			true,
			FLinearColor::Red,
			FLinearColor::Green,
			2.f))
		{
			for (auto& HitResult:HitResults)
			{
				if (ATPItemActorBase* HitItemActor=Cast<ATPItemActorBase>(HitResult.GetActor()))
				{
					{
						NearbyItems.Emplace(HitItemActor);

						FVector OutLocation;
						FRotator OutRotation;
						PlayerControllerPtr->GetPlayerViewPoint(OutLocation,OutRotation);

						double DotValue=FVector::DotProduct(
							(HitResult.GetActor()->GetActorLocation()-OutLocation).GetSafeNormal(),
							OutRotation.Vector().GetSafeNormal());

						double TmpItemAngle=FMath::RadiansToDegrees(FMath::Acos(DotValue));
						if (TmpItemAngle<MinItemAngle)
						{
							MinItemAngle=TmpItemAngle;
							InteractingItem=HitItemActor;
						}
					}
				}
			}
		}
	}
}
UE_ENABLE_OPTIMIZATION