#include "Input.h"

#pragma comment(lib, "dinput8.lib")

// 解放マクロ
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// コンストラクタ
Input::Input(Window* win) :
    win(win), result(S_OK), input(nullptr), key(nullptr)
{
    memset(&keys, 0, sizeof(keys));
    memset(&olds, 0, sizeof(olds));

    memset(&mouseState, 0, sizeof(mouseState));
    memset(&oldmouseState, 0, sizeof(oldmouseState));

    CreateInput();
    CreateKey();
    SetKeyFormat();
    SetKeyCooperative();

    CreateMouse();
    SetMouseFormat();
    SetMouseCooperative();
}
// デストラクタ
Input::~Input()
{
    Release(key);
    Release(mouse);
    Release(input);
    delete win;
}

// インプットの生成
HRESULT Input::CreateInput(void)
{
    result = DirectInput8Create(GetModuleHandle(0),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)(&input),
        NULL);

    return result;
}
// キーデバイスの生成
HRESULT Input::CreateKey(void)
{
    result = input->CreateDevice(GUID_SysKeyboard, &key, NULL);

    return result;
}
// キーフォーマットのセット
HRESULT Input::SetKeyFormat(void)
{
    result = key->SetDataFormat(&c_dfDIKeyboard);

    return result;
}
// キーの協調レベルのセット
HRESULT Input::SetKeyCooperative(void)
{
    result = key->SetCooperativeLevel(win->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    //入力デバイスへのアクセス権利を取得
    key->Acquire();

    return result;
}

// マウスデバイス生成
HRESULT Input::CreateMouse(void)
{
    result = input->CreateDevice(GUID_SysMouse, &mouse, NULL);
    return result;
}
// マウスフォーマットの設定
HRESULT Input::SetMouseFormat(void)
{
    result = mouse->SetDataFormat(&c_dfDIMouse);

    return result;
}
// マウスの協調レベルのセット
HRESULT Input::SetMouseCooperative(void)
{
    result = mouse->SetCooperativeLevel(win->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    //入力デバイスへのアクセス権利を取得
    mouse->Acquire();

    return result;
}

// キー入力
bool Input::CheckKey(UINT index)
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    key->GetDeviceState(sizeof(keys), &keys);
    if (keys[index] & 0x80)
    {
        flag = true;
    }
    olds[index] = keys[index];

    return flag;
}
// トリガーの入力
bool Input::TriggerKey(UINT index)
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    key->GetDeviceState(sizeof(keys), &keys);
    if ((keys[index] & 0x80) && !(olds[index] & 0x80))
    {
        flag = true;
    }
    olds[index] = keys[index];

    return flag;
}

bool Input::CheckMouseLeft()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if (mouseState.rgbButtons[0])
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}
bool Input::TriggerMouseLeft()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if ((mouseState.rgbButtons[0]) && !(oldmouseState.rgbButtons[0]))
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}

bool Input::CheckMouseLight()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if (mouseState.rgbButtons[1])
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}

bool Input::TriggerMouseLight()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if ((mouseState.rgbButtons[1]) && !(oldmouseState.rgbButtons[1]))
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}

bool Input::CheckMouseMiddle()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if (mouseState.rgbButtons[2])
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}

bool Input::TriggerMouseMiddle()
{
    //チェックフラグ
    bool flag = false;

    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);
    if ((mouseState.rgbButtons[2]) && !(oldmouseState.rgbButtons[2]))
    {
        flag = true;
    }
    oldmouseState = mouseState;

    return flag;
}

Vector2 Input::GetMouseVel()
{
    //キー情報を取得
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

    return Vector2((float)oldmouseState.lX, (float)oldmouseState.lY);
}
