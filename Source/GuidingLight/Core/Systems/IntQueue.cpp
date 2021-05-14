// Fill out your copyright notice in the Description page of Project Settings.


#include "IntQueue.h"


bool UIntQueue::Dequeue(int32& value)
{
	size--;
	return _mainQueue.Dequeue(value);
}

bool UIntQueue::Enqueue(int32 value)
{
	size++;
	return _mainQueue.Enqueue(value);
}

void UIntQueue::Empty()
{
	_mainQueue.Empty();
}

bool UIntQueue::IsEmpty()
{
	return _mainQueue.IsEmpty();
}

int32 UIntQueue::Size()
{
	return size;
}

bool UIntQueue::Peek(int32& value)
{
	return _mainQueue.Peek(value);
}