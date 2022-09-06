#pragma once
#include <DirectXMath.h>
#include "../Base/Window.h"
#include "../Base/GraphicsUtility.h"
#include "../Input/Input.h"

class BaseCamera
{
public:
	BaseCamera() {};
	~BaseCamera() {};

	virtual void Initialize();
	virtual void Update();

	void SetInput(Input* input);

	// プロジェクションマトリックス設定
	void SetProjParam(float fov = 45.0f, float nearZ = 0.1f, float farZ = 100.0f);

	/// <summary>
	/// ビューマトリックスの更新
	/// </summary>
	void UpdateViewMatrix();

	/// <summary>
	/// ビューマトリックス取得
	/// </summary>
	/// <returns>ViewMatrix</returns>
	DirectX::XMMATRIX GetViewMatrix();

	/// <summary>
	/// プロジェクションマトリックス取得
	/// </summary>
	/// <returns>ProjMatrix</returns>
	DirectX::XMMATRIX GetProjMatrix();

	// setter, getter
	void SetEye(Vector3 eye);
	Vector3 GetEye() { return eye; }

	void SetTarget(Vector3 target);
	Vector3 GetTarget() { return target; }

	void SetUp(Vector3 up);
	Vector3 GetUp() { return up; }

	void SetRotation(Vector3 rotation);
	Vector3 GetRotation() { return rotation; }

	void SetLength(float length);
	float GetLength() { return length; }

protected:
	void CalcRotation(Vector3 target, Vector3 eye);

	void CalcTarget(float length, Vector3 rotation);

protected:
	RECT rect;

	// ビューマトリックス用
	Vector3 eye;
	Vector3 target;
	Vector3 up;
	DirectX::XMMATRIX view_matrix;

	// プロジェクションマトリックス用
	float fov;
	float aspect;
	float nearZ;
	float farZ;
	DirectX::XMMATRIX proj_matrix;

	bool changed = false;

	float PI = 3.141592f;

	Input* input = nullptr;

	float length = 0;
	Vector3 rotation;
};

