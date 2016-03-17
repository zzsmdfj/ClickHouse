#pragma once

#include <threadpool.hpp>
#include <DB/Databases/IDatabase.h>


namespace DB
{

/** Движок баз данных по-умолчанию.
  * Хранит список таблиц в локальной файловой системе в виде .sql файлов,
  *  содержащих определение таблицы в виде запроса ATTACH TABLE.
  */
class DatabaseOrdinary : public IDatabase
{
private:
	const String name;
	const String path;
	mutable std::mutex mutex;
	Tables tables;

	Logger * log = &Logger::get("DatabaseOrdinary");

public:
	DatabaseOrdinary(const String & name_, const String & path_, Context & context, boost::threadpool::pool * thread_pool);

	bool isTableExist(const String & table_name) const override;
	StoragePtr tryGetTable(const String & table_name) override;

	DatabaseIteratorPtr getIterator() override;

	bool empty() const override;

	void createTable(const String & table_name, const StoragePtr & table, const ASTPtr & query, const String & engine) override;
	StoragePtr removeTable(const String & table_name) override;

	void attachTable(const String & table_name, const StoragePtr & table) override;
	StoragePtr detachTable(const String & table_name) override;

	void renameTable(const String & table_name, IDatabase & to_database, const String & to_table_name) override;

	ASTPtr getCreateQuery(const String & table_name) const override;

	void shutdown() override;
};

}
