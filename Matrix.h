#pragma once
#include <vector>
class Matrix
{
private:
	std::vector<std::vector<float>> data;

public:
	Matrix(int width, int height);
	Matrix(std::vector<std::vector<float>>& data);
	Matrix(const Matrix& m);

	std::vector<std::vector<float>>& getdata() {
		return data;
	}

	void operator+=(const Matrix& other);
	void operator-=(const Matrix& other);
	void operator*=(const Matrix& other);

	void operator+=(float other);
	void operator-=(float other);
	void operator*=(float other);
	void operator/=(float other);

	friend Matrix operator+(const Matrix& first, const Matrix& second);
	friend Matrix operator-(const Matrix& first, const Matrix& second);
	friend Matrix operator*(const Matrix& first, const Matrix& second);

	friend Matrix operator+(float first, const Matrix& second);
	friend Matrix operator-(float first, const Matrix& second);
	friend Matrix operator*(float first, const Matrix& second);

	friend Matrix operator+(const Matrix& first, float second);
	friend Matrix operator-(const Matrix& first, float second);
	friend Matrix operator*(const Matrix& first, float second);
	friend Matrix operator/(const Matrix& first, float second);

	static Matrix matMul(const Matrix& first, const Matrix& second);
};

