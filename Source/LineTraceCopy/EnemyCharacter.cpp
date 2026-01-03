#include "EnemyCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Engine/Engine.h"

AEnemyCharacter::AEnemyCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    CurrentHealth = MaxHealth;

    // Set movement speeds
    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}

void AEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    CurrentHealth = MaxHealth;

    if (GetCharacterMovement())
    {
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
    }
}

void AEnemyCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    const FVector V = GetVelocity();
    const float Speed2D = FVector(V.X, V.Y, 0.f).Size();

    USkeletalMeshComponent* MeshComp = GetMesh();
    FString AnimInfo = TEXT("No AnimBP");

    if (MeshComp && MeshComp->GetAnimInstance())
    {
        UAnimInstance* AnimInst = MeshComp->GetAnimInstance();

        float GroundSpeed = 0.f;
        FProperty* SpeedProp = AnimInst->GetClass()->FindPropertyByName(FName("GroundSpeed"));
        if (!SpeedProp)
        {
            SpeedProp = AnimInst->GetClass()->FindPropertyByName(FName("Speed"));
        }

        if (SpeedProp)
        {
            float* SpeedPtr = SpeedProp->ContainerPtrToValuePtr<float>(AnimInst);
            if (SpeedPtr)
            {
                GroundSpeed = *SpeedPtr;
            }
        }

        AnimInfo = FString::Printf(TEXT("ABP GroundSpeed: %.1f"), GroundSpeed);
    }
}

// Damage handling not currently used in this prototype, but implemented for completeness and potential future use
void AEnemyCharacter::TakeEnemyDamage(float DamageAmount)
{
    CurrentHealth -= DamageAmount;
    CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

    if (IsDead())
    {
        Destroy();
    }
}

bool AEnemyCharacter::IsDead() const
{
    return CurrentHealth <= 0.0f;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}