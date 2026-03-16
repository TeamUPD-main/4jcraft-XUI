#pragma once

#include "../../../Minecraft.World/Platform/stdafx.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cwchar>
#include <filesystem>
#include <fstream>
#include <functional>
#include <list>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

#ifndef S_OK
#define S_OK ((HRESULT)0L)
#endif
#ifndef S_FALSE
#define S_FALSE ((HRESULT)1L)
#endif
#ifndef E_FAIL
#define E_FAIL ((HRESULT)0x80004005L)
#endif
#ifndef E_INVALIDARG
#define E_INVALIDARG ((HRESULT)0x80070057L)
#endif
#ifndef E_NOTIMPL
#define E_NOTIMPL ((HRESULT)0x80004001L)
#endif
#ifndef E_POINTER
#define E_POINTER ((HRESULT)0x80004003L)
#endif
#ifndef E_UNEXPECTED
#define E_UNEXPECTED ((HRESULT)0x8000FFFFL)
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#endif
#ifndef FAILED
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif
#ifndef HRESULT_SUCCEEDED
#define HRESULT_SUCCEEDED(hr) SUCCEEDED(hr)
#endif
#ifndef HRESULT_FAILED
#define HRESULT_FAILED(hr) FAILED(hr)
#endif

#ifndef ASSERT
#define ASSERT(x) assert(x)
#endif

#ifndef XUSER_MAX_COUNT
#define XUSER_MAX_COUNT 4
#endif
#ifndef XUSER_INDEX_ANY
#define XUSER_INDEX_ANY 0xFF
#endif
#ifndef XUSER_INDEX_NONE
#define XUSER_INDEX_NONE 0xFE
#endif

#ifndef XUI_TRANSITION_TO
#define XUI_TRANSITION_TO 1
#endif
#ifndef XUI_TRANSITION_BACKTO
#define XUI_TRANSITION_BACKTO 2
#endif
#ifndef XUI_TRANSITION_ACTION_DESTROY
#define XUI_TRANSITION_ACTION_DESTROY 1
#endif
#ifndef XUI_DISCARD_FONTS
#define XUI_DISCARD_FONTS 1
#endif

#ifndef MAXULONG_PTR
#define MAXULONG_PTR ((ULONG_PTR)~(ULONG_PTR)0)
#endif

struct D3DXVECTOR3
{
    float x;
    float y;
    float z;

    D3DXVECTOR3() : x(0.0f), y(0.0f), z(0.0f) {}
    D3DXVECTOR3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
};

struct D3DXMATRIX
{
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;

    D3DXMATRIX()
        : _11(1), _12(0), _13(0), _14(0),
          _21(0), _22(1), _23(0), _24(0),
          _31(0), _32(0), _33(1), _34(0),
          _41(0), _42(0), _43(0), _44(1)
    {
    }
};

struct XUIRect
{
    float left;
    float top;
    float right;
    float bottom;

    XUIRect() : left(0), top(0), right(0), bottom(0) {}
};

struct XUIMessageInit
{
    void* pvInitData;
};

struct XUIMessageTransition
{
    DWORD dwTransAction;
    DWORD dwTransType;
};

struct XUIMessageTimer
{
    int nId;
};

struct XUIMessageInput
{
    int UserIndex;
    DWORD dwKeyCode;
};

struct XUINotifyFocus
{
    int UserIndex;
};

struct XUINotifyPress
{
    int UserIndex;
};

struct XUIMessageControlNavigate
{
    void* hObjSource;
    void* hObjDest;
    int nControlNavigate;
};

struct XUIMessage
{
    DWORD dwMessage;
    void* pvData;
};

struct TypefaceDescriptor
{
    LPCWSTR szTypeface;
    LPCWSTR szLocator;
    LPCWSTR szFallbackTypeface;
};

namespace LinuxXui
{
    struct Object;
    struct SceneHost;
    struct Resource;
    struct Timer;

    enum class ObjectKind
    {
        Generic,
        Scene,
        Container,
        Text,
        Button,
        Custom
    };

    using HXUIOBJ = Object*;
    using SceneCreateFn = std::function<SceneHost*(HXUIOBJ)>;

    struct SceneHost
    {
        virtual ~SceneHost() {}

        virtual HRESULT OnInit(XUIMessageInit*, BOOL&) { return S_OK; }
        virtual HRESULT OnTransitionStart(XUIMessageTransition*, BOOL&) { return S_OK; }
        virtual HRESULT OnNotifySetFocus(HXUIOBJ, XUINotifyFocus*, BOOL&) { return S_OK; }
        virtual HRESULT OnNotifyPressEx(HXUIOBJ, XUINotifyPress*, BOOL&) { return S_OK; }
        virtual HRESULT OnNavReturn(HXUIOBJ, BOOL&) { return S_OK; }
        virtual HRESULT OnControlNavigate(XUIMessageControlNavigate*, BOOL&) { return S_OK; }
        virtual HRESULT OnKeyDown(XUIMessageInput*, BOOL&) { return S_OK; }
        virtual HRESULT OnTimer(XUIMessageTimer*, BOOL&) { return S_OK; }
        virtual void OnDestroy() {}
    };

    struct Object
    {
        ObjectKind kind;
        std::wstring id;
        std::wstring text;
        std::wstring locator;
        std::wstring sceneFile;
        bool visible;
        bool enabled;
        bool isScene;
        bool stayVisible;

        float width;
        float height;

        D3DXVECTOR3 position;
        D3DXVECTOR3 scale;
        D3DXVECTOR3 pivot;

        Object* parent;
        Object* backScene;
        std::vector<Object*> children;

        std::unique_ptr<SceneHost> host;
        void* userData;

        Object()
            : kind(ObjectKind::Generic),
              visible(true),
              enabled(true),
              isScene(false),
              stayVisible(false),
              width(0.0f),
              height(0.0f),
              position(),
              scale(1.0f, 1.0f, 1.0f),
              pivot(),
              parent(NULL),
              backScene(NULL),
              userData(NULL)
        {
        }
    };

    struct Resource
    {
        std::wstring locator;
        std::vector<std::uint8_t> data;
        bool isMemoryResource;

        Resource() : isMemoryResource(true) {}
    };

    struct Timer
    {
        HXUIOBJ owner;
        int id;
        std::uint64_t intervalMs;
        std::uint64_t nextFireMs;
        bool alive;

        Timer() : owner(NULL), id(0), intervalMs(0), nextFireMs(0), alive(true) {}
    };

    inline std::unordered_map<std::wstring, SceneCreateFn>& SceneRegistry()
    {
        static std::unordered_map<std::wstring, SceneCreateFn> g;
        return g;
    }

    inline std::vector<std::unique_ptr<Object>>& OwnedObjects()
    {
        static std::vector<std::unique_ptr<Object>> g;
        return g;
    }

    inline std::vector<std::unique_ptr<Resource>>& OwnedResources()
    {
        static std::vector<std::unique_ptr<Resource>> g;
        return g;
    }

    inline std::vector<Timer>& Timers()
    {
        static std::vector<Timer> g;
        return g;
    }

    inline std::wstring& Locale()
    {
        static std::wstring g = L"";
        return g;
    }

    inline void*& FontRenderer()
    {
        static void* g = NULL;
        return g;
    }

    inline bool& Initialized()
    {
        static bool g = false;
        return g;
    }

    inline std::wstring NormalizePath(std::wstring in)
    {
        for (size_t i = 0; i < in.size(); ++i)
        {
            if (in[i] == L'\\')
                in[i] = L'/';
        }

        if (in.rfind(L"section://", 0) == 0)
        {
            size_t hashPos = in.find(L'#');
            if (hashPos != std::wstring::npos)
            {
                std::wstring tail = in.substr(hashPos + 1);
                while (!tail.empty() && tail[0] == L'/')
                    tail.erase(tail.begin());
                return tail;
            }
        }

        if (in.rfind(L"file://game:/", 0) == 0)
            return in.substr(std::wcslen(L"file://game:/"));
        if (in.rfind(L"file://UPDATE:/", 0) == 0)
            return in.substr(std::wcslen(L"file://UPDATE:/"));

        return in;
    }

    inline std::uint64_t NowMs()
    {
        using namespace std::chrono;
        return (std::uint64_t)std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    inline Object* MakeObject(ObjectKind kind, const std::wstring& id = L"")
    {
        std::unique_ptr<Object> obj(new Object());
        obj->kind = kind;
        obj->id = id;
        Object* raw = obj.get();
        OwnedObjects().push_back(std::move(obj));
        return raw;
    }

    inline void AddChild(Object* parent, Object* child)
    {
        if (!parent || !child)
            return;
        child->parent = parent;
        parent->children.push_back(child);
    }

    inline Object* FindChildRecursive(Object* root, const std::wstring& id)
    {
        if (!root)
            return NULL;
        if (root->id == id)
            return root;

        for (size_t i = 0; i < root->children.size(); ++i)
        {
            Object* found = FindChildRecursive(root->children[i], id);
            if (found)
                return found;
        }
        return NULL;
    }

    inline void DestroyRecursive(Object* root)
    {
        if (!root)
            return;

        for (size_t i = 0; i < Timers().size(); ++i)
        {
            if (Timers()[i].owner == root)
                Timers()[i].alive = false;
        }

        if (root->host)
        {
            root->host->OnDestroy();
            root->host.reset();
        }

        for (size_t i = 0; i < root->children.size(); ++i)
            DestroyRecursive(root->children[i]);

        root->children.clear();
        root->parent = NULL;
        root->backScene = NULL;
        root->visible = false;
    }

    inline HRESULT RegisterSceneClass(const std::wstring& sceneFile, SceneCreateFn fn)
    {
        if (!fn)
            return E_INVALIDARG;
        SceneRegistry()[sceneFile] = fn;
        return S_OK;
    }

    inline HRESULT UnregisterSceneClass(const std::wstring& sceneFile)
    {
        SceneRegistry().erase(sceneFile);
        return S_OK;
    }

    inline SceneCreateFn FindSceneFactory(const std::wstring& sceneFile)
    {
        auto it = SceneRegistry().find(sceneFile);
        if (it != SceneRegistry().end())
            return it->second;

        std::wstring lower = sceneFile;
        std::transform(lower.begin(), lower.end(), lower.begin(), towlower);

        for (auto it2 = SceneRegistry().begin(); it2 != SceneRegistry().end(); ++it2)
        {
            std::wstring key = it2->first;
            std::transform(key.begin(), key.end(), key.begin(), towlower);
            if (key == lower)
                return it2->second;
        }

        return SceneCreateFn();
    }

    inline HRESULT Initialize()
    {
        Initialized() = true;
        return S_OK;
    }

    inline void Reset()
    {
        Timers().clear();
        OwnedResources().clear();
        OwnedObjects().clear();
        SceneRegistry().clear();
        Initialized() = false;
        Locale() = L"";
        FontRenderer() = NULL;
    }

    inline void UpdateTimers()
    {
        const std::uint64_t now = NowMs();
        for (size_t i = 0; i < Timers().size(); ++i)
        {
            Timer& t = Timers()[i];
            if (!t.alive || !t.owner || !t.owner->host)
                continue;

            if (now >= t.nextFireMs)
            {
                t.nextFireMs = now + t.intervalMs;
                XUIMessageTimer msg;
                msg.nId = t.id;
                BOOL handled = FALSE;
                t.owner->host->OnTimer(&msg, handled);
            }
        }

        Timers().erase(
            std::remove_if(Timers().begin(), Timers().end(),
                [](const Timer& t) { return !t.alive; }),
            Timers().end());
    }

    inline HRESULT RunFrame()
    {
        UpdateTimers();
        return S_OK;
    }

    inline std::vector<std::uint8_t> ReadWholeFile(const std::wstring& locator)
    {
        std::wstring path = NormalizePath(locator);
        std::ifstream f(std::filesystem::path(path), std::ios::binary);
        if (!f)
            return std::vector<std::uint8_t>();

        f.seekg(0, std::ios::end);
        std::streamsize size = f.tellg();
        f.seekg(0, std::ios::beg);

        if (size < 0)
            return std::vector<std::uint8_t>();

        std::vector<std::uint8_t> out((size_t)size);
        if (size > 0)
            f.read(reinterpret_cast<char*>(out.data()), size);
        return out;
    }

    inline Object* CreateGenericSceneObject(const std::wstring& sceneFile, void* initData)
    {
        Object* scene = MakeObject(ObjectKind::Scene, sceneFile);
        scene->isScene = true;
        scene->sceneFile = sceneFile;
        scene->width = 1280.0f;
        scene->height = 720.0f;
        scene->userData = initData;

        if (sceneFile == L"xuiscene_base.xur" || sceneFile == L"xuiscene_base_480.xur")
        {
            for (int i = 0; i < XUSER_MAX_COUNT; ++i)
            {
                std::wstring baseId = L"BasePlayer" + std::to_wstring(i);
                Object* basePlayer = MakeObject(ObjectKind::Container, baseId);
                AddChild(scene, basePlayer);

                AddChild(basePlayer, MakeObject(ObjectKind::Container, L"XuiSceneContainer"));
                AddChild(basePlayer, MakeObject(ObjectKind::Container, L"XuiSceneTutorialContainer"));
                AddChild(basePlayer, MakeObject(ObjectKind::Container, L"XuiSceneChatRoot"));
                AddChild(basePlayer, MakeObject(ObjectKind::Container, L"XuiSceneHudRoot"));
            }

            AddChild(scene, MakeObject(ObjectKind::Text, L"XuiPressStartMessage"));
            AddChild(scene, MakeObject(ObjectKind::Container, L"XuiSceneDebugContainer"));
        }

        return scene;
    }

    inline HRESULT CreateScene(const std::wstring& locatorBase, const std::wstring& sceneFile, void* initData, HXUIOBJ* outScene)
    {
        if (!outScene)
            return E_POINTER;

        std::wstring fullPath = locatorBase + sceneFile;
        Object* scene = NULL;

        SceneCreateFn fn = FindSceneFactory(sceneFile);
        if (fn)
        {
            scene = MakeObject(ObjectKind::Scene, sceneFile);
            scene->isScene = true;
            scene->sceneFile = sceneFile;
            scene->locator = fullPath;
            scene->width = 1280.0f;
            scene->height = 720.0f;
            scene->userData = initData;
            scene->host.reset(fn(scene));
        }
        else
        {
            scene = CreateGenericSceneObject(sceneFile, initData);
            scene->locator = fullPath;
        }

        if (!scene)
            return E_FAIL;

        if (scene->host)
        {
            XUIMessageInit initMsg;
            initMsg.pvInitData = initData;
            BOOL handled = FALSE;
            scene->host->OnInit(&initMsg, handled);
        }

        *outScene = scene;
        return S_OK;
    }
}

using HXUIOBJ = LinuxXui::HXUIOBJ;
using HXUIRESOURCE = LinuxXui::Resource*;

inline HRESULT XuiRegisterSceneClass(LPCWSTR sceneFile, LinuxXui::SceneCreateFn fn)
{
    return LinuxXui::RegisterSceneClass(sceneFile, fn);
}

inline HRESULT XuiUnregisterSceneClass(LPCWSTR sceneFile)
{
    return LinuxXui::UnregisterSceneClass(sceneFile);
}

inline HRESULT XuiSceneCreate(LPCWSTR baseLocator, LPCWSTR sceneFile, void* initData, HXUIOBJ* outScene)
{
    if (!baseLocator || !sceneFile || !outScene)
        return E_INVALIDARG;
    return LinuxXui::CreateScene(baseLocator, sceneFile, initData, outScene);
}

inline HRESULT LoadFirstScene(LPCWSTR baseLocator, LPCWSTR sceneFile, void* initData, HXUIOBJ* outScene)
{
    return XuiSceneCreate(baseLocator, sceneFile, initData, outScene);
}

inline HRESULT XuiSceneNavigateForward(HXUIOBJ current, BOOL stayVisible, HXUIOBJ next, int)
{
    if (!next)
        return E_INVALIDARG;

    next->backScene = current;
    next->stayVisible = (stayVisible != FALSE);
    next->visible = true;

    if (current && !stayVisible)
        current->visible = false;

    if (next->host)
    {
        XUIMessageTransition tr;
        tr.dwTransAction = 0;
        tr.dwTransType = XUI_TRANSITION_TO;
        BOOL handled = FALSE;
        next->host->OnTransitionStart(&tr, handled);
    }

    return S_OK;
}

inline HXUIOBJ XuiSceneGetBackScene(HXUIOBJ scene)
{
    return scene ? scene->backScene : NULL;
}

inline void XuiSceneSetBackScene(HXUIOBJ scene, HXUIOBJ back)
{
    if (scene)
        scene->backScene = back;
}

inline HRESULT XuiSceneNavigateBack(HXUIOBJ current, HXUIOBJ back, int)
{
    if (!current || !back)
        return E_INVALIDARG;

    current->visible = false;
    back->visible = true;

    if (back->host)
    {
        BOOL handled = FALSE;
        back->host->OnNavReturn(back, handled);
    }

    return S_OK;
}

inline HRESULT XuiSceneNavigateBackToFirst(HXUIOBJ current, int)
{
    if (!current)
        return E_INVALIDARG;

    HXUIOBJ first = current;
    while (first->backScene)
        first = first->backScene;

    current->visible = false;
    first->visible = true;

    if (first->host)
    {
        BOOL handled = FALSE;
        first->host->OnNavReturn(first, handled);
    }

    return S_OK;
}

inline void XuiDestroyObject(HXUIOBJ obj)
{
    LinuxXui::DestroyRecursive(obj);
}

inline HRESULT XuiElementGetChildById(HXUIOBJ parent, LPCWSTR id, HXUIOBJ* outObj)
{
    if (!parent || !id || !outObj)
        return E_INVALIDARG;
    *outObj = LinuxXui::FindChildRecursive(parent, id);
    return (*outObj != NULL) ? S_OK : E_FAIL;
}

inline void XuiElementSetShow(HXUIOBJ obj, BOOL show)
{
    if (obj)
        obj->visible = (show != FALSE);
}

inline BOOL XuiElementIsShown(HXUIOBJ obj)
{
    return (obj && obj->visible) ? TRUE : FALSE;
}

inline void XuiControlSetText(HXUIOBJ obj, LPCWSTR text)
{
    if (obj)
        obj->text = (text ? text : L"");
}

inline HRESULT XuiElementSetBounds(HXUIOBJ obj, float width, float height)
{
    if (!obj)
        return E_INVALIDARG;
    obj->width = width;
    obj->height = height;
    return S_OK;
}

inline HRESULT XuiElementGetBounds(HXUIOBJ obj, float* width, float* height)
{
    if (!obj)
        return E_INVALIDARG;
    if (width) *width = obj->width;
    if (height) *height = obj->height;
    return S_OK;
}

inline HRESULT XuiElementSetPosition(HXUIOBJ obj, const D3DXVECTOR3* pos)
{
    if (!obj || !pos)
        return E_INVALIDARG;
    obj->position = *pos;
    return S_OK;
}

inline HRESULT XuiElementGetPosition(HXUIOBJ obj, D3DXVECTOR3* pos)
{
    if (!obj || !pos)
        return E_INVALIDARG;
    *pos = obj->position;
    return S_OK;
}

inline HRESULT XuiElementSetScale(HXUIOBJ obj, const D3DXVECTOR3* scale)
{
    if (!obj || !scale)
        return E_INVALIDARG;
    obj->scale = *scale;
    return S_OK;
}

inline HRESULT XuiElementGetScale(HXUIOBJ obj, D3DXVECTOR3* scale)
{
    if (!obj || !scale)
        return E_INVALIDARG;
    *scale = obj->scale;
    return S_OK;
}

inline HRESULT XuiElementSetPivot(HXUIOBJ obj, const D3DXVECTOR3* pivot)
{
    if (!obj || !pivot)
        return E_INVALIDARG;
    obj->pivot = *pivot;
    return S_OK;
}

inline HRESULT XuiElementGetPivot(HXUIOBJ obj, D3DXVECTOR3* pivot)
{
    if (!obj || !pivot)
        return E_INVALIDARG;
    *pivot = obj->pivot;
    return S_OK;
}

inline HRESULT XuiElementGetFullXForm(HXUIOBJ obj, D3DXMATRIX* outMat)
{
    if (!obj || !outMat)
        return E_INVALIDARG;

    *outMat = D3DXMATRIX();
    outMat->_11 = obj->scale.x;
    outMat->_22 = obj->scale.y;
    outMat->_33 = obj->scale.z;
    outMat->_41 = obj->position.x;
    outMat->_42 = obj->position.y;
    outMat->_43 = obj->position.z;
    return S_OK;
}

inline HRESULT XuiElementSetUserFocus(HXUIOBJ, int)
{
    return S_OK;
}

inline HRESULT XuiObjectFromHandle(HXUIOBJ obj, void** outObj)
{
    if (!outObj)
        return E_POINTER;
    *outObj = obj ? static_cast<void*>(obj->host.get()) : NULL;
    return obj ? S_OK : E_FAIL;
}

inline HRESULT XuiControlGetVisual(HXUIOBJ control, HXUIOBJ* outVisual)
{
    if (!outVisual)
        return E_POINTER;
    *outVisual = control;
    return control ? S_OK : E_FAIL;
}

inline HRESULT XuiElementDiscardResources(HXUIOBJ, DWORD)
{
    return S_OK;
}

inline void XuiFontSetRenderer(void* renderer)
{
    LinuxXui::FontRenderer() = renderer;
}

inline HRESULT XuiBroadcastMessage(HXUIOBJ, XUIMessage*)
{
    return S_OK;
}

inline HRESULT XuiSetTimer(HXUIOBJ owner, int id, unsigned int intervalMs)
{
    if (!owner)
        return E_INVALIDARG;

    std::vector<LinuxXui::Timer>& timers = LinuxXui::Timers();
    for (size_t i = 0; i < timers.size(); ++i)
    {
        if (timers[i].owner == owner && timers[i].id == id)
        {
            timers[i].intervalMs = intervalMs;
            timers[i].nextFireMs = LinuxXui::NowMs() + intervalMs;
            timers[i].alive = true;
            return S_OK;
        }
    }

    LinuxXui::Timer t;
    t.owner = owner;
    t.id = id;
    t.intervalMs = intervalMs;
    t.nextFireMs = LinuxXui::NowMs() + intervalMs;
    timers.push_back(t);
    return S_OK;
}

inline HRESULT XuiKillTimer(HXUIOBJ owner, int id)
{
    std::vector<LinuxXui::Timer>& timers = LinuxXui::Timers();
    for (size_t i = 0; i < timers.size(); ++i)
    {
        if (timers[i].owner == owner && timers[i].id == id)
            timers[i].alive = false;
    }
    return S_OK;
}

inline HRESULT XuiSetLocale(LPCWSTR locale)
{
    LinuxXui::Locale() = locale ? locale : L"";
    return S_OK;
}

inline HRESULT XuiHtmlRegister() { return S_OK; }
inline HRESULT XuiHtmlUnregister() { return S_OK; }
inline HRESULT XuiSoundXACTRegister() { return S_OK; }
inline HRESULT XuiSoundXAudioRegister() { return S_OK; }
inline HRESULT LoadSkin(LPCWSTR) { return S_OK; }
inline HRESULT RegisterDefaultTypeface(LPCWSTR, LPCWSTR, LPCWSTR = NULL, float = 0.0f, LPCWSTR = NULL) { return S_OK; }
inline HRESULT XuiRegisterTypeface(const TypefaceDescriptor*, BOOL) { return S_OK; }

inline void XuiFree(void* p)
{
    delete[] reinterpret_cast<std::uint8_t*>(p);
}

inline HRESULT XuiResourceLoadAllNoLoc(LPCWSTR locator, BYTE** outBytes, UINT* outSize)
{
    if (!locator || !outBytes || !outSize)
        return E_INVALIDARG;

    std::vector<std::uint8_t> data = LinuxXui::ReadWholeFile(locator);
    if (data.empty())
    {
        *outBytes = NULL;
        *outSize = 0;
        return E_FAIL;
    }

    BYTE* buf = new BYTE[data.size()];
    std::memcpy(buf, data.data(), data.size());
    *outBytes = buf;
    *outSize = (UINT)data.size();
    return S_OK;
}

inline HRESULT XuiResourceOpenNoLoc(LPCWSTR locator, HXUIRESOURCE* outRes, BOOL* isMemoryResource)
{
    if (!locator || !outRes)
        return E_INVALIDARG;

    std::unique_ptr<LinuxXui::Resource> res(new LinuxXui::Resource());
    res->locator = locator;
    res->data = LinuxXui::ReadWholeFile(locator);
    if (res->data.empty())
        return E_FAIL;

    if (isMemoryResource)
        *isMemoryResource = TRUE;

    LinuxXui::Resource* raw = res.get();
    LinuxXui::OwnedResources().push_back(std::move(res));
    *outRes = raw;
    return S_OK;
}

inline void XuiResourceClose(HXUIRESOURCE)
{
}

inline HRESULT XuiTextPresenterMeasureText(HXUIOBJ, LPCWSTR text, XUIRect* outRect)
{
    if (!outRect)
        return E_INVALIDARG;

    const std::wstring s = text ? text : L"";
    const float charW = 10.0f;
    const float charH = 20.0f;

    outRect->left = 0.0f;
    outRect->top = 0.0f;
    outRect->right = static_cast<float>(s.size()) * charW;
    outRect->bottom = charH;
    return S_OK;
}

inline HXUIOBJ XuiControlGetNavigation(HXUIOBJ source, int, BOOL, BOOL)
{
    return source;
}
