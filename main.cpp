#include<iostream>
#include<fstream>
#include<vector>
#include<list>
#include<queue>
#include<deque>
#include<bitset>

using namespace std;

int main()
{
    int sum=0,val, n=0;
    vector<int> vec(10,0);
    string str("1000");
    int a[]={1,2,3}, *p;

    /*input check*/
    while(cin>>val,!cin.eof())
            sum+=val;
    cout<<"sum="<<sum<<endl;

    vec.empty();
    vec.size();
    vec.push_back(n);
    vec[n];
    /*v1=v2; 
    v1==v2;
    !=,<,<=,>,>=*/
    for(vector<int>::size_type i=0; i!=10; i++)
        vec.push_back(i);

    for(vector<int>::iteartor ite= vec.begin(); ite!= vec.end(); ite++)
        *ite= 0;
    /*ite+n;
    ite-n;*/

    bitset<32> b(str,1,n);
    b.any();
    b.set();
    b.reset();
    b.flip();
    b.to_ulong();
    p= a;
    p= new int(64);
    delete p;    
    p= NULL;

    /*try{
    }catch(){
    throw{
    }*/
    /*-DDEBUG/__FILE__.__LIEN__.__DATE__.__TIME__*/
    /*typedef void (*fun)(void);
      fun f= lookup;*/
    //class template

    /*const class obj;*/
    //lookup(const int a);
    //lookup(const double b, const string c);

    while(cin>>val, !cin.eof()){
        if(cin.bad()){
            throw runtime_error("IO corrupted");           
        }
        if(cin.fail()){
            cerr<<"bad data retry:";
            cin.clear(istream::badbit| istream::failbit); /*IO failed*/
            continue; 
        }
    }    
    //open();/*in/out/app/ate/trunc/binary*/

    /*C<T> c;
    C c(c2);
    C c(b,e);//iterator
    C c(n,t);//element
    C c(n);*/
    list<int> list;
    deque<int> dqueeu;
    queue<int> queue;
    priority_queue<int> pqueue;
    stack<int> stack;   
  
    vec.capacity();//max space
    vec.size();
    vec.swap(vector<int> vec2);
    /*virtual function(){};*/

   

    
      
    return 0;
}

