#pragma once

#include <queue>

namespace d2dFramework
{
	template <typename T>
	class ObjectPool
	{
	public:
		ObjectPool(unsigned int capacity);
		~ObjectPool();
		ObjectPool(const ObjectPool& other) = delete;
		ObjectPool& operator=(const ObjectPool& other) = delete;

		T* Get();
		void Return(T* object);

		unsigned int GetFreeObjectCount() const;
		unsigned int GetCapacitiy() const;

	private:
		unsigned int mCapacity;
		std::queue<T*> mQueue;
	};

	template <typename T>
	ObjectPool<T>::ObjectPool(unsigned int capacity)
		: mCapacity(capacity)
	{
	}

	template <typename T>
	ObjectPool<T>::~ObjectPool()
	{
		while (!mQueue.empty())
		{
			T* type = mQueue.front();
			mQueue.pop();

			delete type;
		}
	}

	template <typename T>
	T* ObjectPool<T>::Get()
	{
		if (mQueue.empty())
		{
			return new T();
		}

		T* result = mQueue.front();
		mQueue.pop();

		return result;
	}

	template <typename T>
	void ObjectPool<T>::Return(T* obj)
	{
		if (mQueue.size() == mCapacity)
		{
			delete obj;

			return;
		}

		mQueue.push(obj);
	}

	template <typename T>
	unsigned int ObjectPool<T>::GetFreeObjectCount() const
	{
		return mQueue.size();
	}

	template <typename T>
	unsigned int ObjectPool<T>::GetCapacitiy() const
	{
		return mCapacity;
	}
}