#ifndef ConsigneeH
#define ConsigneeH

#include <System.Classes.hpp>

class Consignee {
public:
    String Name;
    String Address;
    String PhoneNumber;

    Consignee() {}
    Consignee(String name, String address, String phone)
        : Name(name), Address(address), PhoneNumber(phone) {}
};

#endif
