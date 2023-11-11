#ifndef DATA_TYPES_H
#define DATA_TYPES_H

#pragma once

enum class UserType {
    Administrator = 1,
    Operator = 2,
    User = 3
};

enum class TaskStatus {
    NotAppointed = 1,
    Appointed = 2,
    InProgress = 3,
    Rejected = 4,
    Completed = 5
};

#endif // DATA_TYPES_H
