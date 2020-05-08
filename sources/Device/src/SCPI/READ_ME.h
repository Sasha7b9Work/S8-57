#pragma once

����� ������ SCPI


+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| �������           |                                                                                                                                                                       |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ������������ �������                                                                                                                                                  |
+...........................................................................................................................................................................................+
| *IDN?             |                                                                                                                                                                       |
| *RST              |                                                                                                                                                                       |
| *CAL?             |                                                                                                                                                                       |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ��������� �������                                                                                                                                                     |
+...........................................................................................................................................................................................+
| :RUN              | ��������� ������� ����� ����������                                                                                                                                    |
| :STOP             | ������������� ������� ����� ����������                                                                                                                                |
| :AUTO             | ��������� ������� ������ �������                                                                                                                                      |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ��������������� �������                                                                                                                                               |
+...........................................................................................................................................................................................+
| :HELP             |                                                                                                                                                                       |
| :TEST             |                                                                                                                                                                       |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ������� ���������� ��������                                                                                                                                           |
+...........................................................................................................................................................................................+
| :CHANNEL{ 1 | 2 } |                                                                                                                                                                       |
|                   | :RANGE     { 2MV | 5MV | 10MV | 20MV | 50MV | 100MV | 200MV | 500MV | 1V | 2V | 5V | 10V | 20V }?                                                                     |
|                   | :SET       { A | B }?                                                                                                                                                 |
|                   | :OFFSET    < -200...200 >?                                                                                                                                            |
|                   | :DISPLAY   { ON | OFF }?                                                                                                                                              |
|                   | :COUPLING  { AC | DC | GND }?                                                                                                                                         |
|                   | :BANDWIDTH { FULL | 20MHZ }?                                                                                                                                          |
|                   | :PROBE     { 1X | 10X }?                                                                                                                                              |
|                   | :BALANCE                                                                                                                                                              |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ������� ���������� ��������                                                                                                                                           |
+...........................................................................................................................................................................................+
| :DISPLAY          | :AVERAGES  { 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 }?                                                                                                              |
|                   | :TYPE      { VECTORS | DOTS }?                                                                                                                                        |
|                   | :GRID      { 1 | 2 | 3 | 4 }?                                                                                                                                         |
|                   | :CLEAR                                                                                                                                                                |
|                   | :BRIGHTNESS <0...100>?                                                                                                                                                |
|                   | :ACCUMULATION { 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256}?                                                                                                            |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ������� �������������� � �����������                                                                                                                                  |
+...........................................................................................................................................................................................+
| :KEY              |                                                                                                                                                                       |
|                   | {:PRESS | :LONG } { FUNCTION | MEASURE | MEMORY | SERVICE | A | B | TIME | START | TRIG | DISPLAY | RANGEA+ | RANGEA- | RSHIFTA+ | RSHIFTA- | RANGEB+ | RANGEB-       |
|                   |                     RSHIFTB+ | RSHIFTB- | TBASE+ | TBASE- | TSHIFT+ | TSHIFT- | TRIGLEV+ | TRIGLEV- | LEFT | RIGHT | UP | DOWN | ENTER | F1 | F2 | F3 | F4 | F5 }     |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                     ������� ���������� ���������                                                                                                                                         |
+...........................................................................................................................................................................................+
| :TIMEBASE         |                                                                                                                                                                       |
|                   | :PEAKDET { ON | OFF }?                                                                                                                                                |
|                   | :MODE    { REAL | EQUAL }?                                                                                                                                            |
|                   | :OFFSET  < -8192..32768 >?                                                                                                                                            |
|                   | :TPOS    < LEFT | CENTER | RIGHT >?                                                                                                                                   |
|                   | :SCALE   { 2NS | 5NS | 10NS | 20NS | 50S | 100NS | 200NS | 500NS | 1US | 2US | 5US | 10US | 20US | 50US | 100US | 200US | 500US | 1MS | 2MS | 5MS | 10MS | 20MS |     |
|                   |          50MS | 100MS | 200MS | 500MS | 1S | 2S | 5S | 10S }?                                                                                                         |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                    ������� ���������� ��������������                                                                                                                                      |
+...........................................................................................................................................................................................+
| :TRIGGER          |                                                                                                                                                                       |
|                   | :MODE     { AUTO | WAIT | ONCE }?                                                                                                                                     |
|                   | :SOURCE   { A | B }?                                                                                                                                                  |
|                   | :LEVEL    < -200...+300 >?                                                                                                                                            |
|                   | :POLARITY { RISE | FALL }?                                                                                                                                            |
|                   | :COUPLING { DC | HF | LF }?                                                                                                                                           |
|                   | :MODEFIND { AUTO | MANUAL }?                                                                                                                                          |
|                   | :START                                                                                                                                                                |
|                   | :STOP                                                                                                                                                                 |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                    ������� ������ � �������                                                                                                                                               |
+...........................................................................................................................................................................................+
| :MEMORY           |                                                                                                                                                                       |
|                   | :LENGTH { 512| 1K | 2K | 4K | 8K }?                                                                                                                                   |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                    ������ �������                                                                                                                                                         |
+...........................................................................................................................................................................................+
| :CALIBRATOR       |                                                                                                                                                                       |
|                   | :MODE { AC | DC }?                                                                                                                                                    |
| :FFT              |                                                                                                                                                                       |
|                   | :SOURCE { A | B | AB }?                                                                                                                                               |
+-------------------+-----------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|                   |                                                                                                                                                                       |