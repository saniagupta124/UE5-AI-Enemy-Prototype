#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

UCLASS()
class LINETRACECOPY_API AEnemyAIController : public AAIController
{
    GENERATED_BODY()

public:
    AEnemyAIController();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY()
    APawn* PlayerPawn = nullptr;

    FTimerHandle ChaseTimerHandle;

    UPROPERTY(EditDefaultsOnly, Category = "Chase")
    float AcceptanceRadius = 0.0f;

    UPROPERTY(EditDefaultsOnly, Category = "Chase")
    float RepathInterval = 0.2f; 

    UPROPERTY(EditDefaultsOnly, Category = "Chase")
    float RepathDistance = 10.f; 

    FVector LastGoalLocation = FVector::ZeroVector;

    void ChasePlayer();
};