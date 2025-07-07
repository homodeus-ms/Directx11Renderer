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
	HRESULT hr = DEVICE->CreateInputLayout(descs.data(), count, blob->GetBufferPointer(), blob->GetBufferSize(), _inputLayout.GetAddressOf());
	check(hr);
}
