﻿#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"
#include "compute.h"
#include <assert.h>
#include <cmath>

// 減算
Vector3 Subtract(Vector3 v1, Vector3 v2);

//加算
Vector3 Add(Vector3 vector1, Vector3 vector2);

// 内積
float Dot(Vector3 v1, Vector3 v2);

// 長さ(ノルム)
float Length(const Vector3& v);

// 正規化
Vector3 Normalize(const Vector3& v);

// X軸回転行列
Matrix4x4 MakeRotateXmatrix(float radian);

// Y軸回転行列
Matrix4x4 MakeRotateYmatrix(float radian);

// Z軸回転行列
Matrix4x4 MakeRotateZmatrix(float radian);

// 平行移動
Matrix4x4 MakeTranslateMatrix(Vector3 translate);

// 拡大縮小
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Multiply(Vector3 vector1, float k);

Vector3 Multiply(Vector3 vector1, Vector3 vector2);

// アフィン変換
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& translate);

// ベクトル変換
Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m);

// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m1);

float Length2(Vector3 v);

Vector3 Normalize2(const Vector3& v);
