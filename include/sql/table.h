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

    class DataDefinition {
    public:
        explicit DataDefinition(DataType type_);

        virtual ~DataDefinition();

        void setName(char *data_name);

    private:
        DataType type;
        char *data_name;
    };

    class CharDefinition final : public DataDefinition {
    public:
        explicit CharDefinition(DataType type_, int len_);

        ~CharDefinition() override;

        int getLen() const;

    private:
        int len;
    };
}

#endif //TABLE_H
