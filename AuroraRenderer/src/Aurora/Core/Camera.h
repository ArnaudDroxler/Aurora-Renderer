#pragma once

#include "aurorapch.h"
#include <DirectXMath.h>

using namespace DirectX;

namespace Aurora
{
	class Camera
	{

	public:

		Camera();
		Camera(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& up = XMFLOAT3(0.0f, 1.0f, 0.0f));


	private:

		void UpdateVectors();

		XMFLOAT3 position;
		XMFLOAT3 target;
		XMFLOAT3 up;

		XMFLOAT3 forward;
		XMFLOAT3 right;

		XMFLOAT4X4 viewMatrix;
		XMFLOAT4X4 projectionMatrix;
		XMFLOAT4X4 viewProjectionMatrix;

		float yaw;
		float pitch;

		float mouseSensitivity;
		float movementSpeed;

		float zoom;

		enum class ProjectionType
		{
			Perspective,
			Orthographic
		};

		ProjectionType projectionType;

		float fov;
		float aspectRatio;
		float nearPlane;
		float farPlane;

		float orthoWidth;
		float orthoHeight;

		bool isViewMatrixDirty;
		bool isProjectionMatrixDirty;
	};

}