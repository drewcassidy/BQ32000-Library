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

#define BQ32000_ADDR 0xD0

/**
 * @brief API for using the Texas Instruments BQ32000 Real Time Clock
 */
class BQ32000 {
public:
    enum Register {
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

    enum DayOfWeek {
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
     * @param reg register to write to
     * @param val data to write
     */
    void write_byte(Register reg, uint8_t val);

    /**
     * @brief Writes a byte to a register
     *
     * @param reg pointer to register to write to
     * @param val pointer to write from
     */
    void write_byte(Register *reg, uint8_t *val);

    /**
     * @brief Reads a byte from a register
     *
     * @param reg register to read from
     *
     * @return register's value
     */
    uint8_t read_byte(Register reg);

    /**
     * @brief Reads a byte from a register
     *
     * @param reg pointer to register to read from
     * @param val pointer to read into
     */
    void read_byte(Register *reg, uint8_t *val);

    /**
     * @brief starts the RTC
     */
    void start();

    /**
     * @brief stops the RTC
     */
    void stop();

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
     * @brief gets last 2 digits of the current year
     *
     * @return year
     */
    uint8_t get_year();

    /**
     * @brief sets the seconds value of the RTC
     *
     * @param seconds seconds
     */
    void set_seconds(uint8_t seconds);

    /**
     * @brief sets the minutes value of the RTC
     *
     * @param minutes minutes
     */
    void set_minutes(uint8_t minutes);

    /**
     * @brief sets the hours value of the RTC
     *
     * @param hours hours
     */
    void set_hours(uint8_t hours);

    /**
     * @brief sets the current date on the RTC
     *
     * @param date current date
     */
    void set_date(uint8_t date);

    /**
     * @brief sets the day of the week on the RTC
     *
     * @param day day of the week (see above)
     */
    void set_day(DayOfWeek day);

    /**
     * @brief sets the current month on the RTC
     * @details 1 = january, 2 = february, etc.
     *
     * @param month current month
     */
    void set_month(uint8_t month);

    /**
     * @brief sets the current year on the RTC
     *
     * @param year current year
     */
    void set_year(uint16_t year);

private:
    I2C *_bus;
};

#endif //BQ32000_H
