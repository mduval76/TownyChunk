#ifndef ARRAY3D_H__
#define ARRAY3D_H__

#include "define.h"

template <class T> // class ou typename
class Array3d {
public:
	Array3d(int x, int y, int z);
	Array3d(const Array3d& array);
	~Array3d();

	T Get(int x, int y, int z) const;
	void Set(int x, int y, int z, T type);
	void Reset(T type);

private:
	int To1dIndex(int x, int y, int z) const;
	int m_x, m_y, m_z;
	T* m_blocks;
};

template <class T>
Array3d<T>::Array3d(int x, int y, int z) : m_x(x), m_y(y), m_z(z) {
	m_blocks = new T[m_x * m_y * m_z];
}

template <class T>
Array3d<T>::~Array3d() {
	delete[] m_blocks;
}

template <class T>
Array3d<T>::Array3d(const Array3d& array) : m_x(array.m_x), m_y(array.m_y), m_z(array.m_z) {
	m_blocks = new T[m_x * m_y * m_z];
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_blocks[i] = array.m_blocks[i];
}

template <class T>
void Array3d<T>::Set(int x, int y, int z, T type) {
	m_blocks[To1dIndex(x, y, z)] = type;
}


template <class T>
T Array3d<T>::Get(int x, int y, int z) const {
	return m_blocks[To1dIndex(x, y, z)];
}
template <class T>
void Array3d<T>::Reset(T type) {
	for (int i = 0; i < m_x * m_y * m_z; ++i)
		m_blocks[i] = type;
}

template <class T>
int Array3d<T>::To1dIndex(int x, int y, int z) const {
	return x + (z * m_x) + (y * m_z * m_x);
}
#endif // ARRAY3D_H__
