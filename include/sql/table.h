//
// Created by @ruitianzhong on 5/11/24.
// Copyright (c) 2024 ruitianzhong All rights reserved.
//

#ifndef TABLE_H
#define TABLE_H

namespace xDB {
    struct TableName {
        char *name;
        char *schema;

        TableName() = default;

        TableName(char *schema_, char *name_): name(name_), schema(schema_) {
        }
    };

    struct ColumnName : TableName {
        char *column_name;

        ColumnName(char *schema, char *name, char *column_name_): TableName(schema, name), column_name(column_name_) {
        }
    };

    enum DataType {
        INTEGER,
        FLOAT,
        CHAR,
    };
}

#endif //TABLE_H
