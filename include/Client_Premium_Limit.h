//
// Created by TOMIC on 28/04/2021.
//

#ifndef SSOO_P3_CLIENT_PREMIUM_LIMIT_H
#define SSOO_P3_CLIENT_PREMIUM_LIMIT_H

class Client_Premium_Limit : public Client_Premium{
private:
    int balance;
public:
    Client_Premium_Limit(int id, std::string word, std::string categoria, int start_balance);
};

#endif //SSOO_P3_CLIENT_PREMIUM_LIMIT_H
