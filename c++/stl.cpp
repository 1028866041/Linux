#include<iostream>
#include<cstddef>
using namespace std;

class alloc{
};

template<class T, class Alloc= alloc>
class vector{
public:
    typedef T* iteartor;

    template<typename I>
    void insert(iteartor position, I first, I last){
        cout<<"insert"<<endl;
    }
};

template<class T, class Alloc= alloc,size_t bufsize= 0>
class deque{
public:   
    deque(){
        cout<<"deque"<<endl;
    }
};
template <class T, class Sequence=deque<T>>
class stack{
    friend bool operator== <T> (const stack<T>&,const stack<T>&);   
    friend bool operator< <T> (const stack<T>&,const stack<T>&); 
public:
    stack(){
        cout<<"stack"<<endl;
    }
private:
    Sequence c;      
};
template <class T,class Sequence>  
bool operator==(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
    { return cout<<"operator=="<<"\t";}
template <class T,class Sequence>
bool operator<(const stack<T, Sequence>& x, const stack<T, Sequence>& y)
    { return cout<<"operator<"<<"\t";}

inline size_t __deque_buf_size(size_t n,size_t sz){
    return n!= 0? n:(sz<512?size_t(512/sz): size_t(1));
}
template <class T, class Ref, class Ptr, size_t BufSiz>
struct __deque_iterator{
    typedef __deque_iterator<T, T&, T*, BufSiz> iteartor;
    typedef __deque_iterator<T, const T&, const T*, BufSiz> const_iteartor;
    static size_t buffer_size() { return __deque_buf_size(BufSiz, sizeof(T));}
};
template <class T, class Alloc=alloc, size_t BufSiz= 0>
class deques{
public:
    typedef __deque_iterator<T, T&, T*, BufSiz> iterator;
};

int main()
{
    int a[]= {0,1,2,3,4};

    vector<int> x;
    vector<int>::iteartor ite;
    x.insert(ite, a, a+5);

    stack<int> s; /*parent */   

    cout<<"deque"<<deques<int>::iterator::buffer_size()<<endl;
    cout<<"deque"<<deques<int,alloc,64>::iterator::buffer_size()<<endl;
    
    stack<int> y;
    stack<int> z;
    //cout<<(y==z)<<endl;
    //cout<<(y<z)<<endl;
    
    return 0;
}

