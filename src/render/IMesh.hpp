#pragma once
#include <d3d11.h>


namespace Render 
{
	class IMesh 
	{
	public:
		virtual ~IMesh() = default;

		virtual void initialize(ID3D11Device* device) = 0;
		virtual void draw(ID3D11DeviceContext* context) const = 0;

		virtual unsigned int getVertexCount() const = 0;
		virtual unsigned int getIndexCount() const = 0;
	};
}
