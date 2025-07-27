#include "aurorapch.h"
#include "Camera.h"



namespace Aurora
{
	Camera::Camera()
	{
		position = Vector3(0.0f, 0.0f, -3.0f);
		target = Vector3(0.0f, 0.0f, 0.0f);
		up = Vector3(0.0f, 1.0f, 0.0f);

		forward = Vector3(0.0f, 0.0f, -1.0f);
		right = Vector3(1.0f, 0.0f, 0.0f);

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

		viewMatrix = Matrix::Identity;
		projectionMatrix = Matrix::Identity;
		viewProjectionMatrix = Matrix::Identity;
	}

	Camera::Camera(const Vector3& position, const Vector3& target, const Vector3& up)
	{
		this->position = position;
		this->target = target;
		this->up = up;

		forward = Vector3(0.0f, 0.0f, -1.0f);
		right = Vector3(1.0f, 0.0f, 0.0f);

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

		viewMatrix = Matrix::Identity;
		projectionMatrix = Matrix::Identity;
		viewProjectionMatrix = Matrix::Identity;

		UpdateVectors();
	}

	Camera::~Camera()
	{

	}

	void Camera::SetPosition(const Vector3& position)
	{
		this->position = position;
		isViewMatrixDirty = true;
	}

	void Camera::SetTarget(const Vector3& target)
	{
		this->target = target;
		isViewMatrixDirty = true;
	}

	void Camera::SetUp(const Vector3& up)
	{
		this->up = up;
		isViewMatrixDirty = true;
	}

	void Camera::SetRotation(float yaw, float pitch)
	{
		this->yaw = yaw;
		this->pitch = pitch;

		if (this->pitch > XM_PIDIV2 - 0.1f)
			this->pitch = XM_PIDIV2 - 0.1f;
		if (this->pitch < -XM_PIDIV2 + 0.1f)
			this->pitch = -XM_PIDIV2 + 0.1f;

		UpdateVectors();
		isViewMatrixDirty = true;
	}

	void Camera::SetPerspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
	{
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;

		projectionType = ProjectionType::Perspective;
		isProjectionMatrixDirty = true;
	}

	void Camera::SetOrthographicProjection(float width, float height, float nearPlane, float farPlane)
	{
		this->orthoWidth = width;
		this->orthoHeight = height;
		this->nearPlane = nearPlane;
		this->farPlane = farPlane;

		projectionType = ProjectionType::Orthographic;
		isProjectionMatrixDirty = true;
	}

	void Camera::SetProjectionType(ProjectionType type)
	{
		projectionType = type;
		isProjectionMatrixDirty = true;
	}

	void Camera::SetMouseSensitivity(float sensitivity)
	{
		mouseSensitivity = sensitivity;
	}

	void Camera::SetMovementSpeed(float speed)
	{
		movementSpeed = speed;
	}

	void Camera::SetZoom(float zoom)
	{
		this->zoom = std::clamp(zoom, 1.0f, 120.0f);

		if (projectionType == ProjectionType::Perspective)
		{
			fov = XMConvertToRadians(zoom);
			isProjectionMatrixDirty = true;
		}
		else if (projectionType == ProjectionType::Orthographic)
		{
			orthoWidth = zoom;
			orthoHeight = zoom * aspectRatio;
			isProjectionMatrixDirty = true;
		}
	}

	void Camera::MoveForward(int direction)
	{
		float velocity = direction * movementSpeed * Time::DeltaTime();
		position += forward * velocity;
		isViewMatrixDirty = true;
	}

	void Camera::MoveRight(int direction)
	{
		float velocity = direction * movementSpeed * Time::DeltaTime();
		position += forward * velocity;
		isViewMatrixDirty = true;
	}

	void Camera::MoveUp(int direction)
	{
		float velocity = direction * movementSpeed * Time::DeltaTime();
		position += up * velocity;
		isViewMatrixDirty = true;
	}

	void Camera::Rotate(float deltaYaw, float deltaPitch)
	{
		yaw += deltaYaw * mouseSensitivity * Time::DeltaTime();
		pitch += deltaPitch * mouseSensitivity * Time::DeltaTime();

		if (pitch > XM_PIDIV2 - 0.1f)
			pitch = XM_PIDIV2 - 0.1f;
		if (pitch < -XM_PIDIV2 + 0.1f)
			pitch = -XM_PIDIV2 + 0.1f;

		UpdateVectors();
		isViewMatrixDirty = true;
	}

	void Camera::Zoom(float delta)
	{
		SetZoom(zoom - delta);
	}

	void Camera::UpdateVectors()
	{
		forward = Vector3(cos(yaw) * cos(pitch), sin(pitch), sin(yaw) * cos(pitch));

		right = forward.Cross(Vector3::Up);
		right.Normalize();

		up = right.Cross(forward);
		up.Normalize();

		target = position + forward;
	}

	void Camera::UpdateViewMatrix()
	{
		viewMatrix = Matrix::CreateLookAt(position, target, up);
	}

	void Camera::UpdateProjectionMatrix()
	{
		projectionMatrix = (projectionType == ProjectionType::Perspective) ? Matrix::CreatePerspectiveFieldOfView(fov, aspectRatio, nearPlane, farPlane) : Matrix::CreateOrthographic(orthoWidth, orthoHeight, nearPlane, farPlane);
	}

	void Camera::UpdateViewProjectionMatrix()
	{
		viewProjectionMatrix = viewMatrix * projectionMatrix;
	}

	const Matrix& Camera::GetViewMatrix()
	{
		if (isViewMatrixDirty)
		{
			UpdateViewMatrix();
			isViewMatrixDirty = false;
		}
		return viewMatrix;
	}

	const Matrix& Camera::GetProjectionMatrix()
	{
		if (isProjectionMatrixDirty)
		{
			UpdateProjectionMatrix();
			isProjectionMatrixDirty = false;
		}
		return projectionMatrix;
		
	}

	const Matrix& Camera::GetViewProjectionMatrix()
	{
		if (isViewMatrixDirty || isProjectionMatrixDirty)
		{
			UpdateViewProjectionMatrix();
			isViewMatrixDirty = false;
			isProjectionMatrixDirty = false;
		}
		return viewProjectionMatrix;
	}
}