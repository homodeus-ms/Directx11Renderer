#include "pch.h"
#include "InputLayout.h"

InputLayout::InputLayout()
{
}

InputLayout::~InputLayout()
{
}

void InputLayout::Create(const vector<D3D11_INPUT_ELEMENT_DESC>& descs, ComPtr<ID3DBlob> blob)
{
	const int32 count = static_cast<int32>(descs.size());
	LPVOID bufferPtr = blob->GetBufferPointer();
	size_t bufferSize = blob->GetBufferSize();

	HRESULT hr = DEVICE->CreateInputLayout(
		descs.data(), count, bufferPtr, bufferSize, _inputLayout.GetAddressOf());
	if (hr != S_OK)
	{
		int a = 3;
	}
	check(hr);
}
