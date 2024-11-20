#pragma once

#define NOMINMAX
#include <windows.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <iterator>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

// Extra

// Own classes
#include "Game.h"
#include "GraphicResources.h"
#include "Camera.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputController.h"
#include "Clock.h"
#include "Model.h"
#include "GameState.h"
#include "Light.h"
#include "ConstantBuffer.h"

// DirectXTK
#include "CommonStates.h"
#include "DDSTextureLoader.h"
#include "DirectXHelpers.h"
#include "Effects.h"
#include "GamePad.h"
#include "GeometricPrimitive.h"
#include "GraphicsMemory.h"
#include "Keyboard.h"
#include "Model.h"
#include "Mouse.h"
#include "PostProcess.h"
#include "PrimitiveBatch.h"
#include "ScreenGrab.h"
#include "SimpleMath.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "VertexTypes.h"
#include "WICTextureLoader.h"
using namespace DirectX;

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

HWND InitWindow(HINSTANCE hInstance);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// GLOBALS //
Game* gGame = nullptr;
GraphicResources* gGR = nullptr;

// CONTROLLER //
InputController* gInputCtrl;

// CLOCK //
Clock* gClock;

// STATES
GameState gGameState;

// IMGUI VARIABLES //
bool drawBoundingVolume = false;

void initializeResources(HWND wndHandle)
{
	//GRAPHIC RESOURCES
	gGR = new GraphicResources(wndHandle);

	//INPUT CONTROLLER
	gInputCtrl = new InputController(wndHandle);

	//Clock
	gClock = new Clock();
	//GAME
	gGame = new Game(gClock, gInputCtrl, gGR);
	gGame->pushState(&gGameState); // Change which state the game is in, not done yet.

	Light lit = Light(gGR->getDevice(), gGR->getDeviceContext(), XMFLOAT4(-50.0f, -100.0f, -100.0f, 0.0f), XMFLOAT4(1.0, 1.0f, 1.0f, 0.0f));
	gGR->getDeviceContext()->PSSetConstantBuffers(0, 1, ConstantBuffer(gGR->getDevice(), &lit, sizeof(lit)).getConstantBuffer());

	//IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	ImGui_ImplWin32_Init(wndHandle);
	ImGui_ImplDX11_Init(gGR->getDevice(), gGR->getDeviceContext());
	ImGui::StyleColorsDark();
}

void imGuiUpdate()
{
	Player* player = gGame->getLevelHandler()->getPlayer();
	static const char* clubNames[]{ "WOOD_1", "WOOD_3", "WOOD_5", "IRON_2", "IRON_3", "IRON_4", "IRON_5", "IRON_6", "IRON_7", "IRON_8", "IRON_9", "P_WEDGE", "S_WEDGE", "PUTTER" };
	static int clubChoice = 0;
	DirectX::XMFLOAT3 playerPos = player->getBall()->getPositionFloat3();
	DirectX::XMFLOAT3 angularVelocity;
	DirectX::XMFLOAT3 velocity;
	DirectX::XMStoreFloat3(&velocity, player->getBall()->getVelocity());
	DirectX::XMStoreFloat3(&angularVelocity, player->getBall()->getAngularVelocity());
	float angularVelocity2 = DirectX::XMVectorGetX(DirectX::XMVector3Length(player->getBall()->getVelocity())) / 0.0214f;
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Mini golf...?");
	if (ImGui::CollapsingHeader("Game instructions", 2))
	{
		ImGui::Text("RightMouse: rotate camera");
		ImGui::Text("WASD      : rotate camera");
		ImGui::Text("LeftMouse : club speed");
		ImGui::Text("Spacebar  : swing club");
	}
	if (ImGui::CollapsingHeader("Camera", 2))
	{
		float camDistance = gGame->getCamera()->getCamDistance();
		float SmoothSpeed = gGame->getCamera()->getSmoothSpeed();
		float LookAtSpeed = gGame->getCamera()->getLookAtSpeed();

		ImGui::SliderFloat("CamDistance", &camDistance, 1.0f, 20.0f);
		ImGui::SliderFloat("Smoothspeed", &SmoothSpeed, 0.0f, 20.0f);
		ImGui::SliderFloat("LookAtSpeed", &LookAtSpeed, 0.0f, 20.0f);

		gGame->getCamera()->setCamDistance(camDistance);
		gGame->getCamera()->setSmoothSpeed(SmoothSpeed);
		gGame->getCamera()->setLookAtSpeed(LookAtSpeed);

	}

	if (ImGui::CollapsingHeader("Player", 2))
	{
		if (ImGui::Button("Reset position"))
		{
			player->getBall()->setVelocity(DirectX::XMVECTOR{ 0.0, 0.0f, 0.0f });
			player->getBall()->setAngularVelocity(DirectX::XMVECTOR{ 0.0, 0.0f, 0.0f });
			player->getBall()->setPosition(DirectX::XMVECTOR{ -1.0, 10.0f, 1.0f });
			player->getBall()->setPrevPos(DirectX::XMVECTOR{ -1.0, 10.0f, 1.0f });
			player->getBall()->setBoundingPrevPos(DirectX::XMVECTOR{ -1.0, 10.0f, 1.0f });
			player->getBall()->setBoundingPos(DirectX::XMVECTOR{ -1.0, 10.0f, 1.0f });
			player->getBall()->setMeansOfMovement(PROJECTILE);
		}

		const char* meansofMovement = "";
		switch (player->getBall()->getMeansofMovement())
		{
		case PROJECTILE:
			meansofMovement = "PROJECTILE";
			break;
		case GLIDING:
			meansofMovement = "GLIDING";
			break;
		case ROLLING:
			meansofMovement = "ROLLING";
			break;
		case REST:
			meansofMovement = "REST";
			break;
		default:
			break;
		}

		ImGui::Text("State: %s", meansofMovement);
		ImGui::Text("Position   : X: %.2f, Y: %.2f, Z: %.2f", playerPos.x, playerPos.y, playerPos.z);
		ImGui::Text("Velocity   : X: %.2f, Y: %.2f, Z: %.2f", velocity.x, velocity.y, velocity.z);
		ImGui::Text("AngularVel : X: %.2f, Y: %.2f, Z: %.2f", angularVelocity.x, angularVelocity.y, angularVelocity.z);
		ImGui::Text("AngularVel2: %.2f", angularVelocity2);
		ImGui::Text("Club speed: %.2f m/s", gGame->getLevelHandler()->getPlayer()->getClubSpeed());
		ImGui::Combo("Selected club", &clubChoice, clubNames, 14);
		player->setClubChoice((ClubType)clubChoice);
	}
	if (ImGui::CollapsingHeader("Scoreboard", 32))
	{
		const char* meansofMovement = "";
		switch (player->getBall()->getMeansofMovement())
		{
		case PROJECTILE:
			meansofMovement = "PROJECTILE";
			break;
		case GLIDING:
			meansofMovement = "GLIDING";
			break;
		case ROLLING:
			meansofMovement = "ROLLING";
			break;
		case REST:
			meansofMovement = "REST";
			break;
		default:
			break;
		}
		ImGui::Text("Ball: %s", meansofMovement);
		ImGui::Text("Score     : %d", player->getNrOfHits());
		ImGui::Text("Prev Score: %d", player->getPrevNrOfHits());
		ImGui::Text("Club speed: %.2f m/s", gGame->getLevelHandler()->getPlayer()->getClubSpeed());
		ImGui::Combo("Selected club", &clubChoice, clubNames, 14);
		player->setClubChoice((ClubType)clubChoice);
	}


	ImGui::Text("Framerate: %.f", ImGui::GetIO().Framerate);
	ImGui::End();
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void update()
{
	gGame->handleInput();
	gGame->update();
}


void render()
{
	// THIS HAS BEEN MOVED TO Level.cpp
	//gGR->getDeviceContext()->VSSetShader(&gGR->getShaderHandler()->getObjectVS()->getVertexShader(), nullptr, 0);
	//gGR->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	//gGR->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	//gGR->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	//gGR->getDeviceContext()->PSSetShader(&gGR->getShaderHandler()->getObjectPS()->getPixelShader(), nullptr, 0);

	//gGR->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//gGR->getDeviceContext()->IASetInputLayout(&gGR->getShaderHandler()->getObjectVS()->getVertexLayout());
	//gGR->getDeviceContext()->PSSetSamplers(0, 1, gGR->getSamplerState());

	gGame->draw();

	gGR->getDeviceContext()->VSSetShader(&gGR->getShaderHandler()->getBoundingVolumeVS()->getVertexShader(), nullptr, 0);
	gGR->getDeviceContext()->HSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->DSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->GSSetShader(nullptr, nullptr, 0);
	gGR->getDeviceContext()->PSSetShader(&gGR->getShaderHandler()->getBoundingVolumePS()->getPixelShader(), nullptr, 0);

	gGR->getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
	gGR->getDeviceContext()->IASetInputLayout(&gGR->getShaderHandler()->getBoundingVolumeVS()->getVertexLayout());
	gGR->getDeviceContext()->PSSetSamplers(0, 0, nullptr);

	if (drawBoundingVolume == true)
	{
		// Loop through all the objects in levelhandler and draw their bounding volumes
		for(int i = 0; i < gGame->getLevelHandler()->getNrOfGameObjects(); i++)
			gGame->getLevelHandler()->getGameObject(i)->drawBoundingVolume(gGR);
	}
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#define _ITERATOR_DEBUG_LEVEL 0
	#define _HAS_ITERATOR_DEBUGGING 0

	MSG msg = { 0 };
	HWND wndHandle = InitWindow(hInstance); // Skapa fönster

	if (wndHandle)
	{
		initializeResources(wndHandle); //GR, SHADERS, IMGUI

		ShowWindow(wndHandle, nCmdShow);
		
		//TEST
		gInputCtrl->setWindow(wndHandle);

		///////////////
		while (WM_QUIT != msg.message)
		{
			gClock->calculateDeltaSeconds(); //Calculate delta seconds
			if (PeekMessage(&msg, wndHandle, 0, 0, PM_REMOVE))
			{
				gInputCtrl->translateMessage(msg);

				 //PRESS ESC TO QUIT PROGRAM
				if (gInputCtrl->getKeyboardState().Escape)
					msg.message = WM_QUIT;
				
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				// RENDER //
				gGR->getDeviceContext()->RSSetState(gGR->getRasterizerState());
				float clearColour[] = {0.04f,0.4f,0.8f};

				gGR->getDeviceContext()->ClearRenderTargetView(*gGR->getBackBuffer(), clearColour);
				gGR->getDeviceContext()->OMSetRenderTargets(1, gGR->getBackBuffer(), gGR->getDepthStencilView());
				gGR->getDeviceContext()->ClearDepthStencilView(gGR->getDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

				update();
				render();
				imGuiUpdate();

				gGR->getSwapChain()->Present(0, 0);

				ImGui::Render();
				ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

				//Reset clock
				gClock->reset();
			}
		}

		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		//CLEAR - kanske i en separat funktion
		delete gGR;
		delete gInputCtrl;
		delete gGame;
		delete gClock;
	}
}

HWND InitWindow(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"BTH_D3D_DEMO";
	if (!RegisterClassEx(&wcex))
		return FALSE;

	RECT rc = { 0, 0, (int)WIDTH, (int)HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND handle = CreateWindow(
		L"BTH_D3D_DEMO",
		L"BTH Direct3D Demo",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		rc.right - rc.left,
		rc.bottom - rc.top,
		nullptr,
		nullptr,
		hInstance,
		nullptr);

	return handle;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// check if IMGUI can handle the message (when we click INSIDE ImGui
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}