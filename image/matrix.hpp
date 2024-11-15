/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2022 - 2024, Csaba Nagy.
 * Copyright (C) 2024, Project Simon, Martin TOUZOT.
 *
 * Based on dng_encoder.cpp by Csaba Nagy, modification by Martin TOUZOT.
 * 
 * matrix.hpp - 3x3 Matrix structure.
 */

struct Matrix
{
Matrix(float m0, float m1, float m2,
	   float m3, float m4, float m5,
	   float m6, float m7, float m8)
	{
		m[0] = m0, m[1] = m1, m[2] = m2;
		m[3] = m3, m[4] = m4, m[5] = m5;
		m[6] = m6, m[7] = m7, m[8] = m8;
	}
	Matrix(float diag0, float diag1, float diag2) : Matrix(diag0, 0, 0, 0, diag1, 0, 0, 0, diag2) {}
	Matrix() {}
	float m[9];
	Matrix T() const
	{
		return Matrix(m[0], m[3], m[6], m[1], m[4], m[7], m[2], m[5], m[8]);
	}
	Matrix C() const
	{
		return Matrix(m[4] * m[8] - m[5] * m[7], -(m[3] * m[8] - m[5] * m[6]), m[3] * m[7] - m[4] * m[6],
					  -(m[1] * m[8] - m[2] * m[7]), m[0] * m[8] - m[2] * m[6], -(m[0] * m[7] - m[1] * m[6]),
					  m[1] * m[5] - m[2] * m[4], -(m[0] * m[5] - m[2] * m[3]), m[0] * m[4] - m[1] * m[3]);
	}
	Matrix Adj() const { return C().T(); }
	float Det() const
	{
		return (m[0] * (m[4] * m[8] - m[5] * m[7]) -
				m[1] * (m[3] * m[8] - m[5] * m[6]) +
				m[2] * (m[3] * m[7] - m[4] * m[6]));
	}
	Matrix Inv() const { return Adj() * (1.0 / Det()); }
	Matrix operator*(Matrix const &other) const
	{
		Matrix result;
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
				result.m[i * 3 + j] =
					m[i * 3] * other.m[j] + m[i * 3 + 1] * other.m[3 + j] + m[i * 3 + 2] * other.m[6 + j];
		return result;
	}
	Matrix operator*(float const &f) const
	{
		Matrix result;
		for (int i = 0; i < 9; i++)
			result.m[i] = m[i] * f;
		return result;
	}
};
