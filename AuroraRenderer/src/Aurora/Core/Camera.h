#pragma once

#include "aurorapch.h"
#include <DirectXMath.h>

#include "Time.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;

namespace Aurora
{
	enum class ProjectionType
	{
		Perspective,
		Orthographic
	};

	class Camera
	{

	public:

		Camera();
		Camera(const Vector3& position, const Vector3& target, const Vector3& up = Vector3(0.0f, 1.0f, 0.0f));

		~Camera();

		void SetPosition(const Vector3& position);
		void SetTarget(const Vector3& target);
		void SetUp(const Vector3& up);

		void SetRotation(float yaw, float pitch);

		void SetPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
		void SetOrthographicProjection(float width, float height, float nearPlane, float farPlane);
		void SetProjectionType(ProjectionType type);

		void SetMouseSensitivity(float sensitivity);
		void SetMovementSpeed(float speed);

		void SetZoom(float zoom);
		
		void MoveForward(int direction);
		void MoveRight(int direction);
		void MoveUp(int direction);

		void Rotate(float deltaX, float deltaY);

		void Zoom(float delta);

		void UpdateVectors();

		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void UpdateViewProjectionMatrix();

		const Vector3& GetPosition() const { return position; }
		const Vector3& GetTarget() const { return target; }
		const Vector3& GetUp() const { return up; }
		const Vector3& GetForward() const { return forward; }
		const Vector3& GetRight() const { return right; }

		const Matrix& GetViewMatrix();
		const Matrix& GetProjectionMatrix();
		const Matrix& GetViewProjectionMatrix();

	private:

		Vector3 position;
		Vector3 target;
		Vector3 up;

		Vector3 forward;
		Vector3 right;

		Matrix viewMatrix;
		Matrix projectionMatrix;
		Matrix viewProjectionMatrix;

		float yaw;
		float pitch;

		float mouseSensitivity;
		float movementSpeed;

		float zoom;

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