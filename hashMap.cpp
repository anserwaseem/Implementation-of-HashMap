#include<iostream>
#include<cassert>
#include<fstream>
#include<string>
#include<sstream>
using namespace std;

template <class v>
struct HashItem
{
    int key;
    v value;
    short int status;
};

template <class v>
class HashMap
{
protected:
    HashItem<v>* hashArray;
    int capacity;
    int currentElements;
    void doubleCapacity()
    {
        int oldCapacity = capacity;
        this->capacity = capacity*2;
        HashItem<v>* newHashArray = new HashItem<v> [capacity] ();

        cout<<endl<<"Doubling the capacity from "<<oldCapacity<<" to "<<this->capacity<<" and rearranging all the key value pairs.\n";
        for(int i=0; i<oldCapacity; i++)
            if(hashArray[i].status==2)//if occupied
                insert_helper(hashArray[i].key, hashArray[i].value, newHashArray);
        
        this->hashArray=newHashArray;
        newHashArray = nullptr;
    }
    virtual int getNextCandidateIndex(int key, int i, HashItem<v>* hashArr)
    {
        int index = (key+i)%capacity;
        if(hashArr[index].status==0)//if this next candidate index is free, then send its index number or check for the next one
            return index;
        return getNextCandidateIndex(key, ++i, hashArr);
    }
    void insert_helper(int const key, v const value, HashItem<v>* arr)
    {
        int index = key%capacity;

        if (arr[index].status==2)//if occupied
            index = getNextCandidateIndex(key, 1, arr);

        arr[index].key=key;
        arr[index].value=value;
        arr[index].status=2;//occupied
        cout<<"index: "<<index<<"    ,key: "<<key<<"    ,value: "<<value<<endl;
    }
public:
    HashMap()
    {
        capacity = 7;//prime number
        currentElements = 0;
        hashArray = new HashItem<v> [capacity] ();
    }
    HashMap(int const _capacity)
    {
        assert(capacity>1);
        capacity = _capacity;
        currentElements = 0;
        hashArray = new HashItem<v> [capacity] ();
    }
    HashItem<v>* getHashArray()
    {
        return this->hashArray;
    }
    void insert(int const key, v const value, HashItem<v>* hashArr)
    {
        if( ( ((float)(currentElements)) / ((float)(capacity)) ) >= 0.75 )//load factor
            doubleCapacity();

        insert_helper(key, value, hashArr);

        currentElements++;
    }
    bool deleteKey(int const key) const
    {
        for(int i=0; i<this->capacity; i++)
            if(hashArray[i].key == key)
            {
                hashArray[i].status = 1;//key deleted
                cout<<key<<" deleted.\n";
                return true;
            }

        cout<<"key not found.\n";
        return false;
    }
    v* get(int const key) const
    {
        for(int i=0; i<this->capacity; i++)
            if(hashArray[i].key == key)
            {
                cout<<"key found at index"<<i<<endl;
                return (&hashArray[i].value);
            }

        cout<<"key not found.\n";
        return nullptr;
    }
    ~HashMap()
    {
        if(hashArray!=nullptr)
            hashArray=nullptr;
        delete hashArray;
    }
};

template<class v>
class QHashMap : public HashMap<v>
{
public:
    int getNextCandidateIndex(int key, int i, HashItem<v>* hashArr)
    {
        // int _key = (key + (i*i));
        // if(_key<0)
        //     _key+=this->capacity;

        int index = (key + (i*i)) % this->capacity;
        if(hashArr[index].status==0)//if free
            return index;
        return getNextCandidateIndex(key, ++i, hashArr);
    }
};

template<class v>
class DHashMap : public HashMap<v>
{
public:
    int getNextCandidateIndex(int key, int i, HashItem<v>* hashArr)
    {
        int first_value = key % this->capacity;
        int second_value = 7 - (key % 7);
        int candidate_index = (first_value + (i*second_value)) % this->capacity;

        if(hashArr[candidate_index].status==0)//if free
            return candidate_index;
        return getNextCandidateIndex(key, ++i, hashArr);
    }
};

void populateHash(string filename, HashMap<string> *hash)
{
    ifstream fin;
    fin.open(filename);
    int Tsize;
    fin>>Tsize;
    fin.ignore();

    while(Tsize>0)
    {
        string name;
        getline(fin, name);

        int att = name.find(' ');
        string _key = name.substr(0, att);
        //int id = stoi(_key, nullptr, 10);
        stringstream id(_key);
        int ID=0;
        id>>ID;

        name=name.substr(att+1, name.size());

        hash->insert(ID, name, hash->getHashArray());

        Tsize--;
    }
}
int main()
{
    cout<<"LINEAR PROBING\n";
    HashMap<string> *map=new HashMap<string>;
    populateHash("students.txt", map);
    cout<<"Calling get function:\t"<<*map->get(9)<<endl<<endl;
    cout<<"Calling deleteKey function:\t";map->deleteKey(9);cout<<endl;
    //assert((map->get(9)==nullptr));
    free (map);

    cout<<"QUADRATIC PROBING\n";
    QHashMap<string> *Qmap=new QHashMap<string>;
    populateHash("students.txt", Qmap);
    cout<<"Calling get function:\t"<<*Qmap->get(9)<<endl<<endl;
    cout<<"Calling deleteKey function:\t";Qmap->deleteKey(9);cout<<endl;
    //assert(Qmap->get(9)==nullptr);
    free (Qmap);

    cout<<"DOUBLE HASHING\n";
    DHashMap<string> *Dmap=new DHashMap<string>;
    populateHash("students.txt", Dmap);
    cout<<"Calling get function:\t"<<*Dmap->get(9)<<endl<<endl;
    cout<<"Calling deleteKey function:\t";Dmap->deleteKey(9);cout<<endl;
    //assert(Dmap->get(9)==nullptr);
    free (Dmap);

    return 0;
}