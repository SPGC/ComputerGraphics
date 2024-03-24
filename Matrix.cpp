#include "Matrix.h"

Matrix::Matrix(int height, int width)
{
	data = std::vector<std::vector<float>>(height, std::vector<float>(width, 0));
}

Matrix::Matrix(std::vector<std::vector<float>>& data) : data(std::move(data))
{
}

Matrix::Matrix(const Matrix& m)
{
	data = std::vector<std::vector<float>>(m.data.size(), std::vector<float>(m.data[0].size()));
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] += m.data[i][j];
		}
	}
}

void Matrix::operator+=(const Matrix& other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] += other.data[i][j];
		}
	}
}

void Matrix::operator-=(const Matrix& other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] -= other.data[i][j];
		}
	}
}

void Matrix::operator*=(const Matrix& other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] *= other.data[i][j];
		}
	}
}

void Matrix::operator+=(float other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] += other;
		}
	}
}

void Matrix::operator-=(float other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] -= other;
		}
	}
}

void Matrix::operator*=(float other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] *= other;
		}
	}
}

void Matrix::operator/=(float other)
{
	for (int i = 0; i < data.size(); ++i)
	{
		for (int j = 0; j < data[0].size(); ++j)
		{
			data[i][j] /= other;
		}
	}
}

Matrix Matrix::matMul(const Matrix& first, const Matrix& second)
{
	Matrix result(first.data.size(), second.data[0].size());
	for (int i = 0; i < result.data.size(); ++i)
	{
		for (int j = 0; j < result.data[0].size(); ++j) 
		{
			for (int k = 0; k < second.data.size(); ++k)
			{
				result.data[i][j] += first.data[i][k] * second.data[k][j];
			}
		}
	}
	return result;
}

Matrix operator+(const Matrix& first, const Matrix& second)
{
	Matrix result(first);
	result += second;
	return result;
}

Matrix operator-(const Matrix& first, const Matrix& second)
{
	Matrix result(first);
	result -= second;
	return result;
}

Matrix operator*(const Matrix& first, const Matrix& second)
{
	Matrix result(first);
	result *= second;
	return result;
}

Matrix operator+(float first, const Matrix& second)
{
	Matrix result(second);
	result += first;
	return result;
}

Matrix operator-(float first, const Matrix& second)
{
	Matrix result(second);
	result *= -1.0f;
	result += first;
	return result;
}

Matrix operator*(float first, const Matrix& second)
{
	Matrix result(second);
	result *= first;
	return result;
}

Matrix operator+(const Matrix& first, float second)
{
	return second + first;
}

Matrix operator-(const Matrix& first, float second)
{
	return first + second * -1;
}

Matrix operator*(const Matrix& first, float second)
{
	return second * first;
}

Matrix operator/(const Matrix& first, float second)
{
	Matrix result(first);
	result /= second;
	return result;
}


