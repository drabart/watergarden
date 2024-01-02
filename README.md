# Watergarden

A webserver written in Python with Flask for interfacing with custom built garden watering system

It allows for the system using ESP8266 modules to connected to wifi network to send GET
requests and receive information about thee desired system state.

From user side webserver sets up a website allowing to make changes in the system.
Website provides a simple login page and UI

It stores information using a mariaDB SQL server
