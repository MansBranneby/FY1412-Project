#include "PixelShader.h"

HRESULT PixelShader::createPixelShader(LPCWSTR fileName, ID3D11Device* device)
{
	//create pixel shader
	ID3DBlob* pPS = nullptr;
	ID3DBlob* errorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(fileName, nullptr,nullptr, "PS_main", "ps_5_0", D3DCOMPILE_DEBUG, 0, &pPS, &errorBlob);

	// compilation failed?
	if (FAILED(result))
	{
		if (errorBlob)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());
			errorBlob->Release();
		}
		if (pPS)
			pPS->Release();

		if (FAILED(result))
			MessageBox(NULL, L"Error", L"Error: createPixelShader in pixelShader.cpp has failed", MB_OK | MB_ICONERROR);

		return result;
	}

	// Compilation successful
	device->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &_pixelShader);
	if (pPS)
		pPS->Release();

	return result;
}

PixelShader::PixelShader()
{
}

PixelShader::PixelShader(LPCWSTR fileName, ID3D11Device * device)
{
	createPixelShader(fileName, device);
}

PixelShader::~PixelShader()
{
	if (_pixelShader)
		_pixelShader->Release();
}

ID3D11PixelShader & PixelShader::getPixelShader() const
{
	return *_pixelShader;
}
