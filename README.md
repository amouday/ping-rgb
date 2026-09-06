# ping rgb 

A rgb led that changes color upon receiving an ICMP Echo packet.

It also outputs a hexdump of the received packet onto the serial console just as a bonus.

```console
I (520) ping-rgb: DHCP Server Started on 192.168.10.1                                                                 
I (1240) wifi:station: 08:71:90:ae:ff:80 join, AID=1, bgn, 40U                                                        
[+] Recived ICMP packet from: '192.168.10.2'                                                                          

45 00 00 54 65 6e 40 00 40 01 3f e7 c0 a8 0a 02         |E..Ten@.@.?.....|                                            
c0 a8 0a 01 08 00 31 67 ab f2 00 01 89 7e 9d 6a         |......1g.....~.j|                                            
00 00 00 00 2c e9 08 00 00 00 00 00 10 11 12 13         |....,...........|                                            
14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23         |............ !"#|                                            
24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33         |$%&'()*+,-./0123|                                            
34 35 36 37                                             |4567|                                                        

[+] Setting color to: 'Red'                                         

[+] Recived ICMP packet from: '192.168.10.2'                                                                          
45 00 00 54 68 f9 40 00 40 01 3c 5c c0 a8 0a 02         |E..Th.@.@.<\....|                                            
c0 a8 0a 01 08 00 ef 5e ab f2 00 02 8a 7e 9d 6a         |.......^.....~.j|
00 00 00 00 6d f0 08 00 00 00 00 00 10 11 12 13         |....m...........|                                            
14 15 16 17 18 19 1a 1b 1c 1d 1e 1f 20 21 22 23         |............ !"#|
24 25 26 27 28 29 2a 2b 2c 2d 2e 2f 30 31 32 33         |$%&'()*+,-./0123|
34 35 36 37                                             |4567|

[+] Setting color to: 'Orange'

```
