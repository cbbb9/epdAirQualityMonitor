# epdAirQualityMonitor
My personal ESP32-powered air quality monitor project with an e-ink display. 

Inspired by Techdregs' [SEN55 Air Quality Sensor](https://github.com/techdregs/SEN55-Air-Quality-Sensor)

It uses the sensors
- Sensirion SEN55: Measures PM (particulate matter), relative humidity, temperature, VOC (volatile organic compounds), and NO<sub>x</sub> (nitrogen oxides)
- BME280: humidity and pressure

# Hardware
## 1. Microcontroller

I used [Waveshare ESP32-C3-Zero](https://www.waveshare.com/esp32-c3-zero.htm) development board for this project.

<img width="500" height="380" alt="image" src="https://github.com/user-attachments/assets/9e156289-070c-4e7a-9f8e-63a1798cf1f9" />

## 2. E-Ink Display

I use Waveshare's 400x300, 4.2 inch E-ink display module. 

## 3. Sensirion SEN55

- Particulate matter PM1.0, PM2.5, PM4, PM10
- Rel. humidity
- Temperature
- VOC
- NO<sub>x</sub>

Uses I<sup>2</sup>C

## 4. BME280
- Barometric pressure
- Temperature
- Humidity

I<sup>2</sup>C and SPI (3-wire and 4-wire) connectivity
