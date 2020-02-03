#include "Database/databaseinterface.h"

#include <iostream>

DatabaseInterface* DatabaseInterface::instance = NULL;

DatabaseInterface::DatabaseInterface(const string &user, const string &password, const string &connString)
{
    if (DatabaseInterface::instance == NULL)
    {
        this->user = user;
        this->password = password;
        this->connString = connString;

        this->environment = NULL;
        this->connection = NULL;

        DatabaseInterface::instance = this;
    }
}

DatabaseInterface::~DatabaseInterface()
{
    if (this->connection)
    {
        this->environment->terminateConnection(this->connection);
        Environment::terminateEnvironment(this->environment);
    }
}

bool DatabaseInterface::connect()
{
    try
    {
        this->environment = Environment::createEnvironment();
        this->connection = this->environment->createConnection(this->user, this->password, this->connString);
    }
    catch (SQLException& exception)
    {
        if (this->environment)
        {
            Environment::terminateEnvironment(this->environment);
        }

        this->connection = NULL;
        DatabaseInterface::instance = NULL;

        return false;
    }

    return this->connection != NULL;
}

vector<vector<MultiBase*> > DatabaseInterface::executeQuery(const string &query, bool& status, string& message)
{
    status = true;
    message = "";

    if (!DatabaseInterface::instance)
    {
        status = false;
        message = "database instance is null";
        return vector<vector<MultiBase*> >();
    }

    if (!DatabaseInterface::instance->connection)
    {
        status = false;
        message = "database connection not available";
        return vector<vector<MultiBase*> >();
    }

    try
    {
        Statement* statement = DatabaseInterface::instance->connection->createStatement(query);
        ResultSet* resultSet = statement->executeQuery();

        vector<MetaData> metaData = resultSet->getColumnListMetaData();
        vector<vector<MultiBase*> > result;

        while (resultSet->next())
        {
            vector<MultiBase*> rowResult;

            for (size_t i = 1; i <= metaData.size(); i++)
            {
                if (resultSet->isNull(i))
                {
                    rowResult.push_back(NULL);
                    continue;
                }

                switch (metaData[i-1].getInt(MetaData::ATTR_DATA_TYPE))
                {
                case OCCI_TYPECODE_INTEGER:
                case OCCI_TYPECODE_SMALLINT:
                    rowResult.push_back(new MultiType<int>(resultSet->getInt(i)));
                    break;
                case OCCI_TYPECODE_REAL:
                case OCCI_TYPECODE_DOUBLE:
                case OCCI_TYPECODE_BDOUBLE:
                case OCCI_TYPECODE_FLOAT:
                case OCCI_TYPECODE_BFLOAT:
                case OCCI_TYPECODE_NUMBER:
                case OCCI_TYPECODE_DECIMAL:
                    rowResult.push_back(new MultiType<double>(resultSet->getDouble(i)));
                    break;
                case OCCI_TYPECODE_VARCHAR:
                case OCCI_TYPECODE_VARCHAR2:
                case OCCI_TYPECODE_CHAR:
                case OCCI_TYPECODE_DATE:
                case OCI_TYPECODE_TIMESTAMP:
                    rowResult.push_back(new MultiType<string>(resultSet->getString(i)));
                    break;
                default:
                    rowResult.push_back(NULL);
                }
            }

            result.push_back(rowResult);
        }

        statement->closeResultSet(resultSet);
        DatabaseInterface::instance->connection->terminateStatement(statement);

        return result;
    }
    catch (SQLException& exception)
    {
        status = false;
        message = exception.what();
        return vector<vector<MultiBase*> >();
    }

    return vector<vector<MultiBase*> >();
}

void DatabaseInterface::clearQueryResult(vector<vector<MultiBase*> >& result)
{
    for (size_t i = 0; i < result.size(); i++)
    {
        for (size_t j = 0; j < result[i].size(); j++)
        {
            if (result[i][j])
            {
                delete result[i][j];
                result[i][j] = NULL;
            }
        }
    }
}

bool DatabaseInterface::executeUpdate(const string& update, string& message)
{
    message = "";

    if (!DatabaseInterface::instance)
    {
        message = "database instance is null";
        return false;
    }

    if (!DatabaseInterface::instance->connection)
    {
        message = "database connection not available";
        return false;
    }

    try
    {
        Statement* statement = DatabaseInterface::instance->connection->createStatement(update);
        statement->executeUpdate();
        DatabaseInterface::instance->connection->terminateStatement(statement);

        return true;
    }
    catch (SQLException& exception)
    {
        message = exception.what();
        return false;
    }

    return false;
}

void DatabaseInterface::commitUpdate(bool commit)
{
    if (!DatabaseInterface::instance)
    {
        return;
    }

    if (!DatabaseInterface::instance->connection)
    {
        return;
    }

    if (commit)
    {
        DatabaseInterface::instance->connection->commit();
    }
    else
    {
        DatabaseInterface::instance->connection->rollback();
    }
}
