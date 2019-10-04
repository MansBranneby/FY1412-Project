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
float gSmoothSpeed = 10.0f;
float gLookAtSpeed = 10.0f;
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

	Light lit = Light(gGR->getDevice(), gGR->getDeviceContext(), XMFLOAT4(0.0f, 40.0f, 0.0f, 0.0f), XMFLOAT4(1.0, 1.0f, 1.0f, 0.0f));
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
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");
	ImGui::Text("Camera");
	ImGui::SliderFloat("Smoothspeed", &gSmoothSpeed, 0.0f, 20.0f);
	ImGui::SliderFloat("LookAtSpeed", &gLookAtSpeed, 0.0f, 20.0f);
	gGame->getCamera()->setSmoothSpeed(gSmoothSpeed);
	gGame->getCamera()->setLookAtSpeed(gLookAtSpeed);

	DirectX::XMFLOAT3 playerPos = gGame->getLevelHandler()->getPlayer()->getPositionFloat3();
	ImGui::Text("Player position: X: %.2f, Y: %.2f, Z: %.2f", playerPos.x, playerPos.y, playerPos.z);

	ImGui::Checkbox("Draw bounding volume", &drawBoundingVolume);
	ImGui::Text("FPS: %.f", ImGui::GetIO().Framerate);
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

		// Temporary solution to drawing bounding volumes (Drawing these in gamestate does not work because we have to set shaders (look above))
		// draw player bounding volume
		gGame->getLevelHandler()->getPlayer()->drawBoundingVolume(gGR);
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
	HWND wndHandle = InitWindow(hInstance); // Skapa f�nster

	if (wndHandle)
	{
		initializeResources(wndHandle); //GR, SHADERS, IMGUI

		ShowWindow(wndHandle, nCmdShow);
		
		//TEST
		gInputCtrl->setWindow(wndHandle);

		///////////////
		while (WM_QUIT != msg.message)
		{
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
				float clearColour[] = {0.0f,0.0f,0.0f};

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
		return false;

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