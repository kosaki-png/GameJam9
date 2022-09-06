#include "BaseCamera.h"

void BaseCamera::Initialize()
{
    HWND window_handle = FindWindow(Window::ClassName, nullptr);
    GetClientRect(window_handle, &rect);

    // �r���[�}�g���b�N�X�p
    eye = Vector3(0.0f, 3.0f, -5.0f);
    target = Vector3(0.0f, 0.0f, 0.0f);
    up = Vector3(0.0f, 1.0f, 0.0f);
    // �r���[�}�g���b�N�X�č\��
    UpdateViewMatrix();

    // �v���W�F�N�V�����}�g���b�N�X�p
    fov = DirectX::XMConvertToRadians(70.0f);
    aspect = (float)(rect.right - rect.left) / (rect.bottom - rect.top);
    nearZ = 0.1f;
    farZ = 100.0f;
    // �v���W�F�N�V�����}�g���b�N�X�č\��
    proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

    // �����v�Z
    length = sqrt(pow(target.X - eye.X, 2) + pow(target.Y - eye.Y, 2) + pow(target.Z - eye.Z, 2));

    // �^�[�Q�b�g�ƃJ�������W����p�x���Z�o
    CalcRotation(target, eye);

    rotation;
}

void BaseCamera::Update()
{
    if (changed)
    {
        UpdateViewMatrix();
    }
}

void BaseCamera::SetInput(Input* input)
{
    this->input = input;
}

void BaseCamera::SetProjParam(float fov, float nearZ, float farZ)
{
    this->fov = DirectX::XMConvertToRadians(fov);
    this->nearZ = nearZ;
    this->farZ = farZ;

    // �v���W�F�N�V�����}�g���b�N�X�č\��
    proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);
}

DirectX::XMMATRIX BaseCamera::GetViewMatrix()
{
    return view_matrix;
}

DirectX::XMMATRIX BaseCamera::GetProjMatrix()
{
    return proj_matrix;
}

void BaseCamera::UpdateViewMatrix()
{
    // XMVECTOR�ɕϊ�
    DirectX::XMVECTOR XMeye = DirectX::XMVectorSet(eye.X, eye.Y, eye.Z, 0.0f);
    DirectX::XMVECTOR XMtarget = DirectX::XMVectorSet(target.X, target.Y, target.Z, 0.0f);
    DirectX::XMVECTOR XMup = DirectX::XMVectorSet(up.X, up.Y, up.Z, 0.0f);

    // �r���[�}�g���b�N�X�č\��
    view_matrix = DirectX::XMMatrixLookAtLH(XMeye, XMtarget, XMup);

    changed = false;
}

void BaseCamera::SetEye(Vector3 eye)
{
    this->eye = eye;

    // �^�[�Q�b�g�ƃJ�������W����p�x���Z�o
    CalcRotation(target, eye);

    changed = true;
}

void BaseCamera::SetTarget(Vector3 target)
{
    this->target = target;

    // �^�[�Q�b�g�ƃJ�������W����p�x���Z�o
    CalcRotation(target, eye);

    changed = true;
}

void BaseCamera::SetUp(Vector3 up)
{
    this->up = up;

    changed = true;
}

void BaseCamera::SetRotation(Vector3 rotation)
{
    this->rotation = rotation;

    CalcTarget(length, rotation);

    changed = true;
}

void BaseCamera::SetLength(float length)
{
    this->length = length;

    CalcTarget(length, rotation);

    changed = true;
}

void BaseCamera::CalcRotation(Vector3 target, Vector3 eye)
{
    rotation.X = atan2(target.X - eye.X, target.Z - eye.Z);
    rotation.X = rotation.X * 180.0f / PI - 90.0f;

    rotation.Y = atan2(target.Y - eye.Y, target.Z - eye.Z);
    rotation.Y = -rotation.Y * 108.0f / PI;
}

void BaseCamera::CalcTarget(float length, Vector3 rotation)
{
    float sinX = sinf(-rotation.X * PI / 180.0f);
    float cosX = cosf(-rotation.X * PI / 180.0f);
    float sinY = sinf(-rotation.Y * PI / 180.0f);
    float cosY = cosf(-rotation.Y * PI / 180.0f);

    target = Vector3(cosX * cosY * length + eye.X,
                     sinY        * length + eye.Y,
                     sinX * cosY * length + eye.Z);
}
