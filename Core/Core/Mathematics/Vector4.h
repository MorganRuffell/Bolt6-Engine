#pragma once

#include <DirectXMath.h>
#include "Vector3.h"

namespace TEMaths
{
	struct Vector4
	{
		FORCED_INLINE Vector4() {}
		FORCED_INLINE Vector4(float x, float y, float z, float w) { m_Vector = XMVectorSet(x, y, z, w); }
		FORCED_INLINE Vector4(const Vector4& Vector) { m_Vector = Vector; }
		FORCED_INLINE Vector4(Scalar& Scalar) { m_Vector = Scalar; }

	public:

		FORCED_INLINE Scalar GetX() const { return Scalar(XMVectorSplatX(m_Vector)); }
		FORCED_INLINE Scalar GetY() const { return Scalar(XMVectorSplatY(m_Vector)); }
		FORCED_INLINE Scalar GetZ() const { return Scalar(XMVectorSplatZ(m_Vector)); }
		FORCED_INLINE Scalar GetW() const { return Scalar(XMVectorSplatW(m_Vector)); }
		FORCED_INLINE void SetX(Scalar x) { m_Vector = XMVectorPermute<4, 1, 2, 3>(m_Vector, x); }
		FORCED_INLINE void SetY(Scalar y) { m_Vector = XMVectorPermute<0, 5, 2, 3>(m_Vector, y); }
		FORCED_INLINE void SetZ(Scalar z) { m_Vector = XMVectorPermute<0, 1, 6, 3>(m_Vector, z); }
		FORCED_INLINE void SetW(Scalar w) { m_Vector = XMVectorPermute<0, 1, 2, 7>(m_Vector, w); }

		FORCED_INLINE void SetXYZ(Vector3 xyz) { m_Vector = XMVectorPermute<0, 1, 2, 7>(xyz, m_Vector); }


	private:

		XMVECTOR m_Vector;


	};

}

