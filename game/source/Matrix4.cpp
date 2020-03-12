#include <Matrix4.hpp>

Matrix4::Matrix4()
{
	matrixData[0] = 1.f; matrixData[4] = 0.f; matrixData[8] = 0.f; matrixData[12] = 0.f;
	matrixData[1] = 0.f; matrixData[5] = 1.f; matrixData[9] = 0.f; matrixData[13] = 0.f;
	matrixData[2] = 0.f; matrixData[6] = 0.f; matrixData[10] = 1.f; matrixData[14] = 0.f;
	matrixData[3] = 0.f; matrixData[7] = 0.f; matrixData[11] = 0.f; matrixData[15] = 1.f;
}

Matrix4::Matrix4(float a00, float a01, float a02,
	float a10, float a11, float a12,
	float a20, float a21, float a22)
{
	matrixData[0] = a00; matrixData[4] = a01; matrixData[8] = 0.f; matrixData[12] = a02;
	matrixData[1] = a10; matrixData[5] = a11; matrixData[9] = 0.f; matrixData[13] = a12;
	matrixData[2] = 0.f; matrixData[6] = 0.f; matrixData[10] = 1.f; matrixData[14] = 0.f;
	matrixData[3] = a20; matrixData[7] = a21; matrixData[11] = 0.f; matrixData[15] = a22;
}

Matrix4::Matrix4(float a00, float a01, float a02,
	float a10, float a11, float a12,
	float a20, float a21, float a22,
	float a30, float a31, float a32)
{
	matrixData[0] = a00; matrixData[4] = a01; matrixData[8] = 0.f; matrixData[12] = a02;
	matrixData[1] = a10; matrixData[5] = a11; matrixData[9] = 0.f; matrixData[13] = a12;
	matrixData[2] = 0.f; matrixData[6] = 0.f; matrixData[10] = 1.f; matrixData[14] = 0.f;
	matrixData[3] = a20; matrixData[7] = a21; matrixData[11] = 0.f; matrixData[15] = a22;
}

Matrix4::~Matrix4()
{
}

const float* Matrix4::GetMatrixData() const
{
	return matrixData;
}

Vector2f Matrix4::Transform(float x, float y) const
{
	return Vector2f(matrixData[0] * x + matrixData[4] * y + matrixData[12],
		matrixData[1] * x + matrixData[5] * y + matrixData[13]);
}

Vector2f Matrix4::Transform(Vector2f point) const
{
	return Transform(point.x, point.y);
}

Matrix4& Matrix4::Combine(const Matrix4& transform)
{
	const float* a = matrixData;
	const float* b = transform.matrixData;

	*this = Matrix4(a[0] * b[0] + a[4] * b[1] + a[12] * b[3],
		a[0] * b[4] + a[4] * b[5] + a[12] * b[7],
		a[0] * b[12] + a[4] * b[13] + a[12] * b[15],
		a[1] * b[0] + a[5] * b[1] + a[13] * b[3],
		a[1] * b[4] + a[5] * b[5] + a[13] * b[7],
		a[1] * b[12] + a[5] * b[13] + a[13] * b[15],
		a[3] * b[0] + a[7] * b[1] + a[15] * b[3],
		a[3] * b[4] + a[7] * b[5] + a[15] * b[7],
		a[3] * b[12] + a[7] * b[13] + a[15] * b[15]);

	return *this;
}

Matrix4& Matrix4::Translate(float x, float y)
{
	Matrix4 translation(1, 0, x,
		0, 1, y,
		0, 0, 1);

	return Combine(translation);
}

Matrix4& Matrix4::Translate(const Vector2f& offset)
{
	return Translate(offset.x, offset.y);
}

Matrix4& Matrix4::CreateOrthographic(float width, float height)
{
	float a = 2.f / width;
	float b = 2.f / height;

	return Matrix4(a, a, a,
		b, b, b,
		0.f, 0.f, 1.f);
}

Matrix4& Matrix4::Scale(float scaleX, float scaleY)
{
	Matrix4 scaling(scaleX, 0, (1 - scaleX),
		0, scaleY, (1 - scaleY),
		0, 0, 1);

	return Combine(scaling);
}

Matrix4 operator*(const Matrix4& left, const Matrix4& right)
{
	return Matrix4(left).Combine(right);
}

Matrix4& operator*=(Matrix4& left, const Matrix4& right)
{
	return left.Combine(right);
}

Vector2f operator*(const Matrix4& left, const Vector2f& right)
{
	return left.Transform(right);
}

bool operator==(const Matrix4& left, const Matrix4& right)
{
	const float* a = left.GetMatrixData();
	const float* b = right.GetMatrixData();

	return ((a[0] == b[0]) && (a[1] == b[1]) && (a[3] == b[3]) &&
		(a[4] == b[4]) && (a[5] == b[5]) && (a[7] == b[7]) &&
		(a[12] == b[12]) && (a[13] == b[13]) && (a[15] == b[15]));
}

bool operator!=(const Matrix4& left, const Matrix4& right)
{
	return !(left == right);
}
