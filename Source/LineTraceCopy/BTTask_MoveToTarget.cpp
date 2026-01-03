#include "BTTask_MoveToTarget.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
    NodeName = "Move To Target";
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController)
        return EBTNodeResult::Failed;

    UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
    if (!BlackboardComp)
        return EBTNodeResult::Failed;

    // Get target location from blackboard
    FVector TargetLocation = BlackboardComp->GetValueAsVector(BlackboardKey.SelectedKeyName);

    // Project to NavMesh
    FNavLocation NavLocation;
    if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
    {
        if (!NavSys->ProjectPointToNavigation(TargetLocation, NavLocation))
        {
            return EBTNodeResult::Failed;
        }
    }

    // Move AI - this returns EPathFollowingRequestResult::Type in UE 5.6
    EPathFollowingRequestResult::Type MoveResult = AIController->MoveToLocation(NavLocation.Location);

    // Check result
    switch (MoveResult)
    {
    case EPathFollowingRequestResult::AlreadyAtGoal:
        return EBTNodeResult::Succeeded;

    case EPathFollowingRequestResult::RequestSuccessful:
        return EBTNodeResult::InProgress;

    default:
        return EBTNodeResult::Failed;
    }
}
