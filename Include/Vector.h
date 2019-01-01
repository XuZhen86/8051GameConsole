#ifndef VECTOR_H_
#define VECTOR_H_

typedef struct Vector_{
    void **elementData;
    unsigned int elementCount;
    unsigned int capacity;
} Vector;

Vector *Vector_new();
void Vector_delete(Vector *v);

bit Vector_add(Vector *v,void *element);
bit Vector_insert(Vector *v,unsigned int index,void *element);

unsigned int Vector_capacity(Vector *v);
unsigned int Vector_size(Vector *v);
bit Vector_setSize(Vector *v,unsigned int newSize);
bit Vector_ensureCapacity(Vector *v,unsigned int minCapacity);

void *Vector_get(Vector *v,unsigned int index);
void *Vector_set(Vector *v,unsigned int index,void *element);
void Vector_removeAllElements(Vector *v);
bit Vector_removeElementAt(Vector *v,unsigned int index);
bit Vector_removeElement(Vector *v,void *element);
bit Vector_removeRange(Vector *v,unsigned int fromIndex,unsigned int toIndex);

unsigned int Vector_indexOf(Vector *v,void *element);
unsigned int Vector_lastIndexOf(Vector *v,void *element);
bit Vector_contains(Vector *v,void *element);

void Vector_toString(Vector *v);

#endif
