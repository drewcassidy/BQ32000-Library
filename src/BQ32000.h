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

#ifndef BQ32000_H
#define BQ32000_H

#include <mbed.h>
#include <stdint.h>

/**
 * @brief API for using the Texas Instruments BQ32000 Real Time Clock
 */
class BQ32000 {
public:
    const uint8_t Address = 0xD0;

    enum Register : uint8_t {
        REG_Seconds   = 0x00,
        REG_Minutes   = 0x01,
        REG_CentHours = 0x02,
        REG_Day       = 0x03,
        REG_Date      = 0x04,
        REG_Month     = 0x05,
        REG_Year      = 0x06,
        REG_Config1   = 0x07,
        REG_Trickle   = 0x08,
        REG_Config2   = 0x09,
        REG_SFKey1    = 0x20,
        REG_SFKey2    = 0x21,
        REG_SFR       = 0x22
    };

    enum DayOfWeek : uint8_t {
        DAY_None,
        DAY_Sunday,
        DAY_Monday,
        DAY_Tuesday,
        DAY_Wednesday,
        DAY_Thursday,
        DAY_Friday,
        DAY_Saturday
    };

    /**
     * @brief Creates a new instance of the BQ32000
     *
     * @param sda SDA pin for I2C
     * @param scl SCL pin for I2C
     */
    BQ32000(PinName sda, PinName scl);

    /**
     * @brief Writes a byte to a register
     *
     * @param reg Register to write to
     * @param data data to write
     */
    void write_byte(Register reg, uint8_t data);

    /**
     * @brief Reads a byte from a register
     *
     * @param reg Register to read from
     *
     * @return register's value
     */
    uint8_t read_byte(Register reg);

    /**
     * @brief gets the number of seconds since the last minute
     *
     * @return seconds
     */
    uint8_t get_seconds();

    /**
     * @brief gets the number of minutes since the last hour
     *
     * @return minutes
     */
    uint8_t get_minutes();

    /**
     * @brief gets the number of hours since midnight
     *
     * @return hours
     */
    uint8_t get_hours();

    /**
     * @brief gets day of the week. See DayOfWeek
     *
     * @return day of the week
     */
    DayOfWeek get_day();

    /**
     * @brief gets the number of days since the start of the month
     *
     * @return date
     */
    uint8_t get_date();

    /**
     * @brief gets month of the year
     * @details 1 = january, 2 = february, etc.
     *
     * @return month
     */
    uint8_t get_month();

    /**
     * @brief gets the last two digits of the current year
     * @details returns the last two digits of the year. eg. 2017 becomes '17'
     *
     * @return year
     */
    uint8_t get_year_short();

    /**
     * @brief gets the current year
     *
     * @return year
     */
    uint16_t get_year();

    void set_seconds(uint8_t seconds);

    void set_minutes(uint8_t minutes);

    void set_hours(uint8_t hours);

    void set_date(uint8_t date);

    void set_day(DayOfWeek day);

    void set_month(uint8_t month);

    void set_year(uint16_t year);

private:
    I2C *_bus;

    uint16_t _base_century = 2000; //shouldnt be relevent until the year 2100

    uint16_t get_century();
};

#endif //BQ32000_H
