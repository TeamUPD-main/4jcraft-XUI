#include "../../../Minecraft.World/Platform/stdafx.h"

#include "../Common/Consoles_App.h"
#include "../../Player/User.h"
#include "../../Minecraft.h"
#include "../../MinecraftServer.h"
#include "../../Network/PlayerList.h"
#include "../../Player/ServerPlayer.h"
#include "../../../Minecraft.World/Level/Level.h"
#include "../../../Minecraft.World/Level/Storage/LevelSettings.h"
#include "../../../Minecraft.World/WorldGen/Biomes/BiomeSource.h"
#include "../../../Minecraft.World/Level/Storage/LevelType.h"

#include "../Common/UI/UIStructs.h"

#include "../Common/XUI/XUI_Intro.h"
#include "../Common/XUI/XUI_MainMenu.h"
#include "../Common/XUI/XUI_NewUpdateMessage.h"
#include "../Common/XUI/XUI_HelpAndOptions.h"
#include "../Common/XUI/XUI_TextEntry.h"
#include "../Common/XUI/XUI_HelpHowToPlay.h"
#include "../Common/XUI/XUI_HowToPlayMenu.h"
#include "../Common/XUI/XUI_HelpControls.h"
#include "../Common/XUI/XUI_LoadSettings.h"
#include "../Common/XUI/XUI_SettingsAll.h"
#include "../Common/XUI/XUI_SettingsOptions.h"
#include "../Common/XUI/XUI_SettingsAudio.h"
#include "../Common/XUI/XUI_SettingsControl.h"
#include "../Common/XUI/XUI_SettingsGraphics.h"
#include "../Common/XUI/XUI_SettingsUI.h"
#include "../Common/XUI/XUI_Leaderboards.h"
#include "../Common/XUI/XUI_Debug.h"
#include "../Common/XUI/XUI_DebugTips.h"
#include "../Common/XUI/XUI_Reinstall.h"
#include "../Common/XUI/XUI_Death.h"
#include "../Common/XUI/XUI_DebugOverlay.h"
#include "../Common/XUI/XUI_DLCOffers.h"
#include "../Common/XUI/XUI_SocialPost.h"
#include "../Common/XUI/XUI_Scene_Container.h"
#include "../Common/XUI/XUI_Scene_Furnace.h"
#include "../Common/XUI/XUI_Scene_Inventory.h"
#include "../Common/XUI/XUI_Scene_Trap.h"
#include "../Common/XUI/XUI_Control_ComboBox.h"
#include "../Common/XUI/XUI_MultiGameInfo.h"
#include "../Common/XUI/XUI_MultiGameJoinLoad.h"
#include "../Common/XUI/XUI_MultiGameCreate.h"
#include "../Common/XUI/XUI_MultiGameLaunchMoreOptions.h"
#include "../Common/XUI/XUI_SignEntry.h"
#include "../Common/XUI/XUI_InGameInfo.h"
#include "../Common/XUI/XUI_TrialExitUpsell.h"
#include "../Common/XUI/XUI_BasePlayer.h"
#include "../Common/XUI/XUI_PartnernetPassword.h"
#include "../Common/XUI/XUI_SaveMessage.h"
#include "../Common/XUI/XUI_Chat.h"
#include "../Common/XUI/XUI_SkinSelect.h"
#include "../Common/XUI/XUI_InGameHostOptions.h"
#include "../Common/XUI/XUI_InGamePlayerOptions.h"
#include "../Common/XUI/XUI_Scene_Enchant.h"
#include "../Common/XUI/XUI_Scene_BrewingStand.h"
#include "../Common/XUI/XUI_XZP_Icons.h"
#include "../Common/XUI/XUI_Controls.h"
#include "../Common/XUI/XUI_Scene_Win.h"
#include "../Common/XUI/XUI_HUD.h"
#include "../Common/XUI/XUI_DebugSetCamera.h"
#include "../Common/XUI/XUI_Scene_Anvil.h"
#include "../Common/XUI/XUI_Scene_Trading.h"
#include "../Common/XUI/XUI_Teleport.h"

#include "../../GameState/Options.h"
#include "Linux_App.h"

CConsoleMinecraftApp app;

#define CONTEXT_GAME_STATE 0

WCHAR* CConsoleMinecraftApp::wchSceneA[] =
{
    L"xuiscene_partnernetpassword",
    L"xuiscene_intro",
    L"xuiscene_savemessage",
    L"xuiscene_main",
    L"xuiscene_fullscreenprogress",
    L"xuiscene_pause",
    L"xuiscene_craftingpanel_2x2",
    L"xuiscene_craftingpanel_3x3",
    L"xuiscene_furnace",
    L"xuiscene_container",
    L"xuiscene_container_large",
    L"xuiscene_inventory",
    L"xuiscene_trap",
    L"xuiscene_debug",
    L"xuiScene_DebugTips",
    L"xuiscene_helpandoptions",
    L"xuiscene_howtoplay",
    L"xuiscene_howtoplay_menu",
    L"xuiscene_controls",
    L"xuiscene_settings_options",
    L"xuiscene_settings_audio",
    L"xuiscene_settings_control",
    L"xuiscene_settings_graphics",
    L"xuiscene_settings_UI",
    L"xuiscene_settings_all",
    L"xuiscene_leaderboards",
    L"xuiscene_credits",
    L"xuiscene_death",
    L"xuiscene_tutorialpopup",
    L"xuiscene_multi_create",
    L"xuiscene_multi_joinload",
    L"xuiscene_multi_gameinfo",
    L"xuiscene_signentry",
    L"xuiscene_ingameinfo",
    L"xuiscene_connectingprogress",
    L"xuiscene_DLCOffers",
    L"xuiscene_socialpost",
    L"xuiscene_trialexitupsell",
    L"xuiscene_load_settings",
    L"xuiscene_chat",
    L"xuiscene_reinstall",
    L"xuiscene_skinselect",
    L"xuiscene_text_entry",
    L"xuiscene_ingame_host_options",
    L"xuiscene_ingame_player_options",
    L"xuiscene_inventory_creative",
    L"xuiscene_multi_launch_more_options",
    L"xuiscene_DLCMain",
    L"xuiscene_NewUpdateMessage",
    L"xuiscene_enchant",
    L"xuiscene_brewingstand",
    L"xuiscene_win",
    L"xuiscene_hud",
    L"xuiscene_trading",
    L"xuiscene_anvil",
    L"xuiscene_teleportmenu"
};

CConsoleMinecraftApp::CConsoleMinecraftApp()
    : CMinecraftApp(),
      m_fontRenderer(NULL),
      m_bFontRendererOverridden(false),
      m_bOverrideFontRenderer(false),
      m_hObjRoot(NULL)
{
    for (int i = 0; i < XUSER_MAX_COUNT; ++i)
    {
        m_bMenuDisplayed[i] = false;
        m_bMenuToBeClosed[i] = false;
        m_bPauseMenuDisplayed[i] = false;
        m_bContainerMenuDisplayed[i] = false;
        m_bIgnoreAutosaveMenuDisplayed[i] = false;
        m_bIgnorePlayerJoinMenuDisplayed[i] = false;
        m_iCountDown[i] = 0;

        m_PlayerBaseScene[i] = NULL;
        m_hFirstScene[i] = NULL;
        m_hCurrentScene[i] = NULL;
        m_hFirstTutorialScene[i] = NULL;
        m_hCurrentTutorialScene[i] = NULL;
        m_hFirstChatScene[i] = NULL;
        m_hCurrentChatScene[i] = NULL;
        m_hFirstHudScene[i] = NULL;
        m_hCurrentHudScene[i] = NULL;
    }

    m_SceneName[0] = 0;
}

void CConsoleMinecraftApp::SetRichPresenceContext(int iPad, int contextId)
{
    ProfileManager.SetRichPresenceContextValue(iPad, CONTEXT_GAME_STATE, contextId);
}

HRESULT CConsoleMinecraftApp::RegisterXuiClasses()
{
    HRESULT hr = S_OK;

    hr = CScene_Intro::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Main::Register(); if (FAILED(hr)) return hr;
    hr = CScene_NewUpdateMessage::Register(); if (FAILED(hr)) return hr;
    hr = CScene_HelpAndOptions::Register(); if (FAILED(hr)) return hr;
    hr = CScene_HowToPlay::Register(); if (FAILED(hr)) return hr;
    hr = CScene_HowToPlayMenu::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Controls::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsAll::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsOptions::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsAudio::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsControl::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsGraphics::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SettingsUI::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Leaderboards::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Death::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SaveMessage::Register(); if (FAILED(hr)) return hr;
    hr = CScene_TextEntry::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Chat::Register(); if (FAILED(hr)) return hr;
    hr = CScene_SkinSelect::Register(); if (FAILED(hr)) return hr;
    hr = CScene_InGameHostOptions::Register(); if (FAILED(hr)) return hr;
    hr = CScene_InGamePlayerOptions::Register(); if (FAILED(hr)) return hr;
    hr = CScene_MultiGameCreate::Register(); if (FAILED(hr)) return hr;
    hr = CScene_MultiGameJoinLoad::Register(); if (FAILED(hr)) return hr;
    hr = CScene_MultiGameInfo::Register(); if (FAILED(hr)) return hr;
    hr = CScene_MultiGameLaunchMoreOptions::Register(); if (FAILED(hr)) return hr;
    hr = CScene_TrialExitUpsell::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Win::Register(); if (FAILED(hr)) return hr;
    hr = CScene_Teleport::Register(); if (FAILED(hr)) return hr;

    hr = CXuiSceneBase::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneContainer::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneFurnace::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneInventory::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneTrap::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneHud::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneAnvil::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneTrading::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneEnchant::Register(); if (FAILED(hr)) return hr;
    hr = CXuiSceneBrewingStand::Register(); if (FAILED(hr)) return hr;

    return S_OK;
}

HRESULT CConsoleMinecraftApp::UnregisterXuiClasses()
{
    CXuiSceneBrewingStand::Unregister();
    CXuiSceneEnchant::Unregister();
    CXuiSceneTrading::Unregister();
    CXuiSceneAnvil::Unregister();
    CXuiSceneHud::Unregister();
    CXuiSceneTrap::Unregister();
    CXuiSceneInventory::Unregister();
    CXuiSceneFurnace::Unregister();
    CXuiSceneContainer::Unregister();
    CXuiSceneBase::Unregister();

    CScene_Teleport::Unregister();
    CScene_Win::Unregister();
    CScene_TrialExitUpsell::Unregister();
    CScene_MultiGameLaunchMoreOptions::Unregister();
    CScene_MultiGameInfo::Unregister();
    CScene_MultiGameJoinLoad::Unregister();
    CScene_MultiGameCreate::Unregister();
    CScene_InGamePlayerOptions::Unregister();
    CScene_InGameHostOptions::Unregister();
    CScene_SkinSelect::Unregister();
    CScene_Chat::Unregister();
    CScene_TextEntry::Unregister();
    CScene_SaveMessage::Unregister();
    CScene_Death::Unregister();
    CScene_Leaderboards::Unregister();
    CScene_SettingsUI::Unregister();
    CScene_SettingsGraphics::Unregister();
    CScene_SettingsControl::Unregister();
    CScene_SettingsAudio::Unregister();
    CScene_SettingsOptions::Unregister();
    CScene_SettingsAll::Unregister();
    CScene_Controls::Unregister();
    CScene_HowToPlayMenu::Unregister();
    CScene_HowToPlay::Unregister();
    CScene_HelpAndOptions::Unregister();
    CScene_NewUpdateMessage::Unregister();
    CScene_Main::Unregister();
    CScene_Intro::Unregister();

    return S_OK;
}

HRESULT CConsoleMinecraftApp::LoadXuiResources()
{
    HRESULT hr = LinuxXui::Initialize();
    if (FAILED(hr))
        return hr;

    OverrideFontRenderer(true);

    hr = LoadSkin(L"media/skin_Minecraft.xur");
    if (FAILED(hr))
        return hr;

    m_hObjRoot = LinuxXui::MakeObject(LinuxXui::ObjectKind::Container, L"Root");
    if (!m_hObjRoot)
        return E_FAIL;

    HXUIOBJ mainBaseScene = NULL;

    if (RenderManager.IsHiDef() || RenderManager.IsWidescreen())
        hr = LoadFirstScene(L"media/", L"xuiscene_base.xur", NULL, &mainBaseScene);
    else
        hr = LoadFirstScene(L"media/", L"xuiscene_base_480.xur", NULL, &mainBaseScene);

    if (FAILED(hr))
        return hr;

    LinuxXui::AddChild((LinuxXui::Object*)m_hObjRoot, (LinuxXui::Object*)mainBaseScene);

    XuiElementGetChildById(mainBaseScene, L"BasePlayer0", &m_PlayerBaseScene[0]);
    XuiElementGetChildById(mainBaseScene, L"BasePlayer1", &m_PlayerBaseScene[1]);
    XuiElementGetChildById(mainBaseScene, L"BasePlayer2", &m_PlayerBaseScene[2]);
    XuiElementGetChildById(mainBaseScene, L"BasePlayer3", &m_PlayerBaseScene[3]);

    for (int idx = 0; idx < XUSER_MAX_COUNT; ++idx)
    {
        XuiElementGetChildById(m_PlayerBaseScene[idx], L"XuiSceneContainer", &m_hCurrentScene[idx]);
        m_hFirstScene[idx] = m_hCurrentScene[idx];

        XuiElementGetChildById(m_PlayerBaseScene[idx], L"XuiSceneTutorialContainer", &m_hCurrentTutorialScene[idx]);
        m_hFirstTutorialScene[idx] = m_hCurrentTutorialScene[idx];

        XuiElementGetChildById(m_PlayerBaseScene[idx], L"XuiSceneChatRoot", &m_hCurrentChatScene[idx]);
        m_hFirstChatScene[idx] = m_hCurrentChatScene[idx];

        XuiElementGetChildById(m_PlayerBaseScene[idx], L"XuiSceneHudRoot", &m_hCurrentHudScene[idx]);
        m_hFirstHudScene[idx] = m_hCurrentHudScene[idx];
    }

    HXUIOBJ hPressStartMessage = NULL;
    if (SUCCEEDED(XuiElementGetChildById(mainBaseScene, L"XuiPressStartMessage", &hPressStartMessage)))
    {
        // If GetString exists in your base app, this is nice.
        // Otherwise replace with a literal.
        XuiControlSetText(hPressStartMessage, L"Press Start to Join");
    }

    SetResourcesLoaded(true);
    return S_OK;
}

void CConsoleMinecraftApp::RunFrame()
{
    OverrideFontRenderer(m_bOverrideFontRenderer);
    LinuxXui::RunFrame();
}

void CConsoleMinecraftApp::OverrideFontRenderer(bool set, bool)
{
    if (set != m_bFontRendererOverridden)
    {
        if (set)
            XuiFontSetRenderer(m_fontRenderer);
        else
            XuiFontSetRenderer(NULL);

        m_bFontRendererOverridden = set;
    }

    m_bOverrideFontRenderer = set;
}

WCHAR* CConsoleMinecraftApp::GetSceneName(EUIScene eScene, bool bAppendToName, bool bSplitscreenScene)
{
    wcscpy(m_SceneName, wchSceneA[eScene]);

    if (bAppendToName)
    {
        if (RenderManager.IsHiDef())
        {
            if (bSplitscreenScene)
                wcscat(m_SceneName, L"_small");
        }
        else
        {
            if (!RenderManager.IsWidescreen())
                wcscat(m_SceneName, L"_480");
        }
    }

    wcscat(m_SceneName, L".xur");
    return m_SceneName;
}

HRESULT CConsoleMinecraftApp::NavigateToScene(int iPad, EUIScene eScene, void* initData, bool forceUsePad, BOOL bStayVisible, HXUIOBJ* phResultingScene)
{
    ASSERT(m_hObjRoot);
    if (!m_hObjRoot)
        return E_UNEXPECTED;

    if (initData == NULL)
        initData = &iPad;

    int idx = 0;
    if (m_bGameStarted || forceUsePad)
    {
        if ((iPad != 255) && (iPad >= 0))
            idx = iPad;
    }

    if (eScene != eUIComponent_TutorialPopup && eScene != eUIComponent_Chat && eScene != eUIScene_HUD)
        SetMenuDisplayed(idx, true);

    HXUIOBJ hScene = NULL;
    HRESULT hr = XuiSceneCreate(L"media/", app.GetSceneName(eScene, true, false), initData, &hScene);
    if (FAILED(hr))
        return hr;

    if (eScene == eUIComponent_TutorialPopup)
    {
        hr = XuiSceneNavigateForward(m_hCurrentTutorialScene[idx], FALSE, hScene, iPad);
        if (FAILED(hr))
        {
            XuiDestroyObject(hScene);
            return hr;
        }
        m_hCurrentTutorialScene[idx] = hScene;
    }
    else if (eScene == eUIComponent_Chat)
    {
        hr = XuiSceneNavigateForward(m_hCurrentChatScene[idx], FALSE, hScene, iPad);
        if (FAILED(hr))
        {
            XuiDestroyObject(hScene);
            return hr;
        }
        m_hCurrentChatScene[idx] = hScene;
    }
    else if (eScene == eUIScene_HUD)
    {
        hr = XuiSceneNavigateForward(m_hCurrentHudScene[idx], FALSE, hScene, iPad);
        if (FAILED(hr))
        {
            XuiDestroyObject(hScene);
            return hr;
        }
        m_hCurrentHudScene[idx] = hScene;
    }
    else
    {
        hr = XuiSceneNavigateForward(m_hCurrentScene[idx], bStayVisible, hScene, iPad);
        if (FAILED(hr))
        {
            XuiDestroyObject(hScene);
            return hr;
        }

        m_hCurrentScene[idx] = hScene;
        m_sceneStack[idx].push_back(SceneStackPair(eScene, hScene));
    }

    if (phResultingScene)
        *phResultingScene = hScene;

    return S_OK;
}

HRESULT CConsoleMinecraftApp::NavigateBack(int iPad, bool forceUsePad, EUIScene eScene)
{
    int idx = 0;
    if (m_bGameStarted || forceUsePad)
    {
        if ((iPad != 255) && (iPad >= 0))
            idx = iPad;
    }

    HXUIOBJ hCur = m_hCurrentScene[idx];
    HXUIOBJ hBack = NULL;

    if (eScene == eUIScene_COUNT)
    {
        hBack = XuiSceneGetBackScene(hCur);
        if (!hBack)
            return S_FALSE;

        if (!m_sceneStack[idx].empty())
            m_sceneStack[idx].pop_back();
    }
    else
    {
        for (std::list<SceneStackPair>::iterator it = m_sceneStack[idx].begin(); it != m_sceneStack[idx].end(); ++it)
        {
            if (it->first == eScene)
            {
                hBack = it->second;
                break;
            }
        }

        if (!hBack)
            return S_FALSE;

        while (!m_sceneStack[idx].empty() && m_sceneStack[idx].back().second != hBack)
            m_sceneStack[idx].pop_back();
    }

    m_hCurrentScene[idx] = hBack;
    return XuiSceneNavigateBack(hCur, hBack, iPad);
}

HRESULT CConsoleMinecraftApp::TutorialSceneNavigateBack(int iPad, bool forceUsePad)
{
    int idx = 0;
    if (m_bGameStarted || forceUsePad)
    {
        if ((iPad != 255) && (iPad >= 0))
            idx = iPad;
    }

    HXUIOBJ hCur = m_hCurrentTutorialScene[idx];
    HXUIOBJ hBack = XuiSceneGetBackScene(m_hCurrentTutorialScene[idx]);
    if (!hBack)
        return S_FALSE;

    m_hCurrentTutorialScene[idx] = hBack;
    return XuiSceneNavigateBack(hCur, hBack, iPad);
}

HRESULT CConsoleMinecraftApp::CloseXuiScenes(int iPad, bool forceUsePad)
{
    int idx = 0;
    if (m_bGameStarted || forceUsePad)
    {
        if ((iPad != 255) && (iPad >= 0))
            idx = iPad;
    }

    HRESULT hr = XuiSceneNavigateBackToFirst(m_hCurrentScene[idx], iPad);
    m_hCurrentScene[idx] = m_hFirstScene[idx];
    m_sceneStack[idx].clear();

    m_bPauseMenuDisplayed[iPad] = false;
    m_bContainerMenuDisplayed[iPad] = false;
    m_bIgnoreAutosaveMenuDisplayed[iPad] = false;
    m_bIgnorePlayerJoinMenuDisplayed[iPad] = false;

    SetMenuDisplayed(iPad, false);
    return hr;
}

HRESULT CConsoleMinecraftApp::CloseAllPlayersXuiScenes()
{
    HRESULT hr = S_OK;
    for (int idx = 0; idx < XUSER_MAX_COUNT; ++idx)
    {
        CloseXuiScenes(idx, true);
        ReloadChatScene(idx, false, true);
        ReloadHudScene(idx, false, true);
    }
    return hr;
}

HRESULT CConsoleMinecraftApp::CloseXuiScenesAndNavigateToScene(int iPad, EUIScene eScene, void* initData, bool forceUsePad)
{
    int idx = 0;
    if (m_bGameStarted || forceUsePad)
    {
        if ((iPad != 255) && (iPad >= 0))
            idx = iPad;
    }

    HRESULT hr = XuiSceneNavigateBackToFirst(m_hCurrentScene[idx], iPad);
    m_hCurrentScene[idx] = m_hFirstScene[idx];
    m_sceneStack[idx].clear();

    SetMenuDisplayed(iPad, false);
    NavigateToScene(iPad, eScene, initData);
    return hr;
}

bool CConsoleMinecraftApp::GetMenuDisplayed(int iPad)
{
    return m_bMenuDisplayed[iPad];
}

void CConsoleMinecraftApp::SetMenuDisplayed(int iPad, bool bVal)
{
    if (iPad == XUSER_INDEX_ANY)
        return;

    if (bVal)
    {
        m_bMenuDisplayed[iPad] = true;
        m_bMenuToBeClosed[iPad] = false;
    }
    else
    {
        m_bMenuToBeClosed[iPad] = true;
        m_iCountDown[iPad] = 4;
    }
}

void CConsoleMinecraftApp::CheckMenuDisplayed()
{
    for (int iPad = 0; iPad < XUSER_MAX_COUNT; ++iPad)
    {
        if (m_bMenuToBeClosed[iPad])
        {
            if (m_iCountDown[iPad] != 0)
            {
                m_iCountDown[iPad]--;
            }
            else
            {
                m_bMenuToBeClosed[iPad] = false;
                m_bMenuDisplayed[iPad] = false;
            }
        }
    }
}

void CConsoleMinecraftApp::SetChatTextDisplayed(int iPad, bool bVal)
{
    if (m_hCurrentChatScene[iPad])
        XuiElementSetShow(m_hCurrentChatScene[iPad], bVal);
}

void CConsoleMinecraftApp::ReloadChatScene(int iPad, bool, bool bForce)
{
    if (m_hFirstChatScene[iPad] == NULL || m_hCurrentChatScene[iPad] == NULL)
        return;

    BOOL chatSceneVisible = FALSE;
    if (m_hFirstChatScene[iPad] != m_hCurrentChatScene[iPad])
        chatSceneVisible = XuiElementIsShown(m_hCurrentChatScene[iPad]);

    if (bForce || m_hFirstChatScene[iPad] == m_hCurrentChatScene[iPad])
    {
        if (m_hFirstChatScene[iPad] != m_hCurrentChatScene[iPad])
            XuiSceneNavigateBack(m_hCurrentChatScene[iPad], m_hFirstChatScene[iPad], iPad);

        m_hCurrentChatScene[iPad] = m_hFirstChatScene[iPad];
        app.NavigateToScene(iPad, eUIComponent_Chat, NULL, true);
        XuiElementSetShow(m_hCurrentChatScene[iPad], chatSceneVisible);
    }
}

void CConsoleMinecraftApp::ReloadHudScene(int iPad, bool, bool bForce)
{
    if (m_hFirstHudScene[iPad] == NULL || m_hCurrentHudScene[iPad] == NULL)
        return;

    BOOL hudSceneVisible = FALSE;
    if (m_hFirstHudScene[iPad] != m_hCurrentHudScene[iPad])
        hudSceneVisible = XuiElementIsShown(m_hCurrentHudScene[iPad]);

    if (bForce || m_hFirstHudScene[iPad] == m_hCurrentHudScene[iPad])
    {
        if (m_hFirstHudScene[iPad] != m_hCurrentHudScene[iPad])
            XuiSceneNavigateBack(m_hCurrentHudScene[iPad], m_hFirstHudScene[iPad], iPad);

        m_hCurrentHudScene[iPad] = m_hFirstHudScene[iPad];
        app.NavigateToScene(iPad, eUIScene_HUD, NULL, true);
        XuiElementSetShow(m_hCurrentHudScene[iPad], hudSceneVisible);
    }
}

void CConsoleMinecraftApp::StoreLaunchData()
{
}

void CConsoleMinecraftApp::ExitGame()
{
    std::exit(0);
}

void CConsoleMinecraftApp::FatalLoadError()
{
    assert(0);
}

void CConsoleMinecraftApp::CaptureSaveThumbnail()
{
}

void CConsoleMinecraftApp::GetSaveThumbnail(std::uint8_t**, unsigned int*)
{
}

void CConsoleMinecraftApp::ReleaseSaveThumbnail()
{
}

void CConsoleMinecraftApp::GetScreenshot(int, std::uint8_t**, unsigned int*)
{
}

int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR*)
{
    return -1;
}

int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR*, eFileExtensionType)
{
    return -1;
}

void CConsoleMinecraftApp::FreeLocalTMSFiles(eTMSFileType)
{
}

int CConsoleMinecraftApp::GetLocalTMSFileIndex(WCHAR*, bool, eFileExtensionType)
{
    return -1;
}

void CConsoleMinecraftApp::TemporaryCreateGameStart()
{
    app.setLevelGenerationOptions(NULL);

    Minecraft* pMinecraft = Minecraft::GetInstance();
    app.ReleaseSaveThumbnail();
    ProfileManager.SetLockedProfile(0);
    pMinecraft->user->name = L"Linux";
    app.ApplyGameSettingsChanged(0);

    MinecraftServer::resetFlags();

    app.SetTutorialMode(false);
    app.SetCorruptSaveDeleted(false);

    app.ClearTerrainFeaturePosition();
    std::wstring wWorldName = L"TestWorld";

    StorageManager.ResetSaveData();
    StorageManager.SetSaveTitle(wWorldName.c_str());

    __int64 seedValue = 0;

    NetworkGameInitData* param = new NetworkGameInitData();
    param->seed = seedValue;
    param->saveData = NULL;

    app.SetGameHostOption(eGameHostOption_Difficulty, 0);
    app.SetGameHostOption(eGameHostOption_FriendsOfFriends, 0);
    app.SetGameHostOption(eGameHostOption_Gamertags, 1);
    app.SetGameHostOption(eGameHostOption_BedrockFog, 1);
    app.SetGameHostOption(eGameHostOption_GameType, GameType::CREATIVE->getId());
    app.SetGameHostOption(eGameHostOption_LevelType, 0);
    app.SetGameHostOption(eGameHostOption_Structures, 1);
    app.SetGameHostOption(eGameHostOption_BonusChest, 0);
    app.SetGameHostOption(eGameHostOption_PvP, 1);
    app.SetGameHostOption(eGameHostOption_TrustPlayers, 1);
    app.SetGameHostOption(eGameHostOption_FireSpreads, 1);
    app.SetGameHostOption(eGameHostOption_TNT, 1);
    app.SetGameHostOption(eGameHostOption_HostCanFly, 1);
    app.SetGameHostOption(eGameHostOption_HostCanChangeHunger, 1);
    app.SetGameHostOption(eGameHostOption_HostCanBeInvisible, 1);

    param->settings = app.GetGameHostOption(eGameHostOption_All);

    g_NetworkManager.FakeLocalPlayerJoined();

    LoadingInputParams* loadingParams = new LoadingInputParams();
    loadingParams->func = &CGameNetworkManager::RunNetworkGameThreadProc;
    loadingParams->lpParam = (LPVOID)param;

    app.SetAutosaveTimerTime();

    C4JThread* thread = new C4JThread(
        loadingParams->func,
        loadingParams->lpParam,
        "RunNetworkGame");
    thread->Run();
}
