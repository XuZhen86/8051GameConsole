#ifndef VECTORINT_H_
#define VECTORINT_H_

typedef struct VectorInt_{
    int *elementData;
    unsigned int elementCount;
    unsigned int capacity;
} VectorInt;

VectorInt *VectorInt_new();
void VectorInt_delete(VectorInt *v);

bit VectorInt_add(VectorInt *v,int element);
bit VectorInt_insert(VectorInt *v,unsigned int index,int element);

unsigned int VectorInt_capacity(VectorInt *v);
unsigned int VectorInt_size(VectorInt *v);
bit VectorInt_setSize(VectorInt *v,unsigned int newSize);
bit VectorInt_ensureCapacity(VectorInt *v,unsigned int minCapacity);

int VectorInt_get(VectorInt *v,unsigned int index);
int VectorInt_set(VectorInt *v,unsigned int index,int element);
void VectorInt_removeAllElements(VectorInt *v);
bit VectorInt_removeElementAt(VectorInt *v,unsigned int index);
bit VectorInt_removeElement(VectorInt *v,int element);
bit VectorInt_removeRange(VectorInt *v,unsigned int fromIndex,unsigned int toIndex);

unsigned int VectorInt_indexOf(VectorInt *v,int element);
unsigned int VectorInt_lastIndexOf(VectorInt *v,int element);
bit VectorInt_contains(VectorInt *v,int element);

void VectorInt_debug(VectorInt *v);

#endif
