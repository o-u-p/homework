#include <iostream>
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
using namespace std;

#define LENGTH 1E6

//take int as exzample, others should impletment serialize;
//and assume the linked list has no loop
class BigLinkedList
{
private:
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version)
    {
        ar & data;
        ar & pre;
        ar & next;
    }

    BigLinkedList* next;
    BigLinkedList* pre;
public:
    BigLinkedList(){}
    BigLinkedList(int d);
    ~BigLinkedList();

    //
    //for the sake of simplicity 5 × 200 thousand
class BigLinkedListArr
{
private:
    int front;
    int rear;
    BigLinkedList* start;
    BigLinkedList* end;
public:
    BigLinkedListArr(int arr[]);
    ~BigLinkedListArr();
    BigLinkedList* head[5];
    BigLinkedList* getNext()
    {
        if (front<4)
        {
            if (front != rear)
            {
                std::ofstream ofs("tmp");
                boost::archive::text_oarchive oa(ofs);
                oa << head[front];     
            }
            if (!head[++front])
            {
                BigLinkedList* newb;
                std::ifstream ifs("tmp");
                boost::archive::text_iarchive ia(ifs);
                ia >> newb;
                head[front] = newb;
            }
            return head[front];
        }else
        {
            return nullptr;
        }
    }

    BigLinkedList* getPre()
    {
        if (rear>0)
        {
            if (front != rear)
            {
                std::ofstream ofs("tmp");
                boost::archive::text_oarchive oa(ofs);
                oa << head[front];     
            }
            if (!head[--rear])
            {
                BigLinkedList* newb;
                std::ifstream ifs("tmp");
                boost::archive::text_iarchive ia(ifs);
                ia >> newb;
                head[rear] = newb;
            }
            return head[rear];
            }else
            {
                return nullptr;
            }
        }
    };
        BigLinkedListArr* pArr;

    //
    int data;

    BigLinkedList* getNext() const
    {
        if (next)
        {
            return next;
        }else
        {
            return pArr->getNext();
        }
        
    }
    void setNext(BigLinkedList* n)
    {
        delete next;
        delete n->pre;
        next = n;
        n->pre = this;
    }
    BigLinkedList* getPre() const
    {
        if (pre)
        {
            return pre;
        }else
        {
            return pArr->getPre();
        }
    }
    void setPre(BigLinkedList* p)//don't concatenate exist node!!!
    {
        delete pre;
        delete p->next;
        pre = p;
        p->next = this;
    }
};

BigLinkedList::BigLinkedList(int d):data(d), next(nullptr), pre(nullptr)
{
}

BigLinkedList::~BigLinkedList()
{
    if (pre)
    {
        delete pre;
        pre=nullptr;
    }
    if (next)
    {
        delete next;
        next=nullptr;
    }
    
}


BigLinkedList::BigLinkedListArr::BigLinkedListArr(int arr[]):front(0),rear(4)
{
    BigLinkedList* b =new BigLinkedList(arr[0]);
    start = b;
    for (size_t i = 0, index=0; i < LENGTH; index++)
    {
        BigLinkedList* tmp = new BigLinkedList(arr[i]);
        head[index] = tmp;
        for (size_t j = 1; j < LENGTH/5; j++,i++)
        {
            b = new BigLinkedList(arr[i+j]);
            head[index]->setNext(b);
        }
    }
    end = b;
}

BigLinkedList::BigLinkedListArr::~BigLinkedListArr()
{
    delete[] head;
}
