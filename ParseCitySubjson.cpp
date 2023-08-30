City ParseCity(
    const Json& city_obj,
    const Country& country_info
    ) {
    return City {
        city_obj["name"s].AsString(),
        city_obj["iso_code"s].AsString(),
        country_info.country_phone_code + city_obj["phone_code"s].AsString(),
        country_info.country_name,
        country_info.country_iso_code,
        country_info.country_time_zone,
        country_info.languages
    };
}

// Дана функция ParseCitySubjson, обрабатывающая JSON-объект со списком городов конкретной страны:
void ParseCitySubjson(
    vector<City>& cities, 
    const Json& json, 
    const Country& country_info
    ) {
    for (const auto& city_json : json.AsList()) {
        cities.push_back(
            ParseCity(city_json.AsObject(), country_info)
        );
    }
}

// ParseCitySubjson вызывается только из функции ParseCountryJson следующим образом:
void ParseCountryJson(vector<Country>& countries, vector<City>& cities, const Json& json) {

    for (const auto& country_json : json.AsList()) {

        const auto& country_obj = country_json.AsObject();
        
        //Country обладает всем необходимым
        countries.push_back({
            country_obj["name"s].AsString(),
            country_obj["iso_code"s].AsString(),
            country_obj["phone_code"s].AsString(),
            country_obj["time_zone"s].AsString(),
        });
        
        Country& country = countries.back();
        for (const auto& lang_obj : country_obj["languages"s].AsList()) {
            country.languages.push_back(FromString<Language>(lang_obj.AsString()));
        }

        ParseCitySubjson(
            cities, 
            country_obj["cities"s], 
            country
        );
    }
}