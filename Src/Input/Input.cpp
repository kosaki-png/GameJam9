#include "Input.h"

#pragma comment(lib, "dinput8.lib")

// ����}�N��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

// �R���X�g���N�^
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
// �f�X�g���N�^
Input::~Input()
{
    Release(key);
    Release(mouse);
    Release(input);
    delete win;
}

// �C���v�b�g�̐���
HRESULT Input::CreateInput(void)
{
    result = DirectInput8Create(GetModuleHandle(0),
        DIRECTINPUT_VERSION,
        IID_IDirectInput8,
        (void**)(&input),
        NULL);

    return result;
}
// �L�[�f�o�C�X�̐���
HRESULT Input::CreateKey(void)
{
    result = input->CreateDevice(GUID_SysKeyboard, &key, NULL);

    return result;
}
// �L�[�t�H�[�}�b�g�̃Z�b�g
HRESULT Input::SetKeyFormat(void)
{
    result = key->SetDataFormat(&c_dfDIKeyboard);

    return result;
}
// �L�[�̋������x���̃Z�b�g
HRESULT Input::SetKeyCooperative(void)
{
    result = key->SetCooperativeLevel(win->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    //���̓f�o�C�X�ւ̃A�N�Z�X�������擾
    key->Acquire();

    return result;
}

// �}�E�X�f�o�C�X����
HRESULT Input::CreateMouse(void)
{
    result = input->CreateDevice(GUID_SysMouse, &mouse, NULL);
    return result;
}
// �}�E�X�t�H�[�}�b�g�̐ݒ�
HRESULT Input::SetMouseFormat(void)
{
    result = mouse->SetDataFormat(&c_dfDIMouse);

    return result;
}
// �}�E�X�̋������x���̃Z�b�g
HRESULT Input::SetMouseCooperative(void)
{
    result = mouse->SetCooperativeLevel(win->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);

    //���̓f�o�C�X�ւ̃A�N�Z�X�������擾
    mouse->Acquire();

    return result;
}

// �L�[����
bool Input::CheckKey(UINT index)
{
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
    key->GetDeviceState(sizeof(keys), &keys);
    if (keys[index] & 0x80)
    {
        flag = true;
    }
    olds[index] = keys[index];

    return flag;
}
// �g���K�[�̓���
bool Input::TriggerKey(UINT index)
{
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�`�F�b�N�t���O
    bool flag = false;

    //�L�[�����擾
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
    //�L�[�����擾
    mouse->GetDeviceState(sizeof(DIMOUSESTATE), &mouseState);

    return Vector2((float)oldmouseState.lX, (float)oldmouseState.lY);
}
