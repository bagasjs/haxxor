#ifndef __EZMATH__
#define __EZMATH__

#include <stdlib.h>
#include <memory.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef MATH_PI
	#define MATH_PI 3.14159265358979323846f
#endif
#ifndef MATH_PI_2
	#define MATH_PI_2 2.0f * MATH_PI
#endif
#ifndef MATH_HALF_PI
	#define MATH_HALF_PI 0.5f * MATH_PI
#endif
#ifndef MATH_QUARTER_PI
	#define MATH_QUARTER_PI 0.25f * MATH_PI
#endif
#define MATH_ONE_OVER_PI 1.0f / MATH_PI
#define MATH_ONE_OVER_TWO_PI 1.0f / MATH_PI_2
#define MATH_SQRT_TWO 1.41421356237309504880f
#define MATH_SQRT_THREE 1.73205080756887729352f
#define MATH_SQRT_ONE_OVER_TWO 0.70710678118654752440f
#define MATH_SQRT_ONE_OVER_THREE 0.57735026918962576450f
#define MATH_DEG2RAD_MULTIPLIER MATH_PI / 180.0f
#define MATH_RAD2DEG_MULTIPLIER 180.0f / MATH_PI
#define MATH_SEC_TO_US_MULTIPLIER (1000.0f * 1000.0f)
#define MATH_SEC_TO_MS_MULTIPLIER 1000.0f
#define MATH_MS_TO_SEC_MULTIPLIER 0.001f
#define MATH_INFINITY 1e30f
#define MATH_FLOAT_EPSILON 1.192092896e-07f

#if defined(_MSC_VER)
	#define INLINE __forceinline
	#define NO_INLINE __declspec(noinline)
#elif defined(__clang__)
	#define INLINE static inline
	#define NO_INLINE
#elif defined(__GNUG__) || defined(__GNUC__)
	#define INLINE inline
	#define NO_INLINE 
#else
	#error "Unsupported Compiler"
#endif

/****************************************************
 ****************************************************
 *
 *  General Math Functions
 *
 ****************************************************
 ****************************************************/

float Sin(float x);
float Cos(float x);
float Tan(float x);
float Acos(float x);
float Sqrt(float x);
float Abs(float x);

// float Random();
// float RandomRange(float min, float max);

/****************************************************
 ****************************************************
 *
 * Vector 2
 *
 ****************************************************
 ****************************************************/

typedef union VEC2 {
	float elements[2];
	struct {
		union {
			float x, r, s, u;
		};
		union {
			float y, g, t, v;
		};
	};
} VEC2;

INLINE VEC2 Vec2Create(float x, float y) {
	VEC2 res = {0};
	res.x = x;
	res.y = y;
	return res;
}

INLINE VEC2 Vec2Zero() {
	return Vec2Create(0.0f, 0.0f);
}

INLINE VEC2 Vec2One() {
	return Vec2Create(1.0f, 1.0f);
}

INLINE VEC2 Vec2Up() {
	return Vec2Create(0.0f, 1.0f);
}

INLINE VEC2 Vec2Down() {
	return Vec2Create(0.0f, -1.0f);
}

INLINE VEC2 Vec2Left() {
	return Vec2Create(-1.0f, 0.0f);
}

INLINE VEC2 Vec2Right() {
	return Vec2Create(1.0f, 0.0f);
}

INLINE VEC2 Vec2Add(VEC2 v0, VEC2 v1) {
	return Vec2Create(
		v0.x + v1.x,
		v0.y + v1.y
	);
}

INLINE VEC2 Vec2Sub(VEC2 v0, VEC2 v1) {
	return Vec2Create(
		v0.x - v1.x,
		v0.y - v1.y
	);
}

INLINE VEC2 Vec2Mul(VEC2 v0, VEC2 v1) {
	return Vec2Create(
		v0.x * v1.x,
		v0.y * v1.y
	);
}

INLINE VEC2 Vec2Div(VEC2 v0, VEC2 v1) {
	return Vec2Create(
		v0.x / v1.x,
		v0.y / v1.y
	);
}

INLINE float Vec2LengthSq(VEC2 vec) {
	return vec.x * vec.x + vec.y * vec.y;
}

INLINE float Vec2Length(VEC2 vec) {
	return Sqrt(Vec2LengthSq(vec));
}

INLINE void Vec2Normalize(VEC2* vec) {
	const float length = Vec2Length(*vec);
	vec->x /= length;
	vec->y /= length;
}

INLINE VEC2 Vec2Normalized(VEC2 vec) {
	Vec2Normalize(&vec);
	return vec;
}

INLINE bool Vec2Compare(VEC2 v0, VEC2 v1, float tolerance) {
	if(Abs(v0.x - v1.x) > tolerance) {
		return false;
	}

	if(Abs(v0.y - v1.y) > tolerance) {
		return false;
	}
	return false;
}

INLINE bool Vec2CompareEps(VEC2 v0, VEC2 v1) {
	return Vec2Compare(v0, v1, MATH_FLOAT_EPSILON);
}

INLINE float Vec2Distance(VEC2 v0, VEC2 v1) {
	VEC2 d = Vec2Create(v0.x - v1.x, v0.y - v1.y);
	return Vec2Length(d);
}

/****************************************************
 ****************************************************
 *
 *  Vector 3
 *
 ****************************************************
 ****************************************************/

typedef union VEC3 {
	float elements[3];
	struct {
		union {
			float x, r, s, u;
		};
		union {
			float y, g, t, v;
		};
		union {
			float z, b, p, w;
		};
	};
} VEC3;

INLINE VEC3 Vec3Create(float x, float y, float z) {
	VEC3 res = {0};
	res.x = x;
	res.y = y;
	res.z = z;
	return res;
}

INLINE VEC3 Vec3Zero() {
	VEC3 res = Vec3Create( 0.0f, 0.0f, 0.0f );
	return res;
}

INLINE VEC3 Vec3One() {
	VEC3 res = Vec3Create( 1.0f, 1.0f, 1.0f );
	return res;
}

INLINE VEC3 Vec3Up() {
	VEC3 res = Vec3Create( 0.0f, 1.0f, 0.0f );
	return res;
}

INLINE VEC3 Vec3Down() {
	VEC3 res = Vec3Create( 0.0f, -1.0f, 0.0f );
	return res;
}

INLINE VEC3 Vec3Left() {
	VEC3 res = Vec3Create( -1.0f, 0.0f, 0.0f );
	return res;
}

INLINE VEC3 Vec3Right() {
	VEC3 res = Vec3Create( 1.0f, 0.0f, 0.0f );
	return res;
}

INLINE VEC3 Vec3Forward() {
	VEC3 res = Vec3Create( 0.0f, 0.0f, -1.0f );
	return res;
}

INLINE VEC3 Vec3Back() {
	VEC3 res = Vec3Create( 0.0f, 0.0f, 1.0f );
	return res;
}

INLINE VEC3 Vec3Add(VEC3 v0, VEC3 v1) {
	VEC3 res = {0};
	res.x = v0.x + v1.x;
	res.y = v0.y + v1.y;
	res.z = v0.z + v1.z;
	return res;
}

INLINE VEC3 Vec3Sub(VEC3 v0, VEC3 v1) {
	VEC3 res = {0};
	res.x = v0.x - v1.x;
	res.y = v0.y - v1.y;
	res.z = v0.z - v1.z;
	return res;
}

INLINE VEC3 Vec3Mul(VEC3 v0, VEC3 v1) {
	VEC3 res = {0};
	res.x = v0.x * v1.x;
	res.y = v0.y * v1.y;
	res.z = v0.z * v1.z;
	return res;
}

INLINE VEC3 Vec3Div(VEC3 v0, VEC3 v1) {
	VEC3 res = {0};
	res.x = v0.x / v1.x;
	res.y = v0.y / v1.y;
	res.z = v0.z / v1.z;
	return res;
}

INLINE VEC3 Vec3MulA(VEC3 v0, float a) {
	VEC3 res = {0};
	res.x = v0.x * a;
	res.y = v0.y * a;
	res.z = v0.z * a;
	return res;
}

INLINE float Vec3LengthSq(VEC3 vec) {
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z;
}

INLINE float Vec3Length(VEC3 vec) {
	return Sqrt(Vec3LengthSq(vec));
}

INLINE void Vec3Normalize(VEC3* vec) {
	const float length = Vec3Length(*vec);
	vec->x /= length;
	vec->y /= length;
	vec->z /= length;
}

INLINE VEC3 Vec3Normalized(VEC3 vec) {
	Vec3Normalize(&vec);
	return vec;
}

INLINE float Vec3Dot(VEC3 v0, VEC3 v1) {
	float p = 0;
	p += v0.x * v1.x;
	p += v0.y * v1.y;
	p += v0.z * v1.z;
	return p;
}

INLINE VEC3 Vec3Cross(VEC3 v0, VEC3 v1) {
	VEC3 res = {0};
	res.x = v0.y * v1.z - v0.z * v1.y;
	res.y = v0.z * v1.x - v0.x * v1.z;
	res.z = v0.x * v1.y - v0.y * v1.x;
	return res;
}

INLINE bool Vec3Compare(VEC3 v0, VEC3 v1, float tolerance) {
	if(Abs(v0.x - v1.x) > tolerance) {
		return false;
	}

	if(Abs(v0.y - v1.y) > tolerance) {
		return false;
	}

	if(Abs(v0.z - v1.z) > tolerance) {
		return false;
	}

	return true;
}

INLINE bool Vec3CompareEps(VEC3 v0, VEC3 v1) {
	return Vec3Compare(v0, v1, MATH_FLOAT_EPSILON);
}

INLINE float Vec3Distance(VEC3 v0, VEC3 v1) {
	VEC3 d = Vec3Create(
		v0.x - v1.x,
		v0.y - v1.y,
		v0.z - v1.z
	);
	return Vec3Length(d);
}

/****************************************************
 ****************************************************
 *
 *  Vector 4
 *
 ****************************************************
 ****************************************************/

typedef union VEC4 {
	float elements[4];
	struct {
		union {
			float x, r, s;
		};
		union {
			float y, g, t;
		};
		union {
			float z, b, p;
		};
		union {
			float w, a, q;
		};
	};
} VEC4;

INLINE VEC4 Vec4Create(float x, float y, float z, float w) {
	VEC4 res;
#if defined(USE_SIMD)
	res.elements = _mm_setr_ps(x, y, z, w);
#else
	res.x = x;
	res.y = y;
	res.z = z;
	res.w = w;
#endif
	return res;
}

INLINE VEC4 Vec4Zero() {
	return Vec4Create(0.0f, 0.0f, 0.0f, 0.0f);
}

INLINE VEC4 Vec4One() {
	return Vec4Create(1.0f, 1.0f, 1.0f, 1.0f);
}

INLINE VEC4 Vec4add(VEC4 v0, VEC4 v1) {
	VEC4 res = Vec4Zero();
	for(int i = 0; i < 4; i++) {
		res.elements[i] = v0.elements[i] + v1.elements[i];
	}
	return res;
}

INLINE VEC4 Vec4Sub(VEC4 v0, VEC4 v1) {
	VEC4 res = Vec4Zero();
	for(int i = 0; i < 4; i++) {
		res.elements[i] = v0.elements[i] - v1.elements[i];
	}
	return res;
}

INLINE VEC4 Vec4Mul(VEC4 v0, VEC4 v1) {
	VEC4 res = Vec4Zero();
	for(int i = 0; i < 4; i++) {
		res.elements[i] = v0.elements[i] * v1.elements[i];
	}
	return res;
}

INLINE VEC4 Vec4Div(VEC4 v0, VEC4 v1) {
	VEC4 res = Vec4Zero();
	for(int i = 0; i < 4; i++) {
		res.elements[i] = v0.elements[i] / v1.elements[i];
	}
	return res;
}

INLINE float Vec4LengthSq(VEC4 vec) {
	return vec.x * vec.x + vec.y * vec.y + vec.z * vec.z + vec.w * vec.w;
}

INLINE float Vec4Length(VEC4 vec) {
	return Sqrt(Vec4LengthSq(vec));
}

INLINE void Vec4Normalize(VEC4* vec) {
	const float length = Vec4Length(*vec);
	vec->x /= length;
	vec->y /= length;
	vec->z /= length;
	vec->w /= length;
}

INLINE VEC4 Vec4Normalized(VEC4 vec) {
	Vec4Normalize(&vec);
	return vec;
}

INLINE float Vec4Dot(VEC4 v0, VEC4 v1) {
	float p = 0;
	p += v0.x * v1.x;
	p += v0.y * v1.y;
	p += v0.z * v1.z;
	p += v0.w * v1.w;
	return p;
}

/****************************************************
 ****************************************************
 *
 *  Matrix
 *
 ****************************************************
 ****************************************************/
typedef union MAT4 {
	float elements[16];
	VEC4 rows[4];
} MAT4;

INLINE MAT4 Mat4Zero() {
	MAT4 res;
	memset(res.elements, 0, sizeof(float) * 16);
	return res;
}

INLINE MAT4 Mat4Identity() {
	MAT4 res;
	memset(res.elements, 0, sizeof(float) * 16);
	res.elements[0] = 1.0f;
	res.elements[5] = 1.0f;
	res.elements[10] = 1.0f;
	res.elements[15] = 1.0f;
	return res;
}

INLINE MAT4 Mat4Multiply(MAT4 m0, MAT4 m1) {
	MAT4 res = Mat4Identity();
	const float* m0_ptr = m0.elements;
	const float* m1_ptr = m1.elements;
	float* dst_ptr = res.elements;

	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			*dst_ptr = 
				m0_ptr[0] * m1_ptr[0 + j] +
				m0_ptr[1] * m1_ptr[4 + j] +
				m0_ptr[2] * m1_ptr[8 + j] +
				m0_ptr[3] * m1_ptr[12 + j];
			dst_ptr++;
		}
		m1_ptr += 4;
	}
	return res;
}

INLINE MAT4 Mat4Orthographic(float left, float right, float bottom, float top, float near, float far) {
	MAT4 res = Mat4Identity();
	float lr = 1.0f / (left - right);
	float bt = 1.0f / (bottom - top);
	float nf = 1.0f / (near - far);

	res.elements[0] = -2.0f * lr;
	res.elements[5] = -2.0f * bt;
	res.elements[10] = 2.0f * nf;

	res.elements[12] = (left + right) * lr;
	res.elements[13] = (top + bottom) * bt;
	res.elements[14] = (near + far) * nf;	

	return res;
}

INLINE MAT4 Mat4Perspective(float fov_radians, float aspect_ratio, float near, float far) {
	float half_tan_fov = Tan(fov_radians * 0.5f);
	MAT4 res = Mat4Zero();
	res.elements[0] = 1.0f / (aspect_ratio * half_tan_fov);
	res.elements[5] = 1.0f / half_tan_fov;
	res.elements[10] = -((far + near) / (far - near));
	res.elements[11] = -1.0f;
	res.elements[14] = -((2.0f * far * near) / (far - near));	
	return res;
}

INLINE MAT4 Mat4LookAt(VEC3 pos, VEC3 target, VEC3 up) {
	MAT4 result;
	VEC3 z_axis;
	z_axis.x = target.x - pos.x;
	z_axis.y = target.y - pos.y;
	z_axis.z = target.z - pos.z;

	z_axis = Vec3Normalized(z_axis);
	VEC3 x_axis = Vec3Normalized(Vec3Cross(z_axis, up));
	VEC3 y_axis = Vec3Cross(x_axis, z_axis);

	result.elements[0] = x_axis.x;
	result.elements[1] = y_axis.x;
	result.elements[2] = -z_axis.x;
	result.elements[3] = 0;
	result.elements[4] = x_axis.y;
	result.elements[5] = y_axis.y;
	result.elements[6] = -z_axis.y;
	result.elements[7] = 0;
	result.elements[8] = x_axis.z;
	result.elements[9] = y_axis.z;
	result.elements[10] = -z_axis.z;
	result.elements[11] = 0;
	result.elements[12] = -Vec3Dot(x_axis, pos);
	result.elements[13] = -Vec3Dot(y_axis, pos);
	result.elements[14] = Vec3Dot(z_axis, pos);
	result.elements[15] = 1.0f;

	return result;
}

INLINE MAT4 Mat4Inverse(MAT4 matrix) {
	const float* m = matrix.elements;

	float t0 = m[10] * m[15];
	float t1 = m[14] * m[11];
	float t2 = m[6] * m[15];
	float t3 = m[14] * m[7];
	float t4 = m[6] * m[11];
	float t5 = m[10] * m[7];
	float t6 = m[2] * m[15];
	float t7 = m[14] * m[3];
	float t8 = m[2] * m[11];
	float t9 = m[10] * m[3];
	float t10 = m[2] * m[7];
	float t11 = m[6] * m[3];
	float t12 = m[8] * m[13];
	float t13 = m[12] * m[9];
	float t14 = m[4] * m[13];
	float t15 = m[12] * m[5];
	float t16 = m[4] * m[9];
	float t17 = m[8] * m[5];
	float t18 = m[0] * m[13];
	float t19 = m[12] * m[1];
	float t20 = m[0] * m[9];
	float t21 = m[8] * m[1];
	float t22 = m[0] * m[5];
	float t23 = m[4] * m[1];

	MAT4 result;
	float* o = result.elements;

	o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
	o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
	o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
	o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

	float d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

   	o[0] = d * o[0];
	o[1] = d * o[1];
	o[2] = d * o[2];
	o[3] = d * o[3];
	o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
	o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
	o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
	o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
	o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
	o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
	o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
	o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
	o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
	o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
	o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
	o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

	return result;
}

INLINE MAT4 Mat4Translation(VEC3 position) {
    MAT4 result = Mat4Identity();
    result.elements[12] = position.x;
    result.elements[13] = position.y;
    result.elements[14] = position.z;
    return result;
}

INLINE MAT4 Mat4Scale(VEC3 scale) {
    MAT4 result = Mat4Identity();
    result.elements[0] = scale.x;
    result.elements[5] = scale.y;
    result.elements[10] = scale.z;
    return result;
}

INLINE MAT4 Mat4Transposed(MAT4 matrix) {
    MAT4 out_matrix = Mat4Identity();
    out_matrix.elements[0] = matrix.elements[0];
    out_matrix.elements[1] = matrix.elements[4];
    out_matrix.elements[2] = matrix.elements[8];
    out_matrix.elements[3] = matrix.elements[12];
    out_matrix.elements[4] = matrix.elements[1];
    out_matrix.elements[5] = matrix.elements[5];
    out_matrix.elements[6] = matrix.elements[9];
    out_matrix.elements[7] = matrix.elements[13];
    out_matrix.elements[8] = matrix.elements[2];
    out_matrix.elements[9] = matrix.elements[6];
    out_matrix.elements[10] = matrix.elements[10];
    out_matrix.elements[11] = matrix.elements[14];
    out_matrix.elements[12] = matrix.elements[3];
    out_matrix.elements[13] = matrix.elements[7];
    out_matrix.elements[14] = matrix.elements[11];
    out_matrix.elements[15] = matrix.elements[15];
    return out_matrix;
}

INLINE MAT4 Mat4EulerX(float angle_radians) {
    MAT4 out_matrix = Mat4Identity();
    float c = Cos(angle_radians);
    float s = Sin(angle_radians);

    out_matrix.elements[5] = c;
    out_matrix.elements[6] = s;
    out_matrix.elements[9] = -s;
    out_matrix.elements[10] = c;
    return out_matrix;
}

INLINE MAT4 Mat4EulerY(float angle_radians) {
    MAT4 out_matrix = Mat4Identity();
    float c = Cos(angle_radians);
    float s = Sin(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[2] = -s;
    out_matrix.elements[8] = s;
    out_matrix.elements[10] = c;
    return out_matrix;
}

INLINE MAT4 Mat4EulerZ(float angle_radians) {
    MAT4 out_matrix = Mat4Identity();

    float c = Cos(angle_radians);
    float s = Sin(angle_radians);

    out_matrix.elements[0] = c;
    out_matrix.elements[1] = s;
    out_matrix.elements[4] = -s;
    out_matrix.elements[5] = c;
    return out_matrix;
}

INLINE MAT4 Mat4EulerXYZ(float x_radians, float y_radians, float z_radians) {
    MAT4 rx = Mat4EulerX(x_radians);
    MAT4 ry = Mat4EulerY(y_radians);
    MAT4 rz = Mat4EulerZ(z_radians);
    MAT4 out_matrix = Mat4Multiply(rx, ry);
    out_matrix = Mat4Multiply(out_matrix, rz);
    return out_matrix;
}

INLINE VEC3 Mat4Forward(MAT4 matrix) {
    VEC3 forward;
    forward.x = -matrix.elements[2];
    forward.y = -matrix.elements[6];
    forward.z = -matrix.elements[10];
    Vec3Normalize(&forward);
    return forward;
}

INLINE VEC3 Mat4Backward(MAT4 matrix) {
    VEC3 backward;
    backward.x = matrix.elements[2];
    backward.y = matrix.elements[6];
    backward.z = matrix.elements[10];
    Vec3Normalize(&backward);
    return backward;
}

INLINE VEC3 Mat4Up(MAT4 matrix) {
    VEC3 up;
    up.x = matrix.elements[1];
    up.y = matrix.elements[5];
    up.z = matrix.elements[9];
    Vec3Normalize(&up);
    return up;
}

INLINE VEC3 Mat4Down(MAT4 matrix) {
    VEC3 down;
    down.x = -matrix.elements[1];
    down.y = -matrix.elements[5];
    down.z = -matrix.elements[9];
    Vec3Normalize(&down);
    return down;
}

INLINE VEC3 Mat4Left(MAT4 matrix) {
    VEC3 left;
    left.x = -matrix.elements[0];
    left.y = -matrix.elements[4];
    left.z = -matrix.elements[8];
    Vec3Normalize(&left);
    return left;
}

INLINE VEC3 Mat4Right(MAT4 matrix) {
    VEC3 right;
    right.x = matrix.elements[0];
    right.y = matrix.elements[4];
    right.z = matrix.elements[8];
    Vec3Normalize(&right);
    return right;
}

/****************************************************
 ****************************************************
 *
 *  Quaternion
 *
 ****************************************************
 ****************************************************/

typedef VEC4 QUAT;

/****************************************************
 ****************************************************
 *
 *  Utilities
 *
 ****************************************************
 ****************************************************/

#ifdef EZMATH_WITH_UTILS

void PrintMat4(MAT4* mat);
void PrintVec4(VEC4* vec);
void PrintVec3(VEC3* mat);
void PrintVec2(VEC2* vec);

#endif // EZMATH_WITH_UTILS

#endif // __EZMATH__