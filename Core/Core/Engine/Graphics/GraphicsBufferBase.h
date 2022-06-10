#pragma once

#include <d3d11.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class GraphicsBufferBase
{
public:

	//Im used to DX12 so I normally think of everything as a resource :D
	//This is the underlying resource of the constant buffer.
	ID3D11Buffer* Resource;


protected:

	int BufferSize;
	int ElementCount;
};
