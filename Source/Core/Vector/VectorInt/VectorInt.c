#include"VectorIntConfig.h"
#include"VectorIntStatic.h"
#include<Debug.h>
#include<VectorInt.h>
#include<stdlib.h>
#include<string.h>

VectorInt *VectorInt_new(){
    VectorInt *v=malloc(sizeof(VectorInt));

    v->elementData=calloc(EXPAND_DEFAULT_SIZE,sizeof(int));
    v->elementCount=0;
    v->capacity=EXPAND_DEFAULT_SIZE;

    return v;
}

void VectorInt_delete(VectorInt *v){
    free(v->elementData);
    free(v);
}

bit VectorInt_add(VectorInt *v,int element){
    if(!VectorInt_ensureCapacity(v,v->elementCount+1)){
        return 0;
    }

    v->elementData[v->elementCount++]=element;

    return 1;
}

bit VectorInt_insert(VectorInt *v,unsigned int index,int element){
    unsigned int i;

    if(!VectorInt_ensureCapacity(v,v->elementCount+1)){
        return 0;
    }

    for(i=v->elementCount++;i>index;i--){
        v->elementData[i]=v->elementData[i-1];
    }
    v->elementData[index]=element;

    return 1;
}

unsigned int VectorInt_capacity(VectorInt *v){
    return v->capacity;
}

unsigned int VectorInt_size(VectorInt *v){
    return v->elementCount;
}

bit VectorInt_setSize(VectorInt *v,unsigned int newSize){
    if(newSize<v->elementCount){
        Debug(WARNING,"Truncate VectorInt@%p size from %u to %u",v,v->elementCount,newSize);
        v->elementCount=newSize;
        return 1;
    }

    if(!VectorInt_ensureCapacity(v,newSize)){
        return 0;
    }

    memset(v->elementData+v->elementCount*sizeof(int),0x00,(newSize-v->elementCount)*sizeof(int));
    v->elementCount=newSize;

    return 1;
}

bit VectorInt_ensureCapacity(VectorInt *v,unsigned int minCapacity){
    if(minCapacity>v->capacity){
        if(!expandN(v,minCapacity-v->capacity)){
            return 0;
        }
    }

    return 1;
}

int VectorInt_get(VectorInt *v,unsigned int index){
    if(index>=v->elementCount){
        return 0;
    }

    return v->elementData[index];
}

int VectorInt_set(VectorInt *v,unsigned int index,int element){
    int prevElement;

    if(index>=v->elementCount){
        return 0;
    }

    prevElement=v->elementData[index];
    v->elementData[index]=element;
    return prevElement;
}

void VectorInt_removeAllElements(VectorInt *v){
    v->elementCount=0;
}

bit VectorInt_removeElementAt(VectorInt *v,unsigned int index){
    unsigned int i;

    if(index>=v->elementCount){
        return 0;
    }

    v->elementCount--;
    for(i=index;i<v->elementCount;i++){
        v->elementData[i]=v->elementData[i+1];
    }

    return 1;
}

bit VectorInt_removeElement(VectorInt *v,int element){
    unsigned int i;

    for(i=0;i<v->elementCount;i++){
        if(v->elementData[i]==element){
            break;
        }
    }

    if(i==v->elementCount){
        return 0;
    }

    v->elementCount--;
    for(;i<v->elementCount;i++){
        v->elementData[i]=v->elementData[i+1];
    }

    return 1;
}

bit VectorInt_removeRange(VectorInt *v,unsigned int fromIndex,unsigned int toIndex){
    unsigned int i,offset;

    if(fromIndex>=toIndex||fromIndex>=v->elementCount||toIndex>=v->elementCount){
        return 0;
    }

    offset=toIndex-fromIndex;
    for(i=fromIndex;i+offset<v->elementCount;i++){
        v->elementData[i]=v->elementData[i+offset];
    }
    v->elementCount-=offset;

    return 1;
}

unsigned int VectorInt_indexOf(VectorInt *v,int element){
    unsigned int i;

    for(i=0;i<v->elementCount;i++){
        if(v->elementData[i]==element){
            break;
        }
    }

    return i;
}

unsigned int VectorInt_lastIndexOf(VectorInt *v,int element){
    unsigned int i;

    for(i=v->elementCount-1;i;i--){
        if(v->elementData[i]==element){
            break;
        }
    }

    if(i==0&&v->elementData[0]!=element){
        return v->elementCount;
    }
    return i;
}

bit VectorInt_contains(VectorInt *v,int element){
    unsigned int i;

    for(i=0;i<v->elementCount;i++){
        if(v->elementData[i]==element){
            return 1;
        }
    }

    return 0;
}

void VectorInt_debug(VectorInt *v){
    unsigned int i;

    Debug(DEBUG,"VectorInt@%p elementCount=%u capacity=%u",v,VectorInt_size(v),VectorInt_capacity(v));
    for(i=0;i<v->elementCount;i++){
        Debug(DEBUG,"    elementData[%u]=%d",i,VectorInt_get(v,i));
    }
}

static bit expandDefault(VectorInt *v){
    return expandN(v,EXPAND_DEFAULT_SIZE);
}

static bit expandN(VectorInt *v,unsigned int n){
    int *newElementData;

    newElementData=malloc((v->capacity+n)*sizeof(int));
    if(newElementData==NULL){
        Debug(WARNING,"Failed to expand VectorInt@%p by %u",v,n);
        return 0;
    }

    memcpy(newElementData,v->elementData,v->capacity*sizeof(int));
    free(v->elementData);
    v->elementData=newElementData;
    v->capacity+=n;

    return 1;
}
