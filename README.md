## Coast Runner CR-1 Firmware for 32M1

This firmware controls the BLDC spindle used on the Coast Runner CR-1.
It provides closed-loop (VFD) spindle control using a trap strategy. 

## Building

1. Clone the repo
1. Install Microchip Studio (previously known as Atmel Studio or AVR Studio): https://www.microchip.com/en-us/tools-resources/develop/microchip-studio
1. Inside Microchip Studio, open existing project and select [repo root] > src > GrBLDC_VFD.atsln
1. Clear out any existing build files (any file named "bldc_sensorless") in the build folder ([repo root] > src > bldc_sensor > default). If these files exist and there has been no change to the code, then the solution won't build.
1. Run Build > Build bldc_sensor
1. The built hex file should now reside at [repo root] > src > bldc_sensor > default > bldc_sensorless.hex
