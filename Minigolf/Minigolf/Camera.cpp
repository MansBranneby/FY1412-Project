#include "Camera.h"

void Camera::handleFreeCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds)
{
	_velocity = { 0.0f, 0.0f, 0.0f };
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0001f, 0.0f);

	if (ms.positionMode == Mouse::MODE_RELATIVE)
	{
		_yaw += XMConvertToRadians((float)ms.x * deltaSeconds * 2000);
		_pitch += XMConvertToRadians((float)ms.y * deltaSeconds * 2000);
		_pitch = min(XM_PI / 2.0f - 0.0001f, max(-XM_PI / 2.0f + 0.0001f, _pitch));
	}

	_rotation = XMMatrixRotationRollPitchYaw(_pitch, _yaw, 0.0f);
	_right = XMVector3TransformCoord(_right, _rotation);
	_up = XMVector3TransformCoord(_up, _rotation);
	_forward = XMVector3TransformCoord(_forward, _rotation);
	_lookAt = XMVector3TransformCoord(_lookAt, _rotation);

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_velocity.z = _distancePerSec * deltaSeconds;
	if (kb.S) //Backwards
		_velocity.z = -_distancePerSec * deltaSeconds;
	if (kb.A)	//Left
		_velocity.x = -_distancePerSec * deltaSeconds;
	if (kb.D)	//Right
		_velocity.x = _distancePerSec * deltaSeconds;
	if (kb.Space) //Up
		_velocity.y = _distancePerSec * deltaSeconds;
	if (kb.LeftControl) //Down
		_velocity.y = -_distancePerSec * deltaSeconds;
	
	// Update position
	_position += _velocity.x * _right;
	_position += _velocity.y * _up;
	_position += _velocity.z * _forward;
	// Update where the camera is looking;
	_lookAt += _position;
}

void Camera::handleOrbitCam(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds, DirectX::XMVECTOR objPosition)
{
	if (ms.positionMode == Mouse::MODE_RELATIVE)
	{
		_phi -= XMConvertToRadians((float)ms.y * deltaSeconds * 500);
		_theta -= XMConvertToRadians((float)ms.x * deltaSeconds * 500);
	}

	//Get current state of keyboard, mouse and gamepad, update the cameras position based on this input.
	if (kb.W) //Forward
		_phi -= _rotationGain * deltaSeconds;
	if (kb.S) //Backwards
		_phi += _rotationGain * deltaSeconds;
	if (kb.A) //Left
		_theta += _rotationGain * deltaSeconds;
	if (kb.D) //Right
		_theta -= _rotationGain * deltaSeconds;

	if (_phi <= 0.0f)
		_phi = 0.01f;
	if (_phi >= XM_PI * 0.5f)
		_phi = XM_PI * 0.5f;

	// Calculate position based on spherical coordinates

	float x = cos(_theta) * sin(_phi);
	float y = cos(_phi);
	float z = sin(_theta) * sin(_phi);

	_posOffset = { _camDistance * x, _camDistance * y, _camDistance * z, 1.0f };
	followObject(objPosition, deltaSeconds);
}

void Camera::followObject(DirectX::XMVECTOR objPosition, float deltaSeconds)
{
	//Smooth camera
	_lookAt	  = XMVectorLerp(_lookAt, objPosition, _lookAtSpeed * deltaSeconds);
	_position = XMVectorLerp(_position, objPosition + _posOffset, deltaSeconds * _smoothSpeed);
}

float Camera::getPhi()
{
	return _phi;
}

Camera::Camera()
{
}

Camera::Camera(ID3D11Device* device, float width, float height)
{
	// Default mode for camera
	_cameraMode = GAME;

	// Setup starting camera properties

	// Base vectors
	_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	_right = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
	_forward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Where camera should look
	_lookAt = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// Camera velocity
	_velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

	// Camera speed in moving direction
	_distancePerSec = 5.0f;

	// Distance from camera
	_camDistance = 2.5f;

	// Rotation
	_yaw = 0;
	_pitch = 0;
	_theta = -XM_PI / 2;
	_phi = XM_PI / 4;
	_rotationGain = 2.0f;

	//Smoothspeed of camera, dictates how fast the it will interpolate
	_smoothSpeed = 10.0f;
	_lookAtSpeed = 10.0f;

	float x = cos(_theta) * sin(_phi);
	float y = cos(_phi);
	float z = sin(_theta)* sin(_phi);

	_position = { _camDistance * x, _camDistance * y, _camDistance * z, 1.0f };
	_posOffset = { _camDistance * x, _camDistance * y, _camDistance * z, 1.0f };

	// Setup space matricies
	//_world = XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	_view = XMMatrixLookAtLH(_position, _lookAt, _up);
	_projection = XMMatrixPerspectiveFovLH(0.45f * DirectX::XM_PI, width / height, 0.1f, 200.0f);
	_view = XMMatrixTranspose(_view);
	_projection = XMMatrixTranspose(_projection);
	_viewProjection = XMMatrixMultiply(_projection, _view);

	_viewFrustum = ViewFrustum(_viewProjection);
}

Camera::~Camera()
{
}

void Camera::updateRotation(float deltaAngle)
{
}

void Camera::setVelocity(XMFLOAT3 velocity)
{
	_velocity = velocity;
}

XMFLOAT3 Camera::getVelocity() const
{
	return _velocity;
}

ViewFrustum Camera::getViewFrustum()
{
	return _viewFrustum;
}

void Camera::setMode(cameraMode cameraMode)
{
	_cameraMode = cameraMode;
}

void Camera::setSmoothSpeed(float smoothSpeed)
{
	_smoothSpeed = smoothSpeed;
}

void Camera::setLookAtSpeed(float lookAtSpeed)
{
	_lookAtSpeed = lookAtSpeed;
}

void Camera::setCamDistance(float camDistance)
{
	_camDistance = camDistance;
}

DirectX::XMVECTOR Camera::getPosition()
{
	return _position;
}

DirectX::XMMATRIX Camera::getViewProjection()
{
	return _viewProjection;
}

float Camera::getSmoothSpeed()
{
	return _smoothSpeed;
}

float Camera::getLookAtSpeed()
{
	return _lookAtSpeed;
}

float Camera::getCamDistance()
{
	return _camDistance;
}

void Camera::handleInput(DirectX::Keyboard::State kb, DirectX::Mouse::State ms, float deltaSeconds, DirectX::XMVECTOR objPosition)
{
	if (_cameraMode == DEBUG)
	{
		// FIRST PERSON CAM
		handleFreeCam(kb, ms, deltaSeconds);
	}
	else if (_cameraMode == GAME)
	{
		// ORBIT CAM
		handleOrbitCam(kb, ms, deltaSeconds, objPosition);
	}

	//Update camera matrices	
	_view = XMMatrixLookAtLH(_position, _lookAt, _up);
	_view = XMMatrixTranspose(_view);
	_viewProjection = XMMatrixMultiply(_projection, _view);
	_viewFrustum = ViewFrustum(_viewProjection);
}
