#include "MyPawn.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Misc/MapErrors.h"

AMyPawn::AMyPawn()
{
	PrimaryActorTick.bCanEverTick = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);
	
	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	MeshComponent->SetupAttachment(CapsuleComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(CapsuleComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void AMyPawn::BeginPlay()
{
	Super::BeginPlay();
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(InputMappingContext,0);
		}
	}
	
}

void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	auto InputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputComp)
	{
		InputComp->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AMyPawn::Move);
		InputComp->BindAction(LookAction,ETriggerEvent::Triggered,this,&AMyPawn::LookAround);
	}
}

void AMyPawn::Move(const FInputActionValue& Input2D)
{
	// 입력값 복사 및 노말라이즈.
	FVector2D InputVector = Input2D.Get<FVector2D>();
	if (InputVector.Length()>1.0)
	{
		InputVector.Normalize();
	}

	// 카메라의 Yaw만 반영한 회전.
	FRotator CameraRot = CameraComponent->GetComponentRotation();
	CameraRot.Pitch = 0.0f;
	CameraRot.Roll = 0.0f;

	// 카메라의 방향 벡터.
	FVector CameraRightVector = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::Y);
	FVector CameraForwardVector = FRotationMatrix(CameraRot).GetUnitAxis(EAxis::X);

	// 입력을 통해 이동해야 할 방향.
	FVector MoveVector = CameraRightVector*InputVector.X + CameraForwardVector*InputVector.Y;

	// 위 방향으로 이동.
	AddActorWorldOffset(MoveVector*GetWorld()->GetDeltaSeconds()*MoveSpeed,true);

	// 이동 방향으로 캐릭터 회전.
	auto InterpRot =  FMath::RInterpConstantTo(GetActorRotation(),MoveVector.Rotation(),GetWorld()->GetDeltaSeconds(),1080.0f);
	SetActorRotation(InterpRot);
}

void AMyPawn::LookAround(const FInputActionValue& Input2D)
{
	// 입력값 복사.
	FVector2D InputVector = Input2D.Get<FVector2D>();

	// 카메라 회전
	SpringArmComponent->AddRelativeRotation(FRotator(InputVector.Y*-1.0f,InputVector.X,0.0f));

	// 카메라 피치회전 제한.
	auto Pitch = FMath::Clamp( SpringArmComponent->GetRelativeRotation().Pitch, -75.0f, 75.0f );
	auto Yaw = SpringArmComponent->GetRelativeRotation().Yaw;
	SpringArmComponent->SetRelativeRotation(FRotator(Pitch,Yaw,0.0f));
}

