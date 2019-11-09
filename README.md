# ISA project - Whois questioner

## About
Program analyze input domain and dump found information. Program gets data from
DNS server, Whois server, and geolocation server **ip-api.com**.

## Dependencies
- BSD socket
- C++17
- resolv.h

## Instalation 
Native environment OS GNU/Linux (Windows not tested).
- ``make`` - compile project.
- ``./isa-tazatel -h`` - dump usage information.

## Usage
``isa-tazatel <arguments>``

Compulsory arguments:
- ``  -q <IP|hostname>  = IP or hostname address to by analyzed.``
- ``  -w <IP|hostname>  = IP or hostname of questioned WHOIS server``

Optional arguments:
- ``  -d <IP|hostname>  = IP or hostanme of questioned DNS server (DNS resolver of OS by default).``
- ``  -h                = Print this help message.``

Example of usage:

```
user@user:~/$ ./isa-tazatel -q vutbr.cz -w whois.ripe.net
==============================---------DNS--------==============================
SOA:            origin rhino.cis.vutbr.cz
SOA:			email hostmaster.vutbr.cz
SOA:			serial 2019110501
SOA:			refresh 28800
SOA:			retry 7200
SOA:			expire 1814400
SOA:			minimum 1814400
A:              147.229.2.90
NS:             rhino.cis.vutbr.cz
NS:             pipit.cis.vutbr.cz
MX:             5 mx.vutbr.cz
TXT:            "google-site-verification=kSdrjCE0ee5GKpv_Xtr-18k9Pm1OzRIVXrkm9kIwEAk"
TXT:            "MS=ms21627876"
TXT:            "v=spf1 ip4:147.229.0.0/16 ip6:2001:67c:1220::/48 include:spf.protection.outlook.com include:_spf.google.com ~all"

==============================--------WHOIS-------==============================
inetnum:        147.229.0.0 - 147.229.254.255
netname:        VUTBRNET
descr:          Brno University of Technology
descr:          VUTBR-NET1
country:        CZ
address:        Brno University of Technology
address:        Antoninska 1
address:        601 90 Brno
address:        The Czech Republic
phone:          +420 541145453
phone:          +420 723047787
admin-c:        CA6319-RIPE

==============================-GEOLOCATION SERVER-==============================
continent       Europe
continentCode   EU
country         Czechia
countryCode     CZ
region          64
regionName      South Moravian
city            Brno
zip             614 00
timezone        Europe/Prague
currency        CZK
isp             VUTBR
org             Brno University of Technology
as              AS197451 Brno University of Technology
asname          VUTBR-AS
reverse         piranha.ro.vutbr.cz
query           147.229.2.90
```

## Repository overview
- ``main.c`` - Starting point of program.
- ``dns_query.h dns_query.cpp`` - Send DNS query and deparse response data with lreslov library.
- ``argument_parser.h argument_parser.cpp`` - Parse command line arguments.
- ``tcp_socket.h tcp_socket.cpp`` - Create send and recieve tcp sockets.
- ``whois_query.h whois_query.cpp`` - Send whois query and deparse recieved data.
- ``geolocation_database.h geolocation_database.cpp`` - Create HTTP request to ip-api.com geolocation database and parse recieved data.

## Third party packages
Application use ip-api.com API with terms and policies described here https://ip-api.com/docs/legal.

