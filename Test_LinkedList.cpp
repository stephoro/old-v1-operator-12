//
//  Test_LinkedList.cpp
//  SLUG
//
//  Created by Steph on 3/3/16.
//
//
#ifndef __SLUG__Test_LinkedList__
#define __SLUG__Test_LinkedList__

#include <stdio.h>
#include "LinkedList.hpp"
#include <string>
#include <assert.h>


using namespace std;
//
//  LinkedList.cpp
//  SLUG
//
//  Created by Steph on 3/2/16.
//
//


int Test_LinkedList(){
    
    
    LinkedList<string> * list = new LinkedList<string>();
    string a ="first";
    string b ="second to first";
    string c ="second to last";
    string d ="last";
    string e ="second middle";
    string f ="first middle";
    LinkedList<string>::Node<string> * ref1 = list->add(&a);
    LinkedList<string>::Node<string> * ref2 = list->add(&b);
    LinkedList<string>::Node<string> * ref3 = list->add(&c);
    LinkedList<string>::Node<string> * ref4 = list->add(&d);
    LinkedList<string>::Node<string> * ref5 = list->addAfter(ref2, &e);
    LinkedList<string>::Node<string> * ref6 = list->addBefore(ref5, &f);
    
    assert(ref1 != ref4 && ref4 != ref6);
    
    assert(list->getSize() == 6);
    list->refocus();
    assert(list->next()->compare("first") == 0);
    assert(list->next()->compare("second to first") == 0);
    assert(list->next()->compare("first middle") == 0);
    assert(list->next()->compare("second middle") == 0);
    assert(list->next()->compare("second to last") == 0);
    assert(list->next()->compare("last") == 0);
    
    assert(list->prev()->compare("second to last") == 0);
    assert(list->prev()->compare("second middle") == 0);
    assert(list->prev()->compare("first middle") == 0);
    assert(list->prev()->compare("second to first") == 0);
    
    assert(list->getLast()->compare("last") == 0);
    assert(list->getFirst()->compare("first") == 0);
    
    assert(list->removeLast()->compare("last") == 0);
    assert(list->getLast()->compare("second to last") == 0);
    
    assert(list->removeFirst()->compare("first") == 0);
    assert(list->getFirst()->compare("second to first") == 0);

    list->remove(ref2);
    list->remove(ref3);
    
    list->refocus();
    assert(list->next()->compare("first middle") == 0);
    assert(list->next()->compare("second middle") == 0);
    
    assert(list->getSize() == 2);
    list->release();
    assert(list->getSize() == 0);
    
    
    int * ary = new int[10];
    ary[0] = 11111;
    ary[2] = 22;
    LinkedList<int *> * listIntArray = new LinkedList<int *>();
    LinkedList<int *>::Node<int *> * refIntArray1 = listIntArray->add(&ary);
    
    listIntArray->refocus();
    int * bary = *listIntArray->next();
    assert(bary[0]==11111);
    assert(bary[2]==22);
    assert(bary==ary);
    bary[1] = 13;
    assert(ary[1]==13);
    listIntArray->remove(refIntArray1);

    
    int anum = 1337;
    LinkedList<int> * listInt = new LinkedList<int>();
    LinkedList<int>::Node<int> * refInt1 = listInt->add(&anum);
    listInt->refocus();
    assert(*(listInt->next())==1337);
    listInt->remove(refInt1);

    return 0;
}

#endif