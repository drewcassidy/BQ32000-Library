/*
   Copyright 2017 Andrew Cassidy

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#include "BQ32000.h"

BQ32000::BQ32000(PinName sda, PinName scl) {
    _bus = new I2C(sda, scl);
    start();
}

void BQ32000::write_byte(Register reg, uint8_t val) {
    char vals[2] = {reg, val};
    _bus->write(BQ32000_ADDR, vals, 2, true);
}

void BQ32000::write_byte(Register *reg, uint8_t *val) {
    _bus->write(BQ32000_ADDR, (char *) reg, 1, true);
    _bus->write(BQ32000_ADDR, (char *) val, 1);
}

uint8_t BQ32000::read_byte(Register reg) {
    uint8_t val;
    read_byte(&reg, &val);

    return val;
}

void BQ32000::read_byte(Register *reg, uint8_t *val) {
    _bus->write(BQ32000_ADDR, (char *) reg, 1);
    _bus->read(BQ32000_ADDR, (char *) val, 1);
}

void BQ32000::start() {
    uint8_t byte = read_byte(REG_Seconds);
    write_byte(REG_Seconds, byte & 0x7F);
}

void BQ32000::stop() {
    uint8_t byte = read_byte(REG_Seconds);
    write_byte(REG_Seconds, byte | 0x80);
}

uint8_t BQ32000::get_seconds() {
    uint8_t byte       = read_byte(REG_Seconds);
    uint8_t seconds_10 = (byte & 0x70) >> 4;
    uint8_t seconds_1  = (byte & 0x0F);
    return (seconds_10 * 10) + seconds_1;
}

uint8_t BQ32000::get_minutes() {
    uint8_t byte       = read_byte(REG_Minutes);
    uint8_t minutes_10 = (byte & 0x70) >> 4;
    uint8_t minutes_1  = (byte & 0x0F);
    return (minutes_10 * 10) + minutes_1;
}

uint8_t BQ32000::get_hours() {
    uint8_t byte     = read_byte(REG_CentHours);
    uint8_t hours_10 = (byte & 0x30) >> 4;
    uint8_t hours_1  = (byte & 0x0F);
    return (hours_10 * 10) + hours_1;
}

BQ32000::DayOfWeek BQ32000::get_day() {
    uint8_t byte = read_byte(REG_Day);
    uint8_t day  = byte & 0x07;
    return (DayOfWeek) day;
}

uint8_t BQ32000::get_date() {
    uint8_t byte    = read_byte(REG_Date);
    uint8_t date_10 = (byte & 0x30) >> 4;
    uint8_t date_1  = (byte & 0x0F);
    return (date_10 * 10) + date_1;
}

uint8_t BQ32000::get_month() {
    uint8_t byte     = read_byte(REG_Month);
    uint8_t month_10 = (byte & 0x10) >> 4;
    uint8_t month_1  = (byte & 0x0F);
    return (month_10 * 10) + month_1;
}

uint8_t BQ32000::get_year() {
    uint8_t byte    = read_byte(REG_Year);
    uint8_t year_10 = (byte & 0xF0) >> 4;
    uint8_t year_1  = (byte & 0x0F);
    return (year_10 * 10) + year_1;
}

void BQ32000::set_seconds(uint8_t seconds) {
    uint8_t byte       = read_byte(REG_Seconds);
    uint8_t seconds_10 = (seconds / 10) & 0x7;
    uint8_t seconds_1  = (seconds % 10) & 0xF;
    byte &= 0x80;
    byte |= (seconds_10 << 4) | seconds_1;
    write_byte(REG_Seconds, byte);
}

void BQ32000::set_minutes(uint8_t minutes) {
    uint8_t byte;
    uint8_t minutes_10 = (minutes / 10) & 0x7;
    uint8_t minutes_1  = (minutes % 10) & 0xF;
    byte &= 0x80;
    byte |= (minutes_10 << 4) | minutes_1;
    write_byte(REG_Minutes, byte);
}

void BQ32000::set_hours(uint8_t hours) {
    uint8_t byte;
    uint8_t hours_10 = (hours / 10) & 0x3;
    uint8_t hours_1  = (hours % 10) & 0xF;
    byte |= (hours_10 << 4) | hours_1;
    write_byte(REG_CentHours, byte);
}

void BQ32000::set_date(uint8_t date) {
    uint8_t date_10 = (date / 10) & 0x3;
    uint8_t date_1  = (date % 10) & 0xF;
    uint8_t byte    = (date_10 << 4) | date_1;
    write_byte(REG_Date, byte);
}

void BQ32000::set_day(BQ32000::DayOfWeek day) {
    write_byte(REG_Day, day);
}

void BQ32000::set_month(uint8_t month) {
    uint8_t month_10 = (month / 10) & 0x1;
    uint8_t month_1  = (month % 10) & 0xF;
    uint8_t byte     = (month_10 << 4) | month_1;
    write_byte(REG_Month, byte);
}

void BQ32000::set_year(uint16_t year) {
    uint8_t short_year = year % 100;
    uint8_t year_10    = (short_year / 10) & 0xF;
    uint8_t year_1     = (short_year % 10) & 0xF;
    uint8_t byte       = (year_10 << 4) | year_1;
    write_byte(REG_Year, byte);
}
