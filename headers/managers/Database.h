#ifndef DATABASE_H
#define DATABASE_H

#include <string>
#include <vector>
#include <sqlite3.h>

struct Row {
    std::vector<std::string> values;
};

class Database {
private:
    sqlite3* m_db;
    std::string m_dbPath;
    bool m_isOpen;

public:
    Database(std::string dbPath);
    ~Database();

    bool open();
    void close();
    bool isOpen() const { return m_isOpen; }

    bool execute(const std::string& sql);
    bool executeWithParams(const std::string& sql, const std::vector<std::string>& params);

    std::vector<Row> select(const std::string& sql) const;
    std::vector<Row> selectWithParams(const std::string& sql, const std::vector<std::string>& params) const;

    void beginTransaction();
    void commitTransaction();
    void rollbackTransaction();

    bool backup(const std::string& backupPath);
};

#endif // DATABASE_H
