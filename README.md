# TipUp-Monitor

A system used to monitor and alert on activity for ice fishing tip-ups. The code is meant to run on a ESP8266. 

## Overview

The idea is that an ESP8266 will monitor the status of a tipup flag using a switch or sensor. 
The ESP8266 will be connected to a wireless network and will send a message to an MQTT queue when a "flag up" condition is detected.

A Rasperry PI will provide both the wireless network and the MQTT service.
