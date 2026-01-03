#include "EnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Navigation/PathFollowingComponent.h"

AEnemyAIController::AEnemyAIController()
{
    PrimaryActorTick.bCanEverTick = false; 
}

void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();
    PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

    if (!GetWorld())
    {
        return;
    }

    ChasePlayer();

    // Keep updating forever
    GetWorld()->GetTimerManager().SetTimer(
        ChaseTimerHandle,
        this,
        &AEnemyAIController::ChasePlayer,
        RepathInterval,
        true
    );
}

void AEnemyAIController::ChasePlayer()
{
    if (!PlayerPawn)
    {
        PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
        if (!PlayerPawn) return;
    }

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    const FVector PlayerLocation = PlayerPawn->GetActorLocation();
    const FVector EnemyLocation = ControlledPawn->GetActorLocation();

    if (FVector::DistSquared(EnemyLocation, PlayerLocation) <= FMath::Square(AcceptanceRadius))
    {
        StopMovement();
        return;
    }

    // Only skip repathing if:
    // 1. Player hasn't moved much AND
    // 2. We're still actively moving (haven't reached the old goal)
    const bool bPlayerMovedSignificantly = FVector::DistSquared(PlayerLocation, LastGoalLocation) >= FMath::Square(RepathDistance);

    UPathFollowingComponent* PathFollowingComp = GetPathFollowingComponent();
    const bool bStillMoving = PathFollowingComp && PathFollowingComp->GetStatus() == EPathFollowingStatus::Moving;

    if (!bPlayerMovedSignificantly && bStillMoving)
    {
        return;
    }

    LastGoalLocation = PlayerLocation;

    // Key settings:
    // bUsePathfinding = true (uses navmesh + avoids obstacles)
    // bProjectGoalToNavigation = true (helps when player isn't exactly on nav)
    MoveToActor(
        PlayerPawn,
        AcceptanceRadius,
        true,   // bStopOnOverlap
        true,   // bUsePathfinding
        true,   // bProjectGoalToNavigation
        nullptr,
        true    // bCanStrafe
    );
}