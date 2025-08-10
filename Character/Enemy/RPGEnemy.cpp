#include "RPGEnemy.h"
#include "AIController.h"
#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAttributeSet.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Message/RPGGameMessage.h"
#include "UI/Common/RPGProgressBar.h"
#include "UI/ViewModel/MVVM_ProgressBar.h"
#include "View/MVVMView.h"
#include "Data/Enemy/RPGEnemyDatabase.h"
#include "AbilitySystemComponent.h"
#include "MotionWarpingComponent.h"
#include "Abilities/GameplayAbility.h"
#include "DropItem/RPGDropItemActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "ObjectPool/RPGPoolManager.h"
#include "UI/ProgressBar/RPGBossHpBar.h"

ARPGEnemy::ARPGEnemy()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AbilitySystemComponent = CreateDefaultSubobject<URPGAbilitySystemComponent>(TEXT("AbilitySystemComp"));
	AttributeSet = CreateDefaultSubobject<URPGAttributeSet>(TEXT("AttributeSet"));
}

void ARPGEnemy::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AAIController* AIController = Cast<AAIController>(NewController))
	{
		UBlackboardData* BlackboardData = BehaviorTree->GetBlackboardAsset();
		AIController->GetBlackboardComponent()->InitializeBlackboard(*BlackboardData);
	}

	
}

void ARPGEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		BindAttributeDelegate();
		InitAttributes();
	});

	
}

void ARPGEnemy::OnHit(const FGameplayEventData* Payload)
{
	ShowHitVFX();
	
}

void ARPGEnemy::UpdateWarpTargetPosition()
{
	AAIController* AIController = Cast<AAIController>(GetController());
	if (AIController == nullptr)
		return;

	UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent();
	if (Blackboard == nullptr)
		return;
	
	if (AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(FName("TargetActor"))))
	{
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocation(FName("FacingTarget"), TargetActor->GetActorLocation());
	}
}

void ARPGEnemy::Die()
{
	Super::Die();
	
	SetLifeSpan(LifeSpan);

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->BrainComponent->StopLogic(TEXT("Die"));
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities();
	}
	

	GiveExpToPlayer();
	DropItems();
	BroadcastKillEnemyMsg();
}


void ARPGEnemy::InitAttributes() 
{
	UCharacterMovementComponent* CharacterMovementComponent = GetCharacterMovement();
	if (AbilitySystemComponent == nullptr || CharacterMovementComponent == nullptr || EnemyId == NAME_None || InitAttributesEffect == nullptr)
		return;

	const FGameplayEffectContextHandle EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	const FGameplayEffectSpecHandle InitAttributeSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(InitAttributesEffect, 0.f, EffectContextHandle);
	if (FGameplayEffectSpec* Spec = InitAttributeSpecHandle.Data.Get())
	{
		if (const FRPGEnemyDataRow* EnemyDataRow = RPGHelper::GetEnemyDataRow_Safe(this, EnemyId))
		{
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attribute.MaxHP.Base")), EnemyDataRow->Hp);
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attribute.HP")), EnemyDataRow->Hp);
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attribute.Attack.Base")), EnemyDataRow->Attack);
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attribute.Defense.Base")), EnemyDataRow->Defense);
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Attribute.MoveSpeed.Base")), EnemyDataRow->MoveSpeed);
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*InitAttributeSpecHandle.Data.Get());

			for (auto& Ability : EnemyDataRow->GrantAbilities)
			{
				AbilitySystemComponent->K2_GiveAbility(Ability, 1);
			}
		}
	}

	AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(FGameplayTag::RequestGameplayTag(FName("GameEvent.OnHit"))).AddUObject(this, &ThisClass::OnHit);
}

void ARPGEnemy::BindAttributeDelegate()
{
	UUserWidget* HpBarWidget = GetHpBarWidget();
	BindHpBarViewModel(HpBarWidget);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHPAttribute()).AddUObject(this, &ThisClass::HandleHpChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHPAttribute()).AddUObject(this, &ThisClass::HandleHpChanged);
}

UUserWidget* ARPGEnemy::GetHpBarWidget() const
{
	return nullptr;
}


void ARPGEnemy::BroadcastKillEnemyMsg()
{
	FQuestTaskSignalMsg QuestSignalMsg;
	QuestSignalMsg.TaskActionType = ERPGQuestTaskActionType::KillEnemy;
	QuestSignalMsg.TargetId = EnemyId;
	QuestSignalMsg.Count = 1;

	RPGMessage::BroadcastMessage(this, QuestSignalMsg, FGameplayTag::RequestGameplayTag(FName("Quest.Notify.TaskSignal")));
}

void ARPGEnemy::DropItems() const
{
	const FRPGEnemyDataRow* EnemyDataRow = RPGHelper::GetEnemyDataRow_Safe(this, EnemyId);
	if (EnemyDataRow == nullptr)
		return;

	auto* PoolManager = RPGHelper::GetPoolManager(this);
	if (PoolManager == nullptr)
		return;
	
	for (const FRPGDropItemInfo& DropItemInfo : EnemyDataRow->DropItems)
	{
		if (const bool IsDrop = FMath::RandRange(0.f, 1.f) < DropItemInfo.DropRate)
		{
			if (ARPGDropItemActor* DropItemActor = Cast<ARPGDropItemActor>(PoolManager->GetPoolObject("DropItemActor")))
			{
				const int32 DropCount = FMath::RandRange(DropItemInfo.MinQuantity, DropItemInfo.MaxQuantity);
				DropItemActor->Init(DropItemInfo.ItemId, DropCount);

				const FVector DropLocation = GetActorLocation() + UKismetMathLibrary::RandomUnitVector() * FMath::RandRange(0.f, DropRadius);
				DropItemActor->SetActorLocation(DropLocation);
			}
		}
	}
	
	
}

void ARPGEnemy::GiveExpToPlayer() const
{
	const FRPGEnemyDataRow* EnemyDataRow = RPGHelper::GetEnemyDataRow_Safe(this, EnemyId);
	if (EnemyDataRow == nullptr)
		return;

	if (ARPGPlayerState* PS = RPGHelper::GetPlayerState_Safe(this))
	{
		PS->GiveExp(EnemyDataRow->Xp);
	}
	
}

void ARPGEnemy::BindHpBarViewModel(UUserWidget* HpBarWidget) const
{
	if (HpBarWidget == nullptr)
		return;
	
	if (const UMVVMView* View = Cast<UMVVMView>(HpBarWidget->GetExtension(UMVVMView::StaticClass())))
	{
		if (const TScriptInterface<INotifyFieldValueChanged> ViewModelInterface = View->GetViewModel(FName("MVVMProgressBar")))
		{
			if (UMVVM_ProgressBar* VM = Cast<UMVVM_ProgressBar>(ViewModelInterface.GetObject()))
			{
				VM->SetMaxValue(AttributeSet->GetMaxHP());
				VM->SetCurrentValue(AttributeSet->GetHP());
    
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHPAttribute()).AddLambda(
					[this, VM](const FOnAttributeChangeData& Data)
				{
					if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
						VM->SetCurrentValue(Data.NewValue);
		}
		);
	
				AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHPAttribute()).AddLambda(
					[this, VM](const FOnAttributeChangeData& Data)
					{
						if (!FMath::IsNearlyEqual(Data.OldValue, Data.NewValue))
							VM->SetMaxValue(Data.NewValue);
					}
				);
			}
		}
	}
}

void ARPGEnemy::HandleHpChanged(const FOnAttributeChangeData& Data)
{
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
		{
			float MaxHP = AttributeSet->GetMaxHP();
			float HP = AttributeSet->GetHP();
			if (MaxHP > 0.f)
			{
				RPGHelper::SetBlackboardValueAsFloat(this, FName("HpRatio"), AttributeSet->GetHP() / AttributeSet->GetMaxHP());
			}
		}
	}
}

void ARPGEnemy::StartBehaviorTree()
{
	if (BehaviorTree == nullptr)
		return;
	
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->RunBehaviorTree(BehaviorTree);
	}
}


