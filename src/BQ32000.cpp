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
    _bus    = new I2C(sda, scl);
    _data   = (uint8_t *) malloc(sizeof(bq32000_registers) + 1);
    _buffer = (bq32000_registers *) (_data + 1);
    memset(_data, 0, sizeof(bq32000_registers) + 1);

    start();
}

void BQ32000::write_buffer() {
    _bus->write(BQ32000_ADDR, (char *) _data, sizeof(bq32000_registers) + 1);
}

void BQ32000::read_buffer() {
    _bus->write(BQ32000_ADDR, (char *) _data, 1);
    _bus->read(BQ32000_ADDR, (char *) _buffer, sizeof(bq32000_registers));
}

void BQ32000::start() {
    _buffer->seconds &= 0x7F;
}

void BQ32000::stop() {
    _buffer->seconds |= 0x80;
}

uint8_t BQ32000::get_seconds() {
    uint8_t seconds_10 = (_buffer->seconds & 0x70) >> 4;
    uint8_t seconds_1  = (_buffer->seconds & 0x0F);
    return (seconds_10 * 10) + seconds_1;
}

uint8_t BQ32000::get_minutes() {
    uint8_t minutes_10 = (_buffer->minutes & 0x70) >> 4;
    uint8_t minutes_1  = (_buffer->minutes & 0x0F);
    return (minutes_10 * 10) + minutes_1;
}

uint8_t BQ32000::get_hours() {
    uint8_t hours_10 = (_buffer->cent_hours & 0x30) >> 4;
    uint8_t hours_1  = (_buffer->cent_hours & 0x0F);
    return (hours_10 * 10) + hours_1;
}

BQ32000::DayOfWeek BQ32000::get_day() {
    return (DayOfWeek) _buffer->day & 0x7;
}

uint8_t BQ32000::get_date() {
    uint8_t date_10 = (_buffer->date & 0x30) >> 4;
    uint8_t date_1  = (_buffer->date & 0x0F);
    return (date_10 * 10) + date_1;
}

uint8_t BQ32000::get_month() {
    uint8_t month_10 = (_buffer->month & 0x10) >> 4;
    uint8_t month_1  = (_buffer->month & 0x0F);
    return (month_10 * 10) + month_1;
}

uint8_t BQ32000::get_year() {
    uint8_t year_10 = (_buffer->year & 0xF0) >> 4;
    uint8_t year_1  = (_buffer->year & 0x0F);
    return (year_10 * 10) + year_1;
}

void BQ32000::set_seconds(uint8_t seconds) {
    uint8_t seconds_10 = (seconds / 10) & 0x7;
    uint8_t seconds_1  = (seconds % 10) & 0xF;
    _buffer->seconds &= 0x80;
    _buffer->seconds |= (seconds_10 << 4) | seconds_1;
}

void BQ32000::set_minutes(uint8_t minutes) {
    uint8_t minutes_10 = (minutes / 10) & 0x7;
    uint8_t minutes_1  = (minutes % 10) & 0xF;
    _buffer->minutes &= 0x80;
    _buffer->minutes |= (minutes_10 << 4) | minutes_1;
}

void BQ32000::set_hours(uint8_t hours) {
    uint8_t hours_10 = (hours / 10) & 0x3;
    uint8_t hours_1  = (hours % 10) & 0xF;
    _buffer->cent_hours &= 0xC0;
    _buffer->cent_hours |= (hours_10 << 4) | hours_1;
}

void BQ32000::set_date(uint8_t date) {
    uint8_t date_10 = (date / 10) & 0x3;
    uint8_t date_1  = (date % 10) & 0xF;
    _buffer->date = (date_10 << 4) | date_1;
}

void BQ32000::set_day(BQ32000::DayOfWeek day) {
    _buffer->day = day;
}

void BQ32000::set_month(uint8_t month) {
    uint8_t month_10 = (month / 10) & 0x1;
    uint8_t month_1  = (month % 10) & 0xF;
    _buffer->month = (month_10 << 4) | month_1;
}

void BQ32000::set_year(uint16_t year) {
    uint8_t short_year = year % 100;
    uint8_t year_10    = (short_year / 10) & 0xF;
    uint8_t year_1     = (short_year % 10) & 0xF;
    _buffer->year = (year_10 << 4) | year_1;
}
