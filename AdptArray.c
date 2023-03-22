/*
* Assignment 1 in System Programming B, spring 2023
* Mayan Hazon
* 22/3/23
* 
* Version 0.1
*
*
* Sources:
* ADT TA lesson in System Programming A
* Consulted with Hadas Evers and Roy Simanovich for help with memory leaks
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "AdptArray.h"

typedef struct AdptArray_
{
    //Fields
    int num; //number of elements
    PElement *MyArr; //array of elements

    //Functions
    COPY_FUNC CpyElement;
    DEL_FUNC DelElement;
    PRINT_FUNC PrintElement;

} AdptArray, *PAdptArray;


PAdptArray CreateAdptArray(COPY_FUNC Cpy, DEL_FUNC Del,PRINT_FUNC Print)
{
    //Allocate the new arr
    PAdptArray pNewArr;
    pNewArr = (PAdptArray)malloc(sizeof(AdptArray));

    if(pNewArr) //If malloc was successful, initialize variables and functions
    {
        pNewArr->num = 0;
        pNewArr->CpyElement = Cpy;
        pNewArr->DelElement = Del;
        pNewArr->PrintElement = Print;
        pNewArr->MyArr = NULL;
    }

    //Will return null if malloc was unsuccessful
    return pNewArr;
}

void DeleteAdptArray(PAdptArray pArr)
{
    //Iterate over the array
    int i;
    for(i = 0; i< pArr->num; i++)
    {
         // If element is not null, delete it
        if(pArr->MyArr[i])
        {
            pArr->DelElement(pArr->MyArr[i]);
        }
    }

    //Free space of the array and the struct
    free(pArr->MyArr);
    free(pArr);
}


Result SetAdptArrayAt(PAdptArray pArr, int i, PElement pElem)
{
    //Resize if necessary
    if(i >= pArr->num)
    {
        int j = pArr->num; //save original size

        //Resize to i+1 elements, from 0 to ith place
        pArr->num = i+1;
        pArr->MyArr = realloc(pArr->MyArr, (i+1)*sizeof(PElement));

        //check for failure to realloc
        if(!(pArr->MyArr))
        {
            return FAIL;
        }

        //Initialize to NULL everything in between- the loop will not enter if i=num
        for(; j <= i; j++)
        {
            pArr->MyArr[j]=NULL;
        }

    }

    PElement pNewElem = pArr->CpyElement(pElem); //Create a copy of the element
    if(pNewElem) //New element created successfully
    {
        //Allocate the copy, freeing the space if needed
        if(pArr->MyArr[i])
        {
            pArr->DelElement(pArr->MyArr[i]);
        }
        pArr->MyArr[i] = pNewElem;


        return SUCCESS;
    }
    else //Null was received by the copy fuction- set unsuccessful
    {
        return FAIL;
    }
}

PElement GetAdptArrayAt(PAdptArray pArr, int i)
{
    //Check that i is within bounds
    if(i >= pArr->num)
    {
        return NULL;
    }

    //Return a copy of the element, null if there was no space for copy
    if(pArr->MyArr[i]) //Check that we don't pass a NULL pointer to the function
    {
        return pArr->CpyElement(pArr->MyArr[i]);
    }
    else //If the element at i was NULL, simply return NULL
    {
        return NULL;
    }
}

int GetAdptArraySize(PAdptArray pArr)
{
    //Check that the adaptive array exists, otherwise answer is -1
    if(pArr)
    {
        return pArr->num;
    }
    return -1;
}

void PrintDB(PAdptArray pArr)
{
    //Print an element when it's not null
    int i;
    for(i = 0; i < pArr->num; i++)
    {
        if(pArr->MyArr[i])
        {
            pArr->PrintElement(pArr->MyArr[i]);
        }
    }
}