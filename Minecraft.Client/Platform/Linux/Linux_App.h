#pragma once

#include "../../../Minecraft.World/Platform/stdafx.h"
#include "../Common/Consoles_App.h"
#include "Linux_XuiShim.h"

#include <list>
#include <utility>

#define GROUP_ID "A9C80F8E-5EAE-4883-89E6-0C456CADE89B"
#define SAVETRANSFER_GROUP_ID "43FD7A62-2747-4489-8E71-F937163DC3C5"

class XUI_FontRenderer;

class CConsoleMinecraftApp : public CMinecraftApp
{
private:
    bool m_bMenuDisplayed[XUSER_MAX_COUNT];
    bool m_bMenuToBeClosed[XUSER_MAX_COUNT];
    bool m_bPauseMenuDisplayed[XUSER_MAX_COUNT];
    bool m_bContainerMenuDisplayed[XUSER_MAX_COUNT];
    bool m_bIgnoreAutosaveMenuDisplayed[XUSER_MAX_COUNT];
    bool m_bIgnorePlayerJoinMenuDisplayed[XUSER_MAX_COUNT];
    int m_iCountDown[XUSER_MAX_COUNT];

    HXUIOBJ m_PlayerBaseScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hFirstScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hCurrentScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hFirstTutorialScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hCurrentTutorialScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hFirstChatScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hCurrentChatScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hFirstHudScene[XUSER_MAX_COUNT];
    HXUIOBJ m_hCurrentHudScene[XUSER_MAX_COUNT];

    typedef std::pair<EUIScene, HXUIOBJ> SceneStackPair;
    std::list<SceneStackPair> m_sceneStack[XUSER_MAX_COUNT];

    static WCHAR* wchSceneA[];
    WCHAR m_SceneName[64];

private:
    XUI_FontRenderer* m_fontRenderer;
    bool m_bFontRendererOverridden;
    bool m_bOverrideFontRenderer;

    HXUIOBJ m_hObjRoot;

public:
    CConsoleMinecraftApp();

    virtual void SetRichPresenceContext(int iPad, int contextId);

    virtual HRESULT RegisterXuiClasses();
    virtual HRESULT UnregisterXuiClasses();
    virtual HRESULT LoadXuiResources();
    virtual void RunFrame();

    virtual void StoreLaunchData();
    virtual void ExitGame();
    virtual void FatalLoadError();

    virtual void CaptureSaveThumbnail();
    virtual void GetSaveThumbnail(std::uint8_t** thumbnailData, unsigned int* thumbnailSize);
    virtual void ReleaseSaveThumbnail();
    virtual void GetScreenshot(int iPad, std::uint8_t** screenshotData, unsigned int* screenshotSize);

    virtual int LoadLocalTMSFile(WCHAR* wchTMSFile);
    virtual int LoadLocalTMSFile(WCHAR* wchTMSFile, eFileExtensionType eExt);
    virtual void FreeLocalTMSFiles(eTMSFileType eType);
    virtual int GetLocalTMSFileIndex(WCHAR* wchTMSFile, bool bFilenameIncludesExtension, eFileExtensionType eEXT = eFileExtensionType_PNG);

    virtual void ReadBannedList(int iPad, eTMSAction action = (eTMSAction)0, bool bCallback = false) {}

    C4JStringTable* GetStringTable() { return NULL; }

    virtual void TemporaryCreateGameStart();

    virtual void OverrideFontRenderer(bool set, bool immediate = true);

    WCHAR* GetSceneName(EUIScene eScene, bool bAppendToName, bool bSplitscreenScene);

    virtual HRESULT NavigateToScene(int iPad, EUIScene eScene, void* initData = NULL, bool forceUsePad = false, BOOL bStayVisible = FALSE, HXUIOBJ* phResultingScene = NULL);
    virtual HRESULT NavigateBack(int iPad, bool forceUsePad = false, EUIScene eScene = eUIScene_COUNT);
    virtual HRESULT TutorialSceneNavigateBack(int iPad, bool forceUsePad = false);
    virtual HRESULT CloseXuiScenes(int iPad, bool forceUsePad = false);
    virtual HRESULT CloseAllPlayersXuiScenes();
    virtual HRESULT CloseXuiScenesAndNavigateToScene(int iPad, EUIScene eScene, void* initData = NULL, bool forceUsePad = false);

    virtual void SetChatTextDisplayed(int iPad, bool bVal);
    virtual void ReloadChatScene(int iPad, bool bJoining = false, bool bForce = false);
    virtual void ReloadHudScene(int iPad, bool bJoining = false, bool bForce = false);

    bool GetMenuDisplayed(int iPad);
    void SetMenuDisplayed(int iPad, bool bVal);
    void CheckMenuDisplayed();

    bool IsPauseMenuDisplayed(int iPad) { return m_bPauseMenuDisplayed[iPad]; }
    bool IsContainerMenuDisplayed(int iPad) { return m_bContainerMenuDisplayed[iPad]; }
    bool IsIgnoreAutosaveMenuDisplayed(int iPad) { return m_bIgnoreAutosaveMenuDisplayed[iPad]; }
    void SetIgnoreAutosaveMenuDisplayed(int iPad, bool displayed) { m_bIgnoreAutosaveMenuDisplayed[iPad] = displayed; }
    bool IsIgnorePlayerJoinMenuDisplayed(int iPad) { return m_bIgnorePlayerJoinMenuDisplayed[iPad]; }
    void SetIgnorePlayerJoinMenuDisplayed(int iPad, bool displayed) { m_bIgnorePlayerJoinMenuDisplayed[iPad] = displayed; }

    HXUIOBJ GetFirstScene(int iPad) { return m_hFirstScene[iPad]; }
    HXUIOBJ GetCurrentTutorialScene(int iPad) { return m_hCurrentTutorialScene[iPad]; }
    HXUIOBJ GetCurrentHUDScene(int iPad) { return m_hCurrentHudScene[iPad]; }
    HXUIOBJ GetCurrentScene(int iPad) { return m_hCurrentScene[iPad]; }
};

extern CConsoleMinecraftApp app;
