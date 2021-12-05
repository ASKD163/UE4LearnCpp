// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyProject2Character.h"
#include "MyProject2Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PawnNoiseEmitterComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AMyProject2Character

AMyProject2Character::AMyProject2Character()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
	PawnNoiseEmitter = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("PawnNoiseEmitter"));

	killed = 0;
	Ammo = 30;

	Stamina = 1.0f;
	Health = 1.0f;
	
	IsSprint = false;
	//Target = 0;

	EnemyInGame = 0;
	IsLoos = false;

	SaveSlot = "Save";
}

void AMyProject2Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, 0))
		SaveGame = Cast<UMySaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot,0));
	else
	{
		SaveGame = Cast<UMySaveGame>(UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass()));
		//SaveGame = UGameplayStatics::CreateSaveGameObject(SaveGameClass);
		if (SaveGame) SaveGame->CurrentRound = 1;
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlot, 0);
	}
	if (SaveGame) Target = SaveGame->CurrentRound * 2;

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
	
	if (WidgetClass)
	{
		UI = CreateWidget<UMyUserWidget>(GetWorld(), WidgetClass);
		if (UI)
		{
			UI->AddToViewport(0);
			SetKilledText(killed);
			SetAmmoText(Ammo);
			SetHealthBarPer(Health);
			SetStaminaBarPer(Stamina);
			UI->TargetText->SetText(FText::FromString(FString::FromInt(Target)));
			
		}
	}

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);

}

void AMyProject2Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FLatentActionInfo LatentInfo;
	LatentInfo.Linkage = 1;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = "StaminaAdd";
	LatentInfo.UUID = __LINE__;
	if (!IsSprint) UKismetSystemLibrary::Delay(this,0.01f, LatentInfo);
}
//////////////////////////////////////////////////////////////////////////
// Input

void AMyProject2Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyProject2Character::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMyProject2Character::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AMyProject2Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMyProject2Character::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMyProject2Character::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMyProject2Character::LookUpAtRate);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyProject2Character::SprintBegin);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyProject2Character::SprintEnd);

	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &AMyProject2Character::PauseCallBack);
}

float AMyProject2Character::TakeDamage(float Damage, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	SetHealthBarPer(Health - Damage);
	if (Health == 0)
	{
		IsLoos = true;
		EndGame();
	}
	return Health;
}

void AMyProject2Character::OnFire()
{
	if (Ammo <= 0) return;

	SetAmmoText(--Ammo);
	PawnNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AMyProject2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<AMyProject2Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), .5f);
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void AMyProject2Character::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMyProject2Character::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AMyProject2Character::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

void AMyProject2Character::SetKilledText(int num)
{
	killed = num;
	if (UI->KilledText) UI->KilledText->SetText(FText::FromString(FString::FromInt(num)));
}

void AMyProject2Character::SetAmmoText(int num)
{
	Ammo = num;
	if (UI->AmmoText) UI->AmmoText->SetText(FText::FromString(FString::FromInt(num)));
}

void AMyProject2Character::SetHealthBarPer(float num)
{
	Health = (num > 0) ? num : 0.f;
	if (UI && UI->HealthBar) UI->HealthBar->SetPercent(num);
}

void AMyProject2Character::SetStaminaBarPer(float num)
{
	if (UI && UI->StaminaBar) UI->StaminaBar->SetPercent(num);
}

int AMyProject2Character::GetKilled()
{
	return killed;
}

int AMyProject2Character::GetAmmo()
{
	return Ammo;
}

int AMyProject2Character::GetTarget()
{
	return Target;
}

void AMyProject2Character::EndGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	if (IsLoos)
	{
		if (LoosUIWidget)
		{
			LoosUI = CreateWidget<ULoosWidget>(GetWorld(), LoosUIWidget);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				FInputModeUIOnly InData;
				PlayerController->SetInputMode(InData);
			}

			if (LoosUI) LoosUI->AddToViewport(0);
			if (LoosUI && LoosUI->RePlayButton)
			{
				FScriptDelegate InDelegate;
				InDelegate.BindUFunction(this, "RePlay");
				LoosUI->RePlayButton->OnPressed.Add(InDelegate);
			}
			if (LoosUI && LoosUI->ExitButton)
			{
				FScriptDelegate InDelegate;
				InDelegate.BindUFunction(this, "Exit");
				LoosUI->ExitButton->OnPressed.Add(InDelegate);
			}
		}
	}
	else
	{
		if (SaveGame) SaveGame->CurrentRound++;
		UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlot, 0);
		if (WinUIWidget)
		{
			WinUI = CreateWidget<UWinWidget>(GetWorld(), WinUIWidget);
			if (PlayerController)
			{
				PlayerController->bShowMouseCursor = true;
				FInputModeUIOnly InData;
				PlayerController->SetInputMode(InData);
			}
			if (WinUI) WinUI->AddToViewport(0);
			if (WinUI && WinUI->PlayButton)
			{
				FScriptDelegate InDelegate;
				InDelegate.BindUFunction(this, "RePlay");
				WinUI->PlayButton->OnPressed.Add(InDelegate);
			}
			if (WinUI && WinUI->ExitButton)
			{
				FScriptDelegate InDelegate;
				InDelegate.BindUFunction(this, "Exit");
				WinUI->ExitButton->OnPressed.Add(InDelegate);
			}
			if (WinUI && WinUI->RoundText)
			{
				FString A = FString::Printf(TEXT("ROUND. %d"), SaveGame->CurrentRound);
				WinUI->RoundText->SetText(FText::FromString(A));
			}
		}
	}
}

void AMyProject2Character::RePlay()
{
	UGameplayStatics::OpenLevel(this, "FirstPersonExampleMap");
	if (WinUI) WinUI->RemoveFromParent();
	else LoosUI->RemoveFromParent();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InData;
		PlayerController->SetInputMode(InData);
	}
}

void AMyProject2Character::PlayFromBegin()
{
	//
	SaveGame->CurrentRound = 1;
	UGameplayStatics::SaveGameToSlot(SaveGame, SaveSlot, 0);
	UGameplayStatics::OpenLevel(GetWorld(), "FirstPersonExampleMap");
	if (PauseUI) PauseUI->RemoveFromParent();
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly InData;
		PlayerController->SetInputMode(InData);
	}
}

void AMyProject2Character::Exit()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, false);
}

void AMyProject2Character::PauseCallBack()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	PauseUI = CreateWidget<UPauseWidget>(GetWorld(), PauseUIWidget);
	PauseUI->AddToViewport(0);
	//Mouse Input
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		FInputModeUIOnly UIOnly;
		PlayerController->SetInputMode(UIOnly);
	}
	//Bind Button
	if (PauseUI && PauseUI->PlayButton)
	{
		FScriptDelegate InDelegate1;
		InDelegate1.BindUFunction(this, "Continue");
		PauseUI->PlayButton->OnPressed.Add(InDelegate1);
	}

	if (PauseUI && PauseUI->RePlayButton)
	{
		FScriptDelegate InDelegate2;
		InDelegate2.BindUFunction(this, "PlayFromBegin");
		PauseUI->RePlayButton->OnPressed.Add(InDelegate2);
	}

	if (PauseUI && PauseUI->ExitButton)
	{
		FScriptDelegate InDelegate3;
		InDelegate3.BindUFunction(this, "Exit");
		PauseUI->ExitButton->OnPressed.Add(InDelegate3);
	}
}

void AMyProject2Character::Continue()
{
	UGameplayStatics::SetGamePaused(this, false);
	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		FInputModeGameOnly GameOnly;
		PlayerController->SetInputMode(GameOnly);
	}
	PauseUI->RemoveFromParent();
}

void AMyProject2Character::SprintBegin()
{
	if (Stamina <= 0) SprintEnd();
	IsSprint = true;
	GetCharacterMovement()-> MaxWalkSpeed = 3000.0f;
	PawnNoiseEmitter->MakeNoise(this, 1.0f, GetActorLocation());
	GetWorldTimerManager().SetTimer(InOutHandle,this, &AMyProject2Character::StaminaDrain, 0.1f, true);
}

void AMyProject2Character::SprintEnd()
{
	GetCharacterMovement()-> MaxWalkSpeed = 600.0f;
	IsSprint = false;
	GetWorldTimerManager().ClearTimer(InOutHandle);
}

void AMyProject2Character::StaminaDrain()
{
	Stamina = Stamina >=0 ? Stamina - 0.1f : 0.0f;
	SetStaminaBarPer(Stamina);
	if (Stamina == 0) SprintEnd();
}

void AMyProject2Character::StaminaAdd()
{
	Stamina = Stamina <= 1.0f ? Stamina + 0.1f : 1.0f;
	SetStaminaBarPer(Stamina);
}

void AMyProject2Character::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMyProject2Character::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMyProject2Character::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMyProject2Character::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AMyProject2Character::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AMyProject2Character::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &AMyProject2Character::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AMyProject2Character::TouchUpdate);
		return true;
	}
	
	return false;
}
