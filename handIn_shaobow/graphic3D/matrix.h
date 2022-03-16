/**
 * Square matrix and its operations
 * 
 * @tparam T the type of data stored in matrix
 * @tparam N the dimension of square matrix, same for vector
 */

#ifndef MATRIX_H_
#define MATRIX_H_

template <class T, const int N>
struct vec
{
	T content[N];
};

template <class T, const int N>
class matrix
{
protected:
	T* content;

public:
	matrix();
	~matrix();

	// Set value to given index
	void set(int row, int col, const T& incoming);

	// Get value from given index
	const T& value(int row, int col) const;

	// Multiply by a scalar
	void scalarMultiply(const T& factor);

	// Multiply by a matrix
	void matrixMultiply(matrix<T, N>& otherMatrix, matrix <T, N>& resultMatrix);

	// Multiply by a vector at back
	void vecMultiply(vec<T, N>& otherVec, vec<T, N>& resultVec);

};

template<class T, int N>
inline matrix<T, N>::matrix()
{
	// allocate space for array
	content = new T[N * N];
}

template<class T, int N>
inline matrix<T, N>::~matrix()
{
	// destruct
	delete[] content;
}

template<class T, int N>
inline void matrix<T, N>::set(int row, int col, const T& incoming)
{
	if (1 <= row && row <= N && 1 <= col && col <= N)
		content[(row - 1) * N + (col - 1)] = incoming;
	else
		throw std::out_of_range("Error: invalid index");
}

template<class T, int N>
inline const T& matrix<T, N>::value(int row, int col) const
{
	if (1 <= row && row <= N && 1 <= col && col <= N)
		return content[(row - 1) * N + (col - 1)];
	else
		throw std::out_of_range("Error: invalid index");
}

template<class T, int N>
inline void matrix<T, N>::scalarMultiply(const T& factor)
{
	for (int i = 0; i < N * N; i++)
		content[i] = factor * content[i];
}

template<class T, int N>
inline void matrix<T, N>::matrixMultiply(matrix<T, N>& otherMatrix, matrix<T, N>& resultMatrix)
{
	for (int i = 0; i < N; i++)
		for (int j = 0; j < N; j++) {
			resultMatrix.content[i * N + j] = 0;
			for (int k = 0; k < N; k++)
				resultMatrix.content[i * N + j] += content[i * N + k] * otherMatrix.content[k * N + j];
		}
}


template<class T, int N>
inline void matrix<T, N>::vecMultiply(vec<T, N>& otherVec, vec<T, N>& resultVec)
{
	for (int i = 0; i < N; i++){
		resultVec.content[i] = 0;
		for (int j = 0; j < N; j++) {
			resultVec.content[i] += content[i * N + j] * otherVec.content[j];
		}
	}
}
#endif // !MATRIX_H_