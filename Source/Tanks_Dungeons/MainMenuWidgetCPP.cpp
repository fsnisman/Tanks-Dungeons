#include "MainMenuWidgetCPP.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TankPlayerController.h"

void UMainMenuWidgetCPP::NativeConstruct()
{
	Super::NativeConstruct();

	PlayAnimation(Animation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f, false);

	if (StartGame)
	{
		StartGame->OnClicked.AddDynamic(this, &UMainMenuWidgetCPP::OnButtonStartGameClicked);
	}

	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &UMainMenuWidgetCPP::OnButtonQuitGameClicked);
	}
}

void UMainMenuWidgetCPP::OnButtonStartGameClicked()
{
	UGameplayStatics::OpenLevel(this, "Level1");
	UWidgetLayoutLibrary::RemoveAllWidgets(this);
}

void UMainMenuWidgetCPP::OnButtonQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(this, nullptr, EQuitPreference::Quit, bIgnorePlatformRestrictions);
}
