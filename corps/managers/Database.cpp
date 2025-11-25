#include "Database.h"
#include <iostream>

Database::Database(std::string dbPath)
    : m_db(nullptr), m_dbPath(std::move(dbPath)), m_isOpen(false) {
}

Database::~Database() {
    close();
}

bool Database::open() {
    if (m_isOpen) {
        return true;
    }

    int rc = sqlite3_open(m_dbPath.c_str(), &m_db);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur ouverture BDD: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    m_isOpen = true;
    std::cout << "✅ Base de données ouverte: " << m_dbPath << std::endl;
    return true;
}

void Database::close() {
    if (m_db) {
        sqlite3_close(m_db);
        m_db = nullptr;
        m_isOpen = false;
    }
}

bool Database::execute(const std::string& sql) {
    if (!m_isOpen) {
        std::cerr << "❌ Base non ouverte!" << std::endl;
        return false;
    }

    char* errorMessage = nullptr;
    int rc = sqlite3_exec(m_db, sql.c_str(), nullptr, nullptr, &errorMessage);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur SQL: " << errorMessage << std::endl;
        sqlite3_free(errorMessage);
        return false;
    }

    return true;
}

bool Database::executeWithParams(const std::string& sql, const std::vector<std::string>& params) {
    if (!m_isOpen) {
        std::cerr << "❌ Base non ouverte!" << std::endl;
        return false;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur prepare: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    rc = sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    if (rc != SQLITE_DONE) {
        std::cerr << "❌ Erreur execute: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    return true;
}

std::vector<Row> Database::select(const std::string& sql) const
{
    return selectWithParams(sql, {});
}

std::vector<Row> Database::selectWithParams(const std::string& sql, const std::vector<std::string>& params) const
{
    std::vector<Row> results;

    if (!m_isOpen) {
        std::cerr << "❌ Base non ouverte!" << std::endl;
        return results;
    }

    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(m_db, sql.c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur prepare: " << sqlite3_errmsg(m_db) << std::endl;
        return results;
    }

    for (size_t i = 0; i < params.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Row row;
        int colCount = sqlite3_column_count(stmt);
        for (int j = 0; j < colCount; ++j) {
            const char* text = reinterpret_cast<const char*>(sqlite3_column_text(stmt, j));
            row.values.push_back(text ? text : "");
        }
        results.push_back(row);
    }

    sqlite3_finalize(stmt);
    return results;
}

void Database::beginTransaction() {
    execute("BEGIN TRANSACTION;");
}

void Database::commitTransaction() {
    execute("COMMIT;");
}

void Database::rollbackTransaction() {
    execute("ROLLBACK;");
}

bool Database::backup(const std::string& backupPath) {
    if (!m_isOpen) {
        std::cerr << "❌ Base non ouverte!" << std::endl;
        return false;
    }

    sqlite3* backupDb;
    int rc = sqlite3_open(backupPath.c_str(), &backupDb);
    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur ouverture backup: " << sqlite3_errmsg(backupDb) << std::endl;
        sqlite3_close(backupDb);
        return false;
    }

    sqlite3_backup *backup = sqlite3_backup_init(backupDb, "main", m_db, "main");
    if (backup) {
        sqlite3_backup_step(backup, -1);
        sqlite3_backup_finish(backup);
    }

    rc = sqlite3_errcode(backupDb);
    sqlite3_close(backupDb);

    if (rc != SQLITE_OK) {
        std::cerr << "❌ Erreur backup: " << sqlite3_errmsg(m_db) << std::endl;
        return false;
    }

    std::cout << "💾 Backup réussi vers: " << backupPath << std::endl;
    return true;
}
