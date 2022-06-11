#pragma once

#include <DirectXMath.h>
#include "Scalar.h"

namespace TEMaths
{
	struct Vector3
	{
		FORCED_INLINE Vector3() {}
		FORCED_INLINE Vector3(float x, float y, float z) { m_Vector = XMVectorSet(x, y, z);}
		FORCED_INLINE Vector3(const Vector3& Vector) { m_Vector = Vector; }
		FORCED_INLINE Vector3(Scalar& Scalar) { m_Vector = Scalar; }

	public:

		FORCED_INLINE Scalar GetX() const { return Scalar(XMVectorSplatX(m_Vector)); }
		FORCED_INLINE Scalar GetY() const { return Scalar(XMVectorSplatY(m_Vector)); }
		FORCED_INLINE Scalar GetZ() const { return Scalar(XMVectorSplatZ(m_Vector)); }

		FORCED_INLINE void SetX(Scalar x) { m_Vector = XMVectorPermute<4, 1, 2, 3>(m_Vector, x); }
		FORCED_INLINE void SetY(Scalar y) { m_Vector = XMVectorPermute<0, 5, 2, 3>(m_Vector, y); }
		FORCED_INLINE void SetZ(Scalar z) { m_Vector = XMVectorPermute<0, 1, 6, 3>(m_Vector, z); }
	private:

		XMVECTOR m_Vector;

	};
}