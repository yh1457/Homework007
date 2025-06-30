#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCapsuleComponent;
class USKeletalMeshComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
UCLASS()
class HOMEWORK007_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	AMyPawn();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Input2D);
	UFUNCTION()
	void LookAround(const FInputActionValue& Input2D);
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",meta=(AllowPrivateAccess="true"))
	UCapsuleComponent* CapsuleComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* MeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",meta=(AllowPrivateAccess="true"))
	USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components",meta=(AllowPrivateAccess="true"))
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess="true"))
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess="true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess="true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess="true"))
	float MoveSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input",meta=(AllowPrivateAccess="true"))
	float LookRotateSpeed;
	
};
