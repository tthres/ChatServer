#pragma once

#include "user.hpp"

// 输出组里成员的所拥有的身份：creator or normal
class GroupUser : public User
{
public:
    void setRole(string role) { this->role = role; }
    string getRole() { return this->role; }

private:
    string role;
};