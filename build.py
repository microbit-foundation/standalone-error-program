import subprocess
import shutil
import os


# names and paths

folder_make  = "microbit-nrfx/armgcc"
folder_build = folder_make + "/_build"
folder_built = "built"

file_bin = "nrfx_nrf52833_xxaa.bin"
file_hex = "nrfx_nrf52833_xxaa.hex"
file_hdr = "led_error_app.h"
cvar_name = "led_error_bin"

slash = "/"

from_bin = folder_build + slash + file_bin
from_hex = folder_build + slash + file_hex

to_bin = folder_built + slash + file_bin
to_hex = folder_built + slash + file_hex
to_hdr = folder_built + slash + file_hdr


# delete output folders for a fresh build

if os.path.isdir( folder_build):
    shutil.rmtree( folder_build)

if os.path.isdir( folder_built):
    shutil.rmtree( folder_built)


# make

p = subprocess.Popen(["make"], cwd=folder_make)
p.communicate()


# copy bin and hex to built folder in root

os.mkdir( folder_built)

if os.path.isfile( from_bin):
  shutil.copy2( from_bin, to_bin)
    
if os.path.isfile( from_hex):
  shutil.copy2( from_hex, to_hex)


# create a C header from the bin

with open( to_bin, 'rb') as f:
  bin = f.read();

code = bin.find(b"\xde\xc0\xce\xfa") + 4

hdr =  "// Program to display error code 0-999 on LEDs: face,E,digit,digit,digit\n\n"

hdr += "unsigned int " + cvar_name + "_len = {};\n\n".format( len(bin))

if code >= 0:
    hdr += "// Offset of 2 byte error code, low byte first\n"
    hdr += "unsigned int " + cvar_name + "_code = {};\n\n".format( code)
    
hdr += "unsigned char " + cvar_name + "_data[] = {\n  "

idx = 0

for b in bin:
    hdr += "0x{0:02x}".format(b);
    if idx == len(bin) - 1:
      hdr += "\n"
      hdr += "};\n"
    elif idx % 12 == 11:
      hdr += ",\n  "
    else:
      hdr += ", "
    idx = idx + 1
    
with open( to_hdr, 'w') as f:
  f.write(hdr);
