#include "BaseCamera.h"

void BaseCamera::Initialize()
{
    HWND window_handle = FindWindow(Window::ClassName, nullptr);
    GetClientRect(window_handle, &rect);

    // ビューマトリックス用
    eye = Vector3(0.0f, 3.0f, -5.0f);
    target = Vector3(0.0f, 0.0f, 0.0f);
    up = Vector3(0.0f, 1.0f, 0.0f);
    // ビューマトリックス再構成
    UpdateViewMatrix();

    // プロジェクションマトリックス用
    fov = DirectX::XMConvertToRadians(70.0f);
    aspect = (float)(rect.right - rect.left) / (rect.bottom - rect.top);
    nearZ = 0.1f;
    farZ = 100.0f;
    // プロジェクションマトリックス再構成
    proj_matrix = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, nearZ, farZ);

    // 距離計算
    length = sqrt(pow(target.X - eye.X, 2) + pow(target.Y - eye.Y, 2) + pow(target.Z - eye.Z, 2));

    // ターゲットとカメラ座標から角度を算出
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

    // プロジェクションマトリックス再構成
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
    // XMVECTORに変換
    DirectX::XMVECTOR XMeye = DirectX::XMVectorSet(eye.X, eye.Y, eye.Z, 0.0f);
    DirectX::XMVECTOR XMtarget = DirectX::XMVectorSet(target.X, target.Y, target.Z, 0.0f);
    DirectX::XMVECTOR XMup = DirectX::XMVectorSet(up.X, up.Y, up.Z, 0.0f);

    // ビューマトリックス再構成
    view_matrix = DirectX::XMMatrixLookAtLH(XMeye, XMtarget, XMup);

    changed = false;
}

void BaseCamera::SetEye(Vector3 eye)
{
    this->eye = eye;

    // ターゲットとカメラ座標から角度を算出
    CalcRotation(target, eye);

    changed = true;
}

void BaseCamera::SetTarget(Vector3 target)
{
    this->target = target;

    // ターゲットとカメラ座標から角度を算出
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
