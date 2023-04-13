#include <cstdint>
#include <cmath>
#include "hal.hpp"
#include "shared_resources.hpp"

const double PHI_ANGLE = 37 * 3.14 / 180;
const double V_PEAK = 230 * sqrt(2);
const double I_RMS = 1000 / (230 * cos(PHI_ANGLE));
const double I_PEAK = I_RMS * sqrt(2);
const uint16_t VOLTAGE_LEVEL_THRESHOLD = 20;
const double OMEGA = 2 * 3.14 * 50;

double T_SIM_MAX = 20e-2;
double DT = 1e-4;
double t_m, t_micro, t_micro_total, t, v_t, i_t, v_estimate = 0;
static double energy = 0;
double t_bounce = 1;
uint8_t flag = -1;
uint16_t prev_lev = 0;
uint16_t count = 0;
uint16_t level = V_PEAK;

// if (voltage_level == HAL_GPIO_PIN_STATE_HIGH)
uint8_t simulate_level_from_voltage(hal_gpio_pin_state_t voltage)
{
    level = 0;
    if (voltage == HAL_GPIO_PIN_STATE_HIGH)
    {
        level = V_PEAK;
    }
    else
    {
        level = 0;
    }
    return level;
}

void task()
{

    // Level debouncing for proper edge detection.
    t_bounce = t_bounce + DT;
    // RiSING EDGE
    if (level > prev_lev)
    {
        // BOUNCE PROOFING
        if (t_bounce >= 0.009)
        {
            t_bounce = 0;
            t_micro = 0;
            v_estimate = 0;
        }
    }

    // FALLIGN EDGE
    else if (level < prev_lev)
    {
        // Bounce Proofing
        if (t_bounce >= 0.009)
        {
            t_bounce = 0.0;
            t_m = t_micro / 2.0;

            if (count < 2){count += 1;}

            v_estimate = 0.0;
        }
    }

    // FIRST TWO CYCLES VESTIMATE = 0
    if (count < 2)
    {
        v_estimate = 0;
    }

    // GENERATE ESTIMATE WAVEFORM
    else
    {
        
        v_estimate = (double) (abs( V_PEAK * cos(OMEGA * (t_micro + DT / 2.0 - t_m))));

        t_micro_total = t_micro_total + DT;
    }

    t_micro = t_micro + DT;
    
    prev_lev = level;
    energy = energy + v_estimate * i_t * DT;
}

double get_Energy()
{
    return energy;
}

void Dont_Reset_Energy()
{
    t_m = 0, t_micro = 0, t = 0, v_t = 0, i_t = 0, v_estimate = 0, t = 0, t_bounce = 1, flag = -1, prev_lev = 0, count = 0;
}

void Reset_Energy()
{
    t_m = 0, t_micro = 0, t_micro_total = 0, t = 0, v_t = 0, i_t = 0, v_estimate = 0, t = 0, t_bounce = 1, flag = -1, prev_lev = 0, count = 0, energy = 0;
}

/**************************************************************************************************
 * Function used to Map 1-1023 to 0-10,000
 *
 * @param   x     Passed Value
 * @param   a     Lowest passed value possible
 * @param   b     hightest passed value value
 * @param   c     lowest mapped value
 * @param   d     highest mapped value
 *
 * @return        Converted value
 *************************************************************************************************/
double map_to_float(double x, double a, double b, double c, double d)
{
  double f = x / (b - a) * (d - c) + c;
  return f;
}

double power_measurement()
{

    while (true)
    {
        Dont_Reset_Energy();


        while (t < T_SIM_MAX)
        {

            t = t + DT;


            hal_gpio_pin_state_t voltage_level = HAL_GPIO_PIN_STATE_LOW;
            hal_gpio_api.pin_read(HAL_GPIO_PORT_1, HAL_GPIO_PIN_3, &voltage_level); // port 1, pin 3

            double i_tt = voltmeter_curr_main_pump.get_voltage_mv()/1000;

            i_t = map_to_float(i_tt, 0, 4, 0, 10);

            simulate_level_from_voltage(voltage_level);
            task();
        }
 

        double measured_energy = get_Energy();

        double measured_power = measured_energy / t_micro_total;


        return measured_power;  // will happen once


    }
}

