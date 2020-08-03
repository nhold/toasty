#pragma once

#include <Vector2.hpp>

class Matrix4
{
    public:
		Matrix4();
		Matrix4(float a00, float a01, float a02,
			float a10, float a11, float a12,
			float a20, float a21, float a22);

		Matrix4(float a00, float a01, float a02,
			float a10, float a11, float a12,
			float a20, float a21, float a22,
			float a30, float a31, float a32);

        ~Matrix4();

		const float* GetMatrixData() const;

		Vector2f Transform(float x, float y) const;
		Vector2f Transform(Vector2f point) const;

		Matrix4& Combine(const Matrix4& transform);
		Matrix4& Translate(float x, float y);
		Matrix4& Translate(const Vector2f& offset);
		Matrix4& Scale(float scaleX, float scaleY);

		static Matrix4& CreateOrthographic(float width, float height);
		float matrixData[16]; ///< 4x4 matrix defining the transformation
    private:
        
};

Matrix4 operator *(const Matrix4& left, const Matrix4& right);

Matrix4& operator *=(Matrix4& left, const Matrix4& right);

Vector2f operator *(const Matrix4& left, const Vector2f& right);

bool operator ==(const Matrix4& left, const Matrix4& right);

bool operator !=(const Matrix4& left, const Matrix4& right);
