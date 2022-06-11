#pragma once
#include <DirectXMath.h>

#define FORCED_INLINE __forceinline

namespace TEMaths
{
	struct Scalar
	{
		FORCED_INLINE Scalar() {}
		FORCED_INLINE Scalar(const Scalar& Scalar) {}
		FORCED_INLINE Scalar(float floatVal) { m_Vector = XMVectorReplicate(floatVal); }

		FORCED_INLINE

	private:

		XMVECTOR m_Vector;

	};
}