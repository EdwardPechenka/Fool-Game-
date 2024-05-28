#pragma once

using namespace std;

template <typename T> 
class Array
{
public:
	Array(): data(nullptr), size(0), capacity(0) {}
	~Array() {
		delete[] data;
	}

	int get_size() const {
		return size;
	}

	//Function adds new element in the end of data
	void push_back(const T& value) {
		if (size == capacity) {
			resize(capacity == 0 ? 1 : capacity * 2);
		}

		data[size] = value;
		size++;
	}
	
	//Function removes element from array with index
	void erase(int index) {
		//If index more than size it will be error
		if (index >= size) {
			throw out_of_range("Index out of range");
		}

		for (int i = index; i < size - 1; i++) {
			data[i] = data[i + 1];
		}

		size--;
	}

	//Clear array 
	void clear() {
		delete[] data;
		data = nullptr;
		size = 0;
		capacity = 0;
	}

	T& operator[] (int index) {
		if (index >= size) {
			throw out_of_range("Index out of range");
		}
		return data[index];
	}

	const T& operator[] (int index) const {
		if (index >= size) {
			throw out_of_range("Index out of range");
		}
		return data[index];
	}

private:
	T* data;
	int size;
	int capacity;
	
	void resize(int new_capacity) {
		//Allocates (виділяє) memory with size new_capacity
		T* new_data = new T[new_capacity];
		for (int i = 0; i < size; i++) {
			new_data[i] = data[i];
		}
		delete[] data;
		data = new_data;
		capacity = new_capacity;
	}
};
