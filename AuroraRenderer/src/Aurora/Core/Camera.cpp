#include "aurorapch.h"
#include "Camera.h"


namespace Aurora
{
	Camera::Camera()
	{
		position = XMFLOAT3(0.0f, 0.0f, -3.0f);
		target = XMFLOAT3(0.0f, 0.0f, 0.0f);
		up = XMFLOAT3(0.0f, 1.0f, 0.0f);

		forward = XMFLOAT3(0.0f, 0.0f, -1.0f);
		right = XMFLOAT3(1.0f, 0.0f, 0.0f);

		yaw = 0.0f;
		pitch = 0.0f;

		mouseSensitivity = 1.5f;
		movementSpeed = 5.0f;
		zoom = 45.0f;

		projectionType = ProjectionType::Perspective;
		fov = XMConvertToRadians(45.0f);

		aspectRatio = 16.0f / 9.0f;
		nearPlane = 0.1f;
		farPlane = 1000.0f;

		orthoWidth = 10.0f;
		orthoHeight = 10.0f;

		isViewMatrixDirty = true;
		isProjectionMatrixDirty = true;

		XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&viewProjectionMatrix, XMMatrixIdentity());
	}

	Camera::Camera(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& up)
	{
		this->position = position;
		this->target = target;
		this->up = up;

		forward = XMFLOAT3(0.0f, 0.0f, -1.0f);
		right = XMFLOAT3(1.0f, 0.0f, 0.0f);

		yaw = 0.0f;
		pitch = 0.0f;

		mouseSensitivity = 1.5f;
		movementSpeed = 5.0f;
		zoom = 45.0f;

		projectionType = ProjectionType::Perspective;
		fov = XMConvertToRadians(45.0f);

		aspectRatio = 16.0f / 9.0f;
		nearPlane = 0.1f;
		farPlane = 1000.0f;

		orthoWidth = 10.0f;
		orthoHeight = 10.0f;

		isViewMatrixDirty = true;
		isProjectionMatrixDirty = true;

		XMStoreFloat4x4(&viewMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&projectionMatrix, XMMatrixIdentity());
		XMStoreFloat4x4(&viewProjectionMatrix, XMMatrixIdentity());

		UpdateVectors();
	}

	void Camera::UpdateVectors()
	{
		XMVECTOR forward 
	}
}