void CheckValueRange(int value, int min_value, int max_value, const string& value_name) {
    if (value < min_value) {
        throw std::domain_error(value_name + " is too small");
    }
    if (value > max_value) {
        throw std::domain_error(value_name + " is too big");
    }
}

void CheckDateTimeValidity(const DateTime& dt) {
    CheckValueRange(dt.year, 1, 9999, "year");
    CheckValueRange(dt.month, 1, 12, "month");

    const bool is_leap_year = (dt.year % 4 == 0) && !(dt.year % 100 == 0 && dt.year % 400 != 0);
    const array month_lengths = {31, 28 + is_leap_year, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    CheckValueRange(dt.day, 1, month_lengths[dt.month - 1], "day");
    CheckValueRange(dt.hour, 0, 23, "hour");
    CheckValueRange(dt.minute, 0, 59, "minute");
    CheckValueRange(dt.second, 0, 59, "second");
}