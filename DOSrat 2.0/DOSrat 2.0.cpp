#include <iostream>
#include <EssCurl.h>

using namespace std;

struct person {
    string name;
    string address;
    int age;

    NLOHMANN_DEFINE_TYPE_INTRUSIVE(person, name, address, age)
};

int main()
{
    person p{ "Ned Flanders", "744 Evergreen Terrace", 60 };

    json j = p;

    cout << j.dump();

}