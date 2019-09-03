#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct Vector_{
    void **elementData;
    unsigned int elementCount;
    unsigned int capacity;
} Vector;

// Allocate Vector object
Vector *Vector_new();
// Deallocate Vector object
void Vector_delete(Vector *v);

// Append element to vector
bit Vector_add(Vector *v,void *element);

// Insert element to vector
bit Vector_insert(Vector *v,unsigned int index,void *element);

// Get capacity without reallocation
unsigned int Vector_capacity(Vector *v);

// Get current size
unsigned int Vector_size(Vector *v);

// Force change size
bit Vector_setSize(Vector *v,unsigned int newSize);

// Ensure vector has enough capacity
bit Vector_ensureCapacity(Vector *v,unsigned int minCapacity);

// Modifiers
void *Vector_get(Vector *v,unsigned int index);
void *Vector_set(Vector *v,unsigned int index,void *element);
void Vector_removeAllElements(Vector *v);
bit Vector_removeElementAt(Vector *v,unsigned int index);
bit Vector_removeElement(Vector *v,void *element);
bit Vector_removeRange(Vector *v,unsigned int fromIndex,unsigned int toIndex);

// Find index
unsigned int Vector_indexOf(Vector *v,void *element);
unsigned int Vector_lastIndexOf(Vector *v,void *element);
bit Vector_contains(Vector *v,void *element);

// Dump content of vector
void Vector_debug(Vector *v);

#endif
