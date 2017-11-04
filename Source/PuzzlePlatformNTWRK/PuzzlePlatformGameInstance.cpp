// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzlePlatformGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "PlatformTrigger.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/NetworkMenuSystem/WBP_MainMenu"));

	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;


}

void UPuzzlePlatformGameInstance::Init()
{
	if (!ensure(MenuClass != nullptr)) return;

	UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MenuClass->GetName());

}

void UPuzzlePlatformGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* world = GetWorld();

	if (!ensure(world != nullptr)) return;

	world->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UPuzzlePlatformGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* Player = GetFirstLocalPlayerController();

	if (!ensure(Player != nullptr)) return;

	Player->ClientTravel(Address,ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if (!ensure(MenuClass != nullptr)) return;

	UUserWidget* menu = CreateWidget<UUserWidget>(this, MenuClass);
	if (!ensure(menu != nullptr)) return;

	menu->AddToViewport();

	APlayerController* Player = GetFirstLocalPlayerController();
	if (!ensure(Player != nullptr)) return;

	FInputModeUIOnly InputModeData;

	InputModeData.SetWidgetToFocus(menu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	Player->SetInputMode(InputModeData);

	Player->bShowMouseCursor = true;

}
