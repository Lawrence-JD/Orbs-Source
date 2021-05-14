// Fill out your copyright notice in the Description page of Project Settings.


#include "MyQueue.h"


bool UMyQueue::Dequeue(UStaticMeshComponent*& value)
{
	size--;
	return _mainQueue.Dequeue(value);
}

bool UMyQueue::Enqueue(UStaticMeshComponent* value)
{
	size++;
	return _mainQueue.Enqueue(value);
}

void UMyQueue::Empty()
{
	_mainQueue.Empty();
}

bool UMyQueue::IsEmpty()
{
	return _mainQueue.IsEmpty();
}

int32 UMyQueue::Size()
{
	return size;
}

bool UMyQueue::Peek(UStaticMeshComponent*& value)
{
	return _mainQueue.Peek(value);
}