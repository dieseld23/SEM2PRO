# This file is automatically generated.
# It contains project source information necessary for synthesis and implementation.

# Block Designs: bd/design_transmitter/design_transmitter.bd
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter || ORIG_REF_NAME==design_transmitter}]

# IP: bd/design_transmitter/ip/design_transmitter_processing_system7_0_0/design_transmitter_processing_system7_0_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_processing_system7_0_0 || ORIG_REF_NAME==design_transmitter_processing_system7_0_0}]

# IP: bd/design_transmitter/ip/design_transmitter_axi_gpio_0_0/design_transmitter_axi_gpio_0_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_0_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_0_0}]

# IP: bd/design_transmitter/ip/design_transmitter_processing_system7_0_axi_periph_0/design_transmitter_processing_system7_0_axi_periph_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_processing_system7_0_axi_periph_0 || ORIG_REF_NAME==design_transmitter_processing_system7_0_axi_periph_0}]

# IP: bd/design_transmitter/ip/design_transmitter_rst_processing_system7_0_100M_0/design_transmitter_rst_processing_system7_0_100M_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==design_transmitter_rst_processing_system7_0_100M_0}]

# IP: bd/design_transmitter/ip/design_transmitter_axi_gpio_1_0/design_transmitter_axi_gpio_1_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_1_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_1_0}]

# IP: bd/design_transmitter/ip/design_transmitter_xbar_0/design_transmitter_xbar_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_xbar_0 || ORIG_REF_NAME==design_transmitter_xbar_0}]

# IP: bd/design_transmitter/ip/design_transmitter_auto_pc_0/design_transmitter_auto_pc_0.xci
set_property DONT_TOUCH TRUE [get_cells -hier -filter {REF_NAME==design_transmitter_auto_pc_0 || ORIG_REF_NAME==design_transmitter_auto_pc_0}]

# XDC: bd/design_transmitter/ip/design_transmitter_processing_system7_0_0/design_transmitter_processing_system7_0_0.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_processing_system7_0_0 || ORIG_REF_NAME==design_transmitter_processing_system7_0_0}] {/inst }]/inst ]]

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_0_0/design_transmitter_axi_gpio_0_0_board.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_0_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_0_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_0_0/design_transmitter_axi_gpio_0_0_ooc.xdc

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_0_0/design_transmitter_axi_gpio_0_0.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_0_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_0_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_rst_processing_system7_0_100M_0/design_transmitter_rst_processing_system7_0_100M_0_board.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==design_transmitter_rst_processing_system7_0_100M_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_rst_processing_system7_0_100M_0/design_transmitter_rst_processing_system7_0_100M_0.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_rst_processing_system7_0_100M_0 || ORIG_REF_NAME==design_transmitter_rst_processing_system7_0_100M_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_rst_processing_system7_0_100M_0/design_transmitter_rst_processing_system7_0_100M_0_ooc.xdc

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_1_0/design_transmitter_axi_gpio_1_0_board.xdc
set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_1_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_1_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_1_0/design_transmitter_axi_gpio_1_0_ooc.xdc

# XDC: bd/design_transmitter/ip/design_transmitter_axi_gpio_1_0/design_transmitter_axi_gpio_1_0.xdc
#dup# set_property DONT_TOUCH TRUE [get_cells [split [join [get_cells -hier -filter {REF_NAME==design_transmitter_axi_gpio_1_0 || ORIG_REF_NAME==design_transmitter_axi_gpio_1_0}] {/U0 }]/U0 ]]

# XDC: bd/design_transmitter/ip/design_transmitter_xbar_0/design_transmitter_xbar_0_ooc.xdc

# XDC: bd/design_transmitter/ip/design_transmitter_auto_pc_0/design_transmitter_auto_pc_0_ooc.xdc

# XDC: bd/design_transmitter/design_transmitter_ooc.xdc
