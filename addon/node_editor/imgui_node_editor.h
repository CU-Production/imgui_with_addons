//------------------------------------------------------------------------------
// LICENSE
//   This software is dual-licensed to the public domain and under the following
//   license: you are granted a perpetual, irrevocable license to copy, modify,
//   publish, and distribute this file as you see fit.
//
// CREDITS
//   Written by Michal Cichon
//------------------------------------------------------------------------------
# ifndef __IMGUI_NODE_EDITOR_H__
# define __IMGUI_NODE_EDITOR_H__
# pragma once


//------------------------------------------------------------------------------
# include <imgui.h>
# include <imgui_internal.h>
# include <cstdint> // std::uintXX_t
# include <utility> // std::move
# include <vector>
# include <string>

//------------------------------------------------------------------------------
namespace imgui_json {
struct value;
} // imgui_json


//------------------------------------------------------------------------------
namespace ax {
namespace NodeEditor {

enum class CanvasSizeMode
{
    FitVerticalView,        // Previous view will be scaled to fit new view on Y axis
    FitHorizontalView,      // Previous view will be scaled to fit new view on X axis
    CenterOnly,             // Previous view will be centered on new view
};

//------------------------------------------------------------------------------
struct NodeId;
struct LinkId;
struct PinId;


//------------------------------------------------------------------------------
enum class TransactionAction: int32_t
{
    Unknown,
    Navigation,
    DragStart,
    DragEnd,
    Resize,
    ClearSelection,
    Select,
    Deselect,
    ToggleSelect
};

const char* ToString(TransactionAction action);

struct IMGUI_API ITransaction
{
    virtual ~ITransaction() { }

    virtual void AddAction(TransactionAction action, const char* name) {}
    virtual void Commit() {}
    virtual void Discard() {}

    // Adds action that act on specific node
    virtual void AddAction(TransactionAction action, LinkId linkId, const char* name); // implemented defaults to 'AddAction(action, name)'
    virtual void AddAction(TransactionAction action, NodeId nodeId, const char* name); // implemented defaults to 'AddAction(action, name)'
};

using TransactionConstructor = ITransaction*(*)(const char* name, void* userPointer);  // Create new instance of the transaction
using TransactionDestructor  = void(*)(ITransaction*, void* userPointer);              // Destroys instance if the transaction

struct TransactionInterfaceStruct
{
    TransactionConstructor  Constructor = nullptr;
    TransactionDestructor   Destructor  = nullptr;
    void*                   UserPointer = nullptr;
};


//------------------------------------------------------------------------------
enum class SaveReasonFlags: uint32_t
{
    None       = 0x00000000,
    Navigation = 0x00000001,
    Position   = 0x00000002,
    Size       = 0x00000004,
    Selection  = 0x00000008,
    Node       = 0x00000010,
    Pin        = 0x00000020,
    Link       = 0x00000040,
    // Others
    User       = 0x10000000
};

inline SaveReasonFlags operator |(SaveReasonFlags lhs, SaveReasonFlags rhs) { return static_cast<SaveReasonFlags>(static_cast<uint32_t>(lhs) | static_cast<uint32_t>(rhs)); }
inline SaveReasonFlags operator &(SaveReasonFlags lhs, SaveReasonFlags rhs) { return static_cast<SaveReasonFlags>(static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)); }

using ConfigSaveSettings     = bool   (*)(const char* data, size_t size, SaveReasonFlags reason, void* userPointer);
using ConfigLoadSettings     = size_t (*)(char* data, void* userPointer);

using ConfigSaveNodeSettings = bool   (*)(NodeId nodeId, const char* data, size_t size, SaveReasonFlags reason, void* userPointer);
using ConfigLoadNodeSettings = size_t (*)(NodeId nodeId, char* data, void* userPointer);

using ConfigSaveSettingsJson = bool              (*)(const imgui_json::value&, SaveReasonFlags reason, void* userPointer);
using ConfigLoadSettingsJson = imgui_json::value (*)(void* userPointer);

using ConfigSaveNodeSettingsJson = bool              (*)(NodeId nodeId, const imgui_json::value& value, SaveReasonFlags reason, void* userPointer);
using ConfigLoadNodeSettingsJson = imgui_json::value (*)(NodeId nodeId, void* userPointer);

using ConfigSession          = void   (*)(void* userPointer);

struct Config
{
    using CanvasSizeModeAlias = ax::NodeEditor::CanvasSizeMode;

    const char*                 SettingsFile;
    ConfigSession               BeginSaveSession;
    ConfigSession               EndSaveSession;
    ConfigSaveSettings          SaveSettings;
    ConfigLoadSettings          LoadSettings;
    ConfigSaveNodeSettings      SaveNodeSettings;
    ConfigLoadNodeSettings      LoadNodeSettings;
    ConfigSaveSettingsJson      SaveSettingsJson;
    ConfigLoadSettingsJson      LoadSettingsJson;
    ConfigSaveNodeSettingsJson  SaveNodeSettingsJson;
    ConfigLoadNodeSettingsJson  LoadNodeSettingsJson;
    CanvasSizeModeAlias         CanvasSizeMode;
    void*                       UserPointer;
    int                         DragButtonIndex;        // Mouse button index drag action will react to (0-left, 1-right, 2-middle)
    int                         SelectButtonIndex;      // Mouse button index select action will react to (0-left, 1-right, 2-middle)
    int                         NavigateButtonIndex;    // Mouse button index navigate action will react to (0-left, 1-right, 2-middle)
    int                         ContextMenuButtonIndex; // Mouse button index context menu action will react to (0-left, 1-right, 2-middle)
    int                         EmulateMiddleButton;    // Mouse middle button emulated add By Dicky
    TransactionInterfaceStruct  TransactionInterface;

    Config()
        : SettingsFile(nullptr)
        , BeginSaveSession(nullptr)
        , EndSaveSession(nullptr)
        , SaveSettings(nullptr)
        , LoadSettings(nullptr)
        , SaveNodeSettings(nullptr)
        , LoadNodeSettings(nullptr)
        , SaveSettingsJson(nullptr)
        , LoadSettingsJson(nullptr)
        , SaveNodeSettingsJson(nullptr)
        , LoadNodeSettingsJson(nullptr)
        , CanvasSizeMode(CanvasSizeModeAlias::FitVerticalView)
        , UserPointer(nullptr)
        , DragButtonIndex(0)
        , SelectButtonIndex(0)
        , NavigateButtonIndex(2)
        , ContextMenuButtonIndex(1)
        , EmulateMiddleButton(1)
    {
    }
};


//------------------------------------------------------------------------------
enum class PinKind
{
    Input,
    Output
};

enum class FlowDirection
{
    Forward,
    Backward
};

//------------------------------------------------------------------------------
enum StyleColor
{
    StyleColor_Bg,
    StyleColor_Grid,
    StyleColor_NodeBg,
    StyleColor_NodeBorder,
    StyleColor_HovNodeBorder,
    StyleColor_SelNodeBorder,
    StyleColor_NodeSelRect,
    StyleColor_NodeSelRectBorder,
    StyleColor_HovLinkBorder,
    StyleColor_SelLinkBorder,
    StyleColor_LinkSelRect,
    StyleColor_LinkSelRectBorder,
    StyleColor_PinRect,
    StyleColor_PinRectBorder,
    StyleColor_Flow,
    StyleColor_FlowMarker,
    StyleColor_FlowDMarker,
    StyleColor_GroupBg,
    StyleColor_GroupBorder,

    StyleColor_Count
};

enum StyleVar
{
    StyleVar_NodePadding,
    StyleVar_NodeRounding,
    StyleVar_NodeBorderWidth,
    StyleVar_HoveredNodeBorderWidth,
    StyleVar_SelectedNodeBorderWidth,
    StyleVar_PinRounding,
    StyleVar_PinBorderWidth,
    StyleVar_LinkStrength,
    StyleVar_SourceDirection,
    StyleVar_TargetDirection,
    StyleVar_ScrollDuration,
    StyleVar_FlowMarkerDistance,
    StyleVar_FlowSpeed,
    StyleVar_FlowDuration,
    StyleVar_PivotAlignment,
    StyleVar_PivotSize,
    StyleVar_PivotScale,
    StyleVar_PinCorners,
    StyleVar_PinRadius,
    StyleVar_PinArrowSize,
    StyleVar_PinArrowWidth,
    StyleVar_GroupRounding,
    StyleVar_GroupBorderWidth,

    StyleVar_Count
};

struct Style
{
    ImVec4  NodePadding;
    float   NodeRounding;
    float   NodeBorderWidth;
    float   HoveredNodeBorderWidth;
    float   SelectedNodeBorderWidth;
    float   PinRounding;
    float   PinBorderWidth;
    float   LinkStrength;
    ImVec2  SourceDirection;
    ImVec2  TargetDirection;
    float   ScrollDuration;
    float   FlowMarkerDistance;
    float   FlowSpeed;
    float   FlowDuration;
    ImVec2  PivotAlignment;
    ImVec2  PivotSize;
    ImVec2  PivotScale;
    float   PinCorners;
    float   PinRadius;
    float   PinArrowSize;
    float   PinArrowWidth;
    float   GroupRounding;
    float   GroupBorderWidth;
    ImVec4  Colors[StyleColor_Count];

    Style()
    {
        NodePadding             = ImVec4(8, 8, 8, 8);
        NodeRounding            = 12.0f;
        NodeBorderWidth         = 1.5f;
        HoveredNodeBorderWidth  = 3.5f;
        SelectedNodeBorderWidth = 3.5f;
        PinRounding             = 4.0f;
        PinBorderWidth          = 0.0f;
        LinkStrength            = 200.0f;
        SourceDirection         = ImVec2(1.0f, 0.0f);
        TargetDirection         = ImVec2(-1.0f, 0.0f);
        ScrollDuration          = 0.35f;
        FlowMarkerDistance      = 20.0f;
        FlowSpeed               = 200.0f;
        FlowDuration            = 0.1f;
        PivotAlignment          = ImVec2(0.5f, 0.5f);
        PivotSize               = ImVec2(0.0f, 0.0f);
        PivotScale              = ImVec2(1, 1);
        PinCorners              = ImDrawFlags_RoundCornersAll;
        PinRadius               = 0.0f;
        PinArrowSize            = 0.0f;
        PinArrowWidth           = 0.0f;
        GroupRounding           = 6.0f;
        GroupBorderWidth        = 1.0f;

        Colors[StyleColor_Bg]                 = ImColor( 60,  60,  70,  50);
        Colors[StyleColor_Grid]               = ImColor(120, 120, 120,  40);
        Colors[StyleColor_NodeBg]             = ImColor( 32,  32,  32, 228);
        Colors[StyleColor_NodeBorder]         = ImColor(255, 255, 255,  96);
        Colors[StyleColor_HovNodeBorder]      = ImColor( 50, 176, 255, 255);
        Colors[StyleColor_SelNodeBorder]      = ImColor(255, 176,  50, 255);
        Colors[StyleColor_NodeSelRect]        = ImColor(  5, 130, 255,  64);
        Colors[StyleColor_NodeSelRectBorder]  = ImColor(  5, 130, 255, 128);
        Colors[StyleColor_HovLinkBorder]      = ImColor( 50, 176, 255, 255);
        Colors[StyleColor_SelLinkBorder]      = ImColor(255, 176,  50, 255);
        Colors[StyleColor_LinkSelRect]        = ImColor(  5, 130, 255,  64);
        Colors[StyleColor_LinkSelRectBorder]  = ImColor(  5, 130, 255, 128);
        Colors[StyleColor_PinRect]            = ImColor( 60, 180, 255, 100);
        Colors[StyleColor_PinRectBorder]      = ImColor( 60, 180, 255, 128);
        Colors[StyleColor_Flow]               = ImColor( 64, 128,  64, 255);
        Colors[StyleColor_FlowMarker]         = ImColor(128, 255, 128, 255);
        Colors[StyleColor_GroupBg]            = ImColor(  0,   0,   0, 160);
        Colors[StyleColor_GroupBorder]        = ImColor(255, 255, 255,  32);
    }
};


//------------------------------------------------------------------------------
struct EditorContext;


//------------------------------------------------------------------------------
IMGUI_API void SetCurrentEditor(EditorContext* ctx);
IMGUI_API EditorContext* GetCurrentEditor();
IMGUI_API EditorContext* CreateEditor(const Config* config = nullptr);
IMGUI_API void DestroyEditor(EditorContext* ctx);

IMGUI_API Style& GetStyle();
IMGUI_API const char* GetStyleColorName(StyleColor colorIndex);

IMGUI_API void PushStyleColor(StyleColor colorIndex, const ImVec4& color);
IMGUI_API void PopStyleColor(int count = 1);

IMGUI_API void PushStyleVar(StyleVar varIndex, float value);
IMGUI_API void PushStyleVar(StyleVar varIndex, const ImVec2& value);
IMGUI_API void PushStyleVar(StyleVar varIndex, const ImVec4& value);
IMGUI_API void PopStyleVar(int count = 1);

IMGUI_API void Begin(const char* id, const ImVec2& size = ImVec2(0, 0));
IMGUI_API void End();
IMGUI_API void Update();

IMGUI_API void BeginNode(NodeId id);
IMGUI_API void BeginPin(PinId id, PinKind kind);
IMGUI_API void PinRect(const ImVec2& a, const ImVec2& b);
IMGUI_API void PinPivotRect(const ImVec2& a, const ImVec2& b);
IMGUI_API void PinPivotSize(const ImVec2& size);
IMGUI_API void PinPivotScale(const ImVec2& scale);
IMGUI_API void PinPivotAlignment(const ImVec2& alignment);
IMGUI_API void EndPin();
IMGUI_API void Group(const ImVec2& size);
IMGUI_API void EndNode();

IMGUI_API bool BeginGroupHint(NodeId nodeId);
IMGUI_API ImVec2 GetGroupMin();
IMGUI_API ImVec2 GetGroupMax();
IMGUI_API ImDrawList* GetHintForegroundDrawList();
IMGUI_API ImDrawList* GetHintBackgroundDrawList();
IMGUI_API void EndGroupHint();

// TODO: Add a way to manage node background channels
IMGUI_API ImDrawList* GetNodeBackgroundDrawList(NodeId nodeId);

IMGUI_API bool Link(LinkId id, PinId startPinId, PinId endPinId, const ImVec4& color = ImVec4(1, 1, 1, 1), float thickness = 1.0f);

IMGUI_API void Flow(LinkId linkId, FlowDirection direction = FlowDirection::Forward);

IMGUI_API bool BeginCreate(const ImVec4& color = ImVec4(1, 1, 1, 1), float thickness = 1.0f);
IMGUI_API bool QueryNewLink(PinId* startId, PinId* endId);
IMGUI_API bool QueryNewLink(PinId* startId, PinId* endId, const ImVec4& color, float thickness = 1.0f);
IMGUI_API bool QueryNewNode(PinId* pinId);
IMGUI_API bool QueryNewNode(PinId* pinId, const ImVec4& color, float thickness = 1.0f);
IMGUI_API bool AcceptNewItem();
IMGUI_API bool AcceptNewItem(const ImVec4& color, float thickness = 1.0f);
IMGUI_API void RejectNewItem();
IMGUI_API void RejectNewItem(const ImVec4& color, float thickness = 1.0f);
IMGUI_API void EndCreate();

IMGUI_API bool BeginDelete();
IMGUI_API bool QueryDeletedLink(LinkId* linkId, PinId* startId = nullptr, PinId* endId = nullptr);
IMGUI_API bool QueryDeletedNode(NodeId* nodeId);
IMGUI_API bool AcceptDeletedItem(bool deleteDependencies = true);
IMGUI_API void RejectDeletedItem();
IMGUI_API void EndDelete();

IMGUI_API void SetNodePosition(NodeId nodeId, const ImVec2& editorPosition);
IMGUI_API void SetNodeSize(NodeId nodeId, const ImVec2& size);
IMGUI_API void SetGroupSize(NodeId nodeId, const ImVec2& size);
IMGUI_API void SetNodeChanged(NodeId nodeId);
IMGUI_API void SetPinChanged(PinId pinId);
IMGUI_API void SetLinkChanged(LinkId linkId);
IMGUI_API ImVec2 GetGroupSize(NodeId nodeId);
IMGUI_API ImVec2 GetNodePosition(NodeId nodeId);
IMGUI_API ImVec2 GetNodeSize(NodeId nodeId);
IMGUI_API void CenterNodeOnScreen(NodeId nodeId);
IMGUI_API void SetNodeZPosition(NodeId nodeId, float z); // Sets node z position, nodes with higher value are drawn over nodes with lower value
IMGUI_API float GetNodeZPosition(NodeId nodeId); // Returns node z position, defaults is 0.0f

IMGUI_API void RestoreNodeState(NodeId nodeId);

enum class StateType: int32_t
{
    All,
    Node,
    Nodes,
    Selection,
    View
};

IMGUI_API bool              HasStateChanged(StateType stateType, const imgui_json::value& state); // Returns true if state changed since last call to GetState()/GetStateString()
IMGUI_API bool              HasStateChanged(StateType stateType, NodeId nodeId, const imgui_json::value& state);
IMGUI_API imgui_json::value GetState(StateType stateType); // Return state serialized to json value.
IMGUI_API imgui_json::value GetState(StateType stateType, NodeId nodeId);
IMGUI_API bool              ApplyState(StateType stateType, const imgui_json::value& state); // Applies state serialized to json value.
IMGUI_API bool              ApplyState(StateType stateType, NodeId nodeId, const imgui_json::value& state);

IMGUI_API bool        HasStateChangedString(StateType stateType, const char* state); // Returns true if state changed since last call to GetState()/GetStateString()
IMGUI_API bool        HasStateChangedString(StateType stateType, NodeId nodeId, const char* state);
IMGUI_API const char* GetStateString(StateType stateType); // Returns state serialized to string. String is valid until next call to GetStateString()
IMGUI_API const char* GetStateString(StateType stateType, NodeId nodeId);
IMGUI_API bool        ApplyStateString(StateType stateType, const char* state); // Applies serialized state string to the editor.
IMGUI_API bool        ApplyStateString(StateType stateType, NodeId nodeId, const char* state);

IMGUI_API void Suspend();
IMGUI_API void Resume();
IMGUI_API bool IsSuspended();

IMGUI_API bool IsActive();

IMGUI_API bool HasSelectionChanged();
IMGUI_API int  GetSelectedObjectCount();
IMGUI_API int  GetSelectedNodes(NodeId* nodes, int size);
IMGUI_API int  GetSelectedLinks(LinkId* links, int size);
IMGUI_API int  GetGroupedNodes(std::vector<NodeId>& nodes, NodeId nodeId, ImVec2 expand = {0, 0});
IMGUI_API void SetNodeGroupID(NodeId nodeId, NodeId groupId);
IMGUI_API bool IsNodeSelected(NodeId nodeId);
IMGUI_API bool IsLinkSelected(LinkId linkId);
IMGUI_API void ClearSelection();
IMGUI_API void SelectNode(NodeId nodeId, bool append = false);
IMGUI_API void SelectLink(LinkId linkId, bool append = false);
IMGUI_API void DeselectNode(NodeId nodeId);
IMGUI_API void DeselectLink(LinkId linkId);

IMGUI_API bool DeleteNode(NodeId nodeId); // Marks node for deletion in next BeginDelete() action
IMGUI_API bool DeleteLink(LinkId linkId); // Marks link for deletion in next BeginDelete() action

IMGUI_API bool HasAnyLinks(NodeId nodeId); // Returns true if node has any link connected
IMGUI_API bool HasAnyLinks(PinId pinId); // Return true if pin has any link connected
IMGUI_API int BreakLinks(NodeId nodeId); // Break all links connected to this node
IMGUI_API int BreakLinks(PinId pinId); // Break all links connected to this pin

IMGUI_API void NavigateToContent(float duration = -1);
IMGUI_API void NavigateToSelection(bool zoomIn = false, float duration = -1);
IMGUI_API void NavigateToOrigin(float duration = -1);
IMGUI_API void SetTheme(std::string theme);
IMGUI_API std::string GetTheme();

IMGUI_API bool ShowNodeContextMenu(NodeId* nodeId);
IMGUI_API bool ShowPinContextMenu(PinId* pinId);
IMGUI_API bool ShowLinkContextMenu(LinkId* linkId);
IMGUI_API bool ShowBackgroundContextMenu();

IMGUI_API void EnableShortcuts(bool enable);
IMGUI_API void EnableDragOverBorder(bool enable);
IMGUI_API void TriggerShowMeters();
IMGUI_API bool AreShortcutsEnabled();
IMGUI_API bool AreDragOverBorderEnabled();

IMGUI_API bool BeginShortcut();
IMGUI_API bool AcceptCut();
IMGUI_API bool AcceptCopy();
IMGUI_API bool AcceptPaste();
IMGUI_API bool AcceptDuplicate();
IMGUI_API bool AcceptCreateNode();
IMGUI_API int  GetActionContextSize();
IMGUI_API int  GetActionContextNodes(NodeId* nodes, int size);
IMGUI_API int  GetActionContextLinks(LinkId* links, int size);
IMGUI_API void EndShortcut();

IMGUI_API float GetCurrentZoom();
IMGUI_API ImVec2 GetCurrentOrigin();

IMGUI_API NodeId GetHoveredNode();
IMGUI_API PinId GetHoveredPin();
IMGUI_API LinkId GetHoveredLink();
IMGUI_API NodeId GetDoubleClickedNode();
IMGUI_API PinId GetDoubleClickedPin();
IMGUI_API LinkId GetDoubleClickedLink();
IMGUI_API bool IsBackgroundClicked();
IMGUI_API bool IsBackgroundDoubleClicked();

IMGUI_API bool GetLinkPins(LinkId linkId, PinId* startPinId, PinId* endPinId); // pass nullptr if particular pin do not interest you

IMGUI_API bool PinHadAnyLinks(PinId pinId);

IMGUI_API ImVec2 GetScreenSize();
IMGUI_API ImVec2 ScreenToCanvas(const ImVec2& pos);
IMGUI_API ImVec2 CanvasToScreen(const ImVec2& pos);

IMGUI_API ImVec2 GetViewSize();
IMGUI_API ImRect GetViewRect();

IMGUI_API int GetNodeCount();                                // Returns number of submitted nodes since Begin() call
IMGUI_API int GetOrderedNodeIds(NodeId* nodes, int size);    // Fills an array with node id's in order they're drawn; up to 'size` elements are set. Returns actual size of filled id's.

IMGUI_API void DrawLastLine(bool light = false);

IMGUI_API ImVector<LinkId> FindLinksForNode(NodeId nodeId);

//------------------------------------------------------------------------------
namespace Details {

template <typename T, typename Tag>
struct SafeType
{
    SafeType(T t)
        : m_Value(std::move(t))
    {
    }

    SafeType(const SafeType&) = default;

    template <typename T2, typename Tag2>
    SafeType(
        const SafeType
        <
            typename std::enable_if<!std::is_same<T, T2>::value, T2>::type,
            typename std::enable_if<!std::is_same<Tag, Tag2>::value, Tag2>::type
        >&) = delete;

    SafeType& operator=(const SafeType&) = default;

    explicit operator T() const { return Get(); }

    T Get() const { return m_Value; }

private:
    T m_Value;
};


template <typename Tag>
struct SafePointerType
    : SafeType<uintptr_t, Tag>
{
    static const Tag Invalid;

    using SafeType<uintptr_t, Tag>::SafeType;

    SafePointerType()
        : SafePointerType(Invalid)
    {
    }

    template <typename T = void> explicit SafePointerType(T* ptr): SafePointerType(reinterpret_cast<uintptr_t>(ptr)) {}
    template <typename T = void> T* AsPointer() const { return reinterpret_cast<T*>(this->Get()); }

    explicit operator bool() const { return *this != Invalid; }
};

template <typename Tag>
const Tag SafePointerType<Tag>::Invalid = { 0 };

template <typename Tag>
inline bool operator==(const SafePointerType<Tag>& lhs, const SafePointerType<Tag>& rhs)
{
    return lhs.Get() == rhs.Get();
}

template <typename Tag>
inline bool operator!=(const SafePointerType<Tag>& lhs, const SafePointerType<Tag>& rhs)
{
    return lhs.Get() != rhs.Get();
}

} // namespace Details

struct NodeId final: Details::SafePointerType<NodeId>
{
    using SafePointerType::SafePointerType;
};

struct LinkId final: Details::SafePointerType<LinkId>
{
    using SafePointerType::SafePointerType;
};

struct PinId final: Details::SafePointerType<PinId>
{
    using SafePointerType::SafePointerType;
};


//------------------------------------------------------------------------------
} // namespace Editor
} // namespace ax

#if IMGUI_BUILD_EXAMPLE
namespace ImGui {
IMGUI_API void ShowNodeEditorWindow();
} // namespace ImGui
#endif
//------------------------------------------------------------------------------
# endif // __IMGUI_NODE_EDITOR_H__