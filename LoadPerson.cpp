struct LoadPersonsOptions {
    string_view db_name;
    int db_connection_timeout;
    bool db_allow_exceptions;
    DBLogLevel db_log_level;
    int min_age;
    int max_age;
    string_view name_filter;
};


DBHandler ConnectToDatabase(const string_view db_name, 
                            int db_connection_timeout,
                            bool db_allow_exceptions, 
                            DBLogLevel db_log_level) {
    DBConnector connector(db_allow_exceptions, db_log_level);
    if (db_name.starts_with("tmp."s)) {
        return connector.ConnectTmp(db_name, db_connection_timeout);
    } else {
        return connector.Connect(db_name, db_connection_timeout);
    }
}

string GenerateSQLQuery(int min_age, int max_age, string_view name_filter) {
    ostringstream query_str;
    query_str << "from Persons "s
              << "select Name, Age "s
              << "where Age between "s << min_age << " and "s << max_age << " "s
              << "and Name like '%"s << db.Quote(name_filter) << "%'"s;
    return query_str.str();
}


vector<Person> LoadPersons(const LoadPersonsOptions& options) {
    
    DBHandler db= ConnectToDatabase(options.db_name, 
                                    options.db_connection_timeout, 
                                    options.db_allow_exceptions, 
                                    options.db_log_level);

    if (!db_allow_exceptions && !db.IsOK()) {
        return {};
    }

    string query_str = GenerateSQLQuery(options.min_age, 
                                        options.max_age, 
                                        options.name_filter);
    DBQuery query(query_str);

    vector<Person> persons;
    for (auto [name, age] : db.LoadRows<string, int>(query)) {
        persons.push_back({move(name), age});
    }
    return persons;
}